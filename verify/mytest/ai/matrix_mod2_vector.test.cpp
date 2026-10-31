#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/linalg/matrix_mod2.hpp"
#include "math/linalg/matrix_mod2_dynamic.hpp"

template <size_t N, size_t C> void check(mt19937 &rng)
{
  MatrixMod2<N, C> a;
  MatrixMod2Dynamic d(N, C);
  bitset<N> row;
  bitset<C> column;
  DynamicBitset vr(N), vc_(C);
  for (size_t i = 0; i < N; ++i)
  {
    if (rng() % 2)
      row[i] = 1, vr.set(i);
    for (size_t j = 0; j < C; ++j)
      if (rng() % 2)
        a(i, j) = 1, d(i, j) = 1;
  }
  for (size_t j = 0; j < C; ++j)
    if (rng() % 2)
      column[j] = 1, vc_.set(j);
  auto x = row * as_const(a);
  auto y = as_const(a) * column;
  static_assert(is_same_v<decltype(x), bitset<C>>);
  static_assert(is_same_v<decltype(y), bitset<N>>);
  auto dx = vr * as_const(d), dy = as_const(d) * vc_;
  assert(dx.size() == C && dy.size() == N);
  for (size_t j = 0; j < C; ++j)
  {
    bool expected = false;
    for (size_t i = 0; i < N; ++i)
      expected ^= row[i] && a(i, j);
    assert(x[j] == expected && dx[j] == expected);
  }
  for (size_t i = 0; i < N; ++i)
  {
    bool expected = false;
    for (size_t j = 0; j < C; ++j)
      expected ^= a(i, j) && column[j];
    assert(y[i] == expected && dy[i] == expected);
  }
  MatrixMod2<1, N> r;
  r[0] = row;
  MatrixMod2<C, 1> c;
  for (size_t j = 0; j < C; ++j)
    c(j, 0) = column[j];
  assert((r * a)[0] == x);
  auto ac = a * c;
  for (size_t i = 0; i < N; ++i)
    assert(ac(i, 0) == y[i]);
  assert(a.rank() == d.rank());
  for (bool rref : {false, true})
    assert(a.row_reduction(rref).template to_vvi<int>() ==
           d.row_reduction(rref).template to_vvi<int>());
}

template <size_t N> void invertible(mt19937 &rng)
{
  MatrixMod2<N> a(1);
  MatrixMod2Dynamic d(N, N, 1);
  if constexpr (N > 1)
    for (int t = 0; t < 10 * int(N); ++t)
    {
      int i = rng() % N, j = rng() % (N - 1);
      if (j >= i)
        ++j;
      a[i] ^= a[j];
      d[i] ^= d[j];
    }
  assert(a.det() == 1 && d.det() == 1);
  auto ai = a.inv();
  auto di = d.inv();
  assert(ai && di);
  assert((a * *ai) == MatrixMod2<N>(1));
  assert(ai->template to_vvi<int>() == di->template to_vvi<int>());
  bitset<N> b;
  DynamicBitset bd(N);
  for (size_t j = 0; j < N; ++j)
    if (rng() % 2)
      b[j] = 1, bd.set(j);
  auto s = a.solve(b);
  auto sd = d.solve(bd);
  assert(s && sd && s->second.empty() && sd->second.empty());
  assert(a * s->first == b && d * sd->first == bd);
  if constexpr (N > 0)
  {
    a[0].reset();
    d[0].reset();
    assert(!a.inv() && !d.inv());
  }
}

int main()
{
  mt19937 rng(20261027);
  repi(t, 5)
  {
    check<0, 0>(rng);
    check<0, 64>(rng);
    check<64, 0>(rng);
    check<1, 1>(rng);
    check<2, 3>(rng);
    check<5, 3>(rng);
    check<63, 64>(rng);
    check<64, 65>(rng);
    check<65, 63>(rng);
    check<65, 129>(rng);
    check<129, 65>(rng);
    check<1, 129>(rng);
  }
  invertible<0>(rng);
  invertible<1>(rng);
  invertible<5>(rng);
  invertible<63>(rng);
  invertible<64>(rng);
  invertible<65>(rng);
  invertible<129>(rng);
  MatrixMod2<3> fixed(vc<string>{"110", "011", "101"});
  auto expected = fixed[0] * fixed;
  fixed[0] = fixed[0] * fixed;
  assert(fixed[0] == expected);
  MatrixMod2Dynamic dynamic(vc<string>{"110", "011", "101"});
  auto de = dynamic[0] * dynamic;
  dynamic[0] = dynamic[0] * dynamic;
  assert(dynamic[0] == de);
  DynamicBitset v = dynamic[0];
  auto column = dynamic * v;
  dynamic[0] = dynamic * dynamic[0];
  assert(dynamic[0] == column);
  cout << "Hello World\n";
}
