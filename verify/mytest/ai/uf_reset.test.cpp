#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/meldable_integer_set.hpp"
#include "ds/uf/uf.hpp"

template <class Data, bool compress = true>
struct InspectUF : UnionFind<Data, compress>
{
  using UF = UnionFind<Data, compress>;
  using UF::UF;
  auto storage() const
  {
    return make_tuple(this->par.data(), this->vdat.data(), this->par.capacity(), this->vdat.capacity());
  }
};

// Test focus: resizing within capacity preserves storage and restores all component metadata.
template <bool compress>
void test_metadata()
{
  using Data = UFDataEverything<ll, true>;
  InspectUF<Data, compress> uf(64);
  auto initial_storage = uf.storage();
  mt19937 rng(3756);
  for (int n : {64, 7, 64, 1, 0, 64, 96, 3, 96})
  {
    auto before = uf.storage();
    size_t capacity = min(get<2>(before), get<3>(before));
    uf.reset(n);
    if (size_t(n) <= capacity) assert(uf.storage() == before);
    if (n <= 64 && capacity == 64) assert(uf.storage() == initial_storage);
    assert(uf.gdat.cmp_cnt == n);
    assert(uf.gdat.min_leader == 0 && uf.gdat.max_leader == n - 1);
    repi(i, n)
    {
      assert(uf.leader(i) == i && uf.size(i) == 1);
      const auto &data = uf.get_vdata(i);
      assert(data.vsum == 1 && data.esum == 0 && data.vlist == vc<ll>{i});
    }
    vc<int> component(n);
    iota(ALL(component), 0);
    repi(iter, n * 8)
    {
      int u = rng() % n, v = rng() % n;
      int old = component[v], target = component[u];
      uf.merge(u, v, iter + 1);
      for (int &c : component) if (c == old) c = target;
      int s = rng() % n;
      vc<ll> expected;
      repi(i, n)
      {
        assert(uf.same(s, i) == (component[s] == component[i]));
        if (component[s] == component[i]) expected.eb(i);
      }
      auto actual = uf.get_vdata(s).vlist;
      sort(ALL(actual));
      assert(actual == expected && uf.size(s) == ll(expected.size()));
      assert(uf.gdat.cmp_cnt == SZ(set<int>(ALL(component))));
    }
  }
  InspectUF<UFDataEmpty<>, compress> empty;
  empty.reset(0);
  empty.reset(4);
  empty.merge(1, 3);
  empty.reset(4);
  assert(!empty.same(1, 3));
}

MeldableIntegerSetPool pool(130, 64);
vc<int> colors;
struct SetData : UFDataEmpty<>
{
  struct VData
  {
    MeldableIntegerSet st = pool.make_set();
    VData() = default;
    VData(int i) { st.insert(colors[i]); }
  };
  template <class UF>
  static void add_edge_diff(UF &uf, int x, int y, ll)
  {
    uf.vdat[x].st.merge(uf.vdat[y].st);
  }
};

// Test focus: move-only pooled sets are replaced safely after pool.clear(), using new vertex values.
void test_pool()
{
  InspectUF<SetData> uf;
  for (int n : {64, 64, 7, 0, 64, 96, 96})
  {
    pool.clear();
    colors.resize(n);
    repi(i, n) colors[i] = (i * 17 + n) % 130;
    auto before = uf.storage();
    size_t capacity = min(get<2>(before), get<3>(before));
    uf.reset(n);
    if (size_t(n) <= capacity) assert(uf.storage() == before);
    assert(pool.node_count() == size_t(n) * 3);
    repi(i, n) assert(uf.get_vdata(i).st.content() == vc<ll>{colors[i]});
    repi(i, n - 1) uf.merge(i, i + 1);
    if (n)
    {
      set<ll> expected(ALL(colors));
      assert(uf.get_vdata(0).st.content() == vc<ll>(ALL(expected)));
    }
  }
}

int main()
{
  test_metadata<true>();
  test_metadata<false>();
  test_pool();
  PRINT("Hello World");
}
