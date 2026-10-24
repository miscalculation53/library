#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/fenwick_tree/fenwick_tree_01.hpp"

template <class Word>
void check()
{
  mt19937 rng(20261022 + sizeof(Word));
  constexpr int B = 8 * sizeof(Word);
  for (int n : {0, 1, B - 1, B, B + 1, 2 * B, 2 * B + 1, 129, 257})
  {
    vc<int> a(n);
    for (auto &x : a) x = rng() & 1;
    FenwickTree01<int, Word> fw(a);
    repi(step, 300)
    {
      if (n > 0)
      {
        int p = rng() % n, x = rng() & 1;
        a[p] = x, fw.set(p, x);
      }
      int l = rng() % (n + 1), r = rng() % (n + 1);
      if (l > r) swap(l, r);
      assert(fw.sum(l, r) == accumulate(a.begin() + l, a.begin() + r, 0));
      assert(fw.sum(0, r) == fw.sum(r));
    }
    repi(l, n + 1)
    {
      int total = 0;
      repi(r, l, n + 1)
      {
        assert(fw.sum(l, r) == total);
        if (r < n) total += a[r];
      }
    }
  }
}

int main()
{
  check<uint8_t>();
  check<uint16_t>();
  check<uint32_t>();
  check<uint64_t>();
  cout << "Hello World" << endl;
}
