#!/usr/bin/env python3
"""Run expansion and cleanup independently of Code Runner's foreground process."""
from __future__ import annotations

import argparse
from contextlib import contextmanager
import fcntl
import hashlib
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import time
import uuid

from submit_code import atomic_write, comment_record_path


TOOL = Path(__file__).with_name("submit_code.py").resolve()
POLL_SECONDS = 0.1


def digest(path: Path) -> str | None:
    try:
        return hashlib.sha256(path.read_bytes()).hexdigest()
    except FileNotFoundError:
        return None


def read_json(path: Path) -> dict:
    try:
        return json.loads(path.read_text())
    except FileNotFoundError:
        return {}


def write_json(path: Path, value: dict) -> None:
    atomic_write(path, json.dumps(value, ensure_ascii=False, indent=2) + "\n")


@contextmanager
def locked(job: Path):
    with (job / "state.lock").open("a") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        yield


def paths(source: Path) -> tuple[Path, Path]:
    name = "bundle.cpp" if source.name == "main.cpp" else source.stem + "-bundle.cpp"
    output = source.with_name(name)
    return output, source.parent / ".submit-code" / name


def status(job: Path, request: dict, phase: str, **details) -> None:
    write_json(job / "status.json", {
        "token": request["token"], "source": request["source"],
        "phase": phase, "updated_at": time.time(), **details,
    })


def start(source: Path) -> None:
    # Running the generated submission or a selected snippet should not bundle it again.
    if (source.name == "bundle.cpp" or source.name.endswith("-bundle.cpp")
            or source.name.startswith("tempCodeRunnerFile.")):
        return
    source_digest = digest(source)
    if source_digest is None:
        raise FileNotFoundError(source)
    output, job = paths(source)
    job.mkdir(parents=True, exist_ok=True)
    with locked(job):
        request = {
            "token": uuid.uuid4().hex, "source": str(source),
            "source_digest": source_digest, "output_digest": digest(output),
        }
        write_json(job / "request.json", request)
        status(job, request, "queued")
        # The worker owns the compiler processes. No stored PID is ever signalled.
        with (job / "worker.log").open("a") as log:
            subprocess.Popen(
                [sys.executable, str(Path(__file__).resolve()), "worker", str(job)],
                stdin=subprocess.DEVNULL, stdout=log,
                stderr=log, start_new_session=True,
            )
    print(f"[oj-b + cln] 裏で準備します → {output.name}")


class Superseded(Exception):
    pass


class Job:
    def __init__(self, directory: Path, request: dict, log):
        self.directory = directory
        self.request = request
        self.source = Path(request["source"])
        self.output, _ = paths(self.source)
        self.expected_output = request["output_digest"]
        self.log = log
        self.published_size: int | None = None
        self.expansion_digest: str | None = None

    def check(self) -> None:
        if read_json(self.directory / "request.json").get("token") != self.request["token"]:
            raise Superseded()
        if digest(self.source) != self.request["source_digest"]:
            raise RuntimeError("元コードが変更されたため中止しました。次の実行で再生成します。")
        if digest(self.output) != self.expected_output:
            raise RuntimeError("提出用ファイルが別の操作で変更されたため中止しました。")

    def update(self, phase: str, **details) -> None:
        with locked(self.directory):
            self.check()
            status(self.directory, self.request, phase,
                   bytes=self.published_size, **details)

    def publish(self, scratch: Path) -> None:
        # Check generation and input again while holding the same lock as start().
        text = scratch.read_text(encoding="utf-8")
        with locked(self.directory):
            self.check()
            # Standalone cleanup uses the source-comment snapshot belonging
            # to the published result, even when a later generation fails.
            comments = comment_record_path(scratch)
            if comments.is_file():
                atomic_write(comment_record_path(self.output), comments.read_text())
            atomic_write(self.output, text)
            self.expected_output = hashlib.sha256(text.encode()).hexdigest()
            self.published_size = len(text.encode())
            write_json(self.directory / "artifact.json", {
                "expansion_digest": self.expansion_digest,
                "output_digest": self.expected_output, "complete": False,
            })
            status(self.directory, self.request, "cleaning", bytes=self.published_size,
                   below_limit=self.published_size < 65536)

    def run(self, arguments: list[str], *, checkpoint: Path | None = None) -> None:
        revision = checkpoint.stat() if checkpoint else None
        process = subprocess.Popen(
            [sys.executable, str(TOOL), *arguments, "--color", "never"],
            cwd=self.source.parent, stdin=subprocess.DEVNULL,
            stdout=self.log, stderr=self.log, start_new_session=True,
        )
        try:
            while True:
                self.check()
                if checkpoint is not None:
                    current = checkpoint.stat()
                    if current != revision:
                        self.publish(checkpoint)
                        revision = current
                result = process.poll()
                if result is not None:
                    if result:
                        raise RuntimeError(f"処理に失敗しました (exit {result})。log.txt を確認してください。")
                    return
                time.sleep(POLL_SECONDS)
        finally:
            if process.poll() is None:
                os.killpg(process.pid, signal.SIGTERM)
                try:
                    process.wait(timeout=1)
                except subprocess.TimeoutExpired:
                    os.killpg(process.pid, signal.SIGKILL)
                    process.wait()

    def execute(self) -> None:
        # Keep the include directory stable so repeated runs reuse the compiler's PCH.
        scratch = self.directory / "work.cpp"
        try:
            self.update("expanding")
            self.run(["expand", str(self.source), "-o", str(scratch)])
            self.expansion_digest = hashlib.sha256(
                scratch.read_bytes() + TOOL.read_bytes()
            ).hexdigest()
            with locked(self.directory):
                self.check()
                previous = read_json(self.directory / "artifact.json")
                if (previous.get("expansion_digest") == self.expansion_digest
                        and previous.get("output_digest") == self.expected_output
                        and self.expected_output is not None):
                    self.published_size = self.output.stat().st_size
                    if previous.get("complete"):
                        status(self.directory, self.request, "done", bytes=self.published_size,
                               below_limit=self.published_size < 65536, reused=True)
                        return
                    # An interrupted run of exactly the same expansion can resume from
                    # its last checked checkpoint, including one already below the limit.
                    atomic_write(scratch, self.output.read_text())
            self.update("cleaning")
            # Expansion alone has not checked the code. Publish only cleanup checkpoints.
            self.run(["cleanup", str(scratch), "-I", str(self.source.parent)],
                     checkpoint=scratch)
            self.publish(scratch)
            with locked(self.directory):
                self.check()
                previous = read_json(self.directory / "artifact.json")
                previous["complete"] = True
                write_json(self.directory / "artifact.json", previous)
                status(self.directory, self.request, "done", bytes=self.published_size,
                       below_limit=self.published_size < 65536)
        finally:
            scratch.unlink(missing_ok=True)


