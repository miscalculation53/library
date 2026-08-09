#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "algo/two_pointers.hpp"

// Test focus: right_ok/left_ok match brute-force boundaries while callbacks always
// observe the documented [l, r) window, including empty arrays.
void test_random()
{
  mt19937 rng(123456789);
  repi(_, 500)
  {
    int n = rng() % 50;
    vc<ll> a(n), pref(n + 1);
    repi(i, n)
    {
      a[i] = rng() % 10;
      pref[i + 1] = pref[i] + a[i];
    }
    ll limit = rng() % 100;
    ll sum = 0;
    auto add_r = [&](int l, int r)
    {
      assert(sum == pref[r] - pref[l]);
      sum += a[r];
    };
    auto del_l = [&](int l, int r)
    {
      assert(sum == pref[r] - pref[l]);
      sum -= a[l];
    };
    auto judge = [&](int l, int r)
    {
      assert(sum == pref[r] - pref[l]);
      return sum <= limit;
    };

    auto rok = right_ok(n, add_r, del_l, judge);
    repi(l, n + 1)
    {
      int r = l;
      while (r < n && pref[r + 1] - pref[l] <= limit)
        r++;
      assert(rok[l] == r);
    }

    sum = 0;
    auto lok = left_ok(n, add_r, del_l, judge);
    repi(r, n + 1)
    {
      int l = r;
      while (l > 0 && pref[r] - pref[l - 1] <= limit)
        l--;
      assert(lok[r] == l);
    }
  }
}

int main()
{
  test_random();
  cout << "Hello World" << endl;
}
