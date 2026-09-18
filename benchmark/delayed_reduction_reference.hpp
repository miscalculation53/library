#pragma once
// Before the delayed-reduction experiment (2026-09-10), plus a suffix-only baseline.
#include "math/dot_product.hpp"
#include "math/prime/sieve/linear_sieve.hpp"
namespace delayed_before {
template<bool suffix=false,class S>
tuple<vvc<S>,int,S> eliminate(vvc<S> a,bool rref) {
  const int n=a.size(),m=n?a[0].size():0; int rk=0; S de=1;
  for(int i=0,j=0;i<n&&j<m;++j) {
    repi(k,i,n) if(a[k][j]!=S(0)) {swap(a[k],a[i]);if(k!=i)de=-de;break;}
    if(a[i][j]==S(0)) {de=0;continue;}
    de*=a[i][j]; S inv=a[i][j].inv();
    repi(l,suffix?j:0,m) a[i][l]*=inv;
    if(rref) repi(k,i) {
      S c=a[k][j];
      if constexpr(suffix) {if(c==S(0))continue;}
      repi(l,suffix?j:0,m) a[k][l]+=-(a[i][l]*c);
    }
    repi(k,i+1,n) {
      S c=a[k][j];
      if constexpr(suffix) {if(c==S(0))continue;}
      repi(l,suffix?j:0,m) a[k][l]+=-(a[i][l]*c);
    }
    ++i;++rk;
  }
  return {a,rk,de};
}
template<int kind,bool direct=false,class S>
vc<S> set_transform(const vc<S> &input) {
  auto a=input;
  const int n=a.size();
  for(int d=kind<4 && (kind&1)?n/2:1; kind<4 && (kind&1)?d>0:d<n; d=kind<4 && (kind&1)?d/2:d*2)
    for(int k=0;k<n;k+=2*d) repi(i,k,k+d) {
      if constexpr(kind==0) a[i+d]+=a[i];
      if constexpr(kind==1) { if constexpr(direct) a[i+d]-=a[i]; else a[i+d]+=-a[i]; }
      if constexpr(kind==2) a[i]+=a[i+d];
      if constexpr(kind==3) { if constexpr(direct) a[i]-=a[i+d]; else a[i]+=-a[i+d]; }
      if constexpr(kind==4) {S x=a[i]+a[i+d],y; if constexpr(direct) y=a[i]-a[i+d]; else y=a[i]+-a[i+d]; a[i]=x;a[i+d]=y;}
    }
  return a;
}
template<int kind,class S>
vc<S> prime_transform(const vc<S> &a) {
  const int n=int(a.size())-1;
  auto b=a;
  for(const auto &p:LinearSieve::primes(n)) {
    if constexpr(kind==0) for(int i=1;i*p<=n;++i) b[i*p]+=b[i];
    if constexpr(kind==1) for(int i=n/p;i>=1;--i) b[i*p]+=-b[i];
    if constexpr(kind==2) for(int i=n/p;i>=1;--i) b[i]+=b[i*p];
    if constexpr(kind==3) for(int i=1;i*p<=n;++i) b[i]+=-b[i*p];
  }
  return b;
}
}
