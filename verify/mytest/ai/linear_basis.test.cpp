#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/linear_basis.hpp"
#include "math/linalg/matrix.hpp"
#include "matroid/linear_matroid.hpp"

namespace
{
template <class T, class = void> struct has_coordinates : false_type {};
template <class T> struct has_coordinates<T, void_t<decltype(declval<T>().coordinates(declval<vc<typename T::S>>()))>> : true_type {};

template <class F>
int matrix_rank(int dimension, const vvc<typename F::S> &values)
{
  Matrix<F> matrix(dimension, values.size());
  for (int e = 0; e < int(values.size()); e++) for (int i = 0; i < dimension; i++)
    matrix[i][e] = values[e][i];
  return matrix.template rank<int>();
}

template <class F, class Tag>
void check_vector(const LinearBasis<F, true, Tag> &basis, const vc<typename F::S> &value)
{
  using S = typename F::S;
  auto coefficient = basis.coordinates(value);
  auto residual = basis.reduce(value);
  assert(basis.reduce(residual) == residual);
  assert(bool(coefficient) == basis.contains(value));
  auto augmented = basis.basis();
  augmented.push_back(value);
  assert(basis.contains(value) == (matrix_rank<F>(basis.dimension(), augmented) == basis.rank()));
  if (coefficient)
  {
    assert(int(coefficient->size()) == basis.rank());
    vc<S> reconstructed(basis.dimension(), F::e0());
    for (int j = 0; j < basis.rank(); j++) for (int i = 0; i < basis.dimension(); i++)
      reconstructed[i] = F::add(reconstructed[i], F::mul((*coefficient)[j], basis.basis()[j][i]));
    assert(reconstructed == value);
  }
}

template <uint P, class Tag = int>
void random_cases()
{
  using S = static_modint32<P>;
  using F = FieldAddSubMulDiv<S>;
  static_assert(has_coordinates<LinearBasis<F, true, Tag>>::value);
  static_assert(!has_coordinates<LinearBasis<F, false, Tag>>::value);
  mt19937_64 rng(2026110200ULL + P);
  for (int trial = 0; trial < 100; trial++)
  {
    int d = rng() % 9;
    LinearBasis<F, true, Tag> tracked(d);
    LinearBasis<F, false, Tag> plain(d), other(d);
    vvc<S> accepted, all;
    for (int step = 0; step < 20; step++)
    {
      vc<S> value(d);
      for (auto &x : value) x = S(rng() % P);
      all.push_back(value);
      bool inserted = tracked.insert(value);
      assert(plain.insert(value) == inserted);
      if (inserted) accepted.push_back(value);
      assert(tracked.basis() == accepted && plain.basis() == accepted);
      assert(tracked.rank() == matrix_rank<F>(d, all));
      assert(plain.rank() == tracked.rank());
      check_vector(tracked, value);
      vc<S> query(d);
      for (auto &x : query) x = S(rng() % P);
      check_vector(tracked, query);
      if (step % 3 == 0) other.insert(value);
    }
    auto copied = tracked;
    copied.merge(copied);
    assert(copied.basis() == tracked.basis());
    auto united = tracked | other;
    auto common = tracked & other;
    assert(common.rank() + united.rank() == tracked.rank() + other.rank());
    for (const auto &value : common.basis()) assert(tracked.contains(value) && other.contains(value));
    for (const auto &value : other.basis()) assert(common.contains(value));
    LinearBasis<F, true, Tag> moved = std::move(copied);
    assert(moved.basis() == tracked.basis());
    tracked.clear();
    assert(tracked.rank() == 0 && tracked.dimension() == d);
    assert(tracked.coordinates(vc<S>(d, F::e0())) == optional<vc<S>>(vc<S>{}));
  }
}

template <class Tag = int>
void exhaustive_intersection()
{
  using S = static_modint32<3>;
  using F = FieldAddSubMulDiv<S>;
  mt19937_64 rng(2026110201);
  for (int trial = 0; trial < 100; trial++)
  {
    int d = rng() % 6;
    LinearBasis<F, false, Tag> a(d);
    LinearBasis<F, true, Tag> b(d);
    for (int j = 0; j < 8; j++)
    {
      vc<S> value(d);
      for (auto &x : value) x = rng() % 3;
      if (rng() % 2) a.insert(value); else b.insert(value);
    }
    auto intersection = a & b, united = a | b;
    assert(intersection.rank() + united.rank() == a.rank() + b.rank());
    int count = 1;
    for (int i = 0; i < d; i++) count *= 3;
    for (int code = 0; code < count; code++)
    {
      int rest = code;
      vc<S> value(d);
      for (auto &x : value) x = rest % 3, rest /= 3;
      assert(intersection.contains(value) == (a.contains(value) && b.contains(value)));
    }
    auto reverse = b & a;
    for (const auto &value : reverse.basis()) assert(intersection.contains(value));
    a.merge(b);
    assert(a.rank() == united.rank());
  }
}

void matroid_order()
{
  using S = modint998244353;
  using F = FieldAddSubMulDiv<S>;
  LinearMatroid<F> m(3, {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {3, 2, 1}, {0, 0, 0}});
  m.build({2, 0, 1});
  assert(m.circuit(3) == optional<vc<int>>(vc<int>{2, 0, 1}));
  assert(m.circuit(4) == optional<vc<int>>(vc<int>{}));
  m.build({2});
  assert(!m.circuit(0));
  LinearMatroid<F> empty(0, {{}, {}});
  assert(empty.circuit(1) == optional<vc<int>>(vc<int>{}));
}
} // namespace

int main()
{
  random_cases<2>();
  random_cases<3>();
  random_cases<998244353>();
  random_cases<2, void>();
  random_cases<3, void>();
  random_cases<998244353, void>();
  exhaustive_intersection<>();
  exhaustive_intersection<void>();
  matroid_order();
  PRINT("Hello World");
}
