from __future__ import annotations

from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

import tools_test_paths

import submit_code


class CommentSafetyTest(unittest.TestCase):
    def assert_programs_equal(self, source: str, cleaned: str) -> None:
        cxx = shutil.which("g++-15") or shutil.which("g++-14")
        if cxx is None:
            self.skipTest("GNU C++ compiler is unavailable")
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            results = []
            for index, text in enumerate((source, cleaned)):
                path = root / f"program-{index}.cpp"
                path.write_text(text, encoding="utf-8", newline="")
                executable = root / f"program-{index}.out"
                compiled = subprocess.run(
                    [cxx, "-std=c++23", str(path), "-o", str(executable)],
                    capture_output=True, text=True,
                )
                self.assertEqual(compiled.returncode, 0, compiled.stderr)
                result = subprocess.run([str(executable)], capture_output=True)
                results.append((result.returncode, result.stdout, result.stderr))
            self.assertEqual(results[0], results[1])
            self.assertEqual(results[0][0], 0)

    def test_line_comments_respect_crlf_and_gcc_continuations(self) -> None:
        for splice in ("\\\n", "\\\r\n", "\\ \n", "\\\t\r\n"):
            with self.subTest(splice=repr(splice)):
                source = (
                    "int main() { int value = 0;\n// comment " + splice
                    + "value = 1;\nreturn value; }\n"
                )
                cleaned = submit_code.strip_comments(source)
                self.assertNotIn("value = 1;", cleaned)
                self.assert_programs_equal(source, cleaned)

    def test_split_comment_openers_and_closers_use_original_offsets(self) -> None:
        block = "/\\\n* comment *\\\n/"
        line = "/\\\r\n/ comment"
        source = f"int {block} main() {{\n{line}\nreturn 0; }}\n"
        spans = list(submit_code.comment_spans(source))
        self.assertEqual([source[start:end] for start, end in spans], [block, line])
        self.assert_programs_equal(source, submit_code.strip_comments(source))

    def test_block_comment_at_end_does_not_consume_later_splices(self) -> None:
        comment = "/*! license */"
        source = comment + "\\\n"
        self.assertEqual(list(submit_code.comment_spans(source)), [(0, len(comment))])
        self.assertEqual(submit_code.important_comments(source), [comment])

    def test_multiline_comments_inside_defines_preserve_macro_semantics(self) -> None:
        source = "#define VALUE 1 /* first\nsecond */ + 2\nint main() { return VALUE - 3; }\n"
        cleaned = submit_code.strip_comments(source)
        self.assertNotIn("/*", cleaned)
        self.assertIn("\\\n", cleaned)
        self.assert_programs_equal(source, cleaned)

    def test_comment_contents_in_a_define_do_not_become_directives(self) -> None:
        source = '#define VALUE /* first\n#include "missing.hpp"\nlast */ 1\nint main() { return VALUE - 1; }\n'
        visible = submit_code.preprocessor_lines(source)
        self.assertFalse(any('missing.hpp' in line for line in visible))
        self.assert_programs_equal(source, submit_code.strip_comments(source))

    def test_directive_view_recognizes_includes_and_pragmas_with_multiline_comments(self) -> None:
        source = '#include "dep.hpp" /* first\nlast */\n#pragma once /* first\nlast */\n'
        visible = submit_code.preprocessor_lines(source)
        self.assertEqual(len(visible), len(source.splitlines()))
        self.assertIsNotNone(submit_code.LOCAL_INCLUDE_DIRECTIVE.match(visible[0]))
        self.assertIsNotNone(submit_code.PRAGMA_ONCE_DIRECTIVE.match(visible[2]))
        self.assertEqual(visible[1].strip(), "")
        self.assertEqual(visible[3].strip(), "")

    def test_raw_literal_newlines_do_not_end_a_define_before_a_comment(self) -> None:
        source = '#define VALUE (sizeof(R"(a\nb)") /* first\nlast */ - 4)\nint main() { return VALUE; }\n'
        self.assert_programs_equal(source, submit_code.strip_comments(source))

    def test_raw_strings_revert_splicing_before_comment_detection(self) -> None:
        raw = 'R"tag(first )tag\\\n" // inside literal\n/* inside literal */\n)tag"'
        source = f"const char *raw = {raw}; // removed\nint main() {{ return raw[0] != 'f'; }}\n"
        cleaned = submit_code.strip_comments(source)
        self.assertIn(raw, cleaned)
        self.assertNotIn("// removed", cleaned)
        self.assertEqual(submit_code.important_comments(source), [])
        self.assert_programs_equal(source, cleaned)

    def test_source_link_protection_preserves_original_comment_lines(self) -> None:
        link = "  // https://github.com/miscalculation53/library/tree/wip/template/template_all.hpp\r\n"
        source = "int first;\n" + link + "int second;\n"
        protected, links = submit_code.protect_source_links(source)
        self.assertEqual(links, [link])
        self.assertEqual(submit_code.restore_source_links(protected, links), source)
        cleaned = submit_code.strip_cleanup_comments(source)
        self.assertTrue(cleaned.startswith(submit_code.source_link_comment(submit_code.LIBRARY_REPOSITORY_URL)))
        self.assertNotIn("tree/wip/template/template_all.hpp", cleaned)
        self.assertIn("#line 1\nint first;\n\nint second;", cleaned)

    def test_link_compaction_preserves_line_observations_and_licenses(self) -> None:
        source = '''#include <cstdio>
// https://github.com/miscalculation53/library/tree/wip/a.hpp
/*! Required license */
int first=__LINE__;
// https://github.com/miscalculation53/library/tree/wip/b.hpp
int main(){printf("%d %d",first,__LINE__);}
'''
        cleaned = submit_code.compact_source_links(source)
        self.assertEqual(cleaned.count(submit_code.LIBRARY_REPOSITORY_URL), 1)
        self.assertIn('/*! Required license */', cleaned)
        self.assertEqual(submit_code.compact_source_links(cleaned), cleaned)
        self.assert_programs_equal(source, cleaned)

    def test_source_links_and_pragmas_inside_raw_strings_stay_literal(self) -> None:
        source = '''const char *raw = R"tag(
  // https://github.com/miscalculation53/library/tree/wip/template/template_all.hpp
#pragma submit_code_source_link 0
)tag";
'''
        self.assertEqual(submit_code.protect_source_links(source), (source, []))
        self.assertEqual(submit_code.restore_source_links(source, []), source)
        self.assertEqual(submit_code.strip_cleanup_comments(source), source)

    def test_source_link_comment_in_a_continued_macro_is_not_a_directive(self) -> None:
        source = (
            '#define VALUE 1 \\\n'
            ' // https://github.com/miscalculation53/library/tree/wip/template/template_all.hpp\n'
            'int main() { return VALUE - 1; }\n'
        )
        self.assertEqual(submit_code.protect_source_links(source), (source, []))
        self.assert_programs_equal(source, submit_code.strip_cleanup_comments(source))

    def test_spliced_important_and_source_comments_are_preserved_verbatim(self) -> None:
        notice = "/\\\n*! Copyright Example\n * License text\n *\\\n/"
        source_comment = "/\\\n/ solution comment"
        source = notice + "\n" + source_comment + "\nint main() {}\n"
        self.assertEqual(submit_code.important_comments(source), [notice])
        self.assertEqual(
            submit_code.strip_cleanup_comments(source, submit_code.source_comments(source)),
            source,
        )

    def test_scoped_notices_cover_pasted_and_spliced_identifiers(self) -> None:
        notice = '/*! @keep-if-code generated_name\n * Full permission and disclaimer.\n */'
        sources = [
            '#define JOIN(a,b) a##b\nint JOIN(generated_,name);\n',
            '#define JOIN(a,b) a%:%:b\nint JOIN(generated_,name);\n',
            '#define JOIN(a,b) a #\\ \r\n# b\nint JOIN(generated_,name);\n',
            'int generated_\\ \r\nname;\n',
        ]
        for source in sources:
            with self.subTest(source=source):
                original = notice + '\n' + source
                self.assertEqual(submit_code.retain_important_comments(original, [notice]), original)


if __name__ == "__main__":
    unittest.main()
