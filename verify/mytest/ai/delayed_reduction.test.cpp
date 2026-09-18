#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#ifndef DELAYED_REDUCTION_ARRAY_ONLY
#include "math/linalg/matrix.hpp"
#endif
#include "math/linalg/matrix_array.hpp"
#include "math/set/zeta_mobius.hpp"
#include "math/set/xor_convolution.hpp"
#include "math/prime/sieve/zeta_mobius_divisor_multiple.hpp"
#include <random>

mt19937 gen(20260910);

template <class F>
tuple<vvc<typename F::S>, int, typename F::S> row_reference(vvc<typename F::S> a, bool rref)
{
  using S = typename F::S;
  int n = a.size(), m = n ? a[0].size() : 0, rk = 0;
  S de = F::e1();
  for (int j = 0; j < m && rk < n; ++j)
  {
    int p = rk;
    while (p < n && a[p][j] == F::e0()) ++p;
    if (p == n) { de = F::e0(); continue; }
    if (p != rk) { swap(a[p], a[rk]); de = F::minus(de); }
    de = F::mul(de, a[rk][j]);
    S inv = F::inv(a[rk][j]);
    for (auto &x : a[rk]) x = F::mul(x, inv);
    repi(i, n) if (i != rk && (rref || i > rk))
    {
      S c = a[i][j];
      repi(k, m) a[i][k] = F::add(a[i][k], F::minus(F::mul(c, a[rk][k])));
    }
    ++rk;
  }
  return {a, rk, de};
}

template <class S>
vvc<S> make_matrix(int n, int m, int mode)
{
  vvc<S> a(n, vc<S>(m));
  repi(i, n) repi(j, m)
  {
    if (mode == 1) a[i][j] = (i + 1) % max(1, n) == j;
    if (mode == 2) a[i][j] = -1;
    if (mode >= 3) a[i][j] = gen();
    if (mode == 4 && (i % 3 == 0 || j % 4 == 0)) a[i][j] = 0;
    if (mode == 5 && i > 0 && i % 2) a[i][j] = a[i - 1][j];
    if (mode == 6)
    {
      // 最下行の末尾に (mod-1)^2 を蓄積し、4/16 回の上限と直後の正規化を通す。
      a[i][j] = (i == j);
      if (j == m - 1) a[i][j] = -1;
      if (i == n - 1 && j < min(n - 1, m - 1)) a[i][j] = 1;
    }
  }
  return a;
}

template <class F>
void test_rows()
{
  using S = typename F::S;
  for (auto [n, m] : vc<pair<int, int>>{{0,0}, {5,0}, {0,5}, {1,1}, {15,33}, {31,32}, {32,31},
      {32,32}, {33,65}, {65,33}, {65,65}, {129,131}})
    repi(mode, 7) for (bool rref : {false, true})
    {
      auto a = make_matrix<S>(n, m, mode);
      auto expected = row_reference<F>(a, rref);
      if constexpr (internal::ordinary_mod32_field<F>::value)
      {
        auto b = a;
        auto [rk, de] = internal::row_reduction_mod32<S>(b, n, m, rref);
        assert(b == get<0>(expected) && rk == get<1>(expected) && de == get<2>(expected));
      }
#ifndef DELAYED_REDUCTION_ARRAY_ONLY
      Matrix<F> b(a);
      auto [got, rk, de] = b.template row_reduction<int>(rref);
      assert(got == get<0>(expected) && rk == get<1>(expected) && de == get<2>(expected));
#endif
    }
}

template <class F, int n, int m>
void test_array()
{
  repi(mode, 7) for (bool rref : {false, true})
  {
    auto a = make_matrix<typename F::S>(n, m, mode);
    auto [want, rank, det] = row_reference<F>(a, rref);
    MatrixArray<F,n,m> b;
    repi(i,n) repi(j,m) b[i][j] = a[i][j];
    auto [got, rk, de] = b.template row_reduction<int>(rref);
    assert(rk == rank && de == det);
    repi(i,n) repi(j,m) assert(got[i][j] == want[i][j]);
  }
}

template <class G>
void test_sets()
{
  using S = typename G::S;
  for (int n : {0,1,2,8,32,512,1024,2048})
  {
    vc<S> a(n), z(n,G::e()), zs(n,G::e()), m(n,G::e()), ms(n,G::e()), h(n,G::e());
    for (auto &x : a) x = gen();
    repi(i,n) repi(j,n)
    {
      S sign = popcount(unsigned(i ^ j)) % 2 ? G::inv(a[j]) : a[j];
      if ((i & j) == j) { z[i] = G::op(z[i],a[j]); m[i] = G::op(m[i],sign); }
      if ((i & j) == i) { zs[i] = G::op(zs[i],a[j]); ms[i] = G::op(ms[i],sign); }
      h[i] = G::op(h[i],popcount(unsigned(i & j)) % 2 ? G::inv(a[j]) : a[j]);
    }
    assert(zeta_subset<G>(a) == z && zeta_supset<G>(a) == zs);
    assert(mobius_subset<G>(a) == m && mobius_supset<G>(a) == ms);
    assert(hadamard_abel_group<G>(a) == h);
    assert(mobius_subset<G>(z) == a && mobius_supset<G>(zs) == a);
  }
}

