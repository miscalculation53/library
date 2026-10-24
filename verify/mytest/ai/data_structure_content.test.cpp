#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/cumulative_sum/cumulative_sum.hpp"
#include "ds/cumulative_sum/cumulative_sum_2d.hpp"
#include "ds/flat_map.hpp"
#include "ds/default_map.hpp"
#include "ds/hash_set.hpp"
#include "ds/dynamic_bitset.hpp"
#include "ds/csr.hpp"
#include "ds/group_index.hpp"
#include "ds/priority_container/monotone_deque.hpp"
#include "ds/uf/uf_undo.hpp"
#include "ds/uf/uf_potential.hpp"
#include "ds/uf/uf_partially_persistent.hpp"

struct PermutationGroup
{
  using S = array<int, 3>;
  static S e() { return {0, 1, 2}; }
  static S op(const S &p, const S &q) { return {p[q[0]], p[q[1]], p[q[2]]}; }
  static S inv(const S &p)
  {
    S q;
    repi(i, 3) q[p[i]] = i;
    return q;
  }
};

// Noncommutative prefix reconstruction and zero-column shapes.
void test_arrays()
{
  const vc<PermutationGroup::S> a{{1, 0, 2}, {0, 2, 1}, {2, 0, 1}, {1, 2, 0}};
  const CumulativeSum<PermutationGroup> cs(a);
  assert(cs.content() == a);
  auto copy = cs.content();
  copy[0] = PermutationGroup::e();
  assert(cs.content() == a);
  assert(CumulativeSum<PermutationGroup>().content().empty());
  assert(CumulativeSum<PermutationGroup>(vc<PermutationGroup::S>{}).content().empty());

  using G = GroupAddSub<ll>;
  for (vvc<ll> a2 : {vvc<ll>{}, vvc<ll>(3), vvc<ll>{{3, -8, 4}, {0, 11, -5}}})
  {
    const CumulativeSum2D<G> cs2(a2);
    assert(cs2.content() == a2);
  }
  assert(CumulativeSum2D<G>().content().empty());

  const CoordinateCompression<int> cc(vc<int>{7, 1, 7, 3});
  assert((cc.content() == vc<int>{1, 3, 7}));
  assert(CoordinateCompression<int>().content().empty());
  FlatMap<int, string> mp(vc<int>{7, 1, 7, 3});
  mp[3] = "three";
  const auto &cmp = mp;
  assert((cmp.content() == vc<pair<int, string>>{{1, ""}, {3, "three"}, {7, ""}}));
  auto entries = cmp.content();
  entries[1].second = "copy";
  assert(cmp.at(3) == "three");
  assert((FlatMap<int, string>().content().empty()));

  mt19937 rng(817232);
  for (int n : {0, 1, 63, 64, 65, 129})
  {
    DynamicBitset bits(n);
    vc<bool> expected(n);
    repi(_, 200)
    {
      if (n)
      {
        int i = rng() % n;
        bits.flip(i);
        expected[i] = !expected[i];
      }
      assert(as_const(bits).content() == expected);
    }
    bits.push_back(true);
    expected.push_back(true);
    assert(as_const(bits).content() == expected);
    bits.resize(0);
    assert(as_const(bits).content().empty());
  }
}

int factory_calls = 0;
string default_string() { ++factory_calls; return "default"; }

template <template <class, class, auto> class Map>
void test_default_map()
{
  Map<int, string, default_string> mp;
  const auto &cmp = mp;
  factory_calls = 0;
  assert(cmp.content().empty() && factory_calls == 0);
  mp[5] = "five";
  mp[1];
  mp[3] = "three";
  mp.erase(3);
  assert(factory_calls == 3);
  auto entries = cmp.content();
  sort(ALL(entries));
  assert((entries == vc<pair<int, string>>{{1, "default"}, {5, "five"}}));
  assert(factory_calls == 3 && mp.size() == 2);
  entries[0].second = "copy";
  assert(cmp[1] == "default");
  mp.clear();
  assert(cmp.content().empty() && factory_calls == 3);
}

// Keys have equality and hashing, with no ordering operator.
struct Key { int value = 0; };
struct KeyHash { size_t operator()(const Key &) const { return 6; } };
struct KeyEqual { bool operator()(const Key &x, const Key &y) const { return x.value == y.value; } };

void test_hashes()
{
  HashMap<Key, int, KeyHash, KeyEqual> mp;
  HashSet<Key, KeyHash, KeyEqual> st;
  map<int, int> expected;
  mt19937 rng(731622);
  repi(iter, 500)
  {
    int key = rng() % 80;
    if (rng() % 3)
    {
      int val = rng() % 100;
      mp.insert_or_assign({key}, val);
      st.insert({key});
      expected[key] = val;
    }
    else
    {
      mp.erase({key});
      st.erase({key});
      expected.erase(key);
    }
    if (iter == 250) mp.reserve(1000), st.reserve(1000);
    auto entries = as_const(mp).content();
    auto keys = as_const(st).content();
    map<int, int> got;
    for (auto [k, val] : entries) assert(got.emplace(k.value, val).second);
    assert(got == expected);
    set<int> got_keys;
    for (auto k : keys) assert(got_keys.insert(k.value).second);
    assert(got_keys.size() == expected.size());
    for (auto [k, val] : expected) assert(got_keys.count(k));
    if (!entries.empty())
    {
      int k = entries[0].first.value, val = entries[0].second;
      entries[0].second++;
      assert(mp.at({k}) == val);
    }
    if (iter % 157 == 0)
    {
      mp.clear(), st.clear(), expected.clear();
      assert(as_const(mp).content().empty() && as_const(st).content().empty());
    }
  }
}

