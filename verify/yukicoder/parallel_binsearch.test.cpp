#define PROBLEM "https://yukicoder.me/problems/no/2786"

#define SINGLE_TESTCASE
// #define MULTI_TESTCASE
// #define AOJ_TESTCASE

#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all.hpp"

#include "math/modint/modint.hpp"
using mint = modint998244353;
// using mint = modint1000000007;
// using mint = static_modint<1000000000>;
// using mint = modint;
#include "math/modint/binomial.hpp"
using bi = Binomial<mint>;

#include "ds/uf/uf.hpp"
#include "algo/parallel_binsearch.hpp"

void init() {}

void main2()
{
  LL(H, W);
  VEC2(ll, H, W, A);
  vc<tlll> UVW;
  rep(i, H) rep(j, W)
  {
    ll u = i * W + j;
    if (i != H - 1)
    {
      ll v = (i + 1) * W + j;
      ll w = max(A.at(i).at(j), A.at(i + 1).at(j));
      UVW.eb(u, v, w);
    }
    if (j != W - 1)
    {
      ll v = i * W + (j + 1);
      ll w = max(A.at(i).at(j), A.at(i).at(j + 1));
      UVW.eb(u, v, w);
    }
  }
  LL(Q);
  vl S(Q), T(Q);
  rep(q, Q)
  {
    LL(si, sj, ti, tj);
    si--, sj--, ti--, tj--;
    S.at(q) = si * W + sj;
    T.at(q) = ti * W + tj;
  }

  vc<bool> res(Q);
  UnionFind<UFDataEmpty<>> uf(H * W);
  vvc<pll> ev_merge(H * W + 1);
  vvc<ll> ev_same(H * W + 1);
  auto judge = [&](const vl &C)
  {
    // w 1 u v: 時刻 w+0.1 に辺 u, v を追加
    // c 2 q -1: 時刻 c+0.2 に S[q], T[q] が連結か答える
    uf = decltype(uf)(H * W);
    rep(time, H * W + 1)
    {
      ev_merge.at(time).clear();
      ev_same.at(time).clear();
    }
    fec([ u, v, w ] : UVW) ev_merge.at(w).eb(u, v);
    rep(q, Q) ev_same.at(C.at(q)).eb(q);
    rep(time, H * W + 1)
    {
      fec([ u, v ] : ev_merge.at(time)) uf.merge(u, v);
      fec(q : ev_same.at(time)) res.at(q) = uf.same(S.at(q), T.at(q));
    }
    return res;
  };
  auto ans = parallel_binsearch(Q, judge, H * W, 0).first;
  PRINTV(ans);
}

void test()
{
  /*
  local(
    rep(testcase, 100000)
    {
      cout << endl;
      dump(testcase);


      // ----- generate cases -----
      ll N = 1 + rand() % 5;
      vl A(N);
      rep(i, N) A.at(i) = 1 + rand() % 10;
      // --------------------------

      // ------ check output ------
      #define INPUT A
      auto god = naive(INPUT);
      auto ans = solve(INPUT);
      if (god != ans)
      {
        dump(INPUT);
        dump(god, ans);
        exit(0);
      }
      // --------------------------
    }
    dump("ok");
  );
  //*/
}

int main()
{
  cauto CERR = [](string val, string color)
  {
    string s = "\033[" + color + "m" + val + "\033[m";
    #ifdef LOCAL
    cerr << s;
    #endif
    /* コードテストで確認する際にコメントアウトを外す
    cerr << val;
    //*/
  };

  #if defined FAST_IO and not defined LOCAL
  CERR("\n[FAST_IO]\n\n", "32");
  #endif
  #if defined FAST_CIO and not defined LOCAL
  CERR("\n[FAST_CIO]\n\n", "32");
  cin.tie(0);
  ios::sync_with_stdio(false);
  #endif
  cout << fixed << setprecision(20);

  test();
  init();

  #if defined AOJ_TESTCASE or (defined LOCAL and defined SINGLE_TESTCASE)
  CERR("\n[AOJ_TESTCASE]\n\n", "35");
  while (true)
  {
    dump("new testcase");
    main2();
  }
  #elif defined SINGLE_TESTCASE
  CERR("\n[SINGLE_TESTCASE]\n\n", "36");
  main2();
  #elif defined MULTI_TESTCASE
  CERR("\n[MULTI_TESTCASE]\n\n", "33");
  dump("T");
  IN(uint, T);
  while (T--)
  {
    dump("new testcase");
    main2();
  }
  #endif
}