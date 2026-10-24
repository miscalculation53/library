#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/fibonacci_heap.hpp"

template <class Key, class Compare>
void random_test(Compare comp)
{
  constexpr int n = 400;
  FibonacciHeap<Key, Compare> q(n, comp);
  vc<Key> keys(n);
  vc<bool> present(n);
  mt19937_64 rng(20151023);
  int size = 0;
  for (int step = 0; step < 100000; step++)
  {
    int v = rng() % n, op = rng() % 100;
    if (op < 45)
    {
      Key key = Key(ll(rng() % 100000) - 50000);
      if (!present[v]) q.push(v, key), present[v] = true, keys[v] = key, size++;
      else if (comp(key, keys[v])) q.decrease_key(v, key), keys[v] = key;
      else q.decrease_key(v, keys[v]);
    }
    else if (op < 98 && size)
    {
      const auto [key, id] = q.top();
      assert(present[id] && key == keys[id]);
      for (int i = 0; i < n; i++) if (present[i]) assert(!comp(keys[i], key));
      q.pop();
      present[id] = false, size--;
    }
    else if (op == 99 && step % 1000 == 0)
      q.clear(), fill(ALL(present), false), size = 0;
    assert(q.size() == size && q.empty() == (size == 0));
    for (int j = 0; j < 3; j++)
    {
      int id = rng() % n;
      assert(q.contains(id) == present[id]);
      if (present[id]) assert(q.key(id) == keys[id]);
    }
  }
  auto copy = q;
  while (!q.empty())
  {
    assert(q.top().first == copy.top().first);
    q.pop(), copy.pop();
  }
  assert(copy.empty());
}

int main()
{
  random_test<ll>(less<ll>{});
  random_test<ll>(greater<ll>{});
  random_test<i128>(less<i128>{});
  FibonacciHeap<ll> q(10000);
  for (int i = 0; i < 10000; i++) q.push(i, 20000 + i);
  q.pop();
  for (int i = 9999; i > 0; i--) q.decrease_key(i, -i);
  for (int i = 9999; i > 0; i--)
  {
    assert(q.top() == make_pair(ll(-i), i));
    q.pop();
  }
  assert(q.empty());
  q.clear();
  q.push_or_decrease(5, 9), q.push_or_decrease(5, 4);
  assert(q.top() == make_pair(4LL, 5));
  cout << "Hello World\n";
}
