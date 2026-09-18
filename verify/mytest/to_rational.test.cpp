#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/modint/modint.hpp"
#include "math/modint/to_rational.hpp"

void test1()
{
  using mint = modint998244353;
  rep(p, -100, 101) rep(q, -100, 101)
  {
    if (q == 0)
      continue;
    mint r = mint(p) / mint(q);
    auto [pp, qq] = mint_to_rat(r);
    assert(p * qq == pp * q);
  }
}

#if defined(LOCAL) && __cplusplus >= 202302L
struct val_only
{
  val_only(int) {}
  int val() const { return 7; }
  friend ostream &operator<<(ostream &os, const val_only &x) { return os << x.val(); }
};

template <class mint>
void test_rat_format()
{
  assert(cp::export_var(mint(1) / mint(2) | cp::rat()) == "1/2");
  assert(cp::export_var(-mint(2) / mint(3) | cp::rat()) == "-2/3");
  assert(cp::export_var(mint(0) | cp::rat()) == "0");
  assert(cp::export_var(mint(3) | cp::rat()) == "3");

  const vector<mint> a{mint(1) / mint(2), -mint(2) / mint(3)};
  assert(cp::export_var(a | cp::rat()) == "[ 1/2, -2/3 ]");
  assert(cp::export_var(a | cp::index() | cp::rat()) == "[ 0: 1/2, 1: -2/3 ]");
  assert(cp::export_var(a | cp::front(1) | cp::rat() | cp::index()) == "[ 0: 1/2, ... ]");

  const auto indexed = a | cp::index();
  auto saved = indexed | cp::rat();
  assert(cp::export_var(saved) == "[ 0: 1/2, 1: -2/3 ]");
  const auto front = cp::front(1);
  assert(cp::export_var(saved | front) == "[ 0: 1/2, ... ]");
  assert(cp::export_var(saved | cp::front(1)) == "[ 0: 1/2, ... ]");
  const vector<vector<mint>> b{a, {}, {mint(3)}};
  auto formatted = b | cp::rat();
  assert(cp::export_var(formatted) == "[ [ 1/2, -2/3 ], [ ], [ 3 ] ]");
  auto owned = vector<vector<mint>>{a} | cp::rat();
  assert(cp::export_var(owned) == "[ [ 1/2, -2/3 ] ]");

  auto first_rat = std::views::take(1) | cp::rat();
  assert(cp::export_var(a | first_rat) == "[ 1/2 ]");
  const auto rat = cp::rat();
  auto first_rat_const = std::views::take(1) | rat;
  assert(cp::export_var(a | first_rat_const) == "[ 1/2 ]");
  auto rat_first = cp::rat() | std::views::take(1);
  assert(cp::export_var(a | rat_first) == "[ 1/2 ]");

  const mint half = mint(1) / mint(2);
  const map<int, mint> m{{1, half}, {2, -half}};
  assert(cp::export_var(m | cp::rat()) == "{ 1: 1/2, 2: -1/2 }");
  assert(cp::export_var(m | cp::rat() | cp::front(1)) == "{ 1: 1/2, ... }");
  const unordered_map<string, mint> um{{"half", half}};
  assert(cp::export_var(um | cp::rat()) == "{ \"half\": 1/2 }");
  const multimap<int, mint> mm{{1, half}, {1, -half}, {2, half}};
  assert(cp::export_var(mm | cp::rat()) == "{ 1 (2): [ 1/2, -1/2 ], 2 (1): [ 1/2 ] }");
  const unordered_multimap<int, mint> umm{{1, half}, {1, half}};
  assert(cp::export_var(umm | cp::rat()) == "{ 1 (2): [ 1/2, 1/2 ] }");

  const map<mint, int, modint_less<mint>> key_mint{{half, 3}};
  assert(cp::export_var(key_mint | cp::rat()) == "{ 1/2: 3 }");
  const map<mint, mint, modint_less<mint>> both_mint{{half, -half}};
  assert(cp::export_var(both_mint | cp::rat()) == "{ 1/2: -1/2 }");
  const map<int, pair<string, vector<mint>>> mixed{{7, {"a", {half}}}};
  assert(cp::export_var(mixed | cp::rat()) == "{ 7: ( \"a\", [ 1/2 ] ) }");
  const vector<map<int, mint>> nested{m};
  assert(cp::export_var(nested | cp::rat()) == "[ { 1: 1/2, 2: -1/2 } ]");
  const map<int, map<int, mint>> maps{{7, m}};
  assert(cp::export_var(maps | cp::rat()) == "{ 7: { 1: 1/2, 2: -1/2 } }");

  auto check_order = [](const auto &value, const auto &command)
  {
    assert(cp::export_var(value | command | cp::rat())
           == cp::export_var(value | cp::rat() | command));
  };
  const vector<mint> c{half, -half, 0, 3, -mint(2) / mint(3)};
  for (const auto &command : {cp::index(), cp::front(1), cp::back(1), cp::both_ends(1),
                              cp::middle(1), cp::bin(4), cp::hex(2)})
  {
    check_order(c, command);
    check_order(b, command);
    check_order(m, command);
    check_order(mm, command);
    check_order(um, command);
    check_order(umm, command);
    check_order(half, command);
    check_order(mixed, command);
  }
  const auto map_command = cp::front(1) | cp::map_kv(cp::bin(4), cp::front(1) | cp::index());
  check_order(mixed, map_command);
  check_order(maps, map_command);
  const vector<vector<mint>> matrix{c, c, c};
  const auto expected = cp::export_var(matrix | cp::rat() | cp::front(2) | cp::back(1) | cp::index());
  assert(cp::export_var(matrix | cp::front(2) | cp::rat() | cp::back(1) | cp::index()) == expected);
  assert(cp::export_var(matrix | cp::front(2) | cp::back(1) | cp::index() | cp::rat()) == expected);
  const auto mixed_tuple = tuple{5, half, string("abc"), true};
  check_order(mixed_tuple, cp::bin(4) | cp::stresc() | cp::boolnum());
  assert(cp::export_var(mixed_tuple | cp::bin(4) | cp::rat())
         == "(  0b0101, 1/2, \"abc\", true )");
  const vector<int> integers{1, 2, 3};
  assert(cp::export_var(integers | cp::index() | cp::bin(4))
         == cp::export_var(integers | cp::bin(4) | cp::index()));

  const auto wrapped_members = tuple{a | cp::index(), m | cp::front(1)};
  assert(cp::export_var(wrapped_members | cp::rat())
         == "( [ 0: 1/2, 1: -2/3 ], { 1: 1/2, ... } )");

  auto owned_map = map<int, mint>{{1, half}} | cp::rat();
  assert(cp::export_var(owned_map) == "{ 1: 1/2 }");
  auto owned_tuple = tuple{string("abc"), half, 7} | cp::rat();
  assert(cp::export_var(owned_tuple) == "( \"abc\", 1/2, 7 )");
  auto move_only = pair{make_unique<int>(42), half} | cp::rat();
  assert(cp::export_var(move_only).find("1/2") != string::npos);

  cp::options::es_style = cp::types::es_style_t::original;
  auto colored = cp::export_var(m | cp::rat());
  assert(colored.find("\x1b[36m1\x1b[0m") != string::npos);
  assert(colored.find("1/2") != string::npos);
  assert(colored.find("\x1b[36m1/2") == string::npos);
  assert(cp::export_var(key_mint | cp::rat()).find("\x1b[36m1/2\x1b[0m") != string::npos);
  check_order(key_mint, cp::front(1));
  check_order(c, cp::index());
  cp::options::es_style = cp::types::es_style_t::no_es;
}

