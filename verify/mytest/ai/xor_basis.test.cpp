#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/xor_basis.hpp"
#include "matroid/binary_matroid.hpp"
#include "matroid/matroid_intersection.hpp"

namespace
{
template <class T, class = void> struct has_coordinates : false_type {};
template <class T> struct has_coordinates<T, void_t<decltype(declval<T>().coordinates(declval<typename T::Value>()))>> : true_type {};

template <class V>
V vector_of(int dimension, ull value)
{
  if constexpr (is_same_v<V, DynamicBitset>)
  {
    V result(dimension);
    for (int i = 0; i < min(64, dimension); i++) if (value >> i & 1) result.set(i);
    return result;
  }
  else return V(value);
}

template <class V, class Tag = int>
void random_small(int cases)
{
  using Traits = internal::XorBasisVector<V>;
  static_assert(has_coordinates<XorBasis<V, true, Tag>>::value);
  static_assert(!has_coordinates<XorBasis<V, false, Tag>>::value);
  mt19937_64 rng(2026110202ULL + sizeof(V));
  for (int trial = 0; trial < cases; trial++)
  {
    int d = rng() % min(10, is_same_v<V, DynamicBitset> ? 10 : Traits::default_dimension);
    XorBasis<V, true, Tag> basis(d);
    XorBasis<V, false, Tag> other(d);
    vc<V> accepted;
    set<V> span{vector_of<V>(d, 0)};
    for (int i = 0; i < 10; i++)
    {
      V value = vector_of<V>(d, rng() % (1 << d));
      bool expected = !span.count(value);
      assert(basis.insert(value) == expected);
      if (expected)
      {
        accepted.push_back(value);
        vc<V> added;
        for (const auto &old : span) added.push_back(old ^ value);
        span.insert(added.begin(), added.end());
      }
      if (i % 3 == 0) other.insert(value);
      assert(basis.basis() == accepted);
      assert(basis.template span_size<ull>() == span.size());
    }
    vc<V> ordered(span.begin(), span.end());
    assert(basis.min_xor() == ordered.front() && basis.max_xor() == ordered.back());
    for (int k = 0; k < int(ordered.size()); k++) assert(basis.kth(k) == optional<V>(ordered[k]));
    assert(!basis.kth(-1));
    assert(!basis.kth(ull(ordered.size())));
    for (int q = 0; q < (1 << d); q++)
    {
      V value = vector_of<V>(d, q);
      assert(basis.contains(value) == bool(span.count(value)));
      auto c = basis.coordinates(value);
      assert(bool(c) == bool(span.count(value)));
      if (c)
      {
        V reconstructed = vector_of<V>(d, 0);
        if constexpr (is_same_v<V, DynamicBitset>) assert(c->size() == basis.rank());
        for (int i = 0; i < basis.rank(); i++)
          if (Traits::coefficient_test(*c, i)) reconstructed ^= basis.basis()[i];
        assert(reconstructed == value);
      }
      assert(basis.reduce(basis.reduce(value)) == basis.reduce(value));
      int position = lower_bound(ordered.begin(), ordered.end(), value) - ordered.begin();
      assert(basis.template order_of_key<ull>(value) == ull(position));
      auto le = upper_bound(ordered.begin(), ordered.end(), value);
      optional<V> expected_le = le == ordered.begin() ? nullopt : optional<V>(*prev(le));
      optional<V> expected_lt = position == 0 ? nullopt : optional<V>(ordered[position - 1]);
      assert(basis.predecessor(value) == expected_le);
      assert(basis.predecessor_strict(value) == expected_lt);
    }
    V affine = vector_of<V>(d, rng() % (1 << d));
    vc<V> affine_span;
    for (const auto &value : span) affine_span.push_back(value ^ affine);
    sort(affine_span.begin(), affine_span.end());
    assert(basis.min_xor(affine) == affine_span.front());
    assert(basis.max_xor(affine) == affine_span.back());
    for (int k = 0; k < int(ordered.size()); k++) assert(basis.kth(k, affine) == optional<V>(affine_span[k]));
    for (int q = 0; q < (1 << d); q++)
    {
      V value = vector_of<V>(d, q);
      auto at = lower_bound(affine_span.begin(), affine_span.end(), value);
      auto after = upper_bound(affine_span.begin(), affine_span.end(), value);
      assert(basis.template order_of_key<ull>(value, affine) == ull(at - affine_span.begin()));
      assert(basis.predecessor(value, affine) == (after == affine_span.begin() ? nullopt : optional<V>(*prev(after))));
      assert(basis.predecessor_strict(value, affine) == (at == affine_span.begin() ? nullopt : optional<V>(*prev(at))));
    }
    auto common = basis & other, united = basis | other;
    assert(common.rank() + united.rank() == basis.rank() + other.rank());
    for (const auto &value : other.basis()) assert(common.contains(value));
    basis.merge(basis);
    assert(basis.basis() == accepted);
    auto copy = basis;
    auto moved = std::move(copy);
    assert(moved.basis() == accepted);
    basis.clear();
    assert(basis.dimension() == d && basis.rank() == 0);
    auto zero = basis.coordinates(vector_of<V>(d, 0));
    assert(zero && Traits::empty(*zero));
  }
}

template <class Tag = int>
void wide_cases()
{
  XorBasis<u128, true, Tag> wide;
  for (int i = 127; i >= 0; i--) assert(wide.insert(u128(1) << i));
  const u128 all = ~u128(0);
  assert(wide.rank() == 128 && wide.max_xor() == all);
  assert(wide.kth(all) == optional<u128>(all));
  assert(wide.kth(ull(-1)) == optional<u128>(u128(ull(-1))));
  assert(wide.template order_of_key<u128>(all) == all);
  assert(wide.predecessor(all) == optional<u128>(all));
  assert(!wide.predecessor_strict(u128(0)));
  assert(wide.coordinates(u128(1)) == optional<u128>(u128(1) << 127));
  assert(!wide.insert(all));

  constexpr int d = 155, r = 140;
  XorBasis<DynamicBitset, true, Tag> dynamic(d);
  for (int i = 0; i < r; i++)
  {
    DynamicBitset unit(d);
    unit.set(i);
    assert(dynamic.insert(unit));
  }
  DynamicBitset low(d), high(d), maximum(d);
  low.set_range(0, 128), high.set(150), maximum.set_range(0, r);
  assert(dynamic.kth(all) == optional<DynamicBitset>(low));
  assert(dynamic.template order_of_key<u128>(low) == all);
  assert(dynamic.predecessor(high) == optional<DynamicBitset>(maximum));
  assert(dynamic.max_xor() == maximum);
  auto coordinate = dynamic.coordinates(maximum);
  assert(coordinate && coordinate->size() == r && coordinate->all());
  assert(!dynamic.contains(high));
  assert(dynamic.reduce(high) == high);
  DynamicBitset unit(d);
  unit.set(139);
  auto c = dynamic.coordinates(unit);
  assert(c && c->size() == r && c->count() == 1 && c->test(139));
  XorBasis<DynamicBitset, true, Tag> empty(0);
  auto kth_zero = empty.kth(0);
  assert(kth_zero && kth_zero->size() == 0);
  assert(empty.span_size() == 1 && empty.order_of_key(DynamicBitset(0)) == 0);
  assert(empty.predecessor(DynamicBitset(0)) && !empty.predecessor_strict(DynamicBitset(0)));
  auto self = dynamic & dynamic;
  assert(self.rank() == r);
  for (const auto &v : dynamic.basis()) assert(self.contains(v));
}

template <class Tag = int>
void intersection_cases()
{
  mt19937_64 rng(2026110203);
  for (int trial = 0; trial < 200; trial++)
  {
    XorBasis<ull, false, Tag> a(8);
    XorBasis<ull, true, Tag> b(8);
    for (int i = 0; i < 10; i++)
      if (rng() % 2) a.insert(rng() % 256); else b.insert(rng() % 256);
    auto common = a & b, united = a | b;
    assert(common.rank() + united.rank() == a.rank() + b.rank());
    for (ull v = 0; v < 256; v++) assert(common.contains(v) == (a.contains(v) && b.contains(v)));
    auto reversed = b & a;
    assert(reversed.rank() == common.rank());
    for (const auto &v : reversed.basis()) assert(common.contains(v));
    a.merge(b);
    assert(a.rank() == united.rank());
  }
}

void dynamic_matroid()
{
  static_assert(is_same_v<BinaryMatroid<u128>::UInt, u128>);
  constexpr int d = 150;
  DynamicBitset a(d), b(d), zero(d);
  a.set(129), b.set(149);
  BinaryMatroid<DynamicBitset> matroid({a, b, a ^ b, zero});
  assert(matroid.dimension() == d && matroid.size() == 4);
  matroid.build({1, 0});
  assert(matroid.circuit(2) == optional<vc<int>>(vc<int>{1, 0}));
  assert(matroid.circuit(3) == optional<vc<int>>(vc<int>{}));
  matroid.build({0});
  assert(!matroid.circuit(1));
  assert(matroid_intersection(matroid, matroid).size() == 2);
  BinaryMatroid<DynamicBitset> empty({});
  assert(empty.size() == 0 && empty.dimension() == 0);
  BinaryMatroid<DynamicBitset> loops({DynamicBitset(0), DynamicBitset(0)});
  assert(loops.circuit(0) == optional<vc<int>>(vc<int>{}));
  assert(matroid_intersection(loops, loops).empty());
  vc<DynamicBitset> columns;
  vc<int> selected;
  DynamicBitset sum(d);
  for (int i = 0; i < 140; i++)
  {
    DynamicBitset unit(d);
    unit.set(i);
    columns.push_back(unit);
    selected.push_back(i);
    sum ^= unit;
  }
  columns.push_back(sum);
  reverse(selected.begin(), selected.end());
  BinaryMatroid<DynamicBitset> high_rank(columns);
  high_rank.build(selected);
  assert(high_rank.circuit(140) == optional<vc<int>>(selected));
}
} // namespace

int main()
{
  random_small<unsigned char>(60);
  random_small<unsigned int>(60);
  random_small<ull>(60);
  random_small<u128>(60);
  random_small<DynamicBitset>(60);
  random_small<ull, void>(60);
  random_small<u128, void>(60);
  random_small<DynamicBitset, void>(60);
  wide_cases<>();
  wide_cases<void>();
  intersection_cases<>();
  intersection_cases<void>();
  dynamic_matroid();
  PRINT("Hello World");
}
