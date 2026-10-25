#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "algo/subset_sum.hpp"
#include "flow/bounded_maxflow.hpp"
#include "flow/min_cost_b_flow.hpp"
#include "flow/project_selection.hpp"
#include "graph/cycle_detection.hpp"
#include "math/crt.hpp"
#include "math/fps/half_gcd.hpp"
#include "math/linalg/dynamic_matrix_mod2.hpp"
#include "math/linalg/matrix.hpp"
#include "math/linalg/matrix_array.hpp"
#include "math/linalg/matrix_mod2.hpp"
#include "math/modint/modint_div.hpp"
#include "math/two_square_sum.hpp"

using mint = modint998244353;
using F = FormalPowerSeries<mint>;
using TestField = FieldAddSubMulDiv<mint>;

// 値が 0 または空でも成功として保持し、解なしと区別する。
void test_scalars_and_sequences()
{
  const auto zero_root = sqrt_mod(mint(0));
  static_assert(is_same_v<decay_t<decltype(zero_root)>, optional<mint>>);
  assert(zero_root && *zero_root == 0);
  assert(!sqrt_mod(mint(3)));
  assert(ModintDiv<mint>(mint(7), 1).to_modint() == mint(0));
  assert(ModintDiv<mint>(mint(7), 0).to_modint() == mint(7));
  assert(!ModintDiv<mint>(mint(7), -1).to_modint());

  const auto square_sum = two_square_sum_prime(2LL);
  static_assert(is_same_v<decay_t<decltype(square_sum)>, optional<pair<ll, ll>>>);
  assert(square_sum && square_sum->first == 1 && square_sum->second == 1);
  assert(!two_square_sum_prime(3LL));

  SubsetSumFromFrequency frequency(vc<int>{}, 0);
  const auto counts = frequency.answer(0);
  assert(counts && counts->empty() && !frequency.answer(1));
  SubsetSum subset(vc<int>{}, 0);
  const auto chosen = subset.answer(0);
  assert(chosen && chosen->empty() && !subset.answer(1));

  assert(!F{}.nz_front());
  const auto first = F{0, 3}.nz_front();
  static_assert(is_same_v<decay_t<decltype(first)>, optional<pair<int, mint>>>);
  assert(first && first->first == 1 && first->second == 3);
  const auto dense = F{}.sqrt(0), sparse = F{}.sqrt_sparse(0);
  assert(dense && dense->empty() && sparse && sparse->empty());
  assert(!F({0, 1}).sqrt(4) && !F({0, 1}).sqrt_sparse(4));
  const auto inverse = polynomial_inv_mod(F{}, F{7});
  assert(inverse && inverse->empty());
  assert(!polynomial_inv_mod(F{}, F{0, 1}));
}

void test_crt()
{
  const auto result = crt2<ll>(2, 3, 3, 5);
  static_assert(is_same_v<decay_t<decltype(result)>, optional<pair<ll, ll>>>);
  assert(result && result->first == 8 && result->second == 15);
  constexpr auto divisible = crt2<ll>(2, 2, 6, 3);
  static_assert(divisible && divisible->first == 2 && divisible->second == 6);
  static_assert(!crt2<ll>(0, 1, 2, 2));
  constexpr auto empty = crt(array<int, 0>{}, array<int, 0>{});
  static_assert(empty && empty->first == 0 && empty->second == 1);
  const auto combined = crt(vc<int>{2, 3}, vc<int>{3, 5});
  assert(combined == result);
  assert(!crt(vc<int>{0, 1}, vc<int>{2, 2}));
}

template <class M, class V>
void check_empty_matrix(const M &a, const V &b)
{
  const auto inverse = a.inv();
  static_assert(is_same_v<decay_t<decltype(inverse)>, optional<M>>);
  assert(inverse && inverse->empty());
  const auto solution = a.solve(b);
  static_assert(is_same_v<decay_t<decltype(solution)>, optional<pair<typename M::V, vc<typename M::V>>>>);
  assert(solution && solution->second.empty());
}

void test_matrices()
{
  check_empty_matrix(Matrix<TestField>(0, 0), vc<mint>{});
  check_empty_matrix(DynamicMatrixMod2(0, 0), DynamicBitset(0));
  check_empty_matrix(MatrixMod2<8>(0, 0), bitset<8>{});
  const MatrixArray<TestField, 0, 0> a;
  const auto inverse = a.inv();
  static_assert(is_same_v<decay_t<decltype(inverse)>, optional<MatrixArray<TestField, 0, 0>>>);
  assert(inverse);
  const auto solution = a.solve(array<mint, 0>{});
  static_assert(is_same_v<decay_t<decltype(solution)>, optional<pair<array<mint, 0>, vc<array<mint, 0>>>>>);
  assert(solution && solution->first.empty() && solution->second.empty());
}

void test_graphs_and_flows()
{
  assert(!cycle_detection(GraphDirected<>(0, vc<pair<int, int>>{})));
  const auto cycle = cycle_detection(GraphDirected<>(1, vc<pair<int, int>>{{0, 0}}));
  assert(cycle && cycle->size() == 1 && cycle->front().index == 0);
  assert(ProjectSelection<int>(0).solve() == 0);
  BoundedMaxFlow<int> bounded(2);
  assert(bounded.flow(0, 1) == 0 && bounded.min_flow(0, 1) == 0);
  BoundedMaxFlow<int> impossible(3);
  impossible.add_edge(0, 1, 1, 1);
  assert(!impossible.flow(0, 2) && !impossible.min_flow(0, 2));
  MinCostBFlow<int, int> empty(0), inconsistent(vc<int>{1});
  assert(empty.flow() == 0 && !inconsistent.flow());
}

int main()
{
  test_scalars_and_sequences();
  test_crt();
  test_matrices();
  test_graphs_and_flows();
  cout << "Hello World\n";
}
