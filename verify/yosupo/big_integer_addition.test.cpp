#define PROBLEM "https://judge.yosupo.jp/problem/addition_of_big_integers"

// #define SINGLE_TESTCASE
#define MULTI_TESTCASE
// #define AOJ_TESTCASE

#ifndef LOCAL
#define FAST_IO
// #define FAST_CIO
// #define INTERACTIVE
#endif

#define INF 4'000'000'000'000'000'037LL
#define EPS 1e-11

#include "template/template_all_but_modint.hpp"

#include "math/bigint.hpp"
using bigint = BigInteger<>;

void init() {}

void main2()
{
  IN(bigint, A, B);
  PRINT(A + B);
}

void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
