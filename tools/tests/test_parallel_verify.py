import argparse
import base64
import datetime
import io
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

import tools_test_paths

import parallel_verify


class ParallelVerifyTest(unittest.TestCase):
    def setUp(self):
        # Simulated workflow annotations stay inside the test process.
        output = mock.patch.object(sys, "stdout", io.StringIO())
        output.start()
        self.addCleanup(output.stop)
        environment = mock.patch.dict(os.environ, {"GITHUB_ACTIONS": "", "GITHUB_STEP_SUMMARY": ""})
        environment.start()
        self.addCleanup(environment.stop)

    def write_artifact(
        self,
        root,
        name,
        targets,
        timestamps=None,
        status=None,
        failed=None,
        uncompleted=None,
    ):
        directory = root / name
        if timestamps is None:
            timestamps = {path: "2026-08-09 00:00:00 +0000" for path in targets}
        if failed is None:
            failed = []
        if uncompleted is None:
            uncompleted = [path for path in targets if path not in timestamps]
        if status is None:
            status = "timed_out" if uncompleted else "success"
        parallel_verify.write_json(directory / "targets.json", targets)
        parallel_verify.write_json(directory / "timestamps.remote.json", timestamps)
        parallel_verify.write_json(
            directory / "result.json",
            {
                "status": status,
                "failed": failed,
                "uncompleted": uncompleted,
            },
        )

    def test_run_verify_streams_and_collects_failed_annotations(self):
        command = [
            sys.executable,
            "-c",
            (
                "print('before'); "
                "print('::error file=verify/a.test.cpp::failed to verify'); "
                "raise SystemExit(1)"
            ),
        ]
        completed, failed = parallel_verify.run_verify(command)
        self.assertEqual(completed.returncode, 1)
        self.assertEqual(failed, ["verify/a.test.cpp"])

    def test_run_verify_omits_ci_failure_details_but_keeps_failures(self):
        command = [
            sys.executable,
            "-c",
            (
                "print('before'); "
                "print('[FAILURE] RE: return code 139'); "
                "print('x' * 1000); "
                "print('::error file=verify/a.test.cpp::failed to verify'); "
                "print('INFO:onlinejudge_verify.verify:verify: verify/b.test.cpp'); "
                "print('after'); "
                "raise SystemExit(1)"
            ),
        ]
        output = io.StringIO()
        with mock.patch.dict(os.environ, {"GITHUB_ACTIONS": "true"}), mock.patch.object(
            sys, "stdout", output
        ):
            completed, failed = parallel_verify.run_verify(command)

        log = output.getvalue()
        self.assertIn("before", log)
        self.assertNotIn("x" * 1000, log)
        self.assertIn("Verification failure details omitted", log)
        self.assertIn("[FAILURE] RE: return code 139", log)
        self.assertIn("::error file=verify/a.test.cpp::failed to verify", log)
        self.assertIn("verify/b.test.cpp", log)
        self.assertIn("after", log)
        self.assertEqual(completed.returncode, 1)
        self.assertEqual(failed, ["verify/a.test.cpp"])

    def test_run_verify_keeps_next_compilation_errors_after_ci_failure(self):
        command = [
            sys.executable,
            "-c",
            (
                "print('[FAILURE] TLE'); "
                "print('large failed test output'); "
                "print('INFO:onlinejudge_verify.languages.cplusplus:$ g++ next.cpp'); "
                "print('next.cpp:3: error: ambiguous function call'); "
                "print('::error file=verify/next.test.cpp::failed to verify'); "
                "raise SystemExit(1)"
            ),
        ]
        output = io.StringIO()
        with mock.patch.dict(os.environ, {"GITHUB_ACTIONS": "true"}), mock.patch.object(
            sys, "stdout", output
        ):
            completed, failed = parallel_verify.run_verify(command)

        log = output.getvalue()
        self.assertNotIn("large failed test output", log)
        self.assertIn("next.cpp:3: error: ambiguous function call", log)
        self.assertEqual(completed.returncode, 1)
        self.assertEqual(failed, ["verify/next.test.cpp"])

    def test_run_verify_identifies_only_interrupted_downloads(self):
        lines = []
        for name, phase, error, compiled in [
            ("eof", "download", "SSLEOFError: UNEXPECTED_EOF_WHILE_READING", False),
            ("certificate", "download", "SSLCertVerificationError", False),
            ("runtime", "test", "SSLEOFError", False),
            ("recovered", "download", "SSLEOFError", True),
        ]:
            path = f"verify/{name}.test.cpp"
            lines.extend([
                f"INFO:onlinejudge_verify.verify:verify: {path}",
                f"INFO:onlinejudge_verify.verify:$ oj {phase} --system example",
                f"[ERROR] {error}",
            ])
            if compiled:
                lines.append("INFO:onlinejudge_verify.languages.cplusplus:$ g++ test.cpp")
            lines.extend([
                f"::error file={path}::failed to verify",
                f"ERROR:onlinejudge_verify.verify:failed: {path}",
            ])
        command = [sys.executable, "-c", f"print({chr(10).join(lines)!r}); raise SystemExit(1)"]
        output = io.StringIO()
        download_errors = set()
        with mock.patch.object(sys, "stdout", output):
            completed, failed = parallel_verify.run_verify(
                command, download_errors=download_errors
            )
        self.assertEqual(completed.returncode, 1)
        self.assertEqual(download_errors, {"verify/eof.test.cpp"})
        self.assertEqual(len(failed), 4)
        self.assertIn("::warning file=verify/eof.test.cpp", output.getvalue())
        self.assertIn("::error file=verify/certificate.test.cpp", output.getvalue())
        self.assertIn("::error file=verify/runtime.test.cpp", output.getvalue())
        self.assertIn("::error file=verify/recovered.test.cpp", output.getvalue())

    def test_download_retry_rechecks_only_interrupted_targets_and_keeps_wa(self):
        paths = [Path("verify/eof.test.cpp"), Path("verify/wa.test.cpp"), Path("verify/ac.test.cpp")]
        command = ["oj-verify", "run", "--timeout", "2100", *map(str, paths)]

        def run(command, *, download_errors):
            if "verify/wa.test.cpp" in command:
                download_errors.add("verify/eof.test.cpp")
                return subprocess.CompletedProcess(command, 1), ["verify/eof.test.cpp", "verify/wa.test.cpp"]
            return subprocess.CompletedProcess(command, 0), []

        with mock.patch.object(parallel_verify, "run_verify", side_effect=run) as verify:
            with mock.patch.object(parallel_verify.time, "sleep") as sleep:
                with mock.patch.object(parallel_verify.time, "monotonic", side_effect=[100, 110]):
                    completed, failed = parallel_verify.run_verify_with_download_retries(command, paths, 2100)
        self.assertEqual(completed.returncode, 1)
        self.assertEqual(failed, ["verify/wa.test.cpp"])
        self.assertEqual(verify.call_count, 2)
        self.assertEqual(verify.call_args.args[0], ["oj-verify", "run", "--timeout", "2085", "verify/eof.test.cpp"])
        sleep.assert_called_once_with(5)

    def test_download_retry_is_bounded_and_final_failure_remains_fatal(self):
        path = "verify/eof.test.cpp"
        paths = [Path(path)]
        command = ["oj-verify", "run", "--timeout", "2100", path]

        def run(command, *, download_errors):
            if download_errors is not None:
                download_errors.add(path)
            return subprocess.CompletedProcess(command, 1), [path]

        with mock.patch.object(parallel_verify, "run_verify", side_effect=run) as verify:
            with mock.patch.object(parallel_verify.time, "sleep") as sleep:
                completed, failed = parallel_verify.run_verify_with_download_retries(command, paths, 2100)
        self.assertEqual(completed.returncode, 1)
        self.assertEqual(failed, [path])
        self.assertEqual(verify.call_count, 3)
        self.assertEqual(sleep.call_args_list, [mock.call(5), mock.call(10)])
        self.assertIsNone(verify.call_args.kwargs["download_errors"])

    def test_download_retry_stops_when_shard_budget_is_spent(self):
        path = "verify/eof.test.cpp"
        command = ["oj-verify", "run", "--timeout", "10", path]

        def run(command, *, download_errors):
            download_errors.add(path)
            return subprocess.CompletedProcess(command, 1), [path]

        output = io.StringIO()
        with mock.patch.object(parallel_verify, "run_verify", side_effect=run) as verify:
            with mock.patch.object(parallel_verify.time, "sleep") as sleep:
                with mock.patch.object(parallel_verify.time, "monotonic", side_effect=[100, 109]):
                    with mock.patch.object(sys, "stdout", output):
                        completed, failed = parallel_verify.run_verify_with_download_retries(command, [Path(path)], 10)
        self.assertEqual(completed.returncode, 1)
        self.assertEqual(failed, [path])
        self.assertEqual(verify.call_count, 1)
        sleep.assert_not_called()
        self.assertIn(f"::error file={path}::failed to verify", output.getvalue())

    def test_download_retry_preserves_process_interruption(self):
        path = "verify/eof.test.cpp"
        command = ["oj-verify", "run", "--timeout", "2100", path]

        def run(command, *, download_errors):
            download_errors.add(path)
            return subprocess.CompletedProcess(command, 130), [path]

        with mock.patch.object(parallel_verify, "run_verify", side_effect=run) as verify:
            with mock.patch.object(parallel_verify.time, "sleep") as sleep:
                completed, failed = parallel_verify.run_verify_with_download_retries(command, [Path(path)], 2100)
        self.assertEqual(completed.returncode, 130)
        self.assertEqual(failed, [path])
        self.assertEqual(verify.call_count, 1)
        sleep.assert_not_called()

    def test_download_retry_recovers_from_tls_eof_with_real_subprocess(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            script = root / "verify.py"
            script.write_text("""from pathlib import Path
import sys
marker = Path(__file__).with_suffix('.attempt')
attempt = int(marker.read_text()) if marker.exists() else 0
marker.write_text(str(attempt + 1))
path = sys.argv[-1]
print(f'INFO:onlinejudge_verify.verify:verify: {path}')
if attempt < 2:
    print('INFO:onlinejudge_verify.verify:$ oj download --system example')
    print('[ERROR] HTTPSConnectionPool: SSLEOFError: UNEXPECTED_EOF_WHILE_READING')
    print(f'::error file={path}::failed to verify')
    print(f'ERROR:onlinejudge_verify.verify:failed: {path}')
    raise SystemExit(1)
print('INFO:onlinejudge_verify.languages.cplusplus:$ g++ test.cpp')
print('[SUCCESS] test success: 10 cases')
""")
            path = Path("verify/eof.test.cpp")
            command = [sys.executable, str(script), "--timeout", "2100", str(path)]
            output = io.StringIO()
            with mock.patch.object(parallel_verify.time, "sleep"):
                with mock.patch.object(sys, "stdout", output):
                    completed, failed = parallel_verify.run_verify_with_download_retries(command, [path], 2100)
            self.assertEqual(completed.returncode, 0)
            self.assertEqual(failed, [])
            self.assertEqual(script.with_suffix(".attempt").read_text(), "3")
            self.assertNotIn("::error file=", output.getvalue())
            self.assertIn("[SUCCESS] test success: 10 cases", output.getvalue())

    def test_ci_targets_and_merge_exclude_manual_atcoder_examples(self):
        import onlinejudge_verify.utils

        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            targets = ["verify/aoj/a.test.cpp", "verify/mytest/b.test.cpp"]
            manual = "verify/atcoder/manual.test.cpp"
            for name in targets + [manual]:
                path = root / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.touch()
            self.write_artifact(root / "artifacts", "shard", targets)
            args = argparse.Namespace(artifacts=root / "artifacts", output=root / "merged.json")
            old_cwd = Path.cwd()
            try:
                os.chdir(root)
                with mock.patch.object(
                    onlinejudge_verify.utils, "iterate_verification_files",
                    side_effect=lambda: iter(map(Path, targets + [manual])),
                ):
                    self.assertEqual(
                        parallel_verify.verification_files(Path("verify")),
                        list(map(Path, targets)),
                    )
                    self.assertEqual(
                        parallel_verify.verification_files(Path("verify/atcoder")), [],
                    )
                    parallel_verify.merge_results(args)
                    self.assertEqual(set(json.loads(args.output.read_text())), set(targets))
                    self.assertEqual(json.loads((root / "timeouts.json").read_text()), [])
                    self.write_artifact(root / "artifacts", "shard", targets[:1])
                    with self.assertRaisesRegex(RuntimeError, "verify/mytest/b.test.cpp"):
                        parallel_verify.merge_results(args)
            finally:
                os.chdir(old_cwd)

    def test_merge_requires_exactly_one_result_per_verification_file(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.write_artifact(root, "shard-1", ["verify/a.test.cpp"])
            self.write_artifact(root, "shard-2", ["verify/b.test.cpp"])
            output = root / "merged.json"
            args = argparse.Namespace(artifacts=root, output=output)
            with mock.patch.object(
                parallel_verify,
                "verification_files",
                return_value=[Path("verify/a.test.cpp"), Path("verify/b.test.cpp")],
            ):
                parallel_verify.merge_results(args)

            self.assertEqual(
                set(json.loads(output.read_text())),
                {"verify/a.test.cpp", "verify/b.test.cpp"},
            )

    def test_merge_rejects_duplicate_results(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.write_artifact(root, "shard-1", ["verify/a.test.cpp"])
            self.write_artifact(root, "shard-2", ["verify/a.test.cpp"])
            args = argparse.Namespace(artifacts=root, output=root / "merged.json")
            with mock.patch.object(
                parallel_verify,
                "verification_files",
                return_value=[Path("verify/a.test.cpp")],
            ):
                with self.assertRaisesRegex(RuntimeError, "duplicate verification path"):
                    parallel_verify.merge_results(args)

    def test_merge_accepts_a_timed_out_target_without_a_timestamp(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.write_artifact(
                root,
                "shard",
                ["verify/a.test.cpp", "verify/b.test.cpp"],
                {"verify/a.test.cpp": "2026-08-09 00:00:00 +0000"},
            )
            output = root / "merged.json"
            args = argparse.Namespace(artifacts=root, output=output)
            with mock.patch.object(
                parallel_verify,
                "verification_files",
                return_value=[Path("verify/a.test.cpp"), Path("verify/b.test.cpp")],
            ):
                parallel_verify.merge_results(args)

            self.assertEqual(
                json.loads(output.read_text()),
                {"verify/a.test.cpp": "2026-08-09 00:00:00 +0000"},
            )
            self.assertEqual(
                json.loads((root / "timeouts.json").read_text()),
                ["verify/b.test.cpp"],
            )

    def test_merge_rejects_an_incomplete_success(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.write_artifact(
                root,
                "shard",
                ["verify/a.test.cpp", "verify/b.test.cpp"],
                {"verify/a.test.cpp": "2026-08-09 00:00:00 +0000"},
                status="success",
                uncompleted=[],
            )
            args = argparse.Namespace(artifacts=root, output=root / "merged.json")
            with mock.patch.object(
                parallel_verify,
                "verification_files",
                return_value=[Path("verify/a.test.cpp"), Path("verify/b.test.cpp")],
            ):
                with self.assertRaisesRegex(RuntimeError, "successful shard is incomplete"):
                    parallel_verify.merge_results(args)

    def test_merge_rejects_parent_directory_paths(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.write_artifact(root, "shard", ["../verify/a.test.cpp"])
            args = argparse.Namespace(artifacts=root, output=root / "merged.json")
            with mock.patch.object(
                parallel_verify,
                "verification_files",
                return_value=[Path("verify/a.test.cpp")],
            ):
                with self.assertRaisesRegex(ValueError, "invalid verification path"):
                    parallel_verify.merge_results(args)

    def test_docs_merge_keeps_failures_and_marks_missing_shards_uncompleted(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            timestamp = "2026-08-09 00:00:00 +0000"
            output = root / "merged.json"
            parallel_verify.write_json(
                output,
                {
                    "verify/a.test.cpp": timestamp,
                    "verify/c.test.cpp": timestamp,
                },
            )
            self.write_artifact(
                root / "artifacts",
                "shard-1",
                ["verify/a.test.cpp", "verify/b.test.cpp"],
                {
                    "verify/a.test.cpp": timestamp,
                    "verify/b.test.cpp": timestamp,
                },
                status="failed",
                failed=["verify/a.test.cpp"],
                uncompleted=[],
            )
            args = argparse.Namespace(
                artifacts=root / "artifacts",
                output=output,
                timeouts_output=root / "timeouts.json",
                failures_output=root / "failures.json",
                allow_failures=True,
                allow_missing=True,
            )
            paths = [
                Path("verify/a.test.cpp"),
                Path("verify/b.test.cpp"),
                Path("verify/c.test.cpp"),
            ]
            with mock.patch.object(
                parallel_verify, "verification_files", return_value=paths
            ):
                parallel_verify.merge_results(args)

            self.assertEqual(
                set(json.loads(output.read_text())),
                {"verify/a.test.cpp", "verify/b.test.cpp", "verify/c.test.cpp"},
            )
            self.assertEqual(
                json.loads(args.failures_output.read_text()),
                ["verify/a.test.cpp"],
            )
            self.assertEqual(
                json.loads(args.timeouts_output.read_text()),
                ["verify/c.test.cpp"],
            )

    def test_timestamp_merge_rejects_failed_shards(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.write_artifact(
                root,
                "shard",
                ["verify/a.test.cpp"],
                status="failed",
                failed=["verify/a.test.cpp"],
                uncompleted=[],
            )
            args = argparse.Namespace(artifacts=root, output=root / "merged.json")
            with mock.patch.object(
                parallel_verify,
                "verification_files",
                return_value=[Path("verify/a.test.cpp")],
            ):
                with self.assertRaisesRegex(
                    RuntimeError, "unsuccessful verification shard"
                ):
                    parallel_verify.merge_results(args)

    def test_docs_merge_accepts_interrupted_shard_after_all_targets_finished(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            timestamp = "2026-08-09 00:00:00 +0000"
            self.write_artifact(
                root / "artifacts",
                "shard",
                ["verify/a.test.cpp"],
                {"verify/a.test.cpp": timestamp},
                status="interrupted",
                uncompleted=[],
            )
            args = argparse.Namespace(
                artifacts=root / "artifacts",
                output=root / "merged.json",
                timeouts_output=root / "timeouts.json",
                failures_output=root / "failures.json",
                allow_failures=True,
                allow_missing=True,
            )
            with mock.patch.object(
                parallel_verify,
                "verification_files",
                return_value=[Path("verify/a.test.cpp")],
            ):
                parallel_verify.merge_results(args)

            self.assertEqual(json.loads(args.output.read_text()), {
                "verify/a.test.cpp": timestamp,
            })
            self.assertEqual(json.loads(args.timeouts_output.read_text()), [])
            self.assertEqual(json.loads(args.failures_output.read_text()), [])

    def test_run_shard_treats_unfinished_files_as_timeout(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            marker = root / ".verify-helper/timestamps.remote.json"
            old_timestamp = "2026-08-09 00:00:00 +0000"
            parallel_verify.write_json(marker, {"verify/a.test.cpp": old_timestamp})
            args = argparse.Namespace(
                directory=Path("verify"),
                shard=0,
                shard_count=1,
                jobs=2,
                timeout=2100,
                artifact_dir=root / "artifact",
            )
            paths = [Path("verify/a.test.cpp"), Path("verify/b.test.cpp")]
            current = datetime.datetime(2026, 8, 10, tzinfo=datetime.timezone.utc)
            summary = root / "summary.md"

            def finish_without_updating_timestamps(*args, **kwargs):
                parallel_verify.write_json(marker, {})
                return subprocess.CompletedProcess([], 0), []

            with mock.patch.object(parallel_verify, "REMOTE_MARKER", marker):
                with mock.patch.object(parallel_verify, "verification_files", return_value=paths):
                    with mock.patch.object(
                        parallel_verify,
                        "run_verify",
                        side_effect=finish_without_updating_timestamps,
                    ):
                        with mock.patch(
                            "onlinejudge_verify.marker.get_last_commit_time_to_verify",
                            return_value=current,
                        ):
                            with mock.patch.dict(
                                os.environ,
                                {"GITHUB_STEP_SUMMARY": str(summary)},
                                clear=False,
                            ):
                                parallel_verify.run_shard(args)

            result = json.loads((args.artifact_dir / "result.json").read_text())
            timestamps = json.loads(
                (args.artifact_dir / "timestamps.remote.json").read_text()
            )
            self.assertEqual(result["status"], "timed_out")
            self.assertEqual(
                result["uncompleted"],
                ["verify/a.test.cpp", "verify/b.test.cpp"],
            )
            self.assertEqual(result["failed"], [])
            self.assertEqual(timestamps, {"verify/a.test.cpp": old_timestamp})
            self.assertIn("Not completed", summary.read_text())

    def test_run_shard_keeps_real_verify_failures_fatal(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            marker = root / ".verify-helper/timestamps.remote.json"
            parallel_verify.write_json(marker, {})
            args = argparse.Namespace(
                directory=Path("verify"),
                shard=0,
                shard_count=1,
                jobs=2,
                timeout=2100,
                artifact_dir=root / "artifact",
            )
            paths = [Path("verify/a.test.cpp")]

            with mock.patch.object(parallel_verify, "REMOTE_MARKER", marker):
                with mock.patch.object(parallel_verify, "verification_files", return_value=paths):
                    with mock.patch.object(
                        parallel_verify,
                        "run_verify",
                        return_value=(
                            subprocess.CompletedProcess([], 1),
                            ["verify/a.test.cpp"],
                        ),
                    ):
                        with self.assertRaises(subprocess.CalledProcessError):
                            parallel_verify.run_shard(args)

            result = json.loads((args.artifact_dir / "result.json").read_text())
            self.assertEqual(result["status"], "failed")
            self.assertEqual(result["failed"], ["verify/a.test.cpp"])
            self.assertEqual(result["uncompleted"], [])

    def test_run_shard_marks_an_aborted_process_as_interrupted(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            marker = root / ".verify-helper/timestamps.remote.json"
            parallel_verify.write_json(marker, {})
            args = argparse.Namespace(
                directory=Path("verify"),
                shard=0,
                shard_count=1,
                jobs=2,
                timeout=2100,
                artifact_dir=root / "artifact",
            )
            paths = [Path("verify/a.test.cpp"), Path("verify/b.test.cpp")]
            current = datetime.datetime(2026, 8, 10, tzinfo=datetime.timezone.utc)

            with mock.patch.object(parallel_verify, "REMOTE_MARKER", marker):
                with mock.patch.object(
                    parallel_verify, "verification_files", return_value=paths
                ):
                    with mock.patch.object(
                        parallel_verify,
                        "run_verify",
                        return_value=(subprocess.CompletedProcess([], 130), []),
                    ):
                        with mock.patch(
                            "onlinejudge_verify.marker.get_last_commit_time_to_verify",
                            return_value=current,
                        ):
                            with self.assertRaises(subprocess.CalledProcessError):
                                parallel_verify.run_shard(args)

            result = json.loads((args.artifact_dir / "result.json").read_text())
            self.assertEqual(result["status"], "interrupted")
            self.assertEqual(result["failed"], [])
            self.assertEqual(
                result["uncompleted"],
                ["verify/a.test.cpp", "verify/b.test.cpp"],
            )

    def test_publish_passes_token_only_through_git_header(self):
        results = [
            subprocess.CompletedProcess([], 0),
            subprocess.CompletedProcess([], 1),
            subprocess.CompletedProcess([], 0),
            subprocess.CompletedProcess([], 0),
        ]
        args = argparse.Namespace(marker=Path(".verify-helper/timestamps.remote.json"))
        environment = {
            "GITHUB_ACTIONS": "true",
            "GITHUB_TOKEN": "secret-token",
            "GITHUB_REPOSITORY": "owner/repository",
            "GITHUB_REF": "refs/heads/main",
            "GITHUB_SHA": "0123456789abcdef",
        }
        with mock.patch.dict(os.environ, environment, clear=True):
            with mock.patch.object(subprocess, "run", side_effect=results) as run:
                parallel_verify.publish_result(args)

        push = run.call_args_list[-1]
        command = push.args[0]
        child_environment = push.kwargs["env"]
        self.assertNotIn("secret-token", " ".join(command))
        encoded = child_environment["GIT_CONFIG_VALUE_0"].removeprefix(
            "AUTHORIZATION: basic "
        )
        self.assertEqual(
            base64.b64decode(encoded).decode(),
            "x-access-token:secret-token",
        )

    def test_docs_preserves_literal_liquid_in_generated_bodies_only(self):
        import onlinejudge_verify.main

        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            destination = root / ".verify-helper/markdown"
            front_matter = "---\nlayout: document\ndata: '{{1, 2}'\n---\n"
            body = "```cpp\nvc<array<int, 2>>{{1, 2}, {3, 4}};\n```\n{% raw %}{{x}}{% endraw %}\n"
            layout = "<main>{{ content }}</main>\n"
            args = argparse.Namespace(jobs=1, timeouts=root / "timeouts.json", failures=root / "failures.json")

            def generate_pages(*, jobs):
                (destination / "algebra").mkdir(parents=True)
                (destination / "algebra/example.hpp.md").write_text(front_matter + body)
                (destination / "_layouts").mkdir()
                (destination / "_layouts/document.html").write_text(layout)
                (destination / "plain.md").write_text("ordinary Markdown\n")

            old_cwd = Path.cwd()
            try:
                os.chdir(root)
                with mock.patch.object(
                    onlinejudge_verify.main, "subcommand_docs", side_effect=generate_pages,
                ):
                    parallel_verify.publish_docs(args)
                rendered = (destination / "algebra/example.hpp.md").read_text()
                self.assertTrue(rendered.startswith(front_matter))
                self.assertIn("{{ '{{' }}1, 2}, {3, 4}}", rendered)
                self.assertIn("{{ '{%' }} raw %}", rendered)
                self.assertEqual((destination / "_layouts/document.html").read_text(), layout)
                self.assertEqual((destination / "plain.md").read_text(), "ordinary Markdown\n")
            finally:
                os.chdir(old_cwd)

    def test_docs_rejects_unterminated_front_matter(self):
        with self.assertRaisesRegex(ValueError, "unterminated front matter"):
            parallel_verify.escape_markdown_liquid("---\nlayout: document\n{{1, 2}\n")

    def test_docs_excludes_local_files_and_clears_stale_pages(self):
        import onlinejudge_verify.documentation.configure as configure
        import onlinejudge_verify.main

        config = tools_test_paths.REPOSITORY_ROOT / ".verify-helper/docs/_config.yml"
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            private = [
                "benchmark/private.cpp", "benchmark/private.md",
                "benchmarks/private.hpp", "benchmarks/private.md",
                "research/private.md", "AGENTS.md", "docs/STYLE.md",
                "docs/ds/flat_dvec_benchmark.md",
                "docs/math/fps/arithmetic_product_benchmark.md",
                "docs/math/fps/bostan_mori_benchmark.md",
                "docs/math/runtime_integer_width.md", "venv/private.md",
                "tools/README.md", "tools/private.cpp",
            ]
            public = ["tree/example.hpp", "verify/example.test.cpp", "docs/tree/example.md"]
            for name in private + public:
                path = root / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text("// fixture\n" if path.suffix != ".md" else "fixture\n")
            destination = root / ".verify-helper/markdown"
            (destination / "research").mkdir(parents=True)
            (destination / "research/private.md").write_text("previously published\n")
            user_config = root / ".verify-helper/docs/_config.yml"
            user_config.parent.mkdir(parents=True)
            shutil.copyfile(config, user_config)
            args = argparse.Namespace(jobs=1, timeouts=root / "timeouts.json", failures=root / "failures.json")
            original_find = configure._find_matched_file_paths

            def inspect_pages(*, jobs):
                self.assertFalse(destination.exists())
                sources = configure._find_source_code_paths(basedir=root)
                markdown = configure.find_markdown_paths(basedir=root)
                self.assertEqual({str(path.relative_to(root)) for path in sources + markdown}, set(public))
                raise RuntimeError("test restoration after a build failure")

            old_cwd = Path.cwd()
            try:
                os.chdir(root)
                with mock.patch.object(onlinejudge_verify.main, "subcommand_docs", side_effect=inspect_pages):
                    with self.assertRaisesRegex(RuntimeError, "test restoration"):
                        parallel_verify.publish_docs(args)
                self.assertIs(configure._find_matched_file_paths, original_find)
            finally:
                os.chdir(old_cwd)

    def test_docs_uses_explicit_failed_and_uncompleted_statuses(self):
        import onlinejudge_verify.documentation.build
        import onlinejudge_verify.documentation.type
        import onlinejudge_verify.main
        import onlinejudge_verify.marker

        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            verify = root / "verify"
            verify.mkdir()
            timed_out = verify / "timed-out.test.cpp"
            failed = verify / "failed.test.cpp"
            manual = verify / "atcoder/manual.test.cpp"
            timed_out.touch()
            failed.touch()
            manual.parent.mkdir()
            manual.touch()
            timeouts = root / "timeouts.json"
            failures = root / "failures.json"
            compat_build = root / "tools/ojverify_pkg_resources_compat/build/lib"
            compat_build.mkdir(parents=True)
            (compat_build / "pkg_resources.py").touch()
            parallel_verify.write_json(timeouts, ["verify/timed-out.test.cpp"])
            parallel_verify.write_json(failures, ["verify/failed.test.cpp"])
            args = argparse.Namespace(jobs=3, timeouts=timeouts, failures=failures)
            marker_class = onlinejudge_verify.marker.VerificationMarker
            def get_status_icon(status):
                return onlinejudge_verify.documentation.build._get_verification_status_icon(
                    status
                )

            waiting = (
                onlinejudge_verify.documentation.type.VerificationStatus.TEST_WAITING_JUDGE
            )

            def inspect_status(*, jobs):
                self.assertEqual(jobs, 3)
                self.assertFalse(compat_build.parent.exists())
                self.assertTrue((root / ".verify-helper/.gitignore").is_file())
                self.assertFalse(marker_class.is_verified(object(), timed_out))
                self.assertFalse(marker_class.is_failed(object(), timed_out))
                self.assertFalse(marker_class.is_verified(object(), failed))
                self.assertTrue(marker_class.is_failed(object(), failed))
                self.assertFalse(marker_class.is_verified(object(), manual))
                self.assertFalse(marker_class.is_failed(object(), manual))
                self.assertEqual(get_status_icon(waiting), ":question:")

            old_cwd = Path.cwd()
            try:
                os.chdir(root)
                with mock.patch.object(
                    marker_class, "is_verified", autospec=True, return_value=True
                ):
                    with mock.patch.object(
                        marker_class, "is_failed", autospec=True, return_value=True
                    ):
                        with mock.patch.object(
                            onlinejudge_verify.main,
                            "subcommand_docs",
                            side_effect=inspect_status,
                        ):
                            parallel_verify.publish_docs(args)
                        self.assertTrue(marker_class.is_verified(object(), failed))
                        self.assertTrue(marker_class.is_failed(object(), failed))
                        self.assertEqual(get_status_icon(waiting), ":warning:")
            finally:
                os.chdir(old_cwd)


if __name__ == "__main__":
    unittest.main()
