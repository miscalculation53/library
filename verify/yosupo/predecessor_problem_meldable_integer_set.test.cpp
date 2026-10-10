#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#define SINGLE_TESTCASE
#define FAST_IO

#include "ds/meldable_integer_set.hpp"

void init() {}
void main2()
{
  LL(N, Q);
  STR(T);
  MeldableIntegerSetPool sets(N);
  auto st = sets.make_set();
  rep(i, N) if (T[i] == '1') st.insert(i);
  rep(_, Q)
  {
    LL(c, k);
    if (c == 0) st.insert(k);
    else if (c == 1) st.erase(k);
    else if (c == 2) PRINT(int(st.contains(k)));
    else if (c == 3)
    {
      ll key = st.geq_min(k);
      PRINT(key == N ? -1 : key);
    }
    else PRINT(st.leq_max(k));
  }
}
void test() {}
#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
