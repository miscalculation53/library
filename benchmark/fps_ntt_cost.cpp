#include "math/convolution/convolution.hpp"
using Clock=chrono::steady_clock;
volatile ull ntt_cost_sink;
template<class M>void measure(int max_n)
{
 for(int n=2;n<=max_n;n*=2)
 {
  vc<M> input(n);repi(i,n)input[i]=i*ll(i+17)+23;vc<M> v=input;ntt(v);intt(v);
  for(int direction:{0,1})
  {
   int iterations=clamp(4000000/(n*int(bit_width(unsigned(n)))),2,20000);vc<double> times;
   for(int r=0;r<7;++r)
   {
    v=input;auto begin=Clock::now();repi(i,iterations){if(direction)intt(v);else ntt(v);}
    times.push_back(chrono::duration<double,micro>(Clock::now()-begin).count()/iterations);ntt_cost_sink=v[n/2].val();
   }
   sort(times.begin(),times.end());cout<<M::mod()<<','<<n<<','<<(direction?"inverse":"forward")<<','<<times[3]<<','<<times[0]<<','<<times[6]<<','<<iterations<<'\n';
  }
 }
}
int main(int argc,char**argv)
{
 int max_n=argc>1?stoi(argv[1]):131072;
 cout<<"mod,length,direction,median_us,min_us,max_us,iterations\n";
 measure<static_modint32<1000000007>>(2);
 measure<modint998244353>(max_n);measure<static_modint32<469762049>>(max_n);measure<static_modint32<1811939329>>(max_n);measure<static_modint32<2013265921>>(max_n);
}
