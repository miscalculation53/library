// g++-15 -std=c++17 -O2 -DNDEBUG -I . benchmarks/arithmetic_product.cpp -o /tmp/arithmetic_product_bench
// /tmp/arithmetic_product_bench > benchmarks/arithmetic_product.csv
#include "math/fps/factorial.hpp"
#include "math/mod_of_linear/split_linear_mod.hpp"

using Clock = chrono::steady_clock;
volatile ull checksum_sink = 0;

template <class mint>
struct Query
{
  ll n;
  mint a, b;
};

const char *names[] = {"factorial_ratio", "wilson", "grow_wilson", "grow_inverse_table",
                       "grow_direct32", "grow_directB", "grow_direct2B", "nearest",
                       "nearest_adaptive", "library"};

template <class mint>
struct Candidates
{
  Factorial<mint> fact;
  int k;
  ll block;
  vc<mint> inverse_blocks;

  explicit Candidates(int k_) : fact(k_), k(k_), block(1LL << k_) {}

  void prepare_inverse_blocks()
  {
    const int last = (mint::mod() - 1) / block;
    inverse_blocks.resize(last + 1);
    mint prod = 1;
    for (int i = 0; i <= last; ++i)
    {
      inverse_blocks[i] = prod;
      prod *= fact.factorial(i * block);
    }
    prod = prod.inv();
    for (int i = last; i >= 0; --i)
    {
      inverse_blocks[i] *= prod;
      prod *= fact.factorial(i * block);
    }
  }

  static mint direct(const Query<mint> &q)
  {
    mint ans = 1, x = q.b;
    for (ll i = 0; i < q.n; ++i, x += q.a) ans *= x;
    return ans;
  }

  template <int method>
  mint product(const Query<mint> &q) const
  {
    const auto [n, a, b] = q;
    if constexpr (method == 9) return fact.arithmetic_product(n, a, b);
    if (n == 0) return 1;
    if (a == 0) return b.pow(n);
    if constexpr (method >= 4)
    {
      const ll threshold = method == 4 || method >= 7 ? 32 : method == 5 ? block : 2 * block;
      if (n <= threshold) return direct(q);
    }
    const ll p = mint::mod(), c = (b / a).val();
    if (c == 0 || n > p - c) return 0;
    const ll end = c + n - 1;
    if constexpr (method == 0)
      return a.pow(n) * fact.factorial(end) / fact.factorial(c - 1);
    else if constexpr (method == 1)
    {
      mint ans = a.pow(n) * fact.factorial(end) * fact.factorial(p - c);
      return c % 2 ? -ans : ans;
    }
    else if constexpr (method >= 7)
    {
      const ll last = (p - 1) >> k;
      const ll left = p - 1 - (min(last, (p - c + block / 2) >> k) << k);
      const ll right = min(last, (end + block / 2) >> k) << k;
      if constexpr (method >= 8)
        if (n <= abs(left - (c - 1)) + abs(right - end)) return direct(q);
      mint ans = fact.factorial(right) * fact.factorial(p - 1 - left), den = 1;
      if (left % 2 == 0) ans = -ans;
      mint tail = 1;
      for (ll j = min(left, c - 1) + 1; j <= max(left, c - 1); ++j) tail *= mint::raw(j);
      if (left < c - 1) den *= tail;
      else ans *= tail;
      tail = 1;
      for (ll j = min(right, end) + 1; j <= max(right, end); ++j) tail *= mint::raw(j);
      if (right > end) den *= tail;
      else ans *= tail;
      if (left < c - 1 || right > end) ans /= den;
      return a.pow(n) * ans;
    }
    else
    {
      const ll right = end >> k << k;
      const ll left = method == 3 ? ((c - 1 + block - 1) >> k << k)
                                : p - 1 - ((p - c) >> k << k);
      if (left >= right) return direct(q);
      mint ans = fact.factorial(right);
      if constexpr (method == 3) ans *= inverse_blocks[left >> k];
      else
      {
        ans *= fact.factorial(p - 1 - left);
        if (left % 2 == 0) ans = -ans;
      }
      for (ll j = c; j <= left; ++j) ans *= mint::raw(j);
      for (ll j = right + 1; j <= end; ++j) ans *= mint::raw(j);
      return a.pow(n) * ans;
    }
  }
};

template <int method, class mint>
double run(const Candidates<mint> &f, const vc<Query<mint>> &qs, ull &checksum)
{
  const auto start = Clock::now();
  checksum = 0;
  for (const auto &q : qs) checksum += f.template product<method>(q).val();
  const auto end = Clock::now();
  checksum_sink ^= checksum;
  return chrono::duration<double, milli>(end - start).count();
}

