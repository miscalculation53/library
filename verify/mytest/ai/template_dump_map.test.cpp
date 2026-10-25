#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#ifdef LOCAL
template <class T>
void check_map(const T &value)
{
  for (auto style : {cp::types::cont_indent_style_t::minimal,
                     cp::types::cont_indent_style_t::when_nested,
                     cp::types::cont_indent_style_t::when_non_tuples_nested,
                     cp::types::cont_indent_style_t::always})
    for (size_t width : {20, 160})
    {
      cp::options::cont_indent_style = style;
      cp::options::max_line_width = width;
      // 色を無効にしたときは、cpp-dump 元来の括弧・改行・省略を維持する。
      const auto &command = cp::_detail::export_command::default_command;
      const auto expected = cp::_detail::export_map(value, "", 0, 0, false, command);
      assert(cp::export_var(value) == expected);
      const auto front = cp::front(1);
      assert(cp::export_var(value | front)
             == cp::_detail::export_map(value, "", 0, 0, false, front));
    }
}

void test_map_dump()
{
  cp::options::es_style = cp::types::es_style_t::no_es;
  check_map(map<int, int>{});
  check_map(map<int, int>{{1, 2}, {3, 4}, {5, 6}});
  check_map(map<string, vector<int>>{{"key", {1, 2}}, {"long key", {3, 4}}});
  check_map(map<int, map<string, int>>{{1, {{"nested", 2}}}});
  check_map(multimap<int, string>{{1, "a"}, {1, "b"}, {2, "c"}});
  check_map(unordered_map<int, int>{{1, 2}, {3, 4}});
  check_map(unordered_multimap<int, int>{{1, 2}, {1, 3}, {2, 4}});
  map<int, unique_ptr<int>> move_only;
  move_only.emplace(1, make_unique<int>(2));
  check_map(move_only);

  cp::options::cont_indent_style = cp::types::cont_indent_style_t::minimal;
  cp::options::max_line_width = 160;
  cp::options::es_style = cp::types::es_style_t::original;
  const map<int, int> m{{17, 29}};
  auto text = cp::export_var(m);
  assert(text.find("\x1b[36m17\x1b[0m") != string::npos);
  assert(text.find("\x1b[36m29") == string::npos);
  const vector<map<int, int>> nested{m};
  assert(cp::export_var(nested).find("\x1b[36m17\x1b[0m") != string::npos);
  const map<int, map<int, int>> maps{{3, m}};
  text = cp::export_var(maps);
  assert(text.find("\x1b[36m3\x1b[0m") != string::npos);
  assert(text.find("\x1b[36m17\x1b[0m") != string::npos);

  cp::options::map_key_color = "\x1b[35m";
  assert(cp::export_var(m).find("\x1b[35m17\x1b[0m") != string::npos);
  cp::options::map_key_color.clear();
  assert(cp::export_var(m).find("\x1b[35m") == string::npos);
  cp::options::es_style = cp::types::es_style_t::no_es;
  assert(cp::export_var(m) == "{ 17: 29 }");
}
#endif

int main()
{
#ifdef LOCAL
  test_map_dump();
#endif
  cout << "Hello World" << endl;
}
