#include "fps_algorithm_candidates.hpp"
#include "math/fps/multipoint_evaluation.hpp"
using namespace fps_algorithm_audit;

template<class A,class B> void same(const A& a,const B& b,const string& label)
{
  if(a.size()!=b.size()) throw runtime_error(label+": size");
  repi(i,a.size()) if(a[i]!=b[i]) throw runtime_error(label+": coefficient "+to_string(i));
}
template<class M> P<M> naive_exp(const P<M>& h,int n)
{
  P<M> r(n); if(n) r[0]=1;
  repi(k,1,n) { repi(i,1,k+1) r[k]+=M(i)*h.get(i)*r[k-i]; r[k]/=M(k); }
  return r;
}
template<class M, bool native> void check()
{
  mt19937 rng(714);
  for(int n : {0,1,2,3,4,5,7,8,9,15,16,17,31,32,33,63,64,65,127,128,129,255,256,257,511,512,513,1024})
    for(int layout=0;layout<4;++layout)
    {
      P<M> h(n + (layout==3 ? 7 : 0));
      repi(i,1,h.sz()) if(layout==0 || layout==3 || (layout==1 && i%7==1) || (layout==2 && i>n/2)) h[i]=rng();
      if(layout==1) h.resize(min(h.sz(),max(1,n/3)));
      auto expected=naive_exp(h,n);
      same(expected,exp_bs_convolution(h,n),"bs convolution n="+to_string(n));
      if constexpr(native)
      {
        for(bool split:{false,true}) for(bool inv:{false,true})
          same(expected,exp_bs(h,n,split,inv),"bs n="+to_string(n));
        same(expected,exp_harvey(h,n,4,false),"Harvey scalar");
        for(int s:{1,2,4,8,16,32}) same(expected,exp_harvey(h,n,s),"Harvey n="+to_string(n)+" s="+to_string(s));
      }
    }
  for(int n:{0,1,2,9,31,64,65,127,128,129,255,512,1024})
    for(int m:{0,1,2,3,16,31,32,63,64,65,128,129,257})
      for(int layout=0;layout<3;++layout)
      {
        FormalPowerSeries<M> f(n); vc<M> xs(m),expect(m);
        repi(i,n) f[i]=rng();
        repi(i,m) xs[i]=layout==0 ? M(rng()) : layout==1 ? M(i%3) : M(0);
        repi(i,m) expect[i]=f.eval(xs[i]);
        same(expect,multipoint_adaptive<M>(f,xs),"adaptive");
        same(expect,multipoint_transposed<M>(f,xs,false,false),"full CRT");
        for(bool cache:{false,true}) same(expect,multipoint_transposed<M>(f,xs,cache),"multipoint n="+to_string(n)+" m="+to_string(m));
      }
  for(int n:{2048,4097,16384})
  {
    P<M> h(n); repi(i,1,n) h[i]=rng();
    using Ref=AuditFPS<M,0>; Ref::exp_ntt_cutoff=Ref::exp_crt_cutoff=-1;
    auto expected=Ref(h.begin(),h.end()).exp(n);
    same(expected,exp_bs_convolution(h,n),"large bs convolution");
    if constexpr(native) { same(expected,exp_bs(h,n,true),"large bs"); for(int s:{1,2,4,8,16,32}) same(expected,exp_harvey(h,n,s),"large Harvey"); }
    FormalPowerSeries<M> f(h.begin(),h.end()); vc<M> xs(n); for(auto& x:xs)x=rng();
    auto eval=multipoint_evaluation(f,xs);
    same(eval,multipoint_transposed<M>(f,xs,false),"large transposed");
    same(eval,multipoint_transposed<M>(f,xs,true),"large cached");
  }
  // Instrumentation must preserve all coefficients, including CRT reconstruction.
  P<M> h(2048); repi(i,1,h.sz())h[i]=rng();
  auto expected=exp_bs_convolution(h,h.sz()); TransformCounts counts; recording=&counts;
  auto actual=exp_bs_convolution(h,h.sz()); recording=nullptr; same(expected,actual,"profile");
  cerr<<"checked mod "<<M::mod()<<"; profiled "<<counts.calls()<<" transforms\n";
}
int main()
{
  check<static_modint32<998244353>,true>();
  check<static_modint32<1811939329>,true>();
  check<static_modint32<1000000007>,false>();
  dynamic_modint32<91>::set_mod(998244353); check<dynamic_modint32<91>,false>();
  // bit_ceil(n) fits the NTT while 2*n exceeds its supported length.
  using Small=static_modint32<17>; using SmallRef=AuditFPS<Small,0>;
  SmallRef::exp_ntt_cutoff=SmallRef::exp_crt_cutoff=-1;
  mt19937 rng(847);
  for(int n=0;n<=16;++n)
  {
    P<Small> h(n); repi(i,1,n)h[i]=rng(); auto expected=naive_exp(h,n);
    same(expected,SmallRef(h.begin(),h.end()).exp(n),"small prime baseline");
    same(expected,exp_bs(h,n,true),"small prime bs");
    same(expected,exp_bs(h,n,true,true),"small prime paper inverse");
    same(expected,exp_harvey(h,n,4),"small prime Harvey");
  }
  cout<<"FPS algorithm candidates: OK\n";
}
