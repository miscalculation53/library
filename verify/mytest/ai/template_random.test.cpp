#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_random.hpp"

// Test focus: closed/half-open integer ranges are uniform, including the full ll range.
void test_integer()
{
  mt.seed(0);
  array<int, 3> cnt{};
  repi(_, 30000)
  {
    int x = randint<int>(-1, 1);
    assert(-1 <= x && x <= 1);
    cnt[x + 1]++;
    int y = randrange<int>(-2, 3);
    assert(-2 <= y && y < 3);
  }
  for (int c : cnt) assert(9000 <= c && c <= 11000);

  randint<ll>(numeric_limits<ll>::lowest(), numeric_limits<ll>::max());
}

// Test focus: real ranges and probability-zero/one/intermediate boolean sampling.
void test_real_and_bool()
{
  mt.seed(1);
  repi(_, 10000)
  {
    double x = randreal(-2.5, 4.5);
    assert(-2.5 <= x && x < 4.5);
    assert(!randbool(0));
    assert(randbool(1));
  }
  int cnt = 0;
  repi(_, 10000) cnt += randbool(0.3);
  assert(2700 <= cnt && cnt <= 3300);
}

// Test focus: range sampling is unique, optionally sorted, and uniform for small cases.
void test_sample()
{
  mt.seed(2);
  assert((random_sample_range_array<0, true>(3, 3).empty()));
  assert((random_sample_range_array<3, true>(-2, 1) == array<ll, 3>{-2, -1, 0}));

  repi(_, 1000)
  {
    auto a = random_sample_range_array<5, true>(-10, 10);
    assert(is_sorted(a.begin(), a.end()));
    assert(a.front() >= -10 && a.back() < 10);
    assert(unique(a.begin(), a.end()) == a.end());
  }

  auto a = random_sample_range_vector<false>(-1000, 1000, 300);
  set<ll> st(a.begin(), a.end());
  assert(int(st.size()) == 300);
  assert(*st.begin() >= -1000 && *st.rbegin() < 1000);

  array<int, 3> cnt{};
  repi(_, 30000)
  {
    auto b = random_sample_range_array<2, true>(0, 3);
    if (b == array<ll, 2>{0, 1}) cnt[0]++;
    else if (b == array<ll, 2>{0, 2}) cnt[1]++;
    else if (b == array<ll, 2>{1, 2}) cnt[2]++;
    else assert(false);
  }
  for (int c : cnt) assert(9000 <= c && c <= 11000);
}

int main()
{
  test_integer();
  test_real_and_bool();
  test_sample();

  cout << "Hello World" << endl;
}
