#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/flat_dvec.hpp"
#include "template/template_vector.hpp"
#include "math/modint/modint.hpp"

void test_element_type_only()
{
  using mint = modint998244353;
  ll n = 3;
  int m = 4;
  FlatDvec<mint> flat({n, 2}, 0);
  assert(flat.ndim() == 2 && flat.size() == 6);
  assert((flat.shape() == vc<size_t>{3, 2}));
  assert(flat.stride(0) == 2 && flat.stride(1) == 1);
  for (mint x : flat) assert(x == 0);
  flat.at(n - 1, 1) = 7;
  flat(n - 1, 1) += 5;
  assert(flat.content().back() == 12);
  const auto &cflat = flat;
  static_assert(is_same_v<decltype(cflat.at(0, 0)), const mint &>);
  assert(cflat(2, 1) == 12);
  fill(flat, 9);
  for (mint x : flat) assert(x == 9);
  flat.fill(0);
  for (mint x : flat) assert(x == 0);

  FlatDvec<mint> three({2, n, m}, 1);
  assert(three.ndim() == 3 && three.size() == 24);
  three(1, 2, 3) += 6;
  assert(three.at(1, 2, 3) == 7);
  FlatDvec<mint> value_initialized({n, 2});
  for (mint x : value_initialized) assert(x == 0);
  FlatDvec<mint, 2> fixed({n, 2}, 0);
  assert(fixed.ndim() == 2 && fixed.size() == 6);
  static_assert(is_same_v<decltype(fixed.shape()), const array<size_t, 2> &>);

  int sizes[] = {2, 3};
  FlatDvec<mint> from_array(sizes, 2);
  FlatDvec<mint> from_std_array(array<int, 2>{2, 3}, 3);
  assert(from_array.at(1, 2) == 2 && from_std_array.at(1, 2) == 3);

  auto copied = three;
  copied(1, 2, 3) = 100;
  assert(three(1, 2, 3) == 7 && copied(1, 2, 3) == 100);
  auto moved = std::move(copied);
  assert(moved.ndim() == 3 && moved.at(1, 2, 3) == 100);

  FlatDvec<bool> bits({n, 2}, false);
  bits.at(2, 1) = true;
  const auto &cbits = bits;
  assert(cbits(2, 1) && !cbits(0, 0));
  fill(bits, true);
  for (bool x : bits) assert(x);

  FlatDvec<mint> empty;
  assert(empty.ndim() == 0 && empty.empty() && empty.shape().empty());
  empty.fill(4);
  for (size_t zero = 0; zero < 3; zero++)
  {
    array<ll, 3> shape{LLONG_MAX, LLONG_MAX, LLONG_MAX};
    shape[zero] = 0;
    FlatDvec<mint> zero_dimension(shape, 0);
    assert(zero_dimension.ndim() == 3 && zero_dimension.empty());
    assert(zero_dimension.size(zero) == 0);
    assert(zero_dimension.stride(2) == 1);
  }
}

void test_construction()
{
  ll n = 3, m = 5;
  FlatDvec dp({n, m, 2}, 7LL);
  static_assert(is_same_v<decltype(dp), FlatDvec<ll, 3>>);
  assert((dp.shape() == array<size_t, 3>{3, 5, 2}));
  assert(dp.size() == 30 && !dp.empty());
  assert(dp.size(0) == 3 && dp.size(1) == 5 && dp.size(2) == 2);
  assert(dp.stride(0) == 10 && dp.stride(1) == 2 && dp.stride(2) == 1);
  for (ll x : dp) assert(x == 7);

  dp.at(1, 4, 0) = 10;
  assert(dp(1, 4, 0) == 10);
  assert(dp.content()[18] == 10);
  assert(&dp(2, 4, 1) == &dp.content().back());

  auto copied = dp;
  copied(1, 4, 0) = -3;
  assert(dp(1, 4, 0) == 10 && copied(1, 4, 0) == -3);
  auto moved = std::move(copied);
  assert(moved.at(1, 4, 0) == -3 && moved.shape() == dp.shape());

  const auto &cdp = dp;
  static_assert(is_same_v<decltype(cdp(0, 0, 0)), const ll &>);
  static_assert(is_same_v<decltype(cdp.at(0, 0, 0)), const ll &>);
  static_assert(is_same_v<decltype(*cdp.begin()), const ll &>);
  assert(cdp.at(1, 4, 0) == 10);
  assert(accumulate(cdp.begin(), cdp.end(), 0LL) == 7 * 30 + 3);

  dp.fill(-2);
  for (auto &x : dp) x++;
  for (ll x : dp) assert(x == -1);
  fill(dp, 3);
  for (ll x : dp) assert(x == 3);

  int sz[] = {2, 4};
  FlatDvec from_array(sz, string("x"));
  from_array(1, 3) += "y";
  assert(from_array(1, 3) == "xy");
  FlatDvec<int, 2> value_initialized({2, 3});
  for (int x : value_initialized) assert(x == 0);

  FlatDvec one({5}, 4);
  assert(one.size() == 5 && one.stride(0) == 1);
  one.at(3) = 9;
  assert(one.content()[3] == 9);

  FlatDvec equal({3, 3, 3}, 0);
  assert(equal.stride(0) == 9 && equal.stride(1) == 3 && equal.stride(2) == 1);

  FlatDvec bits({2, 3}, false);
  bits(1, 2) = true;
  bits.at(0, 1) = bits(1, 2);
  const auto &cbits = bits;
  assert(cbits(1, 2) && cbits.at(0, 1));
  assert(!cbits(1, 0));
  bits.fill(true);
  for (bool x : bits) assert(x);
}

