#include "fps_algorithm_candidates.hpp"
#include "math/fps/interpolation.hpp"
#include "math/fps/composition.hpp"
#include "math/fps/rational_sum.hpp"
#include "math/convolution/convolution_many.hpp"
#define ntt fps_algorithm_audit::forward
#define intt fps_algorithm_audit::backward
#define convolution fps_algorithm_audit::convolve
#include "/tmp/production-profile/fps.hpp"
#include "/tmp/production-profile/middle_product.hpp"
#include "/tmp/production-profile/multipoint_evaluation.hpp"
#include "/tmp/production-profile/interpolation.hpp"
#include "/tmp/production-profile/composition.hpp"
#undef ntt
#undef intt
#undef convolution
#define main old_algorithms_main
#include "fps_algorithms.cpp"
#undef main

template<class M> void adopted(int max_n)
{
 for(int n:{512,4096,4097,16384,65536,65537})if(n<=max_n)
 {
  auto input=coefficients<M>(n,713); input[0]=1;
  BaselineFPS<M> old(input); ProfileFPS<M> f(input),e(input);e[0]=0;BaselineFPS<M> oe(e.begin(),e.end());
  auto square=(f*f).resized(n); ProfileFPS<M> sq(square);BaselineFPS<M> osq(square.begin(),square.end());
  measure<M>("inv",n,0,"dense",{{"baseline",[&]{return old.inv(n);}},{"adopted",[&]{return f.inv(n);}}});
  measure<M>("sqrt",n,0,"dense",{{"baseline",[&]{return osq.sqrt(n).second;}},{"adopted",[&]{return sq.sqrt(n).second;}}});
  measure<M>("exp",n,0,"dense",{{"baseline",[&]{return oe.exp(n);}},{"adopted",[&]{return e.exp(n);}}});
  measure<M>("pow",n,0,"dense",{{"baseline",[&]{return old.pow(1234567,n);}},{"adopted",[&]{return f.pow(1234567,n);}}});
  if(n<=16384) measure<M>("composition",n,0,"dense",{{"baseline",[&]{return baseline_composition(old,oe,n);}},{"adopted",[&]{return profile_composition(f,e,n);}}});
  vc<M> xs=coefficients<M>(n,913);
  measure<M>("multipoint",n,n,"random",{{"baseline",[&]{return baseline_multipoint_evaluation(old,xs);}},{"adopted",[&]{return profile_multipoint_evaluation(f,xs);}}});
  repi(i,n)xs[i]=i; auto ys=coefficients<M>(n,991);
  measure<M>("interpolation",n,n,"distinct",{{"baseline",[&]{return baseline_interpolation(xs,ys);}},{"adopted",[&]{return profile_interpolation(xs,ys);}}});
 }
}
template<class M> void thresholds(int max_n)
{
 using F=ProfileFPS<M>;
 vc<int> supports=M::mod()==998244353?vc<int>{32,64,100,128,160,200,256,320,512}:vc<int>{512,800,1300,1600,2000,2500,3000};
 for(int n:{4096,4097,16384})if(n<=max_n)for(string layout:{"prefix","spread","suffix"})for(int s:supports)
 {
  if(s>=n)continue;
  mt19937 rng(31);vc<int> positions(n-1);iota(positions.begin(),positions.end(),1);
  if(layout=="spread")shuffle(positions.begin(),positions.end(),rng);
  if(layout=="suffix")reverse(positions.begin(),positions.end());
  F f(n),e(n),a(coefficients<M>(n,73));f[0]=1;
  repi(i,s)f[positions[i]]=1+rng()%(M::mod()-1);e=f;e[0]=0;
  auto dense=[&](auto op){F::force_dense=true;auto r=op();F::force_dense=false;return r;};
  measure<M>("inv_threshold",n,s,layout,{{"sparse",[&]{return F{1}.div_sparse(f,n);}},{"dense",[&]{return dense([&]{return f.inv(n);});}},{"selected",[&]{return f.inv(n);}}});
  measure<M>("div_threshold",n,s,layout,{{"sparse",[&]{return a.div_sparse(f,n);}},{"dense",[&]{return dense([&]{return a.div(f,n);});}},{"selected",[&]{return a.div(f,n);}}});
  measure<M>("exp_threshold",n,s,layout,{{"sparse",[&]{return e.exp_sparse(n);}},{"dense",[&]{return dense([&]{return e.exp(n);});}},{"selected",[&]{return e.exp(n);}}});
  measure<M>("pow_threshold",n,s,layout,{{"sparse",[&]{return f.pow_sparse(1234567,n);}},{"dense",[&]{return dense([&]{return f.pow(1234567,n);});}},{"selected",[&]{return f.pow(1234567,n);}}});
  measure<M>("sqrt_threshold",n,s,layout,{{"sparse",[&]{return f.sqrt_sparse(n).second;}},{"dense",[&]{return dense([&]{return f.sqrt(n).second;});}},{"selected",[&]{return f.sqrt(n).second;}}});
 }
}
int main(int argc,char**argv)
{
 string suite=argc>1?argv[1]:"adopted";int mod=argc>2?stoi(argv[2]):998244353,max_n=argc>3?stoi(argv[3]):65536;rounds=argc>4?stoi(argv[4]):5;
 include_kernels=true;
 cout<<"suite,mod,n,points,layout,variant,median_us,min_us,max_us,iterations,rounds,ntt_calls,ntt_weight,transforms,kernels\n";
 auto run=[&]<class M>{if(suite=="adopted")adopted<M>(max_n);else if(suite=="threshold")thresholds<M>(max_n);else throw runtime_error("suite");};
 if(mod==998244353)run.template operator()<modint998244353>();else if(mod==1000000007)run.template operator()<static_modint32<1000000007>>();else throw runtime_error("modulus");
}
