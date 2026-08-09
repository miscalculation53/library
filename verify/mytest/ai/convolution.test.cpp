#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "math/convolution/convolution.hpp"

template <class mint>
vc<mint> naive_convolution(const vc<mint> &a, const vc<mint> &b)
{
  if (a.empty() || b.empty())
    return {};
  vc<mint> c(a.size() + b.size() - 1);
  repi(i, a.size()) repi(j, b.size()) c[i + j] += a[i] * b[j];
  return c;
}

// Test focus: convolution dispatch for modint representations not used by Yosupo.
template <class mint>
void test_convolution()
{
  vc<mint> a(70), b(75);
  repi(i, a.size()) a[i] = mint((ll)(i + 1) * (i + 3) + 1);
  repi(i, b.size()) b[i] = mint((ll)(i + 2) * (i + 5) + 1);
  assert(convolution(a, b) == naive_convolution(a, b));
}

// Test focus: the exact NTT-length predicate and the size-two arbitrary-modulus path.
void test_ntt_length()
{
  assert(!ntt_ok<modint998244353>(0));
  assert(ntt_ok<modint998244353>(4));
  assert(ntt_ok<modint1000000007>(2));
  assert(!ntt_ok<modint1000000007>(4));

  vc<modint1000000007> a = {3, 5};
  ntt(a);
  assert(a == vc<modint1000000007>({8, -2}));
  intt(a);
  assert(a == vc<modint1000000007>({6, 10}));
}

int main()
{
  test_ntt_length();
  // Test focus: modint representations not exercised by the Yosupo convolution tests.
  test_convolution<static_modint32<2013265921>>();
  test_convolution<static_modint64<998244353LL>>();

  cout << "Hello World" << endl;
}
