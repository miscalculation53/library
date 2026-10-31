#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_inout.hpp"

#define TEST_PAIR_TYPE pair<ll, string>
#define TEST_WRAPPED_PAIR_TYPE (pair<ll, string>)

template <class F>
void with_input(const string &text, F f)
{
#if defined FAST_IO and not defined LOCAL
  assert(text.size() + 1 < sizeof(fastio::ibuf));
  memcpy(fastio::ibuf, text.data(), text.size());
  fastio::ibuf[text.size()] = ' ';
  fastio::pil = 0;
  fastio::pir = uint32_t(text.size() + 1);
  f();
#else
  istringstream input(text);
  auto *original = cin.rdbuf(input.rdbuf());
  f();
  cin.rdbuf(original);
#endif
}

int main()
{
  with_input("1 2 3 4 5 a 6 b 7 c 8 d", []
  {
    VEC(pll, 1, old);
    VEC((pll), 1, wrapped_alias);
    VEC((pair<ll, string>), 1, a, b);
    VEC(TEST_PAIR_TYPE, 1, macro_alias);
    VEC(TEST_WRAPPED_PAIR_TYPE, 1, wrapped_macro_alias);
    static_assert(is_same_v<decltype(old), vc<pll>>);
    static_assert(is_same_v<decltype(a), vc<pair<ll, string>>>);
    assert(old == vc<pll>({{1, 2}}));
    assert(wrapped_alias == vc<pll>({{3, 4}}));
    assert((a == vc<pair<ll, string>>({{5, "a"}})));
    assert((b == vc<pair<ll, string>>({{6, "b"}})));
    assert((macro_alias == vc<pair<ll, string>>({{7, "c"}})));
    assert((wrapped_macro_alias == vc<pair<ll, string>>({{8, "d"}})));
  });

  with_input("9 10 f 11", []
  {
    VEC((tuple<ll, pair<int, string>, ll>), 1, nested);
    assert((nested == vc<tuple<ll, pair<int, string>, ll>>{
      {9, {10, "f"}, 11}
    }));
  });

  with_input("1 a 2 b 3 c 4 d 5 e 6 f 0 2 7 g 8 h 1 9 i 77", []
  {
    IN((pair<ll, string>), p, q);
    ARR((pair<ll, string>), 1, a, b);
    VEC2((pair<ll, string>), 1, 2, grid);
    JAG((pair<ll, string>), 3, jagged);
    int shared = 0;
    IN((int &), ref = shared);
    static_assert(is_same_v<decltype(ref), int &>);
    assert((p == pair<ll, string>{1, "a"}));
    assert((q == pair<ll, string>{2, "b"}));
    assert((a == array<pair<ll, string>, 1>{{{3, "c"}}}));
    assert((b == array<pair<ll, string>, 1>{{{4, "d"}}}));
    assert((grid == vvc<pair<ll, string>>{{{5, "e"}, {6, "f"}}}));
    assert((jagged == vvc<pair<ll, string>>{{}, {{7, "g"}, {8, "h"}}, {{9, "i"}}}));
    assert(ref == 77 && &ref == &shared);
  });

  with_input("1 2 3 4 5 6 7 1 8 1 9 10", []
  {
    INT(x);
    ARR(ll, 1, a);
    ARR((ll), 1, b);
    VEC2(ll, 1, 1, c);
    VEC2((ll), 1, 1, d);
    VEC((ll), 1, e);
    IN(ll, f);
    JAG(ll, 1, g);
    JAG((ll), 1, h);
    VEC((pair<ll, string>), 0, empty);
    VEC2((pair<ll, string>), 2, 0, empty_rows);
    JAG((pair<ll, string>), 0, empty_jagged);
    ARR((pair<ll, string>), 0, empty_array);
    INT(last);
    assert(x == 1 && a[0] == 2 && b[0] == 3 && c[0][0] == 4);
    assert(d[0][0] == 5 && e[0] == 6 && f == 7);
    assert(g == vvl({{8}}) && h == vvl({{9}}) && last == 10);
    assert((empty.empty() && empty_rows == vvc<pair<ll, string>>(2)));
    assert(empty_jagged.empty() && empty_array.empty());
  });

  mt19937 rng(20261029);
  for (int trial = 0; trial < 100; ++trial)
  {
    int n = int(rng() % 30);
    vc<pair<ll, string>> expected;
    ostringstream input;
    for (int i = 0; i < n; ++i)
    {
      ll value = ll(rng() % 1001) - 500;
      string word(1 + rng() % 8, char('a' + rng() % 3));
      expected.emplace_back(value, word);
      input << value << ' ' << word << '\n';
    }
    with_input(input.str(), [&]
    {
      VEC((pair<ll, string>), n, actual);
      assert(actual == expected);
    });
  }
  PRINT("Hello World");
}
