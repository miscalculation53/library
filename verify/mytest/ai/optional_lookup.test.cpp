#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "tree/cartesian_tree.hpp"
#include "tree/rooted_tree.hpp"
#include "string/aho_corasick.hpp"
#include "algo/lis.hpp"

void check_extrema()
{
  mt19937 rng(81726);
  for (int n = 0; n <= 40; ++n) for (int trial = 0; trial < 80; ++trial)
  {
    vc<int> a(n);
    for (int &x : a) x = int(rng() % 11) - 5;
    optional<int> maximum, minimum;
    for (int i = 0; i < n; ++i)
    {
      if (!maximum || a[i] > a[*maximum]) maximum = i;
      if (!minimum || a[i] < a[*minimum]) minimum = i;
    }
    assert(ARGMAX<int>(a) == maximum);
    assert(ARGMIN<int>(a) == minimum);
  }
  assert(ARGMAX<unsigned>(vc<int>{1}) == 0);
  assert(ARGMIN<unsigned>(vc<int>{1}) == 0);
  assert(!ARGMAX<unsigned>(vc<int>{}));
}

void check_characters()
{
  assert(ctol<unsigned>('a', "aba") == 0);
  assert(ctol<unsigned>('b', "aba") == 1);
  assert(!ctol<unsigned>('c', "aba"));
  auto empty = stov<int>("", "");
  assert(empty && empty->empty());
  assert(!stov<int>("a", ""));
  assert(stov<int>("abba", "ab") == optional<vc<int>>({{0, 1, 1, 0}}));
  assert(!stov<int>("abc", "ab"));
  static_assert(is_same_v<decltype(stov("a", 'a')), vc<ll>>);
  static_assert(is_same_v<decltype(stov("a", "a")), optional<vc<ll>>>);
}

void check_cartesian()
{
  mt19937 rng(9153);
  for (int n = 1; n <= 35; ++n) for (int trial = 0; trial < 60; ++trial)
  {
    vc<int> a(n);
    for (int &x : a) x = rng() % 10;
    CartesianTree tree(a);
    assert(!tree.parent(tree.root()));
    for (int i = 0; i < n; ++i)
    {
      int l = i, r = i + 1;
      while (l > 0 && a[l - 1] >= a[i]) --l;
      while (r < n && a[r] > a[i]) ++r;
      assert((tree.range<int>(i) == pair<int, int>{l, r}));
      vc<int> children;
      if (auto c = tree.left_child<int>(i))
      {
        assert(*c < i && tree.parent<int>(*c) == i);
        children.push_back(*c);
      }
      if (auto c = tree.right_child<int>(i))
      {
        assert(*c > i && tree.parent<int>(*c) == i);
        children.push_back(*c);
      }
      const auto range = tree.children<int>(i);
      assert(vc<int>(range.begin(), range.end()) == children);
      if (auto p = tree.parent<int>(i)) assert(a[*p] <= a[i]);
    }
  }
  CartesianTree singleton(vc<int>{1});
  assert(singleton.root() == 0 && !singleton.parent(0));
  assert(!singleton.left_child(0) && !singleton.right_child(0));
  static_assert(is_same_v<decltype(singleton.parent<unsigned>(0)), optional<unsigned>>);
}

template <class Compare>
void check_lis(const vc<int> &a, Compare comp)
{
  const int n = a.size();
  int best = 0;
  vc<optional<int>> positions(n);
  for (unsigned mask = 0; mask < (1u << n); ++mask)
  {
    vc<int> indices;
    for (int i = 0; i < n; ++i) if ((mask >> i) & 1) indices.push_back(i);
    bool valid = true;
    for (int j = 1; j < int(indices.size()); ++j)
      if (!comp(a[indices[j - 1]], a[indices[j]])) valid = false;
    if (!valid || int(indices.size()) < best) continue;
    if (int(indices.size()) > best)
    {
      best = indices.size();
      fill(positions.begin(), positions.end(), nullopt);
    }
    for (int j = 0; j < best; ++j) positions[indices[j]] = j;
  }
  LIS lis(a, comp);
  assert(lis.len_of_lis() == best);
  for (int i = 0; i < n; ++i) assert(lis.pos_in_lis<int>(i) == positions[i]);
}

int main()
{
  check_extrema();
  check_characters();
  check_cartesian();
  RootedTree single(1, vc<int>{-1});
  assert(!single.heavy_child(0));
  assert(single.la(0, 0) == 0 && !single.la(0, 1));
  assert(single.jump(0, 0, 0) == 0 && !single.jump(0, 0, 1));
  Trie<2> trie;
  assert(!trie.parent<unsigned>(0) && !trie.child<unsigned>(0, 0));
  assert(trie.insert_last(vc<int>{}) == 0);
  int v = trie.insert_last(vc<int>{0});
  assert(trie.parent<unsigned>(v) == 0 && trie.child<unsigned>(0, 0) == unsigned(v));
  AhoCorasick<2> aho(vvc<int>{{}, {0}});
  assert(!aho.failure<unsigned>(0) && aho.failure<unsigned>(1) == 0);
  mt19937 rng(56217);
  for (int n = 0; n <= 10; ++n) for (int trial = 0; trial < 30; ++trial)
  {
    vc<int> a(n);
    for (int &x : a) x = rng() % 5;
    check_lis(a, less<>{});
    check_lis(a, greater<>{});
    check_lis(a, less_equal<>{});
    check_lis(a, greater_equal<>{});
  }
  cout << "Hello World" << endl;
}
