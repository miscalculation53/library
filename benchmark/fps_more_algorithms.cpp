#include "fps_more_algorithms.hpp"
#define main original_benchmark_main
#include "fps_algorithms.cpp"
#undef main
int main(int argc,char**argv)
{
 int max_n=argc>1?stoi(argv[1]):65536;rounds=argc>2?stoi(argv[2]):5;
 using M=modint998244353;using Ref=AuditFPS<M,0>;using Previous=AuditFPS<M,79>;
 Ref::inv_cutoff=Ref::sqrt_cutoff=Ref::exp_ntt_cutoff=-1;
 cout<<"suite,mod,n,points,layout,variant,median_us,min_us,max_us,iterations,rounds,ntt_calls,ntt_weight,transforms\n";
 for(int n:{512,4096,4097,16384,65536,65537,100000,262144})if(n<=max_n)
 {
  auto input=coefficients<M>(n,881);input[0]=1;Ref ref(input);P<M> f(input);P<M> e(f);e[0]=0;
  auto square=(f*f).resized(n);Ref oldsquare(square.begin(),square.end());Previous prevsquare(square.begin(),square.end());
  vc<Candidate<M>> ci{{"current",[&](){return ref.inv(n);}}};
  vc<Candidate<M>> cs{{"current",[&](){return oldsquare.sqrt(n).second;}},{"high_half",[&](){return prevsquare.sqrt(n).second;}}};
  vc<Candidate<M>> ce{{"full_blocks",[&](){return exp_harvey(e,n,16);}},{"partial_blocks",[&](){return exp_partial(e,n,16);}}};
  for(int s:{2,4,8,16,32})
  {
   ci.push_back({"block_"+to_string(s),[&,s](){return inv_block(f,n,s);}});
   cs.push_back({"block_"+to_string(s),[&,s](){return sqrt_block(square,n,s);}});
  }
  measure<M>("inv",n,0,"dense",ci);measure<M>("sqrt",n,0,"dense",cs);measure<M>("exp_partial",n,0,"dense",ce);
  if(n<=65536)
  {
   vc<M> xs(n),ys(n);repi(i,n)xs[i]=i;for(auto&y:ys)y=1+M(rand());
   vc<Candidate<M>> cp{{"current",[&](){return baseline_interpolation(xs,ys);}},{"shared_tree",[&](){return interpolation_shared(xs,ys);}}};
   measure<M>("interpolation",n,n,"distinct",cp);
  }
 }
}
