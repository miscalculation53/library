#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/query/point_set_range_frequency.hpp"

template <class T>
void check(const vc<T> &alphabet)
{
  mt19937 rng(20261119);
  for (int n : {0, 1, 2, 7, 31, 64, 129})
  {
    vc<T> current(n);
    for (auto &x : current) x = alphabet[rng() % alphabet.size()];
    PointSetRangeFrequency<T> rf(current);
    vc<int> expected;
    repi(step, 700)
    {
      T x = alphabet[rng() % alphabet.size()];
      if (n > 0 && rng() % 2 == 0)
      {
        int p = rng() % n;
        current[p] = x;
        rf.point_set(p, x);
      }
      else
      {
        int l = rng() % (n + 1), r = rng() % (n + 1);
        if (l > r) swap(l, r);
        rf.range_frequency(l, r, x);
        int count = 0;
        repi(i, l, r) count += current[i] == x;
        expected.eb(count);
      }
      if (step % 97 == 0) assert(rf.template run<int>() == expected);
    }
    assert(rf.template run<int>() == expected);
    assert(rf.template run<ll>() == vl(ALL(expected)));
  }
}

int main()
{
  check<int>({INT_MIN, -7, 0, 1, 5, INT_MAX});
  check<string>({"", "a", "b", "pear", "apple"});
  check<i128>({-(i128(1) << 120), 0, i128(1) << 120});
  check<pair<int, int>>({{0, 0}, {-1, 3}, {4, 5}});
  PointSetRangeFrequency<int> empty;
  assert(empty.run().empty());
  empty.range_frequency(0, 0, 3);
  assert((empty.run() == vl{0}));
  // 戻り値の型は、全値の合計でなく各回答を表せればよい。
  vc<int> a(600);
  iota(ALL(a), 0);
  PointSetRangeFrequency<int> small_results(a);
  repi(x, 600) small_results.range_frequency(0, 600, x);
  assert(small_results.run<unsigned char>() == vc<unsigned char>(600, 1));
  cout << "Hello World" << endl;
}
