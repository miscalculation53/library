#!/usr/bin/env python3

import argparse
import base64
import datetime
import json
import os
import pathlib
import re
import shutil
import subprocess
import sys
from typing import Dict, Iterable, List


TIMESTAMP_FORMAT = "%Y-%m-%d %H:%M:%S %z"
REMOTE_MARKER = pathlib.Path(".verify-helper/timestamps.remote.json")
TIMEOUTS_FILE = pathlib.Path(".verify-helper/timeouts.json")
FAILURES_FILE = pathlib.Path(".verify-helper/failures.json")


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
    failed: List[pathlib.Path],
    uncompleted: List[pathlib.Path],
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
            "failed": list(map(str, failed)),
            "uncompleted": list(map(str, uncompleted)),
        },
    )


def report_timeout(timeout: float, paths: List[pathlib.Path]) -> None:
    message = (
        f"Stopped after the {timeout:g} second time limit; "
        f"{len(paths)} verification file(s) remain pending."
    )
    print(f"::warning title=Verification timed out::{message}")
    print(message)


def run_verify(command: List[str]) -> tuple[subprocess.CompletedProcess, List[str]]:
    failed = []
    pattern = re.compile(r"^::error file=(.+)::failed to verify\s*$")
    suppress_failure_details = False
    suppression_reported = False
    process = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        encoding="utf-8",
        errors="replace",
        bufsize=1,
    )
    assert process.stdout is not None
    try:
        for line in process.stdout:
            if line.startswith("INFO:onlinejudge_verify.verify:verify:"):
                suppress_failure_details = False
                suppression_reported = False
            match = pattern.match(line)
            if match:
                failed.append(match.group(1))
            failure = line.startswith("[FAILURE]")
            important = match is not None or failure or line.startswith(
                (
                    "ERROR:",
                    "Traceback",
                    "subprocess.CalledProcessError:",
                    "::warning ",
                )
            )
            if not suppress_failure_details or important:
                sys.stdout.write(line)
                sys.stdout.flush()
            elif not suppression_reported:
                print(
                    "::warning title=Verification failure details omitted::"
                    "Large input and output dumps after a failure are omitted from "
                    "the GitHub Actions log. Failure annotations are still shown.",
                    flush=True,
                )
                suppression_reported = True
            if failure and os.environ.get("GITHUB_ACTIONS"):
                suppress_failure_details = True
    except BaseException:
        if process.poll() is None:
            process.terminate()
        process.wait()
        raise
    finally:
        process.stdout.close()
    return subprocess.CompletedProcess(command, process.wait()), failed


