#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "string/aho_corasick.hpp"

void test_trie()
{
  Trie<3> paths, terminals;
  assert(!paths.parent(0));
  assert(!paths.child(0, 0));
  static_assert(is_same_v<decltype(paths.child<unsigned>(0, 0)), optional<unsigned>>);
  vvc<int> labels(1);
  mt19937 rng(20261119);
  repi(step, 600)
  {
    vc<int> s(rng() % 8);
    for (auto &c : s) c = rng() % 3;
    int from = rng() % paths.num_of_vertices<int>();
    auto path = paths.insert<int>(s, from);
    assert(terminals.insert_last<int>(s, from) == path.back());
    assert(path.size() == s.size() + 1 && path.front() == from);
    repi(i, 1, path.size())
    {
      auto label = labels[path[i - 1]];
      label.eb(s[i - 1]);
      if (path[i] == int(labels.size())) labels.eb(label);
      assert(labels[path[i]] == label);
      assert(paths.parent(path[i]) == path[i - 1]);
    }
  }
  assert(paths.num_of_vertices() == int(labels.size()));
  repi(v, labels.size())
  {
    assert(paths.to_string<int>(v) == labels[v]);
    assert(terminals.to_string<int>(v) == labels[v]);
    assert(paths.parent(v) == terminals.parent(v));
    repi(c, 3) assert(paths.child(v, c) == terminals.child(v, c));
  }
}

void test_aho(const vvc<int> &patterns)
{
  Trie<3> trie;
  map<vc<int>, int> index;
  index[{}] = 0;
  for (const auto &s : patterns)
  {
    auto path = trie.insert<int>(s);
    repi(i, path.size()) index[vc<int>(s.begin(), s.begin() + i)] = path[i];
  }
  AhoCorasick<3> aho(patterns);
  assert(aho.trie.num_of_vertices() == trie.num_of_vertices());
  for (const auto &[s, v] : index)
  {
    assert(aho.trie.to_string<int>(v) == s);
    if (v == 0) assert(!aho.failure(v));
    else
    {
      vc<int> suffix(s.begin() + 1, s.end());
      while (!index.count(suffix)) suffix.erase(suffix.begin());
      assert(aho.failure(v) == index.at(suffix));
    }
    repi(c, 3)
    {
      auto suffix = s;
      suffix.eb(c);
      while (!index.count(suffix)) suffix.erase(suffix.begin());
      assert(aho.next(v, c) == index.at(suffix));
    }
  }
}

void test_aho_long()
{
  const int length = 20000;
  vc<int> chain(length, 0), extended = chain;
  extended.eb(1);
  AhoCorasick<3> aho(vvc<int>{chain, {}, {1}, chain, vc<int>(length / 2, 0), extended});
  assert(aho.trie.num_of_vertices() == length + 3);
  repi(v, 1, length + 1)
  {
    assert(aho.trie.parent(v) == v - 1);
    assert(aho.failure(v) == v - 1);
    assert(aho.next(v, 0) == min(v + 1, length));
    assert(aho.next(v, 1) == (v == length ? length + 2 : length + 1));
    assert(aho.next(v, 2) == 0);
  }
  assert(!aho.trie.child(length, 0));
  assert(aho.trie.child(length, 1) == length + 2);
  assert(aho.failure(length + 1) == 0);
  assert(aho.failure(length + 2) == length + 1);
  assert(aho.next(length + 2, 0) == 1);
  assert(aho.next(length + 2, 1) == length + 1);
}

template <class C>
void test_aho_character_type()
{
  const vvc<int> integers{{}, {0}, {1, 0, 1}, {1}, {0, 1}, {0, 1}};
  vvc<C> patterns;
  fec(s : integers) patterns.eb(s.begin(), s.end());
  AhoCorasick<3> expected(integers), actual(patterns);
  assert(actual.trie.num_of_vertices() == expected.trie.num_of_vertices());
  repi(v, actual.trie.num_of_vertices<int>())
  {
    assert(actual.trie.parent(v) == expected.trie.parent(v));
    assert(actual.failure(v) == expected.failure(v));
    repi(c, 3)
    {
      assert(actual.trie.child(v, c) == expected.trie.child(v, c));
      assert(actual.next(v, c) == expected.next(v, c));
    }
  }
  Trie<3> trie;
  fec(s : patterns)
  {
    auto path = trie.template insert<int>(s);
    assert(trie.template insert_last<int>(s) == path.back());
    assert(trie.template to_string<C>(path.back()) == s);
  }
}

int main()
{
  test_trie();
  test_aho_long();
  test_aho_character_type<bool>();
  test_aho_character_type<signed char>();
  test_aho_character_type<unsigned short>();
  test_aho_character_type<int>();
  test_aho_character_type<ll>();
#ifdef __SIZEOF_INT128__
  test_aho_character_type<__int128_t>();
#endif
  test_aho({});
  test_aho({{}, {}, {0}, {0, 1}, {1}, {1, 0, 1}, {0, 1}});
  mt19937 rng(20261119);
  repi(trial, 150)
  {
    vvc<int> patterns(rng() % 25);
    for (auto &s : patterns)
    {
      s.resize(rng() % 9);
      for (auto &c : s) c = rng() % 3;
    }
    test_aho(patterns);
  }
  cout << "Hello World" << endl;
}
