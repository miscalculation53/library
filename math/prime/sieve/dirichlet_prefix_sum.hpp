#pragma once

#include "math/prime/sieve/dirichlet_convolution.hpp"

/**
 * @brief Dirichlet 積と根付き商集合上の累積和テーブル
 * @docs docs/math/prime/sieve/dirichlet_prefix_sum.md
 */

// 環は DirichletPrefixSum<R> 側で指定し、ラムダには R{} を渡す

inline constexpr auto e_prefix_sum = [](ll n, auto ring)
{ return n == 0 ? decltype(ring)::e0() : decltype(ring)::e1(); };
inline constexpr auto zeta_prefix_sum = [](ll n, auto ring)
{ return internal::multiplicative_from_integer<decltype(ring)>(n); };
inline constexpr auto id_prefix_sum = [](ll n, auto ring)
{
  using R = decltype(ring);
  const auto a = internal::multiplicative_from_integer<R>(n % 2 == 0 ? n / 2 : n);
  const auto b = n % 2 == 0
    ? R::add(internal::multiplicative_from_integer<R>(n), R::e1())
    : internal::multiplicative_from_integer<R>(n / 2 + 1);
  return R::mul(a, b);
};

namespace internal
{
  // D>1 の根付き商集合。large_value は降順、raw_index は元の i から
  // 重複を除いた位置への変換。D は固定定数として扱う。
  template <int D>
  struct dirichlet_root_prefix_sum_index
  {
    static_assert(D >= 1);
    ll n, maximum;
    int k, large;
    vc<ll> large_value;
    vc<int> raw_index;

    explicit dirichlet_root_prefix_sum_index(ll n) : n(n)
    {
      assert(n >= 0);
      maximum = iroot(n, D);
      const ll root = iroot(n, min<ll>(ll(D) + 1, 63));
      // (root+1)^D が ll の範囲を超えても、商は正しく 0 にする。
      const ll raw = ull(n) / pow_limited<ull>(root + 1, D, ull(n) + 1);
      k = int(root);
      large_value.push_back(0);
      raw_index.resize(size_t(raw) + 1);
      for (int i = 1; i <= raw; )
      {
        const ll x = iroot(n / i, D);
        const int end = int(min(raw, n / ipow<ll>(x, D)));
        const int at = int(large_value.size());
        large_value.push_back(x);
        fill(raw_index.begin() + i, raw_index.begin() + end + 1, at);
        i = end + 1;
      }
      large = int(large_value.size()) - 1;
    }
    int size() const { return k + large; }
    // v 以上の最初の位置。集合外の v も区間端点として受け付ける。
    int index(ll v) const
    {
      if (v <= k) return int(v) - 1;
      if (v > maximum) return size();
      return size() - raw_index[size_t(n / ipow<ll>(v, D))];
    }
    ll value(int i) const { return i < k ? ll(i) + 1 : large_value[size() - i]; }
  };

  // 正の商集合を昇順に並べた座標。index(v) は v 以上の最初の位置。
  // index(0)=-1, index(n+1)=size() も内部の区間端点として使う。
  template <>
  struct dirichlet_root_prefix_sum_index<1>
  {
    ll n;
    int k, large;
    explicit dirichlet_root_prefix_sum_index(ll n) : n(n)
    {
      assert(n >= 0);
      const ll root = iroot(n, 2), l = n / (root + 1);
      k = int(root), large = int(l);
    }
    int size() const { return k + large; }
    int index(ll v) const { return v <= k ? int(v) - 1 : size() - int(n / v); }
    ll value(int i) const { return i < k ? ll(i) + 1 : n / (size() - i); }
  };
  using dirichlet_prefix_sum_index = dirichlet_root_prefix_sum_index<1>;

  template <class R, class GetF>
  typename R::S eval_dirichlet_prefix(const GetF &getF, ll n)
  {
    if constexpr (is_invocable_v<const GetF &, ll>)
      return getF(n);
    else
      return getF(n, R{});
  }
}

