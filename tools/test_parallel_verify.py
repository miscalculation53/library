import argparse
import base64
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest import mock

import parallel_verify


class ParallelVerifyTest(unittest.TestCase):
    def write_artifact(self, root, name, targets):
        directory = root / name
        timestamps = {path: "2026-08-09 00:00:00 +0000" for path in targets}
        parallel_verify.write_json(directory / "targets.json", targets)
        parallel_verify.write_json(directory / "timestamps.remote.json", timestamps)

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


if __name__ == "__main__":
    unittest.main()
