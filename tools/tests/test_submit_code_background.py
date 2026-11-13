from pathlib import Path
import hashlib
import io
import shutil
import subprocess
import sys
import tempfile
import time
import unittest
import json
from unittest import mock

import tools_test_paths


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
        if "WAIT_CLEANUP" in text:
            while not Path(source).with_name("continue-cleanup").exists():
                time.sleep(0.03)
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
        shutil.copyfile(tools_test_paths.TOOLS_DIR / self.helper.name, self.helper)
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

    def test_priority_milestone_is_reported_before_the_secondary_milestone(self):
        self.source.write_text('WAIT_CLEANUP FAST ' + 'x' * 70000)
        self.call('start')
        self.phase('cleaning')
        self.assertTrue(self.state()['below_priority_limit'])
        self.assertFalse(self.state()['below_limit'])
        report = self.call('status').stdout
        self.assertIn('131,072 B 未満', report)
        self.assertIn('65,536 B 以上', report)

    def test_expansion_replaces_previous_output_before_first_cleanup_checkpoint(self):
        self.source.write_text("NEW WAIT_CLEANUP FAST")
        self.output.write_text("PREVIOUS")
        self.call("start")
        self.phase("cleaning")
        self.assertEqual(self.content(), self.source.read_text())
        self.assertEqual(self.state()["bytes"], len(self.output.read_bytes()))
        (self.directory / "continue-cleanup").touch()
        self.phase("done")
        self.assertEqual(self.content(), "NEW WAIT_CLEANUP FAST FINAL")

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

    def test_failed_cleanup_retains_latest_expansion(self):
        self.source.write_text("FAIL_CLEANUP")
        self.output.write_text("PREVIOUS")
        self.call("start")
        self.phase("failed")
        self.assertEqual(self.content(), "FAIL_CLEANUP")

    def test_oversized_result_is_failed_and_never_reused_as_complete(self):
        self.source.write_text("FAST " + "x" * 131072)
        self.call("start")
        self.phase("failed")
        self.assertGreaterEqual(len(self.content().encode()), 131072)
        artifact = self.directory / "artifact.json"
        record = json.loads(artifact.read_text())
        self.assertFalse(record["complete"])
        record["complete"] = True
        artifact.write_text(json.dumps(record))
        self.call("start")
        self.phase("failed")
        self.assertNotIn("reused", self.state())
        self.assertTrue(self.content().endswith("FINAL FINAL"))

    def test_completed_result_under_priority_limit_is_available_and_reused(self):
        self.source.write_text('FAST ' + 'x' * 70000)
        self.call('start')
        self.phase('done')
        self.assertTrue(self.state()['below_priority_limit'])
        self.assertFalse(self.state()['below_limit'])
        before = self.output.read_bytes()
        self.call('start')
        self.phase('done')
        self.assertTrue(self.state()['reused'])
        self.assertEqual(self.output.read_bytes(), before)

    def test_feature_helper_changes_invalidate_the_completed_result(self):
        self.source.write_text("FAST")
        self.call("start")
        self.phase("done")
        (self.root / "bundle_fast.py").write_text("# changed helper\n")
        self.call("start")
        self.phase("done")
        self.assertNotIn("reused", self.state())

    def test_source_edit_without_run_cancels_stale_work(self):
        self.call("start")
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT")
        self.source.write_text("UNRUN EDIT")
        self.phase("interrupted")
        self.assertEqual(self.state()["reason"], "source_changed")
        self.assertNotIn("error", self.state())
        self.assertEqual(self.content(), "OLD CHECKPOINT")
        report = self.call("status").stdout
        self.assertIn("編集により中断", report)
        self.assertIn("元コードは生成開始時から変更", report)
        self.source.write_text("NEW FAST")
        self.call("start")
        self.phase("done")
        self.assertEqual(self.content(), "NEW FAST FINAL")

    def test_manual_output_edit_is_preserved(self):
        self.call("start")
        self.wait_for(lambda: self.content() == "OLD CHECKPOINT")
        self.output.write_text("MANUAL")
        self.phase("interrupted")
        self.assertEqual(self.state()["reason"], "output_changed")
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

    def test_real_tool_publishes_verifiable_stamps_and_reuses_completed_output(self):
        if not (shutil.which('g++-15') or shutil.which('g++-14')) or not shutil.which('cppcheck'):
            self.skipTest('GNU C++ compiler or cppcheck is unavailable')
        from submit_code import SubmissionStamp, comment_record_path
        tool = self.root / 'submit_code.py'
        shutil.copyfile(tools_test_paths.TOOLS_DIR / 'submit_code.py', tool)
        for name in ('bundle_fast.py', 'submission_layout.py'):
            shutil.copyfile(tools_test_paths.TOOLS_DIR / name, self.root / name)
        dependency = self.root / 'dependency.hpp'
        dependency.write_text('#pragma once\n/*! Header license. */\nint value = 0;\n')
        self.source.write_text('#include "dependency.hpp"\nint main() { return value; }\n')

        def completed():
            self.wait_for(lambda: self.state().get('phase') in ('done', 'failed'), timeout=15)
            self.assertEqual(self.state()['phase'], 'done', (self.directory / 'log.txt').read_text())

        def view():
            module = tools_test_paths.TOOLS_DIR / 'vscode-submit-status/status.js'
            script = "const s = require(process.argv[1]); s.snapshot(s.paths(process.argv[2])).then(d => console.log(s.present(d).text));"
            return subprocess.run(['node', '-e', script, str(module), str(self.source.resolve())],
                                  capture_output=True, text=True, check=True, timeout=3).stdout

        self.call('start')
        completed()
        stamp, _ = SubmissionStamp.read(self.content())
        record = json.loads(comment_record_path(self.output).read_text())
        self.assertEqual(record['input_digest'], stamp.input_digest)
        if shutil.which('node'):
            self.assertIn('main.cpp と一致', view())
        before = self.output.stat()
        self.call('start')
        completed()
        self.assertTrue(self.state()['reused'])
        self.assertEqual(self.output.stat(), before)
        dependency.write_text(dependency.read_text().replace('value = 0', 'value = 1'))
        if shutil.which('node'):
            self.assertIn('ヘッダに変更あり', view())
        self.call('start')
        completed()
        self.assertNotEqual(SubmissionStamp.read(self.content())[0].input_digest, stamp.input_digest)
        if shutil.which('node'):
            self.assertIn('main.cpp と一致', view())


