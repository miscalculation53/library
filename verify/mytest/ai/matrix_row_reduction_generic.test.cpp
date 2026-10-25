#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#ifndef GENERIC_ROW_ARRAY_ONLY
#include "math/linalg/matrix.hpp"
#endif
#include "math/linalg/matrix_array.hpp"
#include "math/rational.hpp"

// 有理数の中間表現を約分し、小さな整数のまま厳密な体として比較する。
struct QField : FieldAddSubMulDiv<Rational<ll>>
{
  using S=Rational<ll>;
  static S reduce(S x) { auto [n,d]=x.reduced(); return {n,d}; }
  static S add(S a,S b) { return reduce(a+b); }
  static S mul(S a,S b) { return reduce(a*b); }
  static S inv(S a) { return reduce(S(1)/a); }
};

// 変更前と同じ、全列に行基本変形を行う参照実装。
template<class F>
auto reference(vvc<typename F::S> a,bool rref)
{
  using S=typename F::S;
  const int n=a.size(),m=n?a[0].size():0;
  int rk=0; S de=F::e1();
  for(int j=0;j<m && rk<n;++j)
  {
    int p=rk;
    while(p<n && a[p][j]==F::e0()) ++p;
    if(p==n) { de=F::e0(); continue; }
    if(p!=rk) { swap(a[p],a[rk]); de=F::minus(de); }
    de=F::mul(de,a[rk][j]);
    S inv=F::inv(a[rk][j]);
    for(auto &x:a[rk]) x=F::mul(x,inv);
    repi(i,n) if(i!=rk && (rref || i>rk))
    {
      S c=a[i][j];
      repi(k,m) a[i][k]=F::add(a[i][k],F::minus(F::mul(c,a[rk][k])));
    }
    ++rk;
  }
  return tuple{a,rk,de};
}

template<class F,int n,int m>
void exact_cases()
{
  using S=typename F::S;
  repi(mode,5) repi(seed,8)
  {
    vvc<S> a(n,vc<S>(m,F::e0()));
    repi(i,n) repi(j,m)
    {
      if(mode==1) a[i][j]=S(i==j);
      if(mode>=2) a[i][j]=S((i*7+j*3+i*j+seed)%7-3);
      if(mode==3 && (j==0 || j==2)) a[i][j]=F::e0();
      if(mode==4 && i>0) a[i][j]=a[0][j];
    }
    for(bool rref:{false,true})
    {
      auto [want,rank,det]=reference<F>(a,rref);
      MatrixArray<F,n,m> fixed;
      repi(i,n) repi(j,m) fixed[i][j]=a[i][j];
      auto [got,rk,de]=fixed.template row_reduction<int>(rref);
      assert(rk==rank && de==det);
      repi(i,n) repi(j,m) assert(got[i][j]==want[i][j]);
#ifndef GENERIC_ROW_ARRAY_ONLY
      auto [dynamic,drk,dde]=Matrix<F>(a).template row_reduction<int>(rref);
      assert(dynamic==want && drk==rank && dde==det);
#endif
    }
  }
}

template<class S>
void floating_cases()
{
  using F=FieldAddSubMulDiv<S>;
  // 49 の逆数との積は浮動小数点で厳密な 1 とは限らない。
  array<array<S,3>,2> data{{{{49,7,14}},{{98,15,29}}}};
  MatrixArray<F,2,3> a(data);
  auto [r,rk,de]=a.template row_reduction<int>(true);
  assert(rk==2);
  assert(r[0][0]==S(1) && r[1][1]==S(1));
  assert(r[1][0]==S(0) && r[0][1]==S(0));
  assert(abs(r[0][2]-S(1)/7)<S(1e-5) && abs(r[1][2]-1)<S(1e-5));
  auto [ref,rank,det]=a.template row_reduction<int>(false);
  assert(rank==2 && ref[0][0]==S(1) && ref[1][0]==S(0) && ref[1][1]==S(1));
#ifndef GENERIC_ROW_ARRAY_ONLY
  vvc<S> v(2,vc<S>(3));
  repi(i,2) repi(j,3) v[i][j]=a[i][j];
  auto [dynamic,drk,dde]=Matrix<F>(v).template row_reduction<int>(true);
  assert(drk==rk && dde==de);
  repi(i,2) repi(j,3) assert(dynamic[i][j]==r[i][j]);
#endif
}

int main()
{
  exact_cases<QField,0,0>(); exact_cases<QField,3,0>(); exact_cases<QField,0,3>();
  exact_cases<QField,1,1>(); exact_cases<QField,3,3>(); exact_cases<QField,3,5>(); exact_cases<QField,5,3>();
  // modint32 の特殊化に入らない modint64 でも同じ汎用処理を検査する。
  using F=FieldAddSubMulDiv<static_modint64<998244353>>;
  exact_cases<F,33,35>(); exact_cases<F,35,33>();
  floating_cases<float>(); floating_cases<double>(); floating_cases<long double>();
  cout << "Hello World\n";
}