void test_rows_and_deque()
{
  const CSR<int> empty;
  assert(empty.content().empty() && empty.size() == 0 && empty.offset(0) == 0);
  const vvc<int> rows{{3, 2, 3, 1}, {}, {6, 5}};
  CSR<int, true> csr(rows);
  assert(as_const(csr).content() == rows);
  csr.pop_back(0);
  csr.pop_back(2);
  assert((as_const(csr).content() == vvc<int>{{3, 2, 3}, {}, {6}}));
  assert((csr[0].content() == vc<int>{3, 2, 3}));
  assert(as_const(csr)[1].content().empty());
  csr.sortunique();
  assert((as_const(csr).content() == vvc<int>{{2, 3}, {}, {6}}));
  assert(CSR<int>(rows).content() == rows);

  const vc<pair<int, vc<int>>> grouped{{0, {1}}, {2, {0, 2}}, {5, {3}}};
  assert((GroupIndex<int, true, int>({2, 0, 2, 5}).content() == grouped));
  assert((GroupIndexRaw<int, int>({2, 0, 2, 5}).content() == grouped));
  assert(GroupIndex<int>().content().empty());
  assert(GroupIndexRaw<int>().content().empty());

  MonotonePriorityDeque<int, greater<int>> q;
  for (int x : {3, 5, 1, 7, -1, -1}) q.push(x);
  assert((as_const(q).content() == vc<int>{7, 5, 3, 1, -1, -1}));
  q.pop_front(), q.pop_back();
  assert((as_const(q).content() == vc<int>{5, 3, 1, -1}));
  PriorityContainer<MonotonePriorityDeque<int, greater<int>>> wrapped(q);
  assert(as_const(wrapped).content() == as_const(q).content());
  q.clear();
  assert(as_const(q).content().empty());
}

vvc<int> groups(const vc<int> &labels)
{
  map<int, int> gid;
  vvc<int> res;
  repi(v, labels.size())
  {
    auto [it, inserted] = gid.emplace(labels[v], res.size());
    if (inserted) res.eb();
    res[it->second].eb(v);
  }
  return res;
}

// Const snapshots preserve undo, historical connectivity, and potentials.
void test_union_find()
{
  constexpr int n = 27;
  UnionFind<UFDataEmpty<>> uf(n);
  UnionFind<UFDataEmpty<>, false> plain(n);
  UnionFindUndo<UFDataEmpty<>> undo(n);
  UnionFindPotential<GroupAddSub<ll>> potential(n);
  UnionFindPartiallyPersistent<UFDataEmpty<>> persistent(n);
  vc<int> labels(n);
  iota(ALL(labels), 0);
  vvc<int> history{labels};
  mt19937 rng(632917);
  repi(iter, 100)
  {
    int x = rng() % n, y = rng() % n;
    uf.merge(x, y), plain.merge(x, y), undo.merge(x, y);
    potential.merge(x, y, 7LL * (y - x));
    persistent.merge(x, y);
    int old = labels[y], replacement = labels[x];
    for (int &v : labels) if (v == old) v = replacement;
    history.eb(labels);
    auto expected = groups(labels);
    assert(as_const(uf).content() == expected && as_const(plain).content() == expected);
    assert(as_const(undo).content() == expected && as_const(potential).content() == expected);
    assert(as_const(persistent).content() == expected);
    assert(potential.diff(x, y) == 7LL * (y - x));
    assert(as_const(persistent).content(-10) == groups(history[0]));
    assert(as_const(persistent).content(1000) == expected);
    int t = rng() % history.size();
    assert(as_const(persistent).content(t) == groups(history[t]));
  }
  for (int t = int(history.size()) - 2; t >= 0; --t)
  {
    undo.undo();
    assert(as_const(undo).content() == groups(history[t]));
    assert(as_const(persistent).content(t) == groups(history[t]));
  }
  uf.reset(0), potential.reset(0);
  assert(as_const(uf).content().empty() && as_const(potential).content().empty());
  assert((UnionFind<UFDataEmpty<>>().content().empty()));
  assert((UnionFindPartiallyPersistent<UFDataEmpty<>>(0).content().empty()));
}

int main()
{
  test_arrays();
  test_default_map<DefaultMap>();
  test_default_map<DefaultUnorderedMap>();
  test_default_map<DefaultHashMap>();
  test_hashes();
  test_rows_and_deque();
  test_union_find();
  cout << "Hello World" << endl;
}
