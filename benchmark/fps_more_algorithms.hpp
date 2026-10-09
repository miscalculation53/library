#pragma once
#include "fps_algorithm_candidates.hpp"
#include "math/fps/interpolation.hpp"
namespace fps_algorithm_audit
{
// Block Newton: compute the low s blocks, then update only the required high blocks.
template<class mint> P<mint> inv_block(const P<mint>& f,int n,int blocks,bool partial=true)
{
  using F=P<mint>;
  assert(n>=0 && f.get(0)!=0);
  if(!n)return{};if(n==1)return{f[0].inv()};
  int N=bit_ceil(n),s=min(blocks,max(1,N/2)),m=N/(2*s);
  int r=partial ? (n+m-1)/m : 2*s,h=r-s;
  vc<F> ff(r),g(s),gf(s),df(h);
  g[0]=f.resized(m).inv(m); gf[0]=g[0].resized(2*m);forward(gf[0]);
  repi(k,r){ff[k].resize(2*m);repi(i,m)ff[k][i]=f.get(k*m+i);forward(ff[k]);}
  repi(k,1,s)
  {
    F v=block_product(ff,gf,k,m,true).resized(2*m);forward(v);
    repi(i,2*m)v[i]*=gf[0][i];backward(v);v.resize(m);
    mint iz=-mint(2*m).inv();for(auto& x:v)x*=iz;
    g[k]=std::move(v);gf[k]=g[k].resized(2*m);forward(gf[k]);
  }
  repi(k,h){df[k]=-block_product(ff,gf,s+k,m,true);df[k].resize(2*m);forward(df[k]);}
  F result(r*m);
  repi(k,s)copy(g[k].begin(),g[k].end(),result.begin()+k*m);
  repi(k,h){F v=block_product(df,gf,k,m,true);copy(v.begin(),v.end(),result.begin()+(s+k)*m);}
  result.resize(n);return result;
}

// Harvey's block square root; f[0]=1 and the returned root starts with 1.
template<class mint> P<mint> sqrt_block(const P<mint>& f,int n,int blocks,bool partial=true)
{
  using F=P<mint>;using Seed=AuditFPS<mint,79>;
  assert(n>=0 && f.get(0)==1);if(!n)return{};
  int N=bit_ceil(n),s=min(blocks,N),m=N/s,r=partial ? (n+m-1)/m : s;
  auto [ok,seed]=Seed(f.begin(),f.begin()+min(f.sz(),m)).sqrt(m);assert(ok);
  F g(seed.begin(),seed.end());if(g[0]!=1)g=-g;
  if(r==1)return g.resized(n);
  F u=(g.inv(m)*mint(2).inv()).resized(2*m);forward(u);
  vc<F> gf(r);F result(r*m);copy(g.begin(),g.end(),result.begin());
  mint iz=mint(2*m).inv();
  repi(k,1,r)
  {
    gf[k-1]=g.resized(2*m);forward(gf[k-1]);
    F v=block_product(gf,gf,k,m,true);
    repi(i,m)v[i]=f.get(k*m+i)-v[i];v.resize(2*m);forward(v);
    repi(i,2*m)v[i]*=u[i];backward(v);v.resize(m);for(auto& x:v)x*=iz;
    g=std::move(v);copy(g.begin(),g.end(),result.begin()+k*m);
  }
  result.resize(n);return result;
}

template <class mint>
P<mint> exp_partial(const P<mint> &f, int n, int blocks, bool delayed = true)
{
  using F = P<mint>;
  assert(n >= 0 && f.get(0) == 0);
  if (n == 0) return {};
  int N = bit_ceil(n), s = min(blocks, max(1, N / 2)), m = N / (2 * s);
  if (N == 1) return {1};
  assert(has_single_bit(unsigned(s)) && ntt_ok<mint>(2 * m));
  Binomial<mint>::reserve(N - 1);
  int r=(n+m-1)/m, high=r-s;
  vc<F> g(s), gf(s), q(r), qf(r), ef(high);
  g[0] = exp_bs(f.resized(m), m);
  F u = g[0].inv(m).resized(2 * m);
  forward(u);
  gf[0] = g[0].resized(2 * m);
  forward(gf[0]);
  repi(k, s)
  {
    q[k].resize(m);
    repi(i, m) q[k][i] = mint(k * m + i) * f.get(k * m + i);
    qf[k] = q[k].resized(2 * m);
    forward(qf[k]);
  }
  repi(k, 1, s)
  {
    F phi = block_product(gf, qf, k, m, delayed).resized(2 * m);
    forward(phi);
    repi(i, 2 * m) phi[i] *= u[i];
    backward(phi);
    phi.resize(m);
    mint iz = mint(2 * m).inv();
    repi(i, m) phi[i] *= iz * Binomial<mint>::inv_[k * m + i];
    phi.resize(2 * m);
    forward(phi);
    repi(i, 2 * m) phi[i] *= gf[0][i];
    backward(phi);
    phi.resize(m);
    for (auto &v : phi) v *= iz;
    g[k] = std::move(phi);
    gf[k] = g[k].resized(2 * m);
    forward(gf[k]);
  }
  repi(k, s, r)
  {
    F value = block_product(qf, gf, k, m, delayed).resized(2 * m);
    forward(value);
    repi(i, 2 * m) value[i] *= u[i];
    backward(value);
    value.resize(m);
    mint iz = -mint(2 * m).inv();
    for (auto &v : value) v *= iz;
    q[k] = std::move(value);
    qf[k] = q[k].resized(2 * m);
    forward(qf[k]);
  }
  repi(k, high)
  {
    ef[k].resize(2 * m);
    repi(i, m) ef[k][i] = q[s+k][i] * Binomial<mint>::inv_[(s+k) * m + i] - f.get((s+k) * m + i);
    forward(ef[k]);
  }
  F result(r*m);
  repi(k, s)
  {
    copy(g[k].begin(), g[k].end(), result.begin() + k * m);
  }
  repi(k, high)
  {
    F value = block_product(gf, ef, k, m, delayed);
    repi(i, m) result[(s+k) * m + i] = -value[i];
  }
  result.resize(n);
  return result;
}

template<class mint> P<mint> interpolation_shared(const vc<mint>& xs,const vc<mint>& ys)
{
  using F=P<mint>;assert(xs.size()==ys.size());int n=xs.size();if(!n)return{};
  TransposedEvaluation<mint> tree(xs,true);
  F monic=tree.product[1].resized(n+1).rev();
  auto weights=tree.evaluate(monic.diff());
  auto inverses=inv_many<FieldAddSubMulDiv<mint>>(weights);
  vc<F> value(2*tree.base);
  repi(i,tree.base)value[tree.base+i]={i<n ? ys[i]*inverses[i] : mint(0)};
  for(int i=tree.base-1;i>0;--i)
  {
    int k=tree.product[2*i].sz()-1,z=2*k;
    if(!tree.spectrum[2*i].empty())
    {
      F a=value[2*i].resized(z),b=value[2*i+1].resized(z);forward(a),forward(b);
      repi(j,z)a[j]=a[j]*tree.spectrum[2*i+1][j]+b[j]*tree.spectrum[2*i][j];
      backward(a);mint iz=mint(z).inv();for(auto& x:a)x*=iz;value[i]=std::move(a);
    }
    else value[i]=value[2*i]*tree.product[2*i+1]+value[2*i+1]*tree.product[2*i];
    F().swap(value[2*i]);F().swap(value[2*i+1]);
  }
  return value[1].resized(n).rev();
}
} // namespace fps_algorithm_audit