def worker(directory: Path) -> None:
    # A single worker consumes the latest request. Locking the exit transition prevents
    # a request arriving just as it exits from being left without a worker.
    with (directory / "running.lock").open("a") as running:
        with locked(directory):
            try:
                fcntl.flock(running, fcntl.LOCK_EX | fcntl.LOCK_NB)
            except BlockingIOError:
                return
        try:
            os.nice(5)
        except OSError:
            pass  # Some sandboxes disallow even lowering our own priority.
        while True:
            with locked(directory):
                request = read_json(directory / "request.json")
            with (directory / "log.txt").open("w", buffering=1) as log:
                print(f"source: {request['source']}", file=log)
                try:
                    if request.get("stop"):
                        with locked(directory):
                            if read_json(directory / "request.json") == request:
                                status(directory, request, "stopped")
                    else:
                        Job(directory, request, log).execute()
                except Superseded:
                    pass
                except Exception as exc:
                    print(str(exc), file=log)
                    with locked(directory):
                        if read_json(directory / "request.json") == request:
                            previous = read_json(directory / "status.json")
                            status(directory, request, "failed", error=str(exc),
                                   bytes=previous.get("bytes"))
            with locked(directory):
                if read_json(directory / "request.json") == request:
                    fcntl.flock(running, fcntl.LOCK_UN)
                    return


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("start", "status", "stop", "worker"))
    parser.add_argument("source", nargs="?", default="main.cpp")
    args = parser.parse_args()
    source = Path(args.source).expanduser().resolve()
    try:
        if args.command == "worker":
            worker(source)
        elif args.command == "start":
            start(source)
        else:
            output, directory = paths(source)
            if not directory.exists():
                print("自動生成はまだ実行されていません。")
                return 0
            with locked(directory):
                request = read_json(directory / "request.json")
                if args.command == "stop":
                    request.update(token=uuid.uuid4().hex, stop=True)
                    write_json(directory / "request.json", request)
                    status(directory, request, "stopped")
                    print("自動生成を中止します。最後の出力は残します。")
                else:
                    value = read_json(directory / "status.json")
                    names = {"queued": "開始待ち", "expanding": "展開中", "cleaning": "短縮中",
                             "done": "完了", "failed": "失敗", "stopped": "停止"}
                    phase = names.get(value.get("phase"), "不明")
                    print(f"{output.name}: {phase}")
                    if value.get("bytes") is not None:
                        size = value["bytes"]
                        print(f"今回の出力: {size:,} B ({'< 65,536 B' if size < 65536 else 'サイズ制限未達'})")
                    if not request.get("stop") and digest(source) != request.get("source_digest"):
                        print("元コードは生成開始時から変更されています。次の実行で再生成します。")
                    if value.get("error"):
                        print(value["error"])
                    print(f"ログ: {directory / 'log.txt'}")
    except (OSError, ValueError) as exc:
        print(f"[oj-b + cln] {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