void test_rat_dump()
{
  cp::options::es_style = cp::types::es_style_t::no_es;
  cp::options::max_line_width = 1000;
  cp::options::cont_indent_style = cp::types::cont_indent_style_t::minimal;

  test_rat_format<modint998244353>();
  test_rat_format<modint61>();
  modint::set_mod(998244353);
  test_rat_format<modint>();

  auto check_unchanged = [](const auto &x)
  {
    assert(cp::export_var(x | cp::rat()) == cp::export_var(x));
  };
  check_unchanged(42);
  check_unchanged(0.5);
  check_unchanged(true);
  check_unchanged("abc");
  check_unchanged(string("abc"));
  check_unchanged(string_view("abc"));
  check_unchanged(std::filesystem::path("abc/def"));
  check_unchanged(vector<int>{1, 2});
  check_unchanged(vector<bool>{true, false});
  check_unchanged(vector<vector<int>>{{1, 2}, {}, {3}});
  check_unchanged(vector<string>{"abc", "def"});
  check_unchanged(pair{1, string("abc")});
  check_unchanged(map<int, string>{{1, "abc"}});
  check_unchanged(set<string>{"abc", "def"});
  check_unchanged(set<vector<int>>{{1, 2}, {3}});
  check_unchanged(val_only(0));
  check_unchanged(vector<val_only>{val_only(0)});
  check_unchanged(make_unique<int>(42));
  vector<unique_ptr<int>> pointers;
  pointers.push_back(make_unique<int>(42));
  check_unchanged(pointers);

  cp::options::print_expr = false;
  cp::options::log_label_func = nullptr;
  ostringstream out;
  auto *buf = clog.rdbuf(out.rdbuf());
  using mint = modint998244353;
  dump(mint(1) / mint(2) | cp::rat(), 42 | cp::rat(), "abc" | cp::rat());
  clog.rdbuf(buf);
  assert(out.str() == "1/2, 42, \"abc\"\n");
}
#endif

int main()
{
  test1();
#if defined(LOCAL) && __cplusplus >= 202302L
  test_rat_dump();
#endif

  cout << "Hello World" << endl;
}