// F(x) を ⌊(n/i)^(1/D)⌋ の位置で持つ (D=1 が通常)
// 乗除は O(n^{2/(2D+1)}) 時間、D=1 なら O(n^{2/3}) 時間
// R は可換環、除算を使うなら体
// コンストラクタ: n, getF(, multiplicative)
template <class R, int D = 1>
struct DirichletPrefixSum
{
  using S = typename R::S;

private:
  ll n_;
  int k_, l_;
  vc<S> small_, large_;
  bool multiplicative_;
  internal::dirichlet_root_prefix_sum_index<D> coordinates_;

  struct same_shape {};
  // コンストラクタ: n, getF(, multiplicative)
  DirichletPrefixSum(const DirichletPrefixSum &a, same_shape)
    : n_(a.n_), k_(a.k_), l_(a.l_), small_(k_ + 1, R::e0()),
      large_(l_ + 1, R::e0()), multiplicative_(false), coordinates_(a.coordinates_) {}

  // k_ より大きい保持位置の、降順・1-indexed の添字。
  int large_index(ll x) const
  {
    if constexpr (D == 1) return int(n_ / x);
    else return coordinates_.raw_index[size_t(n_ / ipow<ll>(x, D))];
  }

  S coefficient(int i) const
  {
    assert(1 <= i && i <= k_);
    return R::add(small_[i], R::minus(small_[i - 1]));
  }

public:
  DirichletPrefixSum() : DirichletPrefixSum(0) {}

  explicit DirichletPrefixSum(ll n)
    : n_(n), multiplicative_(false), coordinates_(n)
  {
    k_ = coordinates_.k, l_ = coordinates_.large;
    small_.assign(k_ + 1, R::e0());
    large_.assign(l_ + 1, R::e0());
  }

  // getF: F(x) の値を得る関数
  template <class GetF, enable_if_t<is_invocable_v<const GetF &, ll>
    || is_invocable_v<const GetF &, ll, R>, int> = 0>
  DirichletPrefixSum(ll n, const GetF &getF, bool multiplicative = false)
    : DirichletPrefixSum(n)
  {
    for (int x = 1; x <= k_; x++)
      small_[x] = internal::eval_dirichlet_prefix<R>(getF, x);
    for (int i = 1; i <= l_; i++)
      large_[i] = internal::eval_dirichlet_prefix<R>(getF, value(size() - i));
    multiplicative_ = multiplicative;
    assert(!multiplicative || n == 0 || small_[1] == R::e1());
  }

  static DirichletPrefixSum unit(ll n)
  { return DirichletPrefixSum(n, e_prefix_sum, true); }

