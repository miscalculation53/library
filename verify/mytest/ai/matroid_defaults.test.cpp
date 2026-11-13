#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "matroid/matroid.hpp"
#include "math/modint/random.hpp"

template <class F, class Predicate>
void check_representation(const LinearMatroid<F> &m, const Predicate &independent)
{
  for (int mask = 0; mask < (1 << m.size()); mask++)
  {
    vc<int> selected;
    for (int e = 0; e < m.size(); e++) if (mask >> e & 1) selected.push_back(e);
    Matrix<F> matrix(m.dimension(), selected.size());
    for (int j = 0; j < int(selected.size()); j++) for (int i = 0; i < m.dimension(); i++)
      matrix[i][j] = m.columns()[selected[j]][i];
    assert((matrix.template rank<int>() == int(selected.size())) == independent(mask));
  }
}

struct CustomField
{
  struct S
  {
    int value = 0;
    S() = default;
    friend bool operator==(S a, S b) { return a.value == b.value; }
    friend bool operator!=(S a, S b) { return !(a == b); }
  };
  static S make(int x) { S s; s.value = (x % 7 + 7) % 7; return s; }
  static S e0() { return make(0); }
  static S e1() { return make(1); }
  static S add(S a, S b) { return make(a.value + b.value); }
  static S minus(S a) { return make(-a.value); }
  static S mul(S a, S b) { return make(a.value * b.value); }
  static S inv(S a)
  {
    assert(a != e0());
    for (int x = 1; x < 7; x++) if (a.value * x % 7 == 1) return make(x);
    return e0();
  }
};

struct WideResidue
{
  ull value;
  explicit WideResidue(ull value) : value(value) {}
  static constexpr ull mod() { return 18446744073709551557ULL; }
};

