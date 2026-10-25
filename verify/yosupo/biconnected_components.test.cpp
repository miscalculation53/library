#define PROBLEM "https://judge.yosupo.jp/problem/biconnected_components"

#define SINGLE_TESTCASE
#ifndef LOCAL
#define FAST_IO
#endif

/*! SPDX-License-Identifier: CC0-1.0 */

#include "graph/biconnected_components.hpp"

void init() {}
void main2()
{
  INT(n, m);
  VEC(pll, m, es);
  const BiconnectedComponents bcc(GraphUndirected<>(n, es));
  PRINT(bcc.size());
  for (int b = 0; b < bcc.size(); ++b)
  {
    const auto vertices = bcc.vertices(b);
    WRITE(vertices.size());
    for (int v : vertices) WRITE(' ', v);
    PRINT();
  }
}
void test() {}

#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