  ll n() const { return n_; }
  int size() const { return k_ + l_; }
  int index(ll x) const
  {
    assert(x >= 1 && contains(x));
    return x <= k_ ? int(x) - 1 : size() - large_index(x);
  }
  ll value(int i) const
  {
    assert(0 <= i && i < size());
    if constexpr (D == 1) return i < k_ ? ll(i) + 1 : n_ / (size() - i);
    else return i < k_ ? ll(i) + 1 : coordinates_.large_value[size() - i];
  }
  bool is_multiplicative() const { return multiplicative_; }
  // F(x) を保持しているかどうか
  bool contains(ll x) const
  {
    if constexpr (D == 1)
      return 0 <= x && x <= n_ && (x <= k_ || n_ / (n_ / x) == x);
    else
      return 0 <= x && x <= coordinates_.maximum
        && (x <= k_ || coordinates_.large_value[large_index(x)] == x);
  }
  const S &F(ll x) const
  {
    assert(contains(x));
    return x <= k_ ? small_[size_t(x)] : large_[large_index(x)];
  }
  void setF(ll x, const S &value)
  {
    assert(x >= 1 && contains(x));
    (x <= k_ ? small_[size_t(x)] : large_[large_index(x)]) = value;
    multiplicative_ = false;
  }
  DirichletSeries<R> to_series() const
  {
    vc<S> a(k_ + 1, R::e0());
    for (int i = 1; i <= k_; i++)
      a[i] = coefficient(i);
    return DirichletSeries<R>(move(a), multiplicative_);
  }
  map<ll, S> content() const
  {
    map<ll, S> res;
    for (int i = 0; i < size(); i++)
    {
      const ll x = value(i);
      res.emplace_hint(res.end(), x, F(x));
    }
    return res;
  }

private:
  void check_shape(const DirichletPrefixSum &g) const
  { assert(n_ == g.n_); (void)g; }
  void assign_series(const DirichletSeries<R> &a)
  {
    assert(k_ == a.n());
    small_[0] = R::e0();
    for (int i = 1; i <= k_; i++)
      small_[i] = R::add(small_[i - 1], a.f(i));
    multiplicative_ = a.is_multiplicative();
  }

public:
  DirichletPrefixSum operator-() const
  {
    DirichletPrefixSum res(*this);
    for (int i = 1; i <= k_; i++)
      res.small_[i] = R::minus(res.small_[i]);
    for (int i = 1; i <= l_; i++)
      res.large_[i] = R::minus(res.large_[i]);
    res.multiplicative_ = false;
    return res;
  }
  DirichletPrefixSum &operator+=(const DirichletPrefixSum &g)
  {
    check_shape(g);
    for (int i = 1; i <= k_; i++)
      small_[i] = R::add(small_[i], g.small_[i]);
    for (int i = 1; i <= l_; i++)
      large_[i] = R::add(large_[i], g.large_[i]);
    multiplicative_ = false;
    return *this;
  }
  DirichletPrefixSum &operator-=(const DirichletPrefixSum &g)
  {
    check_shape(g);
    for (int i = 1; i <= k_; i++)
      small_[i] = R::add(small_[i], R::minus(g.small_[i]));
    for (int i = 1; i <= l_; i++)
      large_[i] = R::add(large_[i], R::minus(g.large_[i]));
    multiplicative_ = false;
    return *this;
  }
  DirichletPrefixSum &operator*=(const S &a)
  {
    const S value = a;
    for (int i = 1; i <= k_; i++)
      small_[i] = R::mul(small_[i], value);
    for (int i = 1; i <= l_; i++)
      large_[i] = R::mul(large_[i], value);
    multiplicative_ = false;
    return *this;
  }
  DirichletPrefixSum &operator/=(const S &a)
  {
    const internal::dirichlet_divisor<R> divide(a);
    for (int i = 1; i <= k_; i++)
      small_[i] = divide(small_[i]);
    for (int i = 1; i <= l_; i++)
      large_[i] = divide(large_[i]);
    multiplicative_ = false;
    return *this;
  }
  DirichletPrefixSum operator+(const DirichletPrefixSum &g) const { return DirichletPrefixSum(*this) += g; }
  DirichletPrefixSum operator-(const DirichletPrefixSum &g) const { return DirichletPrefixSum(*this) -= g; }
  DirichletPrefixSum operator*(const S &a) const { return DirichletPrefixSum(*this) *= a; }
  DirichletPrefixSum operator/(const S &a) const { return DirichletPrefixSum(*this) /= a; }
  friend DirichletPrefixSum operator*(const S &a, const DirichletPrefixSum &f) { return f * a; }

private:
  // D>1 の区間集約。根の値が重複する元の添字 i を圧縮座標へ写す。
  DirichletPrefixSum convolve_root_impl(const DirichletPrefixSum &g, bool force_sparse) const
  {
    check_shape(g);
    const auto a = to_series(), b = g.to_series();
    DirichletPrefixSum res(*this, same_shape{});
    res.assign_series(force_sparse ? a.convolve_sparse(b) : a.convolve(b));
    vc<int> ai, bi;
    const int root = int(iroot(coordinates_.maximum, 2));
    long double sparse_work = 0, dense_work = 0;
    for (int i = 1; i <= root; i++)
    {
      const bool nonzero_a = a.f(i) != R::e0(), nonzero_b = b.f(i) != R::e0();
      if (nonzero_a) ai.push_back(i);
      if (nonzero_b) bi.push_back(i);
      if (!force_sparse)
      {
        const int count = min(l_, size() - coordinates_.index(ll(i) * i));
        dense_work += 2.0L * count;
        sparse_work += (int(nonzero_a) + int(nonzero_b)) * (long double)count;
      }
    }
    const bool sparse = force_sparse || ai.size() + bi.size() < size_t(root);
    const int cut = int(iroot(n_, min<ll>(2LL * D + 1, 63)));
    int direct_end = l_;
    if (!force_sparse && (sparse ? sparse_work : dense_work) > 16.0L * cut * cut)
    {
      direct_end = min(l_, size() - coordinates_.index((ll(cut) + 1) * (cut + 1)));
      const int begin = direct_end + 1;
      if (begin <= l_)
      {
        res.large_[begin] = R::minus(R::mul(small_[cut], g.small_[cut]));
        for (int i = 1; i <= cut; i++)
        {
          const S fi = a.f(i), gi = b.f(i);
          if (fi == R::e0() && gi == R::e0()) continue;
          for (int j = begin; j <= l_; )
          {
            const ll q = coordinates_.large_value[j] / i;
            const ll raw_end = min<ll>(coordinates_.raw_index.size() - 1,
              n_ / ipow<ll>(ll(i) * q, D));
            const int end = coordinates_.raw_index[size_t(raw_end)];
            const S term = R::add(R::mul(fi, g.F(q)), R::mul(gi, F(q)));
            res.large_[j] = R::add(res.large_[j], term);
            if (end < l_) res.large_[end + 1] = R::add(res.large_[end + 1], R::minus(term));
            j = end + 1;
          }
          // 全正方形を引いた分から、uv>x になった位置で寄与を戻す。
          const ll before = coordinates_.large_value[l_] / i;
          if (before >= cut) continue;
          for (int j = max(i, int(before) + 1); j <= cut; j++)
          {
            const int at = max(begin, size() - coordinates_.index(ll(i) * j) + 1);
            S term = R::mul(fi, b.f(j));
            if (i != j) term = R::add(term, R::mul(gi, a.f(j)));
            res.large_[at] = R::add(res.large_[at], term);
          }
        }
        S value = R::e0();
        for (int j = begin; j <= l_; j++)
          res.large_[j] = value = R::add(value, res.large_[j]);
      }
    }
    for (int j = 1; j <= direct_end; j++)
    {
      const ll x = coordinates_.large_value[j];
      const int m = int(iroot(x, 2));
      S value = R::minus(R::mul(small_[m], g.small_[m]));
      if (sparse)
      {
        for (int i : ai)
        {
          if (i > m) break;
          value = R::add(value, R::mul(a.f(i), g.F(x / i)));
        }
        for (int i : bi)
        {
          if (i > m) break;
          value = R::add(value, R::mul(b.f(i), F(x / i)));
        }
      }
      else
        for (int i = 1; i <= m; i++)
          value = R::add(value, R::add(R::mul(a.f(i), g.F(x / i)), R::mul(b.f(i), F(x / i))));
      res.large_[j] = value;
    }
    return res;
  }