template <class G>
void test_primes()
{
  using S = typename G::S;
  for (int n : {0,1,2,32,257,1025})
  {
    vc<S> a(n); for (auto &x:a) x = gen();
    auto z=a, zs=a, m=a, ms=a;
    repi(i,1,n) for (int j=2*i;j<n;j+=i)
    {
      z[j] = G::op(z[j],a[i]); zs[i] = G::op(zs[i],a[j]);
      m[j] = G::op(m[j],G::inv(m[i]));
    }
    for (int i=n-1;i>=1;--i) for (int j=2*i;j<n;j+=i)
      ms[i] = G::op(ms[i],G::inv(ms[j]));
    assert(zeta_divisor<G>(a)==z && zeta_multiple<G>(a)==zs);
    assert(mobius_divisor<G>(a)==m && mobius_multiple<G>(a)==ms);
    assert(mobius_divisor<G>(z)==a && mobius_multiple<G>(zs)==a);
  }
}

using mint = modint998244353;
struct ShiftedGroup
{
  using S=mint;
  static S op(S a,S b) { return a+b+1; }
  static S e() { return -1; }
  static S inv(S a) { return -a-2; }
};
struct CustomField : FieldAddSubMulDiv<mint>
{
  static inline int calls=0;
  static mint mul(mint a,mint b) { ++calls; return a*b; }
};

template <class F>
void test_array_operations()
{
  using S=typename F::S;
  using A=MatrixArray<F,33,33>;
  A a(S(1));
  repi(i,33) repi(j,i+1,33) a[i][j]=gen();
  auto [ok,inv]=a.inv();
  assert(ok && a*inv==A::e1());
  array<S,33> x; for(auto &v:x) v=gen();
  auto b=a*x;
  auto [solvable,sol,basis]=a.solve(b);
  assert(solvable && sol==x && basis.empty());
  a[32]=a[0]; b=a*x;
  auto [consistent,particular,kernel]=a.solve(b);
  assert(consistent && a*particular==b && kernel.size()==1);
  assert((a*kernel[0]==array<S,33>{}));
  b[32]+=S(1);
  assert(!get<0>(a.solve(b)));
}

template <class S>
void test_mod()
{
  using F=FieldAddSubMulDiv<S>;
  test_rows<F>();
  test_array_operations<F>();
  test_array<F,0,0>(); test_array<F,3,0>(); test_array<F,0,3>();
  test_array<F,31,33>(); test_array<F,33,65>(); test_array<F,65,33>();
  test_sets<GroupAddSub<S>>();
  test_primes<GroupOfRingAdd<F>>();
  static_assert(internal::ordinary_mod32_add_group<GroupOfRingAdd<F>>::value);
  static_assert(internal::ordinary_mod32_add_group<GroupOfRingAdd<RingAddSubMul<S>>>::value);
  // 上限付近の係数を、多数の段にわたって積み重ねる。
  vc<S> a(1<<17,S(-1)); auto b=hadamard_abel_group<GroupOfRingAdd<F>>(a);
  assert(b[0]==-S(a.size())); repi(i,1,b.size()) assert(b[i]==S(0));
  repi(i,a.size()) a[i]=i%2?S(-1):S(0);
  b=hadamard_abel_group<GroupOfRingAdd<F>>(a);
  assert(b[0]==-S(a.size()/2) && b[1]==S(a.size()/2));
  repi(i,2,b.size()) assert(b[i]==S(0));
  if (S::mod()!=2) assert(hadamard_field<F>(b,true)==a);
}

int main()
{
  test_mod<mint>();
  test_mod<static_modint32<2147483647>>();
  test_mod<static_modint32<2>>();
  using D=dynamic_modint32<73>;
  for (int mod : {17,998244353,2147483647}) { D::set_mod(mod); test_mod<D>(); }
  test_sets<GroupAddSub<static_modint32<1073741824>>>();
  test_sets<GroupAddSub<static_modint32<2147483646>>>();
  test_sets<GroupAddSub<static_modint64<998244353>>>();
  test_sets<ShiftedGroup>(); test_primes<ShiftedGroup>();
  test_rows<CustomField>(); test_array<CustomField,33,33>(); assert(CustomField::calls>0);
  static_assert(!internal::ordinary_mod32_add_group<ShiftedGroup>::value);
  static_assert(!internal::ordinary_mod32_field<CustomField>::value);
  static_assert(!internal::ordinary_mod32<static_modint64<998244353>>::value);
  vc<ll> a{1,2,3,4};
  assert((mobius_subset<GroupAddSub<ll>>(a)==vc<ll>{1,1,2,0}));
  assert((hadamard_abel_group<GroupAddSub<ll>>(a)==vc<ll>{10,-2,-4,0}));
  cout << "Hello World\n";
}
