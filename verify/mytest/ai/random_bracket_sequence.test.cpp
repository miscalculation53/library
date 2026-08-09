#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "random/bracket_sequence.hpp"

bool is_valid(const string &s)
{
  int bal = 0;
  for (char c : s)
  {
    bal += c == '(' ? 1 : -1;
    if (bal < 0) return false;
  }
  return bal == 0;
}

// Test focus: every generated sequence has the requested size and valid prefix balances.
int main()
{
  mt.seed(0);
  rep(n, 101) repi(_, 100)
  {
    string s = random_bracket_sequence(n);
    assert(SZ(s) == 2 * n);
    assert(is_valid(s));
  }

  cout << "Hello World" << endl;
}