  DirichletPrefixSum convolve_impl(const DirichletPrefixSum &g, bool force_sparse) const
  {
    if constexpr (D != 1) return convolve_root_impl(g, force_sparse);
    check_shape(g);
    const auto a = to_series(), b = g.to_series();
    DirichletPrefixSum res(*this, same_shape{});
    res.assign_series(force_sparse ? a.convolve_sparse(b) : a.convolve(b));
    vc<int> ai, bi;
    const int root = int(iroot(n_, 2));
    long double sparse_work = 0, dense_work = 0;
    for (int i = 1; i <= root; i++)
    {
      const bool nonzero_a = a.f(i) != R::e0(), nonzero_b = b.f(i) != R::e0();
      if (nonzero_a) ai.push_back(i);
      if (nonzero_b) bi.push_back(i);
      if (!force_sparse)
      {
        const ll count = min<ll>(l_, n_ / i / i);
        dense_work += 2.0L * count;
        sparse_work += (int(nonzero_a) + int(nonzero_b)) * (long double)count;
      }
    }
    const bool sparse = force_sparse || ai.size() + bi.size() < size_t(root);
    const int cut = int(iroot(n_, 3));
    int direct_end = l_;
    // 直接列挙が十分少ないときは、そのまま計算する。
    if (!force_sparse && (sparse ? sparse_work : dense_work) > 16.0L * cut * cut)
    {
      direct_end = int(min<ll>(l_, n_ / (ll(cut) + 1) / (ll(cut) + 1)));
      const int begin = direct_end + 1;
      if (begin <= l_)
      {
        // x=N/j < (cut+1)^2 では、積 uv<=x の片方は cut 以下。
        // j の区間に差分で加え、最後に累積和を取る。
        res.large_[begin] = R::minus(R::mul(small_[cut], g.small_[cut]));
        for (int i = 1; i <= cut; i++)
        {
          const S fi = a.f(i), gi = b.f(i);
          if (fi == R::e0() && gi == R::e0()) continue;
          const ll ni = n_ / i;
          for (int j = begin; j <= l_; )
          {
            const ll value = ni / j;
            const int end = int(min<ll>(l_, ni / value));
            const S term = R::add(R::mul(fi, g.F(value)), R::mul(gi, F(value)));
            res.large_[j] = R::add(res.large_[j], term);
            if (end < l_) res.large_[end + 1] = R::add(res.large_[end + 1], R::minus(term));
            j = end + 1;
          }
          // 両方 cut 以下の重複を引く。全正方形を引いた分から、
          // ij > N/d になった添字対だけを d=floor(N/(ij))+1 で戻す。
          const ll before = ni / l_;
          if (before >= cut) continue;
          for (int j = max(i, int(before) + 1); j <= cut; j++)
          {
            const int at = max(begin, int(ni / j) + 1);
            S term = R::mul(fi, b.f(j));
            if (i != j) term = R::add(term, R::mul(gi, a.f(j)));
            res.large_[at] = R::add(res.large_[at], term);
          }
        }
        S value = R::e0();
        for (int j = begin; j <= l_; j++)
          res.large_[j] = value = R::add(value, res.large_[j]);
      }
    }
    for (int j = 1; j <= direct_end; j++)
    {
      const int m = int(iroot(n_ / j, 2));
      S value = R::minus(R::mul(small_[m], g.small_[m]));
      if (sparse)
      {
        for (int i : ai)
        {
          if (i > m) break;
          value = R::add(value, R::mul(a.f(i), g.F(n_ / (ll(i) * j))));
        }
        for (int i : bi)
        {
          if (i > m) break;
          value = R::add(value, R::mul(b.f(i), F(n_ / (ll(i) * j))));
        }
      }
      else
        for (int i = 1; i <= m; i++)
          value = R::add(value, R::add(R::mul(a.f(i), g.F(n_ / (ll(i) * j))),
                                     R::mul(b.f(i), F(n_ / (ll(i) * j)))));
      res.large_[j] = value;
    }
    return res;
  }
public:
  DirichletPrefixSum convolve(const DirichletPrefixSum &g) const { return convolve_impl(g, false); }
  DirichletPrefixSum convolve_sparse(const DirichletPrefixSum &g) const { return convolve_impl(g, true); }
  DirichletPrefixSum operator*(const DirichletPrefixSum &g) const { return convolve(g); }
  DirichletPrefixSum &operator*=(const DirichletPrefixSum &g) { return *this = *this * g; }

