// g++-15 -std=c++20 -O2 -I. benchmark/delayed_reduction.cpp -o /tmp/delayed-bench
// /tmp/delayed-bench [dynamic|upper]
#include "math/linalg/matrix.hpp"
#include "math/set/zeta_mobius.hpp"
#include "math/set/xor_convolution.hpp"
#include "math/prime/sieve/zeta_mobius_divisor_multiple.hpp"
#include "delayed_reduction_reference.hpp"
#include "delayed_reduction_candidates.hpp"
#include <random>

volatile ull sink=0;
template<class S> ull digest(const vc<S>&a)
{ ull v=0; for(auto &x:a) v=(v^x.val())*1000000007; return v; }
template<class S> ull digest(const tuple<vvc<S>,int,S>&a)
{ ull v=get<1>(a)^get<2>(a).val(); for(auto &r:get<0>(a)) v=(v^digest(r))*1000000007; return v; }

template<class Old,class New>
void measure(string name,int iterations,Old old,New now)
{
  assert(old()==now());
  vc<double> t[2];
  repi(round,-1,5) repi(order,2)
  {
    int k=order^(round&1); auto start=chrono::steady_clock::now();
    repi(it,iterations)
    {
      asm volatile("":::"memory");
      if(k) sink=digest(now()); else sink=digest(old());
    }
    double us=chrono::duration<double,micro>(chrono::steady_clock::now()-start).count()/iterations;
    if(round>=0) t[k].push_back(us);
  }
  for(auto &x:t) sort(ALL(x));
  cout<<name<<','<<t[0][2]<<','<<t[1][2]<<','<<t[0][2]/t[1][2]<<endl;
}
mt19937 rng(20260910);
template<class S> vc<S> input(int n)
{ vc<S>a(n); for(auto &x:a)x=rng()%S::mod(); return a; }

template<int kind,class S> vc<S> set_after(const vc<S>&a)
{
  using G=GroupOfRingAdd<FieldAddSubMulDiv<S>>;
  if constexpr(kind==0)return zeta_subset<G>(a);
  if constexpr(kind==1)return mobius_subset<G>(a);
  if constexpr(kind==2)return zeta_supset<G>(a);
  if constexpr(kind==3)return mobius_supset<G>(a);
  if constexpr(kind==4)return hadamard_abel_group<G>(a);
}
template<int kind,class S> vc<S> prime_after(const vc<S>&a)
{
  using G=GroupOfRingAdd<FieldAddSubMulDiv<S>>;
  if constexpr(kind==0)return zeta_divisor<G>(a);
  if constexpr(kind==1)return mobius_divisor<G>(a);
  if constexpr(kind==2)return zeta_multiple<G>(a);
  if constexpr(kind==3)return mobius_multiple<G>(a);
}

template<int kind,class S> void transforms(string tag)
{
  const string sets[]={"subset_zeta","subset_mobius","supset_zeta","supset_mobius","hadamard"};
  const string primes[]={"divisor_zeta","divisor_mobius","multiple_zeta","multiple_mobius"};
  for(int n:{256,4096,65536,1048576})
  {
    auto a=input<S>(n); int its=max(1,131072/n);
    string name=tag+"/"+sets[kind]+"/"+to_string(n);
    auto old=[&]{return delayed_before::set_transform<kind>(a);};
    measure(name+"/adopted",its,old,[&]{return set_after<kind>(a);});
    if constexpr(kind==4)
      measure(name+"/direct_subtract_baseline",its,[&]{return delayed_before::set_transform<kind,true>(a);},[&]{return set_after<kind>(a);});
    if constexpr(kind<4)
    {
      measure(name+"/raw64_trial",its,old,[&]{return delayed_candidate::set_transform<kind>(a);});
      LinearSieve::reserve(n);
      name=tag+"/"+primes[kind]+"/"+to_string(n-1);
      auto before=[&]{return delayed_before::prime_transform<kind>(a);};
      measure(name+"/adopted",its,before,[&]{return prime_after<kind>(a);});
      measure(name+"/raw64_trial",its,before,[&]{return delayed_candidate::prime_transform<kind>(a);});
    }
  }
}

template<class S> void rows(string tag,int n,int m,string mode,bool rref)
{
  using F=FieldAddSubMulDiv<S>;
  vvc<S>a(n,vc<S>(m));
  for(auto &r:a) r=input<S>(m);
  if(mode=="zero") for(auto &r:a) fill(ALL(r),0);
  if(mode=="identity") repi(i,n) repi(j,m) a[i][j]=(i==j);
  if(mode=="sparse") repi(i,n) repi(j,m) if(rng()%100!=0)a[i][j]=0;
  if(mode=="rank1") repi(i,1,n) a[i]=a[0];
  int its=max(1,512/max(n,m));
  const Matrix<F> matrix(a);
  auto now=[&]
  {
    auto [b,rk,de]=matrix.template row_reduction<int>(rref);
    return tuple<vvc<S>,int,S>{std::move(b),rk,de};
  };
  string name=tag+"/row/"+to_string(n)+"x"+to_string(m)+"/"+mode+(rref?"/rref":"/ref");
  measure(name+"/original",its,[&]{return delayed_before::eliminate(a,rref);},now);
  if(mode=="dense") measure(name+"/suffix",its,[&]{return delayed_before::eliminate<true>(a,rref);},now);
}

template<class S> void run(string tag)
{
  for(int n:{8,32,64,128,256,512}) for(bool rref:{false,true}) rows<S>(tag,n,n,"dense",rref);
  for(auto mode:{"zero","identity","sparse","rank1"}) for(bool rref:{false,true}) rows<S>(tag,256,256,mode,rref);
  for(auto [n,m]:vc<pair<int,int>>{{32,256},{256,32},{128,256}}) for(bool rref:{false,true}) rows<S>(tag,n,m,"dense",rref);
  transforms<0,S>(tag); transforms<1,S>(tag); transforms<2,S>(tag); transforms<3,S>(tag); transforms<4,S>(tag);
}
int main(int argc,char**argv)
{
  cout<<"case,before_us,after_us,speedup\n";
  if(argc==1) run<modint998244353>("static998");
  else if(string(argv[1])=="upper") run<static_modint32<2147483647>>("static_upper");
  else { dynamic_modint32<72>::set_mod(1000000007); run<dynamic_modint32<72>>("dynamic1e9"); }
}
