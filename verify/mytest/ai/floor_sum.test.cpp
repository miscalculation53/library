#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "math/mod_of_linear/floor_sum.hpp"
#include "math/mod_of_linear/min_of_mod_of_linear.hpp"
#include "math/bigint.hpp"
#include "math/modint/modint.hpp"

i128 naive_floor(ll l, ll r, ll m, ll a, ll b)
{
  i128 ans = 0;
  for (ll i = l; i < r; ++i)
  {
    i128 v = i128(a) * i + b;
    ans += v / m - (v % m < 0);
  }
  return ans;
}

void small()
{
  for (ll m = 1; m <= 25; ++m)
    for (ll a = -30; a <= 30; ++a)
      for (ll b = -30; b <= 30; ++b)
        for (ll n : {0, 1, 2, 9, 30})
        {
          const i128 expected = naive_floor(0, n, m, a, b);
          assert(floor_sum<i128>(n, m, a, b) == expected);
          assert(floor_sum<ll>(n, m, a, b) == expected);
          assert(floor_sum<modint998244353>(n, m, a, b) == modint998244353(expected));
          assert(floor_sum<static_modint32<2>>(n, m, a, b) == static_modint32<2>(expected));
          assert(floor_sum<i128>(-n, n, m, a, b) == naive_floor(-n, n, m, a, b));
        }

  mt19937_64 rng(20260923);
  for (int it = 0; it < 30000; ++it)
  {
    ll m = 1 + rng() % 80, n = rng() % 160;
    ll a = ll(rng() % 401) - 200, b = ll(rng() % 401) - 200;
    ll l = rng() % (m + 1), r = rng() % (m + 1);
    if (l > r) swap(l, r);
    ll count = 0, mn = m;
    for (ll i = 0; i < n; ++i)
    {
      ll v = ((a * i + b) % m + m) % m;
      count += l <= v && v < r;
      chmin(mn, v);
    }
    assert(count_mod_of_linear(n, m, a, b, l, r) == count);
    if (n > 0) assert(min_of_mod_of_linear(n, m, a, b) == mn);
  }
}

void boundaries()
{
  using Big = BigInteger<>;
  using mint = modint998244353;
  const ll hi = numeric_limits<ll>::max(), lo = numeric_limits<ll>::min();
  const vc<ll> vals = {lo, lo + 1, -1, 0, 1, hi - 1, hi};
  for (ll a : vals) for (ll b : vals) for (ll m : {1LL, 2LL, 97LL, hi})
  {
    assert(floor_sum<i128>(15, m, a, b) == naive_floor(0, 15, m, a, b));
    assert(floor_sum<i128>(-15, 15, m, a, b) == naive_floor(-15, 15, m, a, b));
    ll mn = m, cnt = 0;
    for (ll i = 0; i < 15; ++i)
    {
      ll v = (i128(a) * i + b) % m;
      if (v < 0) v += m;
      chmin(mn, v);
      cnt += v < m / 2;
    }
    assert(min_of_mod_of_linear(15, m, a, b) == mn);
    assert(count_mod_of_linear(15, m, a, b, 0, m / 2) == cnt);
  }
  // 答えが 128 bit を超える場合も、商・剰余は入力幅に応じた型でよい。
  Big tri = Big(hi) * Big(hi - 1) / 2;
  assert(floor_sum<Big>(hi, 1, hi, lo) == Big(hi) * tri + Big(lo) * Big(hi));
  mint tri_mod = mint(hi) * mint(hi - 1) / 2;
  assert(floor_sum<mint>(hi, 1, hi, lo) == mint(hi) * tri_mod + mint(lo) * mint(hi));
  // 区間長が ll に収まらない場合と、a*l+b が ll を超える場合。
  assert(floor_sum<i128>(lo, hi, 1, 1, 0) == -i128(hi) * 2 - 1);
  assert(floor_sum<i128>(lo, hi, hi, hi - 1, hi) == i128(3) - hi);
  assert(floor_sum<i128>(lo, lo + 10, hi, hi, lo) == i128(10) * lo + 45 - 20);
  for (ll m : {1LL, 2LL, 97LL, hi})
  {
    assert(count_mod_of_linear(hi, m, hi - 1, lo, 0, m) == hi);
    ll b = lo % m;
    if (b < 0) b += m;
    assert(min_of_mod_of_linear(hi, m, hi - 1, lo) == b % gcd(hi - 1, m));
  }
  // 正規化後の floor sum は最大級の n,m でも i128 に収まる。
  assert(floor_sum<i128>(hi, hi, hi - 1, 0) == i128(hi - 1) * (hi - 2) / 2);
  assert(count_mod_of_linear(hi, hi, hi - 1, 0, 0, hi / 2) == hi / 2);
}

using Big = BigInteger<>;

// 多倍長整数による基準値。区間の平行移動と符号付きの床除算を直接行う。
Big reference_floor(Big n, Big m, Big a, Big b)
{
  Big ans = 0;
  while (n > 0)
  {
    Big qa = divfloor(a, m), qb = divfloor(b, m);
    ans += qa * n * (n - 1) / 2 + qb * n;
    a -= qa * m;
    b -= qb * m;
    Big y = a * n + b;
    if (y < m) break;
    n = y / m;
    b = y % m;
    swap(a, m);
  }
  return ans;
}

