#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#define SINGLE_TESTCASE
#define FAST_IO

#include "ds/fast_integer_set.hpp"

void init() {}
void main2()
{
  INT(N, Q);
  STR(T);
  FastIntegerSet st(N, [&](int key) { return T[key] == '1'; });
  repi(_, Q)
  {
    INT(c, k);
    if (c == 0) st.insert(k);
    else if (c == 1) st.erase(k);
    else if (c == 2) PRINT(int(st.contains(k)));
    else if (c == 3)
    {
      int key = st.next(k);
      PRINT(key == N ? -1 : key);
    }
    else PRINT(st.prev(k));
  }
}
void test() {}
#include "template/template_main.hpp"
Main<init, main2, test> main_dummy;
int main() {}
