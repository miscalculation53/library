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

#include "ds/uf/uf_partially_persistent.hpp"

void init() {}

void main2()
{
  LL(H, W);
  VEC2(ll, H, W, A);

  UnionFindPartiallyPersistent<UFDataEmpty<>> uf(H * W);
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
  sort(ALL(UVW), [&](tlll t1, tlll t2)
       { return get<2>(t1) < get<2>(t2); });
  fec([ u, v, w ] : UVW) uf.merge(u, v, w);

  LL(Q);
  rep(_, Q)
  {
    LL(si, sj, ti, tj);
    si--, sj--, ti--, tj--;
    ll s = si * W + sj;
    ll t = ti * W + tj;
    auto judge = [&](ll time)
    { return uf.same(time, s, t); };
    ll time = expsearch(judge, 0, true).first;
    PRINT(get<2>(UVW.at(time - 1)));
  }
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