template <size_t D>
void test_random()
{
  mt19937 rng(12345 + D);
  for (int trial = 0; trial < 100; trial++)
  {
    array<int, D> shape;
    size_t total = 1;
    for (auto &n : shape) n = rng() % 5, total *= n;
    FlatDvec dp(shape, -1);
    FlatDvec<int> dynamic(shape, -1);
    assert(dynamic.ndim() == D && dynamic.size() == total);
    assert(dp.size() == total && dp.empty() == (total == 0));
    if (total == 0) continue;

    // 全座標への書き込みを、宣言順の一次元配列と照合する。
    vc<int> expected(total, -1);
    for (int step = 0; step < 500; step++)
    {
      array<int, D> ids;
      size_t index = 0;
      for (size_t d = 0; d < D; d++)
      {
        ids[d] = rng() % shape[d];
        index = index * shape[d] + ids[d];
      }
      int value = int(rng() % 100000);
      apply([&](auto... is) { dp.at(is...) = value; }, ids);
      apply([&](auto... is) { dynamic.at(is...) = value; }, ids);
      expected[index] = value;
    }
    vc<bool> seen(total, false);
    for (size_t index = 0; index < total; index++)
    {
      array<int, D> ids;
      size_t rest = index;
      for (size_t d = D; d-- > 0;)
        ids[d] = rest % shape[d], rest /= shape[d];
      const int &value = apply([&](auto... is) -> const int & { return dp(is...); }, ids);
      assert(value == expected[index]);
      assert(apply([&](auto... is) { return dynamic(is...); }, ids) == expected[index]);
      size_t pos = &value - dp.content().data();
      assert(pos == index && !seen[pos]);
      seen[pos] = true;
    }
    assert(dynamic.content() == dp.content());

    size_t stride = 1;
    for (size_t d = D; d-- > 0;)
    {
      assert(dp.stride(d) == stride);
      stride *= shape[d];
    }
  }
}

void test_empty()
{
  FlatDvec<ll, 3> empty;
  assert(empty.empty() && empty.size() == 0);
  assert((empty.shape() == array<size_t, 3>{0, 0, 0}));
  assert(empty.stride(0) == 0 && empty.stride(1) == 0 && empty.stride(2) == 1);
  assert(empty.begin() == empty.end());
  for (size_t zero = 0; zero < 3; zero++)
  {
    array<ll, 3> sz{LLONG_MAX, LLONG_MAX, LLONG_MAX};
    sz[zero] = 0;
    FlatDvec dp(sz, 4);
    assert(dp.empty() && dp.size() == 0 && dp.size(zero) == 0);
    dp.fill(9);
    assert(dp.begin() == dp.end());
  }
}

void test_lcs()
{
  // 非正方形の DP で、更新途中の読み取りも入れ子の vector と一致する。
  string s = "abracadabra", t = "avada";
  auto expected = dvec({s.size() + 1, t.size() + 1}, 0);
  FlatDvec dp({ll(s.size()) + 1, ll(t.size()) + 1}, 0);
  for (size_t i = 0; i < s.size(); i++)
    for (size_t j = 0; j < t.size(); j++)
    {
      expected[i + 1][j + 1] = s[i] == t[j] ? expected[i][j] + 1 : max(expected[i][j + 1], expected[i + 1][j]);
      dp(i + 1, j + 1) = s[i] == t[j] ? dp(i, j) + 1 : max(dp(i, j + 1), dp(i + 1, j));
      assert(dp.at(i + 1, j + 1) == expected[i + 1][j + 1]);
    }
}

int main()
{
  test_element_type_only();
  test_construction();
  test_random<1>();
  test_random<2>();
  test_random<3>();
  test_random<4>();
  test_random<5>();
  test_empty();
  test_lcs();
  cout << "Hello World" << endl;
}