def write_shard_summary(
    paths: List[pathlib.Path],
    failed: List[pathlib.Path],
    uncompleted: List[pathlib.Path],
) -> None:
    failed_set = set(failed)
    uncompleted_set = set(uncompleted)
    verified_count = len(paths) - len(failed_set) - len(uncompleted_set)
    print(
        "verification result: "
        f"{verified_count} verified, {len(failed_set)} failed, "
        f"{len(uncompleted_set)} not completed"
    )

    summary_path = os.environ.get("GITHUB_STEP_SUMMARY")
    if not summary_path:
        return
    with pathlib.Path(summary_path).open("a", encoding="utf-8") as summary:
        summary.write("## Verification shard result\n\n")
        summary.write("| Result | Count |\n")
        summary.write("| --- | ---: |\n")
        summary.write(f"| Verified | {verified_count} |\n")
        summary.write(f"| Failed | {len(failed_set)} |\n")
        summary.write(f"| Not completed | {len(uncompleted_set)} |\n\n")
        summary.write("<details><summary>All targets</summary>\n\n")
        summary.write("| Result | Verification file |\n")
        summary.write("| --- | --- |\n")
        for path in paths:
            if path in failed_set:
                status = "Failed"
            elif path in uncompleted_set:
                status = "Not completed"
            else:
                status = "Verified"
            summary.write(f"| {status} | `{path}` |\n")
        summary.write("\n</details>\n")


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
    try:
        completed, reported_failed = run_verify(command)
    except BaseException:
        timestamps = load_object(REMOTE_MARKER)
        pending = unverified_paths(paths, timestamps)
        for path in pending:
            key = str(path)
            if key in selected:
                timestamps.setdefault(key, selected[key])
        write_shard_artifact(
            args.artifact_dir, paths, timestamps, "interrupted", [], pending
        )
        write_shard_summary(paths, [], pending)
        raise

    timestamps = load_object(REMOTE_MARKER)
    pending = unverified_paths(paths, timestamps)
    target_set = set(map(str, paths))
    failed_set = {
        pathlib.Path(path) for path in reported_failed if path in target_set
    }
    failed = sorted(failed_set)
    uncompleted = sorted(set(pending) - failed_set)
    for path in set(failed) | set(uncompleted):
        key = str(path)
        if key in selected:
            timestamps.setdefault(key, selected[key])
    if completed.returncode != 0:
        status = "failed" if failed else "interrupted"
        write_shard_artifact(
            args.artifact_dir, paths, timestamps, status, failed, uncompleted
        )
        write_shard_summary(paths, failed, uncompleted)
        raise subprocess.CalledProcessError(completed.returncode, command)

    if uncompleted:
        write_shard_artifact(
            args.artifact_dir, paths, timestamps, "timed_out", [], uncompleted
        )
        report_timeout(args.timeout, uncompleted)
        write_shard_summary(paths, [], uncompleted)
        return

    write_shard_artifact(args.artifact_dir, paths, timestamps, "success", [], [])
    write_shard_summary(paths, [], [])
    print(f"verified all {len(paths)} files")


def merge_results(args: argparse.Namespace) -> None:
    expected = {str(path) for path in verification_files(pathlib.Path("verify"))}
    allow_failures = getattr(args, "allow_failures", False)
    allow_missing = getattr(args, "allow_missing", False)
    owners: Dict[str, pathlib.Path] = {}
    merged: Dict[str, str] = {}
    if allow_missing and args.output.exists():
        merged = {
            path: timestamp
            for path, timestamp in load_object(args.output).items()
            if path in expected
        }
    failed = set()
    uncompleted = set()
    manifests = sorted(args.artifacts.rglob("targets.json"))
    if not manifests and not allow_missing:
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
            or result.get("status")
            not in {"success", "timed_out", "failed", "interrupted"}
            or not isinstance(result.get("failed"), list)
            or not all(isinstance(path, str) for path in result["failed"])
            or not isinstance(result.get("uncompleted"), list)
            or not all(isinstance(path, str) for path in result["uncompleted"])
        ):
            raise ValueError(f"invalid verification result in {result_path}")
        shard_failed = set(result["failed"])
        shard_uncompleted = set(result["uncompleted"])
        if shard_failed & shard_uncompleted:
            raise RuntimeError(f"failed and uncompleted overlap in {manifest.parent}")
        if not (shard_failed | shard_uncompleted) <= set(targets):
            raise RuntimeError(
                f"result contains non-targets in {manifest.parent}"
            )
        status = result["status"]
        if status == "success":
            if shard_failed or shard_uncompleted or set(timestamps) != set(targets):
                raise RuntimeError(f"successful shard is incomplete in {manifest.parent}")
        elif status == "timed_out":
            if shard_failed or not shard_uncompleted:
                raise RuntimeError(f"timed-out shard is inconsistent in {manifest.parent}")
        elif status == "failed":
            if not shard_failed:
                raise RuntimeError(f"failed shard has no failures in {manifest.parent}")
        elif shard_failed:
            raise RuntimeError(f"interrupted shard is inconsistent in {manifest.parent}")
        if status != "success":
            missing_timestamps = set(targets) - set(timestamps)
            if not missing_timestamps <= shard_failed | shard_uncompleted:
                raise RuntimeError(f"incomplete shard is inconsistent in {manifest.parent}")
        if status in {"failed", "interrupted"} and not allow_failures:
            raise RuntimeError(f"unsuccessful verification shard: {manifest.parent}")

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

        failed.update(shard_failed)
        uncompleted.update(shard_uncompleted)

        for target, timestamp in timestamps.items():
            datetime.datetime.strptime(timestamp, TIMESTAMP_FORMAT)
            merged[target] = timestamps[target]

    missing = sorted(expected - set(owners))
    if missing and not allow_missing:
        raise RuntimeError("verification artifacts do not cover:\n" + "\n".join(missing))
    uncompleted.update(missing)

    timeouts_output = getattr(args, "timeouts_output", None)
    if timeouts_output is None:
        timeouts_output = args.output.with_name("timeouts.json")
    failures_output = getattr(args, "failures_output", None)
    if failures_output is None:
        failures_output = args.output.with_name("failures.json")
    write_json(args.output, merged)
    write_json(timeouts_output, sorted(uncompleted))
    write_json(failures_output, sorted(failed))
    print(
        f"merged {len(manifests)} shards covering {len(owners)} files "
        f"with {len(merged)} timestamps, {len(failed)} failure(s), and "
        f"{len(uncompleted)} uncompleted file(s)"
    )


