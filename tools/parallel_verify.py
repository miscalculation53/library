#!/usr/bin/env python3

import argparse
import base64
import datetime
import json
import os
import pathlib
import re
import subprocess
from typing import Dict, Iterable, List


TIMESTAMP_FORMAT = "%Y-%m-%d %H:%M:%S %z"
REMOTE_MARKER = pathlib.Path(".verify-helper/timestamps.remote.json")
TIMEOUTS_FILE = pathlib.Path(".verify-helper/timeouts.json")


def load_object(path: pathlib.Path) -> Dict[str, str]:
    with path.open(encoding="utf-8") as file:
        data = json.load(file)
    if not isinstance(data, dict) or not all(
        isinstance(key, str) and isinstance(value, str)
        for key, value in data.items()
    ):
        raise ValueError(f"{path} must contain a JSON object of string pairs")
    return data


def load_string_list(path: pathlib.Path) -> List[str]:
    with path.open(encoding="utf-8") as file:
        data = json.load(file)
    if not isinstance(data, list) or not all(isinstance(value, str) for value in data):
        raise ValueError(f"{path} must contain a JSON array of strings")
    return data


def write_json(path: pathlib.Path, data: object) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + ".tmp")
    with temporary.open("w", encoding="utf-8") as file:
        json.dump(data, file, ensure_ascii=True, sort_keys=True, indent=0)
        file.write("\n")
    os.replace(temporary, path)


def verification_files(directory: pathlib.Path) -> List[pathlib.Path]:
    import onlinejudge_verify.utils

    directory = directory.resolve(strict=True)
    cwd = pathlib.Path.cwd().resolve(strict=True)
    files = []
    for path in onlinejudge_verify.utils.iterate_verification_files():
        absolute = path.resolve(strict=True)
        if absolute.is_relative_to(directory):
            files.append(absolute.relative_to(cwd))
    return sorted(files)


def unverified_paths(
    paths: Iterable[pathlib.Path], timestamps: Dict[str, str]
) -> List[pathlib.Path]:
    import onlinejudge_verify.marker

    epoch = datetime.datetime.fromtimestamp(0, tz=datetime.timezone.utc)
    pending = []
    errors = []
    for path in paths:
        key = str(path)
        value = timestamps.get(key)
        if value is None:
            pending.append(path)
            continue
        try:
            verified_at = datetime.datetime.strptime(value, TIMESTAMP_FORMAT)
        except ValueError:
            errors.append(f"invalid timestamp: {key}: {value}")
            continue
        current = onlinejudge_verify.marker.get_last_commit_time_to_verify(path)
        if current <= epoch:
            errors.append(f"failed to inspect dependencies: {key}")
        elif verified_at < current:
            pending.append(path)
    if errors:
        raise RuntimeError("\n".join(errors))
    return pending


def write_shard_artifact(
    artifact_dir: pathlib.Path,
    paths: List[pathlib.Path],
    timestamps: Dict[str, str],
    status: str,
    pending: List[pathlib.Path],
) -> None:
    targets = list(map(str, paths))
    artifact_timestamps = {
        target: timestamps[target] for target in targets if target in timestamps
    }
    write_json(artifact_dir / "targets.json", targets)
    write_json(artifact_dir / "timestamps.remote.json", artifact_timestamps)
    write_json(
        artifact_dir / "result.json",
        {
            "status": status,
            "pending": list(map(str, pending)),
        },
    )


def report_timeout(timeout: float, paths: List[pathlib.Path]) -> None:
    message = (
        f"Stopped after the {timeout:g} second time limit; "
        f"{len(paths)} verification file(s) remain pending."
    )
    print(f"::warning title=Verification timed out::{message}")
    print(message)

    summary_path = os.environ.get("GITHUB_STEP_SUMMARY")
    if summary_path:
        with pathlib.Path(summary_path).open("a", encoding="utf-8") as summary:
            summary.write("## Verification timed out\n\n")
            summary.write(message + "\n\n")
            summary.write("Any previous timestamps were preserved for these files:\n\n")
            for path in paths:
                summary.write(f"- `{path}`\n")


