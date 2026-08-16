#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/my_deque.hpp"

// Test focus: wraparound, reallocation, both-end operations, reserve, and random access match std::deque.
int main()
{
  mt19937 rng(123456789);
  MyDeque<int> deq;
  deque<int> expected;
  deq.reserve(3);

  repi(iter, 200000)
  {
    int op = expected.empty() ? rng() % 2 : rng() % 9;
    if (op == 0)
    {
      int x = rng();
      deq.push_front(x), expected.push_front(x);
    }
    else if (op == 1)
    {
      int x = rng();
      deq.push_back(x), expected.push_back(x);
    }
    else if (op == 2)
      deq.pop_front(), expected.pop_front();
    else if (op == 3)
      deq.pop_back(), expected.pop_back();
    else if (op == 4)
      assert(deq.front() == expected.front());
    else if (op == 5)
      assert(deq.back() == expected.back());
    else if (op == 6)
    {
      int i = rng() % expected.size();
      assert(deq[i] == expected[i] && deq.at(i) == expected.at(i));
    }
    else if (op == 7)
    {
      deq.clear(), expected.clear();
    }
    else
      deq.reserve(expected.size() + rng() % 20);
    assert(deq.size() == ssize(expected));
    assert(deq.empty() == expected.empty());
    const MyDeque<int> &const_deq = deq;
    repi(i, expected.size()) assert(const_deq[i] == expected[i]);
  }
  PRINT("Hello World");
}
