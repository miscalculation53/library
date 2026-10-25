#define PROBLEM "https://judge.yosupo.jp/problem/assignment"
#define FAST_IO

#include "flow/hungarian.hpp"
#include "template/template_inout.hpp"

int main()
{
  INT(n);
  vvc<int> a(n, vc<int>(n));
  READ(a);
  const Hungarian<ll> result(a);
  PRINT(result.cost);
  PRINT(result.row_to_col);
}