def run_shard(args: argparse.Namespace) -> None:
    if args.shard_count <= 0:
        raise ValueError("--shard-count must be positive")
    if not 0 <= args.shard < args.shard_count:
        raise ValueError("--shard must be in [0, shard-count)")

    all_paths = verification_files(args.directory)
    paths = all_paths[args.shard :: args.shard_count]
    if not paths:
        raise RuntimeError("the shard contains no verification files")

    old_timestamps = load_object(REMOTE_MARKER) if REMOTE_MARKER.exists() else {}
    selected = {str(path): old_timestamps[str(path)] for path in paths if str(path) in old_timestamps}
    write_json(REMOTE_MARKER, selected)

    print(
        f"shard {args.shard + 1}/{args.shard_count}: "
        f"{len(paths)}/{len(all_paths)} files under {args.directory}"
    )
    command = [
        "oj-verify",
        "run",
        "-j",
        str(args.jobs),
        "--timeout",
        str(args.timeout),
        *map(str, paths),
    ]
    completed = subprocess.run(command, check=False)

    timestamps = load_object(REMOTE_MARKER)
    pending = unverified_paths(paths, timestamps)
    for path in pending:
        key = str(path)
        if key in selected:
            timestamps.setdefault(key, selected[key])
    if completed.returncode != 0:
        write_shard_artifact(args.artifact_dir, paths, timestamps, "failed", pending)
        raise subprocess.CalledProcessError(completed.returncode, command)

    if pending:
        write_shard_artifact(args.artifact_dir, paths, timestamps, "timed_out", pending)
        report_timeout(args.timeout, pending)
        return

    write_shard_artifact(args.artifact_dir, paths, timestamps, "success", [])
    print(f"verified all {len(paths)} files")


def merge_results(args: argparse.Namespace) -> None:
    expected = {str(path) for path in verification_files(pathlib.Path("verify"))}
    owners: Dict[str, pathlib.Path] = {}
    merged: Dict[str, str] = {}
    timed_out = set()
    manifests = sorted(args.artifacts.rglob("targets.json"))
    if not manifests:
        raise RuntimeError(f"no verification artifacts found under {args.artifacts}")

    for manifest in manifests:
        with manifest.open(encoding="utf-8") as file:
            targets = json.load(file)
        if not isinstance(targets, list) or not all(isinstance(path, str) for path in targets):
            raise ValueError(f"{manifest} must contain a JSON array of paths")

        timestamps_path = manifest.with_name("timestamps.remote.json")
        timestamps = load_object(timestamps_path)
        if not set(timestamps) <= set(targets):
            raise RuntimeError(f"timestamps contain non-targets in {manifest.parent}")

        result_path = manifest.with_name("result.json")
        with result_path.open(encoding="utf-8") as file:
            result = json.load(file)
        if (
            not isinstance(result, dict)
            or result.get("status") not in {"success", "timed_out"}
            or not isinstance(result.get("pending"), list)
            or not all(isinstance(path, str) for path in result["pending"])
        ):
            raise ValueError(f"invalid verification result in {result_path}")
        pending = set(result["pending"])
        if not pending <= set(targets):
            raise RuntimeError(f"pending files contain non-targets in {manifest.parent}")
        if result["status"] == "success":
            if pending or set(timestamps) != set(targets):
                raise RuntimeError(f"successful shard is incomplete in {manifest.parent}")
        else:
            missing_timestamps = set(targets) - set(timestamps)
            if not pending or not missing_timestamps <= pending:
                raise RuntimeError(f"timed-out shard is inconsistent in {manifest.parent}")

        for target in targets:
            path = pathlib.PurePosixPath(target)
            if path.is_absolute() or ".." in path.parts or str(path) != target:
                raise ValueError(f"invalid verification path in {manifest}: {target}")
            if target not in expected:
                raise RuntimeError(f"unexpected verification path in {manifest}: {target}")
            if target in owners:
                raise RuntimeError(
                    f"duplicate verification path: {target} "
                    f"({owners[target].parent} and {manifest.parent})"
                )
            owners[target] = manifest

        if result["status"] == "timed_out":
            timed_out.update(pending)

        for target, timestamp in timestamps.items():
            datetime.datetime.strptime(timestamp, TIMESTAMP_FORMAT)
            merged[target] = timestamps[target]

    missing = sorted(expected - set(owners))
    if missing:
        raise RuntimeError("verification artifacts do not cover:\n" + "\n".join(missing))

    timeouts_output = getattr(args, "timeouts_output", None)
    if timeouts_output is None:
        timeouts_output = args.output.with_name("timeouts.json")
    write_json(args.output, merged)
    write_json(timeouts_output, sorted(timed_out))
    print(
        f"merged {len(manifests)} shards covering {len(owners)} files "
        f"with {len(merged)} timestamps and {len(timed_out)} timeout(s)"
    )


