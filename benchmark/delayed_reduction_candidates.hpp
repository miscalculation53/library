#pragma once
#include "math/dot_product.hpp"
#include "math/prime/sieve/linear_sieve.hpp"
// Experiments retained for comparison; these 64-bit zeta/Mobius variants are not dispatched by the library.
namespace delayed_candidate {
// kind: 0 subset zeta, 1 subset mobius, 2 superset zeta, 3 superset mobius, 4 Hadamard.
template<int kind,class S>
vc<S> set_transform(const vc<S> &a) {
  const int n=a.size(); vc<ll> b(n);
  repi(i,n) b[i]=a[i].val();
  for(int d=kind<4 && (kind&1)?n/2:1; kind<4 && (kind&1)?d>0:d<n; d=kind<4 && (kind&1)?d/2:d*2)
    for(int k=0;k<n;k+=2*d) repi(i,k,k+d) {
      if constexpr(kind==0) b[i+d]+=b[i];
      if constexpr(kind==1) b[i+d]-=b[i];
      if constexpr(kind==2) b[i]+=b[i+d];
      if constexpr(kind==3) b[i]-=b[i+d];
      if constexpr(kind==4) {ll x=b[i],y=b[i+d];b[i]=x+y;b[i+d]=x-y;}
    }
  vc<S> res(n); repi(i,n) res[i]=S(b[i]); return res;
}
template<int kind,class S>
vc<S> prime_transform(const vc<S> &a) {
  const int n=int(a.size())-1;
  vc<ll> b(a.size()); repi(i,a.size()) b[i]=a[i].val();
  for(int p:LinearSieve::primes(n)) {
    for(int i=kind==1||kind==2?n/p:1;kind==1||kind==2?i>=1:i<=n/p; i+=kind==1||kind==2?-1:1) {
      if constexpr(kind==0) b[i*p]+=b[i];
      if constexpr(kind==1) b[i*p]-=b[i];
      if constexpr(kind==2) b[i]+=b[i*p];
      if constexpr(kind==3) b[i]-=b[i*p];
    }
  }
  vc<S> res(a.size()); repi(i,a.size()) res[i]=S(b[i]); return res;
}
}
