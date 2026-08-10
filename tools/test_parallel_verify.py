import argparse
import base64
import datetime
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

import parallel_verify


class ParallelVerifyTest(unittest.TestCase):
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
            timed_out.touch()
            failed.touch()
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
                self.assertFalse(marker_class.is_verified(object(), timed_out))
                self.assertFalse(marker_class.is_failed(object(), timed_out))
                self.assertFalse(marker_class.is_verified(object(), failed))
                self.assertTrue(marker_class.is_failed(object(), failed))
                self.assertEqual(get_status_icon(waiting), ":question:")

            old_cwd = Path.cwd()
            try:
                os.chdir(root)
                with mock.patch.object(
                    marker_class, "is_verified", autospec=True, return_value=True
                ):
                    with mock.patch.object(
                        marker_class, "is_failed", autospec=True, return_value=False
                    ):
                        with mock.patch.object(
                            onlinejudge_verify.main,
                            "subcommand_docs",
                            side_effect=inspect_status,
                        ):
                            parallel_verify.publish_docs(args)
                        self.assertTrue(marker_class.is_verified(object(), failed))
                        self.assertFalse(marker_class.is_failed(object(), failed))
                        self.assertEqual(get_status_icon(waiting), ":warning:")
            finally:
                os.chdir(old_cwd)


if __name__ == "__main__":
    unittest.main()