def publish_docs(args: argparse.Namespace) -> None:
    import onlinejudge_verify.main
    import onlinejudge_verify.marker

    timed_out = set(load_string_list(args.timeouts)) if args.timeouts.exists() else set()
    cwd = pathlib.Path.cwd().resolve(strict=True)
    marker_class = onlinejudge_verify.marker.VerificationMarker
    original_is_failed = marker_class.is_failed

    def is_failed(marker: object, path: pathlib.Path) -> bool:
        try:
            absolute = path if path.is_absolute() else cwd / path
            relative = absolute.resolve(strict=True).relative_to(cwd)
        except (OSError, ValueError):
            return original_is_failed(marker, path)
        if relative.as_posix() in timed_out:
            return False
        return original_is_failed(marker, path)

    marker_class.is_failed = is_failed
    try:
        onlinejudge_verify.main.subcommand_docs(jobs=args.jobs)
    finally:
        marker_class.is_failed = original_is_failed


def publish_result(args: argparse.Namespace) -> None:
    if os.environ.get("GITHUB_ACTIONS") != "true":
        raise RuntimeError("publish is only available in GitHub Actions")

    token = os.environ.get("GITHUB_TOKEN")
    repository = os.environ.get("GITHUB_REPOSITORY", "")
    ref = os.environ.get("GITHUB_REF", "")
    sha = os.environ.get("GITHUB_SHA", "")
    if not token:
        raise RuntimeError("GITHUB_TOKEN is not set")
    if not re.fullmatch(r"[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+", repository):
        raise RuntimeError("GITHUB_REPOSITORY is invalid")
    if not re.fullmatch(r"refs/heads/[A-Za-z0-9_./-]+", ref) or ".." in ref:
        raise RuntimeError("GITHUB_REF is not a branch")

    subprocess.run(["git", "add", "--", str(args.marker)], check=True)
    changed = subprocess.run(["git", "diff", "--cached", "--quiet"], check=False)
    if changed.returncode == 0:
        print("verification timestamps are unchanged")
        return
    if changed.returncode != 1:
        raise subprocess.CalledProcessError(changed.returncode, changed.args)

    subprocess.run(
        [
            "git",
            "-c",
            "user.name=GitHub",
            "-c",
            "user.email=noreply@github.com",
            "commit",
            "-m",
            f"[auto-verifier] verify commit {sha}",
        ],
        check=True,
    )

    credentials = base64.b64encode(f"x-access-token:{token}".encode()).decode()
    env = os.environ.copy()
    env.update(
        GIT_CONFIG_COUNT="1",
        GIT_CONFIG_KEY_0="http.https://github.com/.extraheader",
        GIT_CONFIG_VALUE_0=f"AUTHORIZATION: basic {credentials}",
    )
    subprocess.run(
        [
            "git",
            "push",
            f"https://github.com/{repository}.git",
            f"HEAD:{ref}",
        ],
        env=env,
        check=True,
    )
    print(f"published {args.marker} to {ref}")


def make_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Run oj-verify in shards and merge their timestamp files."
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    run = subparsers.add_parser("run")
    run.add_argument("--directory", type=pathlib.Path, required=True)
    run.add_argument("--shard", type=int, required=True)
    run.add_argument("--shard-count", type=int, required=True)
    run.add_argument("--jobs", type=int, default=2)
    run.add_argument("--timeout", type=float, default=2400)
    run.add_argument("--artifact-dir", type=pathlib.Path, required=True)
    run.set_defaults(function=run_shard)

    merge = subparsers.add_parser("merge")
    merge.add_argument("--artifacts", type=pathlib.Path, required=True)
    merge.add_argument("--output", type=pathlib.Path, default=REMOTE_MARKER)
    merge.add_argument("--timeouts-output", type=pathlib.Path, default=TIMEOUTS_FILE)
    merge.set_defaults(function=merge_results)

    docs = subparsers.add_parser("docs")
    docs.add_argument("--jobs", type=int, default=2)
    docs.add_argument("--timeouts", type=pathlib.Path, default=TIMEOUTS_FILE)
    docs.set_defaults(function=publish_docs)

    publish = subparsers.add_parser("publish")
    publish.add_argument("--marker", type=pathlib.Path, default=REMOTE_MARKER)
    publish.set_defaults(function=publish_result)
    return parser


def main() -> None:
    args = make_parser().parse_args()
    args.function(args)


if __name__ == "__main__":
    main()
