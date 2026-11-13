import io
from pathlib import Path
import tempfile
import unittest
from unittest.mock import Mock, patch

import tools_test_paths
import submit_code as api
from submission_layout import CheckpointLayout, compact_library_layout


class SubmissionLayoutTest(unittest.TestCase):
    def test_first_size_checkpoints_publish_before_optional_layout(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / 'bundle.cpp'
            writer = api.CheckpointWriter(output, api.Reporter(stream=io.StringIO()))
            writer.preserve_below = api.DEFAULT_BYTE_LIMIT
            writer.display_layout = Mock(return_value='unexpected optional layout\n')

            priority = ' ' * api.DEFAULT_BYTE_LIMIT + '#line 42\nint main() {}\n'
            self.assertTrue(writer.save('priority checkpoint', priority))
            self.assertEqual(output.read_text(), priority)
            self.assertEqual(writer.reached_limits, {api.PRIORITY_BYTE_LIMIT})
            writer.display_layout.assert_not_called()

            secondary = '#line 42\nint main() {}\n'
            self.assertTrue(writer.save('secondary checkpoint', secondary))
            self.assertEqual(output.read_text(), secondary)
            self.assertEqual(writer.reached_limits, {api.PRIORITY_BYTE_LIMIT, api.DEFAULT_BYTE_LIMIT})
            writer.display_layout.assert_not_called()

            tidied = 'int main() {}\n'
            writer.display_layout.return_value = tidied
            self.assertTrue(writer.save('later checkpoint', secondary))
            writer.display_layout.assert_called_once_with(secondary, False)
            self.assertEqual(output.read_text(), tidied)
            self.assertEqual(writer.analysis_current, secondary)

    def test_final_layout_can_reach_secondary_after_priority_is_published(self):
        token = 'a' * 32
        notice = '/*! License\n\n  Preserve this layout. */\n'
        statement = '    n +=  1;\n'
        library = 'int f() {\n    int n = 0;\n' + statement * 5500 + '    return n;\n}\n'
        user = 'int main() {  return f() == 5500 ? 0 : 1; }\n'
        original = (api.bundle_fast.marker(token, True, 'math') + '\n' + notice + library
                    + api.bundle_fast.marker(token, False, 'math') + '\n' + user)
        candidate = notice + library + user
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / 'bundle.cpp'
            stamp = api.SubmissionStamp('a' * 64, 'b' * 64)
            writer = api.CheckpointWriter(output, api.Reporter(stream=io.StringIO()), stamp=stamp)
            writer.preserve_below = api.DEFAULT_BYTE_LIMIT
            writer.layout_started = 20
            compiler = Mock()
            writer.display_layout = CheckpointLayout(api, original, token, writer, compiler, api.DEFAULT_BYTE_LIMIT)

            def verify_layout(before, after):
                self.assertEqual(api.SubmissionStamp.read(output.read_text()), (stamp, candidate))
                self.assertEqual(before, candidate)
                self.assertTrue(after.startswith(notice))
                self.assertTrue(after.endswith(user))
                return True

            compiler.same_submission_layout.side_effect = verify_layout
            with patch.object(api.time, 'monotonic', return_value=30):
                self.assertTrue(writer.save('priority checkpoint', candidate))
                compiler.same_submission_layout.assert_not_called()
                self.assertEqual(writer.reached_limits, {api.PRIORITY_BYTE_LIMIT})
                self.assertTrue(writer.save('final', candidate, force=True))

            compiler.same_submission_layout.assert_called_once()
            self.assertLess(output.stat().st_size, api.DEFAULT_BYTE_LIMIT)
            self.assertEqual(writer.reached_limits, {api.PRIORITY_BYTE_LIMIT, api.DEFAULT_BYTE_LIMIT})
            final_stamp, body = api.SubmissionStamp.read(output.read_text())
            self.assertEqual(final_stamp, stamp)
            self.assertEqual(body.count('  n += 1;\n'), 5500)

    def test_keeps_a_ready_checkpoint_until_final_layout_when_raw_size_exceeds_limit(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / 'bundle.cpp'
            writer = api.CheckpointWriter(output, api.Reporter(stream=io.StringIO()))
            writer.preserve_below = api.DEFAULT_BYTE_LIMIT
            old = 'int main() { return 0; }\n'
            raw = ' ' * api.DEFAULT_BYTE_LIMIT + 'int main() {}\n'
            formatted = 'int main() {}\n'
            writer.display_layout = Mock(return_value=formatted)

            self.assertTrue(writer.save('checkpoint', old))
            self.assertFalse(writer.save('intermediate', raw))
            writer.display_layout.assert_not_called()
            self.assertEqual(output.read_text(), old)
            self.assertTrue(writer.save('final', raw, force=True))
            writer.display_layout.assert_called_once_with(raw, True)
            self.assertEqual(output.read_text(), formatted)

    def test_delayed_compaction_uses_only_verified_library_rows(self):
        token = 'a' * 32
        library = '    int  local =  4;\n    int  shared =  1;\n'
        user = '  int shared = 1;\n'
        original = (api.bundle_fast.marker(token, True, 'math') + '\n' + library
                    + api.bundle_fast.marker(token, False, 'math') + '\n' + user)
        text = library + user
        writer = api.CheckpointWriter(None, api.Reporter())
        writer.layout_started = 20
        compiler = Mock()
        layout = CheckpointLayout(api, original, token, writer, compiler, len(text))
        with patch.object(api.time, 'monotonic', return_value=29.9):
            self.assertEqual(layout(text, True), text)
        compiler.same_submission_layout.assert_not_called()
        with patch.object(api.time, 'monotonic', return_value=30):
            formatted = '  int local = 4;\n    int  shared =  1;\n' + user
            self.assertEqual(layout(text, True), formatted)
        compiler.same_submission_layout.assert_called_once_with(text, formatted)

    def test_only_library_lines_are_compacted(self):
        source = (
            "    int  x =  12'345; \t\n"
            "      int  y =  x;\n"
            "  return  y;\n"
        )
        self.assertEqual(compact_library_layout(api, source, [True, True, False]), (
            "  int x = 12'345;\n"
            "   int y = x;\n"
            "  return  y;\n"
        ))
        self.assertEqual(compact_library_layout(api, source, [True]), source)

    def test_literals_keep_their_whitespace(self):
        source = '    auto  s = "a  \\"  b";\n    auto  c = \' \';\n'
        self.assertEqual(compact_library_layout(api, source, [True] * 2),
                         '  auto s = "a  \\"  b";\n  auto c = \' \';\n')
        raw = '    auto  r = R"fmt(a  b\n    x  y\n    )fmt";\n'
        self.assertEqual(compact_library_layout(api, raw, [True] * 3), raw)

    def test_directives_comments_and_spliced_lines_stay_intact(self):
        protected = (
            '    #define  FOO(x)  #x \\\r\n'
            '      "  text  "\r\n'
            '    %:line  321\r\n'
            '    /*! A  notice\r\n'
            '      further text */\r\n'
            '    int  x; //! important\r\n'
            '    auto  s = "a\\\r\n'
            '      b  c";\r\n'
        )
        source = protected + '    FOO(  a  b  );\r\n'
        self.assertEqual(compact_library_layout(api, source, [True] * 9),
                         protected + '  FOO( a b );\r\n')


if __name__ == "__main__":
    unittest.main()
