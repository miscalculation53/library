// g++-15 -std=c++20 -O2 -DNDEBUG -I. benchmark/fps_algorithms.cpp -o /tmp/fps-algorithms
#include "fps_algorithm_candidates.hpp"
#include "math/fps/multipoint_evaluation.hpp"
#include "fps_baseline.hpp"
using namespace fps_algorithm_audit;
using Clock=chrono::steady_clock;
volatile ull fps_algorithms_sink=0;
int rounds=5;
bool include_kernels=false;

template<class A,class B> void same(const A& a,const B& b)
{
  if(a.size()!=b.size() || !equal(a.begin(),a.end(),b.begin())) throw runtime_error("coefficient mismatch");
}
template<class M> struct Candidate { string name; function<vc<M>()> run; };
template<class M> double elapsed(const Candidate<M>& c,int iterations)
{
  vc<M> result;
  auto start=Clock::now();
  repi(i,iterations) result=c.run();
  double us=chrono::duration<double,micro>(Clock::now()-start).count()/iterations;
  ull sum=0; for(auto x:result) sum=sum*1000000007+x.val(); fps_algorithms_sink=sum;
  return us;
}
template<class M> void measure(string suite,int n,int points,string layout,const vc<Candidate<M>>& candidates)
{
  auto expected=candidates.front().run();
  vc<int> iterations; vc<vc<double>> times(candidates.size()); vc<TransformCounts> counts;
  for(auto& c:candidates)
  {
    same(expected,c.run());
    iterations.push_back(clamp(int(2000/max(1.0,elapsed(c,1))),1,128));
    TransformCounts ct; recording=&ct;
    auto value=c.run(); recording=nullptr; same(expected,value); counts.push_back(ct);
  }
  repi(r,rounds) repi(j,candidates.size())
  {
    int who=(r+j)%candidates.size(); times[who].push_back(elapsed(candidates[who],iterations[who]));
  }
  repi(i,candidates.size())
  {
    auto& t=times[i]; sort(t.begin(),t.end());
    cout<<suite<<','<<M::mod()<<','<<n<<','<<points<<','<<layout<<','<<candidates[i].name
        <<','<<t[t.size()/2]<<','<<t.front()<<','<<t.back()<<','<<iterations[i]<<','<<rounds
        <<','<<counts[i].calls()<<','<<counts[i].weight()<<',';
    bool first=true; for(auto [length,c]:counts[i].lengths)
    { if(!first)cout<<';'; first=false; cout<<length<<':'<<c[0]<<'/'<<c[1]; }
    if(include_kernels)
    {
      cout<<',';bool first_kernel=true;
      for(auto [key,c]:counts[i].kernels){if(!first_kernel)cout<<';';first_kernel=false;cout<<key.first<<'@'<<key.second<<':'<<c[0]<<'/'<<c[1];}
    }
    cout<<'\n';
  }
  cout.flush();
}
template<class M> vc<M> coefficients(int n,int seed,bool zero_constant=false)
{
  mt19937 rng(seed); vc<M> f(n); for(auto& x:f)x=1+rng()%(M::mod()-1);
  if(zero_constant && n)f[0]=0;
  return f;
}
template<class M> void exp_suite(int max_n)
{
  using Ref=AuditFPS<M,0>; using Prev=AuditFPS<M,131>;
  Ref::exp_ntt_cutoff=Ref::exp_crt_cutoff=Prev::exp_ntt_cutoff=Prev::exp_crt_cutoff=-1;
  for(int n:{512,1024,4096,4097,16384,65536,100000,262144,524288})
  {
    if(n>max_n)continue;
    auto input=coefficients<M>(n,831,true);
    Ref ref(input); Prev prev(input); P<M> f(input);
    Binomial<M>::reserve(bit_ceil(n)-1);
    vc<Candidate<M>> cs{
      {"current",[&](){return ref.exp(n);}},
      {"previous",[&](){return prev.exp(n);}}
    };
    if constexpr(M::mod()!=1000000007)
    {
      cs.push_back({"bs_short",[&](){return exp_bs(f,n);}});
      cs.push_back({"bs_split",[&](){return exp_bs(f,n,true);}});
      cs.push_back({"bs_inverse",[&](){return exp_bs(f,n,false,true);}});
      cs.push_back({"bs_inverse_split",[&](){return exp_bs(f,n,true,true);}});
      cs.push_back({"harvey_scalar_4",[&](){return exp_harvey(f,n,4,false);}});
      for(int s:{1,2,4,8,16,32,64}) cs.push_back({"harvey_"+to_string(s),[&,s](){return exp_harvey(f,n,s);}});
    }
    else cs.push_back({"bs_convolution",[&](){return exp_bs_convolution(f,n);}});
    measure<M>("exp",n,0,"dense",cs);
  }
}
template<class F> vc<typename F::value_type> remainder_evaluation(const F& f,const vc<typename F::value_type>& xs)
{
  using M=typename F::value_type;
  int m0=xs.size(); if(m0==0)return{}; if(m0==1)return{f.eval(xs[0])};
  int m=bit_ceil(m0),h=min(6,int(bit_width(unsigned(m0)))-1);
  vc<F> node(2*m,{1}); repi(i,m0)node[m+i]={-xs[i],1};
  repi(i,m-1,0,-1)node[i]=node[2*i]*node[2*i+1];
  node[1]=f%node[1]; repi(i,2,m>>(h-1))node[i]=node[i/2]%node[i];
  vc<M> r(m0); repi(i,m0)r[i]=node[(m+i)>>h].eval(xs[i]); return r;
}
template<class M> void multipoint_suite(int max_n)
{
  vc<pair<int,int>> sizes;
  for(int n:{64,256,1024,4096,4097,16384,65536,262144})if(n<=max_n)sizes.emplace_back(n,n);
  for(auto [n,m]:vc<pair<int,int>>{{16,4096},{64,4096},{256,4096},{4096,64},{4096,256},{1024,4097},{16384,4096},{4096,16384}})
    if(max(n,m)<=max_n)sizes.emplace_back(n,m);
  for(auto [n,m]:sizes)
  {
    FormalPowerSeries<M> f(coefficients<M>(n,915)); P<M> g(f.begin(),f.end()); AuditFPS<M,0> ref(f.begin(),f.end());
    for(string layout:{"random","repeated","zero"})
    {
      if(layout!="random" && !(n==m && (n==1024 || n==16384)))continue;
      vc<M> xs=coefficients<M>(m,511);
      if(layout=="repeated")repi(i,m)xs[i]=i%8;
      if(layout=="zero")fill(xs.begin(),xs.end(),M(0));
      Binomial<M>::reserve(max(n,m));
      vc<Candidate<M>> cs{
        {"current",[&](){return baseline_multipoint_evaluation(ref,xs);}},
        {"remainder_moves",[&](){return remainder_evaluation(g,xs);}},
        {"transposed",[&](){return multipoint_transposed<M>(f,xs,false);}},
        {"transposed_cache",[&](){return multipoint_transposed<M>(f,xs,true);}},
        {"adaptive",[&](){return multipoint_adaptive<M>(f,xs);}}
      };
      if constexpr(M::mod()==1000000007)cs.push_back({"transposed_full_crt",[&](){return multipoint_transposed<M>(f,xs,false,false);}});
      // Timing and profiling both use the frozen baseline.
      measure<M>("multipoint",n,m,layout,cs);
    }
  }
}
template<class M> void threshold_suite(int max_n)
{
  using Ref=AuditFPS<M,0>; using Prev=AuditFPS<M,131>;
  Prev::exp_ntt_cutoff=Prev::exp_crt_cutoff=-1;
  vc<int> supports = M::mod()==1000000007 ? vc<int>{512,800,1300,2000,3000,4000} : vc<int>{64,100,128,200,256,320,512};
  for(int n:{4096,4097,16384}) if(n<=max_n)
  {
    if(M::mod()==1000000007 && n!=16384)continue;
    Binomial<M>::reserve(bit_ceil(n)-1);
    for(string layout:{"prefix","spread","suffix"}) for(int terms:supports)
    {
      if(terms>=n)continue;
      mt19937 rng(123); vc<int> positions(n-1); iota(positions.begin(),positions.end(),1);
      if(layout=="spread")shuffle(positions.begin(),positions.end(),rng);
      if(layout=="suffix")reverse(positions.begin(),positions.end());
      vc<M> input(n); repi(i,terms)input[positions[i]]=1+rng()%(M::mod()-1);
      Ref ref(input); Prev prev(input); P<M> f(input);
      vc<Candidate<M>> cs{
        {"current_sparse",[&](){return ref.exp_sparse(n);}},
        {"optimized_sparse",[&](){return f.exp_sparse(n);}},
        {"previous_dense",[&](){return prev.exp(n);}}
      };
      if constexpr(M::mod()==1000000007)cs.push_back({"new_dense",[&](){return exp_bs_convolution(f,n);}});
      else
      {
        cs.push_back({"new_dense",[&](){return exp_bs(f,n,true);}});
        cs.push_back({"harvey_dense",[&](){return exp_harvey(f,n,bit_ceil(n)<=1024 ? 4 : 16);}});
      }
      measure<M>("exp_threshold",n,terms,layout,cs);
    }
  }
}
int main(int argc,char** argv)
{
  if(argc>1 && string(argv[1])=="memory")
  {
    int mod=stoi(argv[2]),n=stoi(argv[3]); string kind=argv[4];
    auto one=[&]<class M>()
    {
      FormalPowerSeries<M> f(coefficients<M>(n,915)); auto xs=coefficients<M>(n,511);
      vc<M> result;
      if(kind=="current")result=baseline_multipoint_evaluation(BaselineFPS<M>(f.begin(),f.end()),xs);
      else if(kind=="transposed")result=multipoint_transposed<M>(f,xs,false);
      else if(kind=="transposed_cache")result=multipoint_transposed<M>(f,xs,true);
      else throw runtime_error("unsupported evaluator");
      ull sum=0;for(auto x:result)sum=sum*1000000007+x.val();cout<<sum<<'\n';
    };
    if(mod==998244353)one.template operator()<static_modint32<998244353>>();
    else if(mod==1000000007)one.template operator()<static_modint32<1000000007>>();
    else throw runtime_error("unsupported modulus");
    return 0;
  }
  string suite=argc>1?argv[1]:"exp"; int mod=argc>2?stoi(argv[2]):998244353;
  int max_n=argc>3?stoi(argv[3]):65536; rounds=argc>4?stoi(argv[4]):5;
  cout<<"suite,mod,n,points,layout,variant,median_us,min_us,max_us,iterations,rounds,ntt_calls,ntt_weight,transforms\n";
  auto run=[&]<class M>() { if(suite=="exp") exp_suite<M>(max_n); else if(suite=="threshold") threshold_suite<M>(max_n); else if(suite=="multipoint") multipoint_suite<M>(max_n); else throw runtime_error("unsupported suite"); };
  if(mod==998244353)run.template operator()<static_modint32<998244353>>();
  else if(mod==1811939329)run.template operator()<static_modint32<1811939329>>();
  else if(mod==1000000007)run.template operator()<static_modint32<1000000007>>();
  else throw runtime_error("unsupported modulus");
  return 0;
}
