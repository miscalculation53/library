#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_A"

#define SINGLE_TESTCASE
#ifndef LOCAL
#define FAST_IO
#endif

#include "graph/lowlink.hpp"

void init() {}
void main2()
{
  INT(n, m);
  VEC(pll, m, es);
  const LowLink lowlink(GraphUndirected<>(n, es));
  for (int v : lowlink.articulations()) PRINT(v);
}
void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