  DirichletPrefixSum operator/(const DirichletPrefixSum &g) const
  {
    check_shape(g);
    DirichletPrefixSum res(*this, same_shape{});
    res.multiplicative_ = multiplicative_ && g.multiplicative_;
    if (n_ == 0)
      return res;
    const internal::dirichlet_divisor<R> divide(g.small_[1]);
    const auto &coordinates = [&]() -> decltype(auto)
    {
      if constexpr (D == 1) return internal::dirichlet_prefix_sum_index(n_);
      else return (coordinates_);
    }();
    const int count = size();
    const auto sub = [](const S &a, const S &b) { return R::add(a, R::minus(b)); };
    // 1-indexed の商位置の累積和。位置 0 は F(0)。
    auto at = [&](const DirichletPrefixSum &a, int i) -> const S &
    { return i <= k_ ? a.small_[i] : a.large_[count + 1 - i]; };
    vc<S> delta(size_t(count) + 2, R::e0());
    for (int i = 1; i <= count; i++) delta[i] = sub(at(*this, i), at(*this, i - 1));
    res.small_[1] = divide(small_[1]);
    const S h1 = res.small_[1];
    // u,v>=2 の既知の寄与を、結果の商位置 [lo,hi] から差し引く。
    auto subtract = [&](int lo, int hi, const S &term)
    {
      delta[lo] = sub(delta[lo], term);
      delta[hi + 1] = R::add(delta[hi + 1], term);
    };
    for (int i = 2; i <= count; i++)
    {
      const ll x = value(i - 1);
      using Index = conditional_t<D == 1, int, ll>;
      const Index z = [&]() -> Index
      {
        if constexpr (D == 1) return count + 1 - i;
        else return n_ / ipow<ll>(x, D);
      }();
      S gathered = R::e0();
      // max(u,v)>z の寄与はこの商位置だけに加える。
      // 対称な二領域をまとめ、u=v は小さい側へ割り当てる。
      for (int u = 2; ; u++)
      {
        const Index lo = max<Index>(u, z);
        const ll hi = x / u;
        if (hi <= lo) break;
        gathered = R::add(gathered, R::add(
          R::mul(res.coefficient(u), sub(g.F(hi), g.small_[lo])),
          R::mul(g.coefficient(u), sub(res.F(hi), res.small_[lo]))));
      }
      subtract(i, i, gathered);
      const S block = divide(sub(delta[i], R::mul(h1, sub(at(g, i), at(g, i - 1)))));
      (i <= k_ ? res.small_[i] : res.large_[count + 1 - i]) = R::add(at(res, i - 1), block);
      if (i > k_) continue;
      // max(u,v)=i<=z の寄与は、uv<=Q<=⌊(N/i)^(1/D)⌋ へ区間加算。
      const S hi = res.coefficient(i), gi = g.coefficient(i);
      const ll bound = iroot(n_ / i, D);
      const int end = coordinates.index(bound) + 1;
      const int limit = int(min<ll>(i - 1, bound / i));
      for (int v = 2; v <= limit; v++)
      {
        const S term = R::add(R::mul(hi, g.coefficient(v)), R::mul(gi, res.coefficient(v)));
        subtract(coordinates.index(ll(i) * v) + 1, end, term);
      }
      // 対角 u=v は gather から除外しているので、全ての Q>=i^2 へ反映する。
      const int diagonal = coordinates.index(ll(i) * i) + 1;
      if constexpr (D == 1) subtract(diagonal, count, R::mul(hi, gi));
      else if (diagonal <= count) subtract(diagonal, count, R::mul(hi, gi));
    }
    return res;
  }
  DirichletPrefixSum &operator/=(const DirichletPrefixSum &g) { return *this = *this / g; }
  DirichletPrefixSum inv() const { return unit(n_) / *this; }
  DirichletPrefixSum pow(ll exponent) const
  {
    assert(exponent >= 0);
    DirichletPrefixSum res = unit(n_), base(*this);
    while (exponent > 0)
    {
      if (exponent & 1)
        res *= base;
      exponent >>= 1;
      if (exponent > 0)
        base *= base;
    }
    return res;
  }
};

// D=1 のとき、sum[ij <= n] a(i)b(j)、O(√n) 時間
// 一般には sum[ij <= ⌊n^(1/D)⌋] a(i) b(j)、O(n^(1/(2D))) 時間
template <class R, int D = 1>
typename R::S dirichlet_convolution_sum(const DirichletPrefixSum<R, D> &a, const DirichletPrefixSum<R, D> &b)
{
  assert(a.n() == b.n());
  const ll n = iroot(a.n(), D);
  const int m = int(iroot(n, 2));
  auto res = R::minus(R::mul(a.F(m), b.F(m)));
  for (int i = 1; i <= m; i++)
  {
    const auto ai = R::add(a.F(i), R::minus(a.F(i - 1)));
    const auto bi = R::add(b.F(i), R::minus(b.F(i - 1)));
    res = R::add(res, R::add(R::mul(ai, b.F(n / i)), R::mul(bi, a.F(n / i))));
  }
  return res;
}
