#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_algo.hpp"
#include "algebra/acted_monoid/add_min.hpp"
#include "algebra/algebra_basic_ops.hpp"
#include "ds/segtree/lazy_segtree.hpp"

constexpr auto pair_sum = MonoidAdd<pll>::op({3, -4}, {2, 7});
static_assert(pair_sum.first == 5 && pair_sum.second == 3);
constexpr auto tuple_neg = -tlll{3, -4, 2};
static_assert(get<0>(tuple_neg) == -3 && get<1>(tuple_neg) == 4);
constexpr auto array_diff = array<i128, 2>{9, 4} - array<ll, 2>{2, 7};
static_assert(array_diff[0] == 7 && array_diff[1] == -3);
constexpr auto pair_inf = default_infty<const pair<int, i128>>();
static_assert(pair_inf.first == (1 << 30) - 1 && pair_inf.second == i128(INF) * INF);
constexpr auto tuple_inf = default_infty<tlll>();
static_assert(get<0>(tuple_inf) == INF && get<2>(tuple_inf) == INF);
constexpr auto array_inf = default_infty<array<int, 3>>();
static_assert(array_inf[0] == (1 << 30) - 1 && array_inf[2] == (1 << 30) - 1);
static_assert(default_infty<tuple<>>() == tuple<>{});
static_assert(default_infty<array<ll, 0>>().size() == 0);
static_assert(MonoidMax<pll>::e().first == -INF);
static_assert(MonoidMin<array<int, 2>>::e()[1] == (1 << 30) - 1);
static_assert(ActedMonoidAddMax<pll>::mapping({2, 0}, {3, 7}).first == 5);
static_assert(GroupAddSub<tlll>::inv(tlll{1, 2, 3}) == tlll{-1, -2, -3});

using Nested = pair<tuple<array<ll, 2>, ll>, array<pll, 2>>;
constexpr auto nested_inf = default_infty<Nested>();
static_assert(get<0>(nested_inf.first)[1] == INF && nested_inf.second[1].second == INF);
constexpr auto nested_zero = nested_inf - nested_inf;
static_assert(get<0>(nested_zero.first)[0] == 0 && nested_zero.second[0].first == 0);

template <class T>
T random_value(mt19937 &rng);

template <class T, size_t... I>
T random_product(mt19937 &rng, index_sequence<I...>)
{
  return T{random_value<tuple_element_t<I, T>>(rng)...};
}

template <class T>
T random_value(mt19937 &rng)
{
  if constexpr (is_integral_ext<T>) return T(int(rng() % 21) - 10);
  else return random_product<T>(rng, make_index_sequence<tuple_size_v<T>>{});
}

template <class T>
void add_naive(T &a, const T &b);

template <class T, size_t... I>
void add_product_naive(T &a, const T &b, index_sequence<I...>)
{
  (add_naive(get<I>(a), get<I>(b)), ...);
}

template <class T>
void add_naive(T &a, const T &b)
{
  if constexpr (is_integral_ext<T>) a += b;
  else add_product_naive(a, b, make_index_sequence<tuple_size_v<T>>{});
}

template <class T>
void check_tree(mt19937 &rng)
{
  using Min = ActedMonoidAddMin<T>;
  using Max = ActedMonoidAddMax<T>;
  for (int n : {0, 1, 2, 17, 48})
  {
    vc<T> values(n);
    for (auto &x : values) x = random_value<T>(rng);
    LazySegmentTree<Min> low(values);
    LazySegmentTree<Max> high(values);
    if (n == 0)
    {
      low.apply(0, 0, Min::id());
      high.apply(0, 0, Max::id());
      assert(low.all_prod() == Min::e() && high.all_prod() == Max::e());
      continue;
    }
    for (int step = 0; step < 500; ++step)
    {
      int l = int(rng() % n), r = l + 1 + int(rng() % (n - l));
      auto shift = random_value<T>(rng);
      low.apply(l, r, shift);
      high.apply(l, r, shift);
      for (int i = l; i < r; ++i) add_naive(values[i], shift);
      if (step % 3 == 0)
      {
        int p = rng() % n;
        values[p] = random_value<T>(rng);
        low.set(p, values[p]);
        high.set(p, values[p]);
      }
      l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      T mn = Min::e(), mx = Max::e();
      for (int i = l; i < r; ++i)
        mn = min(mn, values[i]), mx = max(mx, values[i]);
      assert(low.prod(l, r) == mn && high.prod(l, r) == mx);
      assert(low.all_prod() == *min_element(values.begin(), values.end()));
      assert(high.all_prod() == *max_element(values.begin(), values.end()));
    }
    assert(low.content() == values && high.content() == values);
  }
}

struct MoveOnly
{
  unique_ptr<ll> value;
  explicit MoveOnly(ll x) : value(make_unique<ll>(x)) {}
  MoveOnly &operator+=(const MoveOnly &other) { *value += *other.value; return *this; }
  MoveOnly &operator-=(const MoveOnly &other) { *value -= *other.value; return *this; }
  MoveOnly operator-() const { return MoveOnly(-*value); }
};

void check_references_and_types()
{
  ll a = 2, b = 5;
  auto refs = tie(a, b);
  static_assert(is_same_v<decltype(refs += pll{}), tuple<ll &, ll &> &>);
  refs += tie(b, a);
  assert(a == 7 && b == 12);
  refs -= pll{1, 2};
  assert(a == 6 && b == 10);
  pair<ll &, ll &> pair_refs(a, b);
  pair_refs += pair_refs;
  assert(a == 12 && b == 20);

  const pair<short, ll> shift{2, 3};
  pair<int, i128> mixed{7, i128(1) << 80};
  static_assert(is_same_v<decltype(mixed + shift), pair<int, i128>>);
  mixed = mixed + shift - shift;
  assert(mixed.first == 7 && mixed.second == (i128(1) << 80));
  array<u128, 2> wide{u128(1) << 80, 10};
  wide += array<unsigned, 2>{3, 4};
  wide -= wide;
  assert(wide[0] == 0 && wide[1] == 0);
  auto mixed_tuple = tuple<int, ll>{2, 3} + array<short, 2>{4, 5};
  assert((mixed_tuple == tuple<int, ll>{6, 8}));
  array<int, 0> empty{};
  empty += empty;
  assert((-empty + empty - empty).empty());
  auto empty_tuple = tuple<>{};
  empty_tuple += empty_tuple;
  assert((-empty_tuple + empty_tuple - empty_tuple) == tuple<>{});

  pair<MoveOnly, MoveOnly> movable{MoveOnly(7), MoveOnly(-2)};
  const pair<MoveOnly, MoveOnly> operand{MoveOnly(3), MoveOnly(5)};
  movable += operand;
  movable -= operand;
  auto negative = -move(movable);
  assert(*negative.first.value == -7 && *negative.second.value == 2);

  Nested p{{{1, 2}, 3}, {{{4, 5}, {6, 7}}}};
  const Nested q{{{8, 9}, 10}, {{{11, 12}, {13, 14}}}};
  const Nested expected{{{9, 11}, 13}, {{{15, 17}, {19, 21}}}};
  assert(p + q == expected);
  assert(p + q - q == p);
  assert(-(-p) == p);
}

int main()
{
  check_references_and_types();
  mt19937 rng(20261029);
  check_tree<pll>(rng);
  check_tree<tlll>(rng);
  check_tree<array<ll, 3>>(rng);
  check_tree<pair<int, i128>>(rng);
  check_tree<tuple<short, ll, i128>>(rng);
  check_tree<array<i128, 2>>(rng);
  check_tree<Nested>(rng);
  PRINT("Hello World");
}
