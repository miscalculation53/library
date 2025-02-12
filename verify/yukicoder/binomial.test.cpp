#define PROBLEM "https://yukicoder.me/problems/no/117"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

// #define FAST_IO
#define FAST_CIO
// #define INTERACTIVE

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "../../template/template_all.hpp"

#include "../../math/modint/modint.hpp"
// using mint = modint998244353;
using mint = modint1000000007;
// using mint = static_modint<1000000000>;
// using mint = modint;
#include "../../math/modint/binomial.hpp"
using bi = Binomial<mint>;

void init() {}

void main2()
{
  CHAR(t);
  char _;
  READ(_);
  LL(N);
  READ(_);
  LL(K);
  READ(_);
  if (t == 'P')
    PRINT(bi::P(N, K));
  else if (t == 'C')
    PRINT(bi::C(N, K));
  else if (t == 'H')
    PRINT(bi::H(N, K));
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
    /* コードテストで確認する際にコメントアウトする
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