def publish_docs(args: argparse.Namespace) -> None:
    import onlinejudge_verify.documentation.build
    import onlinejudge_verify.documentation.type
    import onlinejudge_verify.main
    import onlinejudge_verify.marker

    compat_build = pathlib.Path("tools/ojverify_pkg_resources_compat/build")
    if compat_build.exists():
        shutil.rmtree(compat_build)

    uncompleted = (
        set(load_string_list(args.timeouts)) if args.timeouts.exists() else set()
    )
    failed = set(load_string_list(args.failures)) if args.failures.exists() else set()
    if failed & uncompleted:
        raise RuntimeError("failed and uncompleted verification files overlap")
    cwd = pathlib.Path.cwd().resolve(strict=True)
    marker_class = onlinejudge_verify.marker.VerificationMarker
    original_is_verified = marker_class.is_verified
    original_is_failed = marker_class.is_failed
    original_status_icon = (
        onlinejudge_verify.documentation.build._get_verification_status_icon
    )

    def relative_key(path: pathlib.Path) -> str | None:
        try:
            absolute = path if path.is_absolute() else cwd / path
            relative = absolute.resolve(strict=True).relative_to(cwd)
        except (OSError, ValueError):
            return None
        return relative.as_posix()

    def is_verified(marker: object, path: pathlib.Path) -> bool:
        key = relative_key(path)
        if key in failed or key in uncompleted:
            return False
        return original_is_verified(marker, path)

    def is_failed(marker: object, path: pathlib.Path) -> bool:
        key = relative_key(path)
        if key in failed:
            return True
        if key in uncompleted:
            return False
        return original_is_failed(marker, path)

    def status_icon(status: object) -> str:
        if (
            status
            == onlinejudge_verify.documentation.type.VerificationStatus.TEST_WAITING_JUDGE
        ):
            return ":question:"
        return original_status_icon(status)

    marker_class.is_verified = is_verified
    marker_class.is_failed = is_failed
    onlinejudge_verify.documentation.build._get_verification_status_icon = status_icon
    try:
        onlinejudge_verify.main.subcommand_docs(jobs=args.jobs)
    finally:
        marker_class.is_verified = original_is_verified
        marker_class.is_failed = original_is_failed
        onlinejudge_verify.documentation.build._get_verification_status_icon = (
            original_status_icon
        )


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
    merge.add_argument("--failures-output", type=pathlib.Path, default=FAILURES_FILE)
    merge.add_argument("--allow-failures", action="store_true")
    merge.add_argument("--allow-missing", action="store_true")
    merge.set_defaults(function=merge_results)

    docs = subparsers.add_parser("docs")
    docs.add_argument("--jobs", type=int, default=2)
    docs.add_argument("--timeouts", type=pathlib.Path, default=TIMEOUTS_FILE)
    docs.add_argument("--failures", type=pathlib.Path, default=FAILURES_FILE)
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
