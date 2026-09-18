from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import time
import unittest
import json


FAKE_TOOL = '''\
from pathlib import Path
import os
import sys
import time

def comment_record_path(output):
    return output.parent / ".submit-code" / output.name / "comments.json"

def atomic_write(path, text):
    path = Path(path)
    temporary = path.with_name(path.name + ".tmp")
    temporary.write_text(text)
    os.replace(temporary, path)

if __name__ == "__main__":
    command, source = sys.argv[1:3]
    text = Path(source).read_text()
    if "FAIL_EXPAND" in text:
        sys.exit(1)
    if command == "expand":
        dependency = Path(source).with_name("dependency.hpp")
        if dependency.exists():
            text += dependency.read_text()
        atomic_write(Path(sys.argv[sys.argv.index("-o") + 1]), text)
    else:
        if "FAIL_CLEANUP" in text:
            sys.exit(1)
        atomic_write(Path(source), text + " CHECKPOINT")
        time.sleep(0.5 if "FAST" in text else 30)
        atomic_write(Path(source), text + " FINAL")
'''


class BackgroundTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="background tests ")
        self.root = Path(self.temporary.name)
        self.helper = self.root / "submit_code_background.py"
        shutil.copyfile(Path(__file__).with_name(self.helper.name), self.helper)
        (self.root / "submit_code.py").write_text(FAKE_TOOL)
        self.source = self.root / "main.cpp"
        self.source.write_text("OLD")
        self.output = self.root / "bundle.cpp"
        self.directory = self.root / ".submit-code" / "bundle.cpp"

    def call(self, command, source=None):
        return subprocess.run(
            [sys.executable, str(self.helper), command, str(source or self.source)],
            capture_output=True, text=True, check=True, timeout=3,
        )

    def wait_for(self, predicate, timeout=5):
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline:
            if predicate():
                return
            time.sleep(0.03)
        self.fail("background condition timed out")

    def state(self):
        path = self.directory / "status.json"
        return json.loads(path.read_text()) if path.exists() else {}

    def phase(self, value):
        self.wait_for(lambda: self.state().get("phase") == value)

    def content(self):
        return self.output.read_text() if self.output.exists() else ""

    def tearDown(self):
        # Let workers release their locks before removing their directories.
        import fcntl
        for job in (self.root / ".submit-code").glob("*"):
            request = json.loads((job / "request.json").read_text())
            self.call("stop", Path(request["source"]))
            lock_path = job / "running.lock"
            if lock_path.exists():
                with lock_path.open("a") as lock:
                    def released():
                        try:
                            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
                            return True
                        except BlockingIOError:
                            return False
                    self.wait_for(released)
        self.temporary.cleanup()

    def test_launcher_returns_while_cleanup_runs_and_publishes_checkpoint(self):
        before = time.monotonic()
        self.call("start")
        self.assertLess(time.monotonic() - before, 2)
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT")
        self.assertEqual(self.state()["phase"], "cleaning")

    def test_new_request_cancels_old_work_and_publishes_latest(self):
        self.call("start")
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT")
        self.source.write_text("NEW FAST")
        self.call("start")
        self.phase("done")
        self.assertEqual(self.content(), "NEW FAST FINAL")
        self.assertTrue(self.state()["below_limit"])

    def test_unchanged_completed_expansion_is_reused(self):
        self.source.write_text("FAST")
        self.call("start")
        self.phase("done")
        before = self.output.stat()
        self.call("start")
        self.phase("done")
        self.assertTrue(self.state()["reused"])
        self.assertEqual(self.output.stat(), before)

    def test_header_change_invalidates_completed_result(self):
        self.source.write_text("FAST")
        dependency = self.root / "dependency.hpp"
        dependency.write_text(" HEADER 1")
        self.call("start")
        self.phase("done")
        dependency.write_text(" HEADER 2")
        self.call("start")
        self.phase("done")
        self.assertEqual(self.content(), "FAST HEADER 2 FINAL")
        self.assertFalse(self.state().get("reused", False))

    def test_unchanged_interrupted_expansion_resumes_checkpoint(self):
        self.call("start")
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT")
        self.call("start")
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT CHECKPOINT")
        self.assertEqual(self.state()["phase"], "cleaning")

    def test_rapid_requests_do_not_lose_last_request(self):
        for index in range(8):
            self.source.write_text(f"FAST {index}")
            self.call("start")
        self.phase("done")
        self.assertEqual(self.content(), "FAST 7 FINAL")

    def test_failed_expansion_preserves_existing_output(self):
        self.source.write_text("FAIL_EXPAND")
        self.output.write_text("PREVIOUS")
        self.call("start")
        self.phase("failed")
        self.assertEqual(self.content(), "PREVIOUS")

    def test_failed_cleanup_does_not_publish_unchecked_expansion(self):
        self.source.write_text("FAIL_CLEANUP")
        self.output.write_text("PREVIOUS")
        self.call("start")
        self.phase("failed")
        self.assertEqual(self.content(), "PREVIOUS")

    def test_source_edit_without_run_cancels_stale_work(self):
        self.call("start")
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT")
        self.source.write_text("UNRUN EDIT")
        self.phase("failed")
        self.assertEqual(self.content(), "OLD CHECKPOINT")
        self.assertIn("元コードは生成開始時から変更", self.call("status").stdout)

    def test_manual_output_edit_is_preserved(self):
        self.call("start")
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT")
        self.output.write_text("MANUAL")
        self.phase("failed")
        self.assertEqual(self.content(), "MANUAL")

    def test_stop_retains_checkpoint(self):
        self.call("start")
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT")
        self.call("stop")
        self.phase("stopped")
        self.assertEqual(self.content(), "OLD CHECKPOINT")

    def test_output_file_and_selection_are_not_bundled(self):
        for name in ("bundle.cpp", "answer-bundle.cpp", "tempCodeRunnerFile.cpp"):
            path = self.root / name
            path.write_text("FAST")
            self.call("start", path)
        self.assertFalse((self.root / ".submit-code").exists())

    def test_other_source_name_gets_separate_output(self):
        source = self.root / "answer with spaces.cpp"
        source.write_text("FAST")
        self.call("start", source)
        output = self.root / "answer with spaces-bundle.cpp"
        self.wait_for(lambda: output.exists() and output.read_text() == "FAST FINAL")
        self.assertFalse(self.output.exists())


if __name__ == "__main__":
    unittest.main()