class ReuseRecordTests(unittest.TestCase):
    def setUp(self):
        import submit_code
        import submit_code_background as background
        self.api = submit_code
        self.background = background
        self.temporary = tempfile.TemporaryDirectory(prefix='reuse records ')
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve()
        self.source = self.root / 'main.cpp'
        self.source.write_text('int main() { return 0; }\n')
        self.output, self.directory = background.paths(self.source)
        self.directory.mkdir(parents=True)
        self.expansions = 0
        self.cleanups = 0
        self.interrupt = False
        self.inputs = {str(self.source): background.digest(self.source)}

    def run_tool(self, arguments, *, checkpoint=None):
        if arguments[0] == 'expand':
            self.expansions += 1
            scratch = Path(arguments[arguments.index('-o') + 1])
            token = f'{self.expansions:032x}'
            body = (self.api.bundle_fast.marker(token, True, 'core_types') + '\n'
                    + self.source.read_text()
                    + self.api.bundle_fast.marker(token, False, 'core_types') + '\n')
            stamp = self.api.SubmissionStamp(self.inputs[str(self.source)],
                                            hashlib.sha256(body.encode()).hexdigest())
            self.api.save_source_comments(scratch, set(), provenance={
                'source': str(self.source), 'inputs': dict(self.inputs),
                'input_digest': stamp.input_digest, 'feature_token': token,
            })
            self.api.atomic_write(scratch, stamp.render(body))
        else:
            self.cleanups += 1
            scratch = Path(arguments[1])
            stamp, body = self.api.SubmissionStamp.read(scratch.read_text())
            record = self.background.read_json(self.api.comment_record_path(scratch))
            self.assertEqual(stamp.input_digest, record['input_digest'])
            if self.interrupt:
                raise self.background.GenerationInterrupted('test', 'interrupted')
            if 'submit-code-feature:' in body:
                self.assertIn(self.api.bundle_fast.marker(record['feature_token'], True), body)
            self.api.atomic_write(scratch, stamp.render(self.source.read_text()))

    def execute(self):
        request = {'token': f'job-{self.expansions}', 'source': str(self.source),
                   'source_digest': self.background.digest(self.source),
                   'output_digest': self.background.digest(self.output)}
        self.background.write_json(self.directory / 'request.json', request)

        def fingerprint(scratch):
            _, body = self.api.SubmissionStamp.read(scratch.read_text())
            record = self.background.read_json(self.api.comment_record_path(scratch))
            return self.api.bundle_fast.feature_lines(self.api, body, record['feature_token'])[0].encode()

        with mock.patch.object(self.api, 'expansion_reuse_fingerprint', side_effect=fingerprint), \
                mock.patch.object(self.background.Job, 'run', side_effect=self.run_tool):
            self.background.Job(self.directory, request, io.StringIO()).execute()

    def assert_record_matches(self):
        stamp, _ = self.api.SubmissionStamp.read(self.output.read_text())
        record = self.background.read_json(self.api.comment_record_path(self.output))
        self.assertEqual(record['input_digest'], stamp.input_digest)
        self.assertEqual(record['inputs'], self.inputs)

    def test_resume_preserves_checkpoint_stamp_and_feature_token_together(self):
        self.interrupt = True
        with self.assertRaises(self.background.GenerationInterrupted):
            self.execute()
        self.assert_record_matches()
        first = self.api.comment_record_path(self.output).read_text()
        self.interrupt = False
        self.execute()
        self.assertEqual(self.expansions, 2)
        self.assertEqual(self.cleanups, 2)
        self.assert_record_matches()
        self.assertEqual(self.api.comment_record_path(self.output).read_text(), first)
        before = self.output.stat()
        self.execute()
        self.assertEqual(self.cleanups, 2)
        self.assertEqual(self.output.stat(), before)
        self.assert_record_matches()

    def test_completed_output_with_mismatched_record_is_regenerated(self):
        self.execute()
        path = self.api.comment_record_path(self.output)
        record = self.background.read_json(path)
        record['input_digest'] = '0' * 64
        self.background.write_json(path, record)
        self.execute()
        self.assertEqual(self.cleanups, 2)
        self.assert_record_matches()

    def test_identical_expansion_with_changed_input_records_is_regenerated(self):
        self.execute()
        self.inputs[str(self.root / 'dependency.hpp')] = '1' * 64
        self.execute()
        self.assertEqual(self.cleanups, 2)
        self.assert_record_matches()


if __name__ == "__main__":
    unittest.main()