Big reference_floor(Big l, Big r, Big m, Big a, Big b)
{
  return reference_floor(r - l, m, a, a * l + b);
}

template <class R, class... Args>
void check_integer_result(const Big &expected, Args... args)
{
  Big modulus = 1;
  for (int i = 0; i < int(sizeof(R) * 8); ++i) modulus *= 2;
  assert(safemod(Big(floor_sum<R>(args...)) - expected, modulus) == 0);
}

template <class... Args>
void check_types(Args... args)
{
  const Big expected = reference_floor(Big(args)...);
  check_integer_result<signed char>(expected, args...);
  check_integer_result<unsigned char>(expected, args...);
  check_integer_result<short>(expected, args...);
  check_integer_result<unsigned short>(expected, args...);
  check_integer_result<int>(expected, args...);
  check_integer_result<unsigned int>(expected, args...);
  check_integer_result<ll>(expected, args...);
  check_integer_result<ull>(expected, args...);
  check_integer_result<i128>(expected, args...);
  check_integer_result<u128>(expected, args...);
  assert(floor_sum<Big>(args...) == expected);
  using mint = modint998244353;
  assert(floor_sum<mint>(args...) == mint(stoll(safemod(expected, Big(mint::mod())).to_string())));
}

void inferred_inputs_and_wrapping()
{
  const ll hi = numeric_limits<ll>::max(), lo = numeric_limits<ll>::min();
  const ull umax = numeric_limits<ull>::max();
  assert(floor_sum<ll>(2, 1, hi, 1) == lo + 1);
  assert(floor_sum<ull>(2, 1, hi, 1) == ull(hi) + 2);
  assert(floor_sum<ll>(1u, 1ull, 0, lo) == lo);
  check_types(0, 1, lo, umax);
  check_types(numeric_limits<int>::max(), 1, numeric_limits<int>::max(), numeric_limits<int>::max());
  check_types(4u, 5ull, -3, lo);
  check_types(umax, 1, umax, umax);
  check_types(umax, umax, umax - 1, umax - 1);
  check_types(umax, umax, -1, lo);
  check_types(lo, hi, 1u, lo, umax);
  check_types(lo, umax, umax, lo, umax);
  check_types(lo, lo, umax, umax, lo);
  check_types(umax, umax, 1, umax, umax);
  check_types((short)-200, (unsigned short)500, (unsigned char)13, (signed char)-127, umax);

  mt19937_64 rng(20260924);
  for (int it = 0; it < 200; ++it)
  {
    ull n = rng(), m = rng() | 1, a = rng(), b = rng();
    check_types(n, m, a, b);
    ll l = -ll(rng() >> 1), sa = -ll(rng() >> 1);
    check_types(l, n, m, sa, b);
    // 全引数 int の経路。
    check_types(int(rng() >> 33), 1 + int(rng() >> 33), -int(rng() >> 33), int(rng() >> 33));
  }

  const u128 top = ~u128(0), half = u128(1) << 127;
  const i128 low = numeric_limits<i128>::min();
  check_types(top, 1, top, top);
  check_types(top, top, top - 1, top - 1);
  check_types(top, half + 13, half - 11, top - 27);
  check_types(low, top, top - 3, top - 5, low);
  check_types(low, low + 7, top, low, top);
  for (int it = 0; it < 30; ++it)
  {
    auto rand128 = [&]() { return (u128(rng()) << 64) | rng(); };
    u128 n = rand128(), m = rand128() | 1, a = rand128(), b = rand128();
    check_types(n, m, a, b);
  }
}

void runtime_width_boundaries()
{
  const ull cut = numeric_limits<uint>::max();
  for (ull n : {cut - 1, cut, cut + 1}) for (ull m : {cut - 1, cut, cut + 1})
  {
    check_types(n, m, m - 1, m - 1);
    check_types(-ll(n), n, m, -ll(m) - 1, numeric_limits<ll>::min());
    // 入力の型や係数の大きさと、正規化後の計算幅は独立。
    check_types(u128(n), u128(m), (u128(1) << 100) + 123, numeric_limits<i128>::min());
    check_types(u128(n), u128(m), u128(m - 1), u128(m - 1));
    const ll a = ll(m / 2 + 1), b = ll(m - 3);
    ll low = 0, high = m;
    while (low < high)
    {
      const ll mid = (low + high) / 2;
      Big count = reference_floor(Big(n), Big(m), Big(a), Big(b) + m)
                - reference_floor(Big(n), Big(m), Big(a), Big(b) + m - (mid + 1));
      if (count > 0) high = mid;
      else low = mid + 1;
    }
    assert(min_of_mod_of_linear(n, m, a, b) == low);
  }
  // 128 bit 型に入っている 64 bit の値。
  check_types(u128(1) << 50, (u128(1) << 55) + 9, (u128(1) << 48) + 7, 5);
}

int main()
{
  small();
  boundaries();
  inferred_inputs_and_wrapping();
  runtime_width_boundaries();
  cout << "Hello World\n";
}