template <class mint>
void compare(const Candidates<mint> &f, const string &label, const vc<Query<mint>> &qs)
{
  // 全クエリの一致を確認してから、順番を入れ替えて 5 回計測する。
  for (const auto &q : qs)
  {
    mint expected = f.template product<0>(q);
    if (f.template product<1>(q) != expected || f.template product<2>(q) != expected
        || f.template product<3>(q) != expected || f.template product<4>(q) != expected
        || f.template product<5>(q) != expected || f.template product<6>(q) != expected
        || f.template product<7>(q) != expected || f.template product<8>(q) != expected
        || f.template product<9>(q) != expected)
      abort();
  }
  array<vc<double>, 10> times;
  array<ull, 10> checksums{};
  for (int rep = 0; rep < 5; ++rep) for (int j = 0; j < 10; ++j)
  {
    const int method = (j + 3 * rep) % 10;
    double t = 0;
    switch (method)
    {
      case 0: t = run<0>(f, qs, checksums[0]); break;
      case 1: t = run<1>(f, qs, checksums[1]); break;
      case 2: t = run<2>(f, qs, checksums[2]); break;
      case 3: t = run<3>(f, qs, checksums[3]); break;
      case 4: t = run<4>(f, qs, checksums[4]); break;
      case 5: t = run<5>(f, qs, checksums[5]); break;
      case 6: t = run<6>(f, qs, checksums[6]); break;
      case 7: t = run<7>(f, qs, checksums[7]); break;
      case 8: t = run<8>(f, qs, checksums[8]); break;
      case 9: t = run<9>(f, qs, checksums[9]); break;
    }
    times[method].push_back(t);
  }
  for (int method = 0; method < 10; ++method)
  {
    sort(times[method].begin(), times[method].end());
    cout << mint::mod() << ',' << f.k << ',' << label << ',' << qs.size() << ','
         << names[method] << ',' << times[method][2] << ',' << checksums[method] << '\n';
  }
  cout.flush();
}

template <class mint>
vc<Query<mint>> contest_queries(ll n, ll a, ll b, ll m)
{
  a %= m;
  b %= m;
  const ll g = gcd(a, m), period = m / g;
  vc<Query<mint>> qs;
  if (n >= period) qs.push_back({period, mint(g), mint(b % g)});
  for (auto part : split_linear_mod(n % period, m, a, b))
    qs.push_back({part.n, mint(part.a), mint(part.b)});
  return qs;
}

template <class mint>
void benchmark(int k)
{
  const auto start = Clock::now();
  Candidates<mint> f(k);
  const auto built = Clock::now();
  f.prepare_inverse_blocks();
  const auto inverted = Clock::now();
  cout << mint::mod() << ',' << k << ",setup,1,factorial_table,"
       << chrono::duration<double, milli>(built - start).count() << ",0\n";
  cout << mint::mod() << ',' << k << ",setup,1,inverse_table_extra,"
       << chrono::duration<double, milli>(inverted - built).count() << ",0\n";
  mt19937_64 rng(20260924);
  const ll p = mint::mod();
  constexpr int count = 12000;
  for (auto label : {"short_1_32", "short_33_halfB", "around_block", "long", "contains_zero", "short_zero"})
  {
    vc<Query<mint>> qs;
    for (int i = 0; i < count; ++i)
    {
      ll n;
      if (string(label) == "short_1_32") n = 1 + rng() % 32;
      else if (string(label) == "short_33_halfB") n = 33 + rng() % (f.block / 2 - 32);
      else if (string(label) == "short_zero") n = 1 + rng() % f.block;
      else if (string(label) == "around_block") n = f.block / 2 + rng() % (f.block * 3 / 2 + 1);
      else n = 1000000 + rng() % 100000000;
      const bool zero = string(label) == "contains_zero" || string(label) == "short_zero";
      const ll c = zero ? p - rng() % n : 1 + rng() % (p - n);
      mint a = mint::raw(1 + rng() % (p - 1));
      qs.push_back({n, a, a * mint(c)});
    }
    compare(f, label, qs);
  }
  compare(f, "contest_sample4", contest_queries<mint>(600000000, 998244353, 206, 924844033));
  compare(f, "contest_sample5", contest_queries<mint>(1000000000, 444444444, 314159265, 897932384));
}

int main()
{
  cout << fixed << setprecision(3);
  cout << "mod,k,dataset,queries,method,median_ms,checksum\n";
  benchmark<modint998244353>(9);
  benchmark<modint1000000007>(11);
}
