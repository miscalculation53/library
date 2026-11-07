#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/convolution/convolution_ntt.hpp"
#include "math/fps/factorial.hpp"
static_assert(ntt_ok<modint998244353>(1 << 22));
static_assert(!ntt_ok<modint1000000007>(1 << 22));
template<class Mint> void check() {
  std::mt19937 rng(321);
  for (int n : {0, 1, 7, 61, 127, 128, 129, 600}) for(int m : {0, 2, 61, 129}) {
    vc<Mint> a(n), b(m);
    for(auto &v:a) v = rng()%100;
    for(auto &v:b) v = rng()%100;
    auto want = (n == 0 || m == 0) ? vc<Mint>{} : internal::convolution_naive(a,b);
    assert(ntt_convolution(a,b) == want);
    assert(convolution(a,b) == want);
  }
}
int main() {
 check<modint998244353>();
 check<static_modint32<167772161>>();
 for(int p : {2, 17, 97}) {
   using mint = dynamic_modint32<20>;
   mint::set_mod(p);
   Factorial<mint> a;
   Factorial<mint> b(-1);
   mint expected=1;
   for(int n=0;n<=p+2;++n) {
     assert(a.factorial(n)==expected && b.factorial(n)==expected);
     expected*=n+1;
   }
 }
 using mint=modint998244353;
 Factorial<mint> a,b(-1);
 for(ll n : {0LL,1LL,511LL,512LL,1000LL,100000000LL,998244352LL,998244353LL}) assert(a.factorial(n)==b.factorial(n));
 std::cout << "Hello World\n";
}