int main()
{
  using F = FieldAddSubMulDiv<modint998244353>;
  using F2 = FieldAddSubMulDiv<static_modint32<2>>;
  using F3 = FieldAddSubMulDiv<static_modint32<3>>;
  mt.seed(20261108);
  const auto saved = mt;
  auto uniform = UniformMatroid(6, 3).linear_representation();
  static_assert(is_same_v<decltype(uniform), LinearMatroid<F>>);
  assert(mt == saved);
  check_representation(uniform, [](int mask) { return __builtin_popcount(unsigned(mask)) <= 3; });
  check_representation(UniformMatroid(3, 3).linear_representation<F3>(), [](int) { return true; });
  check_representation(UniformMatroid(5, 1).linear_representation<F2>(),
                       [](int mask) { return __builtin_popcount(unsigned(mask)) <= 1; });
  check_representation(UniformMatroid(3, 2).linear_representation<CustomField>(),
                       [](int mask) { return __builtin_popcount(unsigned(mask)) <= 2; });
  check_representation(UniformMatroid(0, 0).linear_representation(), [](int) { return true; });
  check_representation(UniformMatroid(4, 0).linear_representation(), [](int mask) { return mask == 0; });

  const vc<ll> group{0, 1, 0, 1, 2, 2};
  const vc<u128> capacity{2, 1, 0};
  auto partition = PartitionMatroid(group, capacity).linear_representation();
  assert(mt == saved);
  assert(partition.columns() == partition_linear_representation(group, capacity).columns());
  check_representation(partition, [&](int mask)
  {
    vc<int> count(3);
    for (int e = 0; e < 6; e++) if (mask >> e & 1) count[group[e]]++;
    return count[0] <= 2 && count[1] <= 1 && count[2] == 0;
  });
  check_representation(PartitionMatroid({0, 1, 0, 1}, {2, 2}).linear_representation<F2>(),
                       [](int) { return true; });
  check_representation(PartitionMatroid({0, 0, 0, 0}, {1}).linear_representation<F2>(),
                       [](int mask) { return __builtin_popcount(unsigned(mask)) <= 1; });
  check_representation(PartitionMatroid({0, 0, 1}, {1, 1}).linear_representation<CustomField>(),
                       [](int mask) { return (mask & 3) != 3; });
  assert(partition_linear_representation({}, {}).size() == 0);
  assert(uniform_linear_representation(2, vc<modint998244353>{0, 1, 2}).size() == 3);
  assert(UniformMatroid(3, 2).linear_representation({0, 1, 2}).size() == 3);
  assert(PartitionMatroid(group, capacity).linear_representation({0, 0, 1, 1, 0, 1}).size() == 6);

  GraphicMatroid graphic(3, {{0, 1}, {1, 2}, {2, 0}});
  assert(graphic.linear_representation().columns() == graphic_linear_representation(3, {{0, 1}, {1, 2}, {2, 0}}).columns());
  auto binary = BinaryMatroid<>({3, 5, 6}).linear_representation();
  static_assert(is_same_v<decltype(binary), LinearMatroid<F2>>);
  check_representation(binary, [](int mask) { return mask != 7; });
  auto odd = graphic_linear_representation(0, vc<pair<int, int>>{});
  assert(odd.size() == 0);
  assert(binary_linear_representation(vc<ull>{1, 2, 3}).dimension() == 64);

  auto sample = [state = make_unique<int>(0)]() mutable { return modint998244353(++*state); };
  const vvc<ll> adjacency{{0, 0}, {0, 1}};
  assert(transversal_linear_representation(2, adjacency, sample).columns()[0][0] == 1);
  assert(transversal_linear_representation(2, CSR<ll>(adjacency)).size() == 2);
  assert(transversal_linear_representation(2, {{0}, {0, 1}}).size() == 2);
  GraphUndirected<> graph(4, vc<pair<int, int>>{{0, 2}, {1, 2}, {1, 3}});
  vc<int> color{0, 0, 1, 1};
  mt.seed(7);
  check_representation(TransversalMatroid(graph, color).linear_representation(), [](int) { return true; });
  assert(transversal_linear_representation(graph, color).size() == 2);
  check_representation(BicircularMatroid(1, {{0, 0}, {0, 0}}).linear_representation(),
                       [](int mask) { return mask != 3; });
  assert(bicircular_linear_representation(1, {{0, 0}}).size() == 1);
  auto custom_sample = [] { return CustomField::e1(); };
  assert(BicircularMatroid(1, {{0, 0}}).linear_representation<CustomField>(custom_sample).size() == 1);
  LinearMatroid<CustomField> custom(2, {{CustomField::e1(), CustomField::e0()},
                                      {CustomField::e0(), CustomField::e1()}});
  assert(linear_matroid_parity_size(custom, custom_sample) == 1);

  LinearMatroid m(4, {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
  static_assert(is_same_v<decltype(m), LinearMatroid<>>);
  const vc<pair<ll, u128>> pairs{{0, 2}, {1, 3}};
  assert(linear_matroid_parity_size(m) == 2);
  assert(linear_matroid_parity_size(m, 2) == 2);
  assert(linear_matroid_parity_size(m, pairs, 2) == 2);
  assert(linear_matroid_parity_restore(m).size() == 2);
  assert(linear_matroid_parity_restore(m, 2).size() == 2);
  assert(linear_matroid_parity_restore(m, pairs).size() == 2);
  assert(linear_matroid_parity_restore(m, pairs, 2).size() == 2);
  assert(linear_matroid_parity_restore(m, {{0, 1}, {2, 3}}, 2).size() == 2);
  assert(linear_matroid_parity_restore(m, {}).empty());
  assert(linear_matroid_parity_size(m, {}) == 0);
  assert(linear_matroid_intersection_restore(m, m).size() == 4);
  assert(linear_matroid_intersection_size(m, m, 2) == 4);
  assert(linear_matroid_parity_restore(LinearMatroid<>(0, vvc<modint998244353>{})).empty());
  assert(linear_matroid_intersection_size(uniform, partition) == 3);

  mt.seed(20261108);
  vc<modint998244353> old_samples(128);
  for (auto &x : old_samples) x = randrange(1, modint998244353::mod());
  const auto old_state = mt;
  mt.seed(20261108);
  for (auto expected : old_samples) assert(bbla::random_sample_mint<modint998244353>() == expected);
  assert(mt == old_state);
  bool seen_zero = false, seen_one = false, seen_wide = false;
  for (int i = 0; i < 128; i++)
  {
    auto x = random_modint<static_modint32<2>>();
    seen_zero |= x == 0, seen_one |= x == 1;
    assert((random_modint<static_modint32<2>, true>() == 1));
    auto w = random_modint<WideResidue>();
    assert(w.value < WideResidue::mod());
    seen_wide |= w.value > ull(LLONG_MAX);
  }
  assert(seen_zero && seen_one && seen_wide);
  cout << "Hello World\n";
}
