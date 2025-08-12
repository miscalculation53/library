#pragma once

#include "template_all_but_modint.hpp"

template <auto init, auto main2, auto test>
struct Main
{
  Main()
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
  
    init();
    #ifdef LOCAL
    test();
    #endif
  
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
    local(while (true))
    {
      dump("T");
      IN(uint, T);
      while (T--)
      {
        dump("new testcase");
        main2();
      }
    }
    #endif
  }
};
