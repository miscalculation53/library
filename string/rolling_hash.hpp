#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../math/modint/modint.hpp"

/**
 * @brief Rolling Hash
 * @docs docs/string/rolling_hash.md
 */

template <class V, class mint, int id, int hash_count>
struct RollingHash;
template <class V, class mint, int id, int hash_count>
struct RollingHashConcat;
template <class V, class mint, int id, int hash_count>
struct RollingHashDeque;
template <class Sequence>
struct RollingHashRange;
template <class L, class R>
struct RollingHashConcatView;

template <class T>
inline constexpr bool is_rolling_hash_view = false;
template <class Sequence>
inline constexpr bool is_rolling_hash_view<RollingHashRange<Sequence>> = true;
template <class L, class R>
inline constexpr bool is_rolling_hash_view<RollingHashConcatView<L, R>> = true;

template <class T>
inline constexpr bool is_rolling_hash_sequence = is_rolling_hash_view<T>;
template <class V, class mint, int id, int hash_count>
inline constexpr bool is_rolling_hash_sequence<RollingHash<V, mint, id, hash_count>> = true;
template <class V, class mint, int id, int hash_count>
inline constexpr bool is_rolling_hash_sequence<RollingHashConcat<V, mint, id, hash_count>> = true;
template <class V, class mint, int id, int hash_count>
inline constexpr bool is_rolling_hash_sequence<RollingHashDeque<V, mint, id, hash_count>> = true;

template <class Sequence, class T, bool = is_rolling_hash_sequence<decay_t<Sequence>>, class = void>
struct is_rolling_hash_range_for : false_type {};
template <class Sequence, class T>
struct is_rolling_hash_range_for<Sequence, T, true,
                                 void_t<decltype(declval<const decay_t<Sequence> &>().size()),
                                        decltype(declval<const decay_t<Sequence> &>().get(0))>>
    : bool_constant<is_convertible_v<decltype(declval<const decay_t<Sequence> &>().get(0)), T>> {};
template <class Sequence, class T>
struct is_rolling_hash_range_for<Sequence, T, false,
                                 void_t<decltype(declval<const decay_t<Sequence> &>().size()),
                                        decltype(declval<const decay_t<Sequence> &>()[0])>>
    : bool_constant<is_convertible_v<decltype(declval<const decay_t<Sequence> &>()[0]), T>> {};

template <class mint = modint61, int id = INT_MIN, int hash_count = 1>
struct RollingHashBase
{
  static_assert(hash_count >= 1);
  using H = conditional_t<hash_count == 1, mint, array<mint, hash_count>>;

  static H zero()
  {
    if constexpr (hash_count == 1) return mint(0);
    else
    {
      H res;
      res.fill(mint(0));
      return res;
    }
  }
  static H one()
  {
    if constexpr (hash_count == 1) return mint(1);
    else
    {
      H res;
      res.fill(mint(1));
      return res;
    }
  }
  template <class T>
  static H make(const T &x)
  {
    if constexpr (hash_count == 1) return mint(x);
    else
    {
      H res;
      res.fill(mint(x));
      return res;
    }
  }
  static H add(H a, const H &b)
  {
    if constexpr (hash_count == 1) return a + b;
    else
    {
      rep(i, hash_count) a[i] += b[i];
      return a;
    }
  }
  static H sub(H a, const H &b)
  {
    if constexpr (hash_count == 1) return a - b;
    else
    {
      rep(i, hash_count) a[i] -= b[i];
      return a;
    }
  }
  static H mul(H a, const H &b)
  {
    if constexpr (hash_count == 1) return a * b;
    else
    {
      rep(i, hash_count) a[i] *= b[i];
      return a;
    }
  }
  static H inv(H a)
  {
    if constexpr (hash_count == 1) return a.inv();
    else
    {
      for (auto &x : a) x = x.inv();
      return a;
    }
  }

  static H &base()
  {
    static H val = []
    {
      if constexpr (hash_count == 1)
      {
        mint x = local_oj(1000, mt());
        return x == mint(0) ? mint(1) : x;
      }
      else
      {
        H res;
        rep(i, hash_count)
        {
          mint x = local_oj(1000 + i, mt());
          res[i] = x == mint(0) ? mint(1) : x;
        }
        return res;
      }
    }();
    return val;
  }
  static H inv_base()
  {
    static H val = inv(base());
    return val;
  }

  template <bool inverse>
  static H pow_impl(int n)
  {
    assert(n >= 0);
    static auto mod = mint::mod();
    static vc<H> pw = {one()};
    if (mod != mint::mod())
    {
      mod = mint::mod();
      pw = {one()};
    }
    const H b = inverse ? inv_base() : base();
    int i = pw.size();
    if (n >= i)
    {
      pw.resize(n + 1);
      for (; i <= n; i++) pw[i] = mul(pw[i - 1], b);
    }
    return pw[n];
  }
  static H pow(int n) { return pow_impl<false>(n); }
  static H inv_pow(int n) { return pow_impl<true>(n); }
};

template <class mint = modint61, int id = INT_MIN, int hash_count = 1>
struct RollingHashValue
{
private:
  using Base = RollingHashBase<mint, id, hash_count>;
  using H = typename Base::H;

  H val, pw, ipw;
  ll len;

  RollingHashValue(H val, H pw, H ipw, ll len) : val(val), pw(pw), ipw(ipw), len(len) {}

  template <class V, class M, int i, int n>
  friend struct RollingHash;
  template <class V, class M, int i, int n>
  friend struct RollingHashDeque;

public:
  using hash_type = H;
  template <class V>
  using rolling_hash_type = RollingHash<V, mint, id, hash_count>;
  template <class V>
  using rolling_hash_concat_type = RollingHashConcat<V, mint, id, hash_count>;

  RollingHashValue() : val(Base::zero()), pw(Base::one()), ipw(Base::one()), len(0) {}
  RollingHashValue(char c) : RollingHashValue(Base::make(c), base(), Base::inv_base(), 1) {}
  template <class V>
  RollingHashValue(const V &s) : RollingHashValue()
  {
    const H b = base(), ib = Base::inv_base();
    for (const auto &x : s)
    {
      val = Base::add(Base::mul(val, b), Base::make(x));
      pw = Base::mul(pw, b);
      ipw = Base::mul(ipw, ib);
      len++;
    }
  }
  template <size_t n>
  RollingHashValue(const char (&s)[n]) : RollingHashValue(string_view(s, n - 1)) {}

  static H base() { return Base::base(); }
  static H power(int n) { return Base::pow(n); }
  static H concat_hash(H lhs, const H &rhs, int rhs_len)
  {
    return Base::add(Base::mul(lhs, power(rhs_len)), rhs);
  }
  H value() const { return val; }
  ll size() const { return len; }

  RollingHashValue &operator+=(const RollingHashValue &rhs)
  {
    val = Base::add(Base::mul(val, rhs.pw), rhs.val);
    pw = Base::mul(pw, rhs.pw);
    ipw = Base::mul(ipw, rhs.ipw);
    len += rhs.len;
    return *this;
  }
  friend RollingHashValue operator+(RollingHashValue lhs, const RollingHashValue &rhs) { return lhs += rhs; }

  RollingHashValue remove_prefix(const RollingHashValue &prefix) const
  {
    assert(prefix.len <= len);
    H suffix_pw = Base::mul(pw, prefix.ipw);
    return {Base::sub(val, Base::mul(prefix.val, suffix_pw)), suffix_pw,
            Base::mul(ipw, prefix.pw), len - prefix.len};
  }

  friend bool operator==(const RollingHashValue &lhs, const RollingHashValue &rhs)
  {
    return lhs.len == rhs.len && lhs.val == rhs.val;
  }
  friend bool operator!=(const RollingHashValue &lhs, const RollingHashValue &rhs) { return !(lhs == rhs); }
  friend auto safe_hash_key(const RollingHashValue &x) { return pair{x.val, x.len}; }

  CPP_DUMP_DEFINE_DATA(val, len);
};

template <class V>
RollingHashValue(const V &) -> RollingHashValue<>;

template <class Derived>
struct RollingHashSequenceOps
{
private:
  const Derived &self() const { return static_cast<const Derived &>(*this); }

public:
  auto content() const
  {
    using T = remove_cv_t<typename Derived::element_type>;
    using V = conditional_t<is_same_v<T, char>, string, vc<T>>;
    V res;
    res.reserve(self().size());
    for (ll i = 0; i < self().size(); i++) res.push_back(self().get(i));
    return res;
  }

  RollingHashRange<Derived> substr(ll l, ll r) const &;
  RollingHashRange<Derived> substr(ll l = 0) const &;
  RollingHashRange<Derived> substr(ll, ll) const && = delete;
  RollingHashRange<Derived> substr(ll = 0) const && = delete;

  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  ll lcp(const Other &rhs) const
  {
    static_assert(is_same_v<decltype(self().hash()), decltype(rhs.hash())>);
    ll ok = 0, ng = min(self().size(), rhs.size()) + 1;
    while (ng - ok > 1)
    {
      ll mid = (ok + ng) / 2;
      (self().hash_value(0, mid) == rhs.hash_value(0, mid) ? ok : ng) = mid;
    }
    return ok;
  }
  ll lcp(string_view rhs) const
  {
    using Value = decltype(self().hash());
    using RH = typename Value::template rolling_hash_type<string_view>;
    return lcp(RH(rhs));
  }

  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  ll lcs(const Other &rhs) const
  {
    static_assert(is_same_v<decltype(self().hash()), decltype(rhs.hash())>);
    ll ok = 0, ng = min(self().size(), rhs.size()) + 1;
    while (ng - ok > 1)
    {
      ll mid = (ok + ng) / 2;
      (self().hash_value(self().size() - mid, self().size()) == rhs.hash_value(rhs.size() - mid, rhs.size()) ? ok : ng) = mid;
    }
    return ok;
  }
  ll lcs(string_view rhs) const
  {
    using Value = decltype(self().hash());
    using RH = typename Value::template rolling_hash_type<string_view>;
    return lcs(RH(rhs));
  }

  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  int compare(const Other &rhs) const
  {
    if (self().size() && rhs.size())
    {
      if (self().get(0) < rhs.get(0)) return -1;
      if (rhs.get(0) < self().get(0)) return 1;
    }
    ll k = lcp(rhs);
    if (k == min(self().size(), rhs.size()))
      return self().size() < rhs.size() ? -1 : self().size() > rhs.size() ? 1 : 0;
    if (self().get(k) < rhs.get(k)) return -1;
    if (rhs.get(k) < self().get(k)) return 1;
    return 0;
  }
  int compare(string_view rhs) const
  {
    using Value = decltype(self().hash());
    using RH = typename Value::template rolling_hash_type<string_view>;
    return compare(RH(rhs));
  }

  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  bool operator==(const Other &rhs) const
  {
    static_assert(is_same_v<decltype(self().hash()), decltype(rhs.hash())>);
    return self().size() == rhs.size() && self().hash_value(0, self().size()) == rhs.hash_value(0, rhs.size());
  }
  bool operator==(string_view rhs) const
  {
    using Value = decltype(self().hash());
    using RH = typename Value::template rolling_hash_type<string_view>;
    return *this == RH(rhs);
  }
  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  bool operator!=(const Other &rhs) const { return !(*this == rhs); }
  bool operator!=(string_view rhs) const { return !(*this == rhs); }
  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  bool operator<(const Other &rhs) const { return compare(rhs) < 0; }
  bool operator<(string_view rhs) const { return compare(rhs) < 0; }
  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  bool operator<=(const Other &rhs) const { return compare(rhs) <= 0; }
  bool operator<=(string_view rhs) const { return compare(rhs) <= 0; }
  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  bool operator>(const Other &rhs) const { return compare(rhs) > 0; }
  bool operator>(string_view rhs) const { return compare(rhs) > 0; }
  template <class Other, enable_if_t<is_rolling_hash_sequence<decay_t<Other>>, int> = 0>
  bool operator>=(const Other &rhs) const { return compare(rhs) >= 0; }
  bool operator>=(string_view rhs) const { return compare(rhs) >= 0; }
};

template <class Sequence, enable_if_t<is_rolling_hash_sequence<decay_t<Sequence>>, int> = 0>
bool operator==(string_view lhs, const Sequence &rhs) { return rhs == lhs; }
template <class Sequence, enable_if_t<is_rolling_hash_sequence<decay_t<Sequence>>, int> = 0>
bool operator!=(string_view lhs, const Sequence &rhs) { return rhs != lhs; }
template <class Sequence, enable_if_t<is_rolling_hash_sequence<decay_t<Sequence>>, int> = 0>
bool operator<(string_view lhs, const Sequence &rhs) { return rhs > lhs; }
template <class Sequence, enable_if_t<is_rolling_hash_sequence<decay_t<Sequence>>, int> = 0>
bool operator<=(string_view lhs, const Sequence &rhs) { return rhs >= lhs; }
template <class Sequence, enable_if_t<is_rolling_hash_sequence<decay_t<Sequence>>, int> = 0>
bool operator>(string_view lhs, const Sequence &rhs) { return rhs < lhs; }
template <class Sequence, enable_if_t<is_rolling_hash_sequence<decay_t<Sequence>>, int> = 0>
bool operator>=(string_view lhs, const Sequence &rhs) { return rhs <= lhs; }

template <class Sequence>
struct RollingHashRange : RollingHashSequenceOps<RollingHashRange<Sequence>>
{
private:
  const Sequence *seq;
  ll l, r;

  template <class V, class mint, int id, int hash_count>
  friend struct RollingHashConcat;

public:
  using element_type = typename Sequence::element_type;

  RollingHashRange(const Sequence &seq, ll l, ll r) : seq(&seq), l(l), r(r)
  {
    assert(0 <= l && l <= r && r <= seq.size());
  }

  ll size() const { return r - l; }
  auto hash() const { return seq->hash(l, r); }
  auto hash(ll a, ll b) const
  {
    assert(0 <= a && a <= b && b <= size());
    return seq->hash(l + a, l + b);
  }
  auto hash_value(ll a, ll b) const
  {
    assert(0 <= a && a <= b && b <= size());
    return seq->hash_value(l + a, l + b);
  }
  decltype(auto) get(ll i) const
  {
    assert(0 <= i && i < size());
    return seq->get(l + i);
  }
};

template <class Derived>
RollingHashRange<Derived> RollingHashSequenceOps<Derived>::substr(ll l, ll r) const &
{
  return {self(), l, r};
}

template <class Derived>
RollingHashRange<Derived> RollingHashSequenceOps<Derived>::substr(ll l) const &
{
  return {self(), l, self().size()};
}

template <class L, class R>
struct RollingHashConcatView : RollingHashSequenceOps<RollingHashConcatView<L, R>>
{
private:
  L lhs;
  R rhs;

  template <class V, class mint, int id, int hash_count>
  friend struct RollingHashConcat;

public:
  using element_type = common_type_t<typename L::element_type, typename R::element_type>;

  RollingHashConcatView(L lhs, R rhs) : lhs(lhs), rhs(rhs) {}

  ll size() const { return lhs.size() + rhs.size(); }
  auto hash() const { return hash(0, size()); }
  auto hash(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    if (r <= lhs.size()) return lhs.hash(l, r);
    if (lhs.size() <= l) return rhs.hash(l - lhs.size(), r - lhs.size());
    return lhs.hash(l, lhs.size()) + rhs.hash(0, r - lhs.size());
  }
  auto hash_value(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    if (r <= lhs.size()) return lhs.hash_value(l, r);
    if (lhs.size() <= l) return rhs.hash_value(l - lhs.size(), r - lhs.size());
    using Value = decltype(lhs.hash(l, lhs.size()));
    static_assert(is_same_v<Value, decltype(rhs.hash(0, r - lhs.size()))>);
    ll nr = r - lhs.size();
    return Value::concat_hash(lhs.hash_value(l, lhs.size()), rhs.hash_value(0, nr), nr);
  }
  decltype(auto) get(ll i) const
  {
    assert(0 <= i && i < size());
    return i < lhs.size() ? lhs.get(i) : rhs.get(i - lhs.size());
  }
};

template <class L, class R,
          enable_if_t<is_rolling_hash_sequence<decay_t<L>> && is_rolling_hash_sequence<decay_t<R>> &&
                          (is_rolling_hash_view<decay_t<L>> || is_lvalue_reference_v<L &&>) &&
                          (is_rolling_hash_view<decay_t<R>> || is_lvalue_reference_v<R &&>),
                      int> = 0>
auto operator+(L &&lhs, R &&rhs)
{
  auto as_view = [](const auto &x)
  {
    using T = decay_t<decltype(x)>;
    if constexpr (is_rolling_hash_view<T>) return x;
    else return x.substr();
  };
  auto l = as_view(lhs);
  auto r = as_view(rhs);
  return RollingHashConcatView<decltype(l), decltype(r)>(l, r);
}

template <class R,
          enable_if_t<is_rolling_hash_sequence<decay_t<R>> &&
                          (is_rolling_hash_view<decay_t<R>> || is_lvalue_reference_v<R &&>),
                      int> = 0>
auto operator+(string_view lhs, R &&rhs)
{
  using Value = decltype(rhs.hash());
  using Owned = typename Value::template rolling_hash_concat_type<string>;
  Owned l{string(lhs)};
  auto r = [&]
  {
    if constexpr (is_rolling_hash_view<decay_t<R>>) return rhs;
    else return rhs.substr();
  }();
  return RollingHashConcatView<Owned, decltype(r)>(std::move(l), r);
}

template <class L,
          enable_if_t<is_rolling_hash_sequence<decay_t<L>> &&
                          (is_rolling_hash_view<decay_t<L>> || is_lvalue_reference_v<L &&>),
                      int> = 0>
auto operator+(L &&lhs, string_view rhs)
{
  using Value = decltype(lhs.hash());
  using Owned = typename Value::template rolling_hash_concat_type<string>;
  auto l = [&]
  {
    if constexpr (is_rolling_hash_view<decay_t<L>>) return lhs;
    else return lhs.substr();
  }();
  Owned r{string(rhs)};
  return RollingHashConcatView<decltype(l), Owned>(l, std::move(r));
}

template <class V = string, class mint = modint61, int id = INT_MIN, int hash_count = 1>
struct RollingHash : RollingHashSequenceOps<RollingHash<V, mint, id, hash_count>>
{
private:
  using Base = RollingHashBase<mint, id, hash_count>;
  using H = typename Base::H;

  shared_ptr<V> owned;
  const V *s;
  vc<H> sm;

  void build()
  {
    sm.resize(s->size() + 1);
    sm[0] = Base::zero();
    for (ll i = 0; i < (ll)s->size(); i++)
      sm[i + 1] = Base::add(Base::mul(sm[i], base()), Base::make((*s)[i]));
    Base::pow(s->size());
    Base::inv_pow(s->size());
  }

public:
  using element_type = typename V::value_type;

  RollingHash(const V &s) : s(&s)
  {
    build();
  }
  RollingHash(V &&s) : owned(make_shared<V>(std::move(s))), s(owned.get())
  {
    build();
  }
  template <size_t n, class W = V, enable_if_t<is_same_v<W, string>, int> = 0>
  RollingHash(const char (&s)[n]) : RollingHash(V(s, n - 1)) {}

  static H base() { return Base::base(); }
  ll size() const { return s->size(); }
  RollingHashValue<mint, id, hash_count> hash() const { return hash(0, size()); }
  RollingHashValue<mint, id, hash_count> hash(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    int len = r - l;
    H pw = Base::pow(len);
    return {hash_value(l, r), pw, Base::inv_pow(len), len};
  }
  H hash_value(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    return Base::sub(sm[r], Base::mul(sm[l], Base::pow(r - l)));
  }
  decltype(auto) get(ll i) const
  {
    assert(0 <= i && i < size());
    return (*s)[i];
  }
};

template <class V>
RollingHash(const V &) -> RollingHash<V>;
template <size_t n>
RollingHash(const char (&)[n]) -> RollingHash<string>;

template <class V = string, class mint = modint61, int id = INT_MIN, int hash_count = 1>
struct RollingHashDeque : RollingHashSequenceOps<RollingHashDeque<V, mint, id, hash_count>>
{
private:
  using Base = RollingHashBase<mint, id, hash_count>;
  using H = typename Base::H;
  using Value = RollingHashValue<mint, id, hash_count>;
  using T = typename V::value_type;

  vc<T> lf, rg;
  vc<H> lh = {Base::zero()}, rh = {Base::zero()};

  static Value value(H x, int len)
  {
    return {x, Base::pow(len), Base::inv_pow(len), len};
  }

  template <class Sequence>
  static decltype(auto) elem(const Sequence &s, ll i)
  {
    if constexpr (is_rolling_hash_sequence<decay_t<Sequence>>) return s.get(i);
    else return s[i];
  }

  void rebuild_hash()
  {
    lh.assign(1, Base::zero());
    for (int i = 0; i < (int)lf.size(); i++)
      lh.push_back(Base::add(lh.back(), Base::mul(Base::make(lf[i]), Base::pow(i))));
    rh.assign(1, Base::zero());
    H b = Base::base();
    for (const T &x : rg) rh.push_back(Base::add(Base::mul(rh.back(), b), Base::make(x)));
  }

  void rebalance(int nlf)
  {
    vc<T> a(size());
    for (ll i = 0; i < size(); i++) a[i] = get(i);
    lf.clear(), rg.clear();
    for (int i = nlf - 1; i >= 0; i--) lf.push_back(std::move(a[i]));
    for (int i = nlf; i < (int)a.size(); i++) rg.push_back(std::move(a[i]));
    rebuild_hash();
  }

public:
  using element_type = T;

  RollingHashDeque() = default;
  RollingHashDeque(const V &s) { push_back(s); }
  RollingHashDeque(V &&s) { push_back(s); }
  template <size_t n, class W = V, enable_if_t<is_same_v<W, string>, int> = 0>
  RollingHashDeque(const char (&s)[n]) { push_back(string_view(s, n - 1)); }

  ll size() const { return lf.size() + rg.size(); }
  bool empty() const { return lf.empty() && rg.empty(); }

  void reserve(int n)
  {
    assert(n >= size());
    lf.reserve(n), rg.reserve(n);
    lh.reserve(n + 1), rh.reserve(n + 1);
    Base::pow(n);
    Base::inv_pow(n);
  }

  void push_front(const T &x)
  {
    lh.push_back(Base::add(lh.back(), Base::mul(Base::make(x), Base::pow(lf.size()))));
    lf.push_back(x);
  }
  void push_back(const T &x)
  {
    rg.push_back(x);
    rh.push_back(Base::add(Base::mul(rh.back(), Base::base()), Base::make(x)));
  }
  void pop_front()
  {
    assert(!empty());
    if (lf.empty()) rebalance((size() + 1) / 2);
    lf.pop_back(), lh.pop_back();
  }
  void pop_back()
  {
    assert(!empty());
    if (rg.empty()) rebalance(size() / 2);
    rg.pop_back(), rh.pop_back();
  }
  void clear()
  {
    lf.clear(), rg.clear();
    lh.assign(1, Base::zero()), rh.assign(1, Base::zero());
  }

  template <class Sequence, enable_if_t<is_rolling_hash_range_for<Sequence, T>::value, int> = 0>
  void push_front(const Sequence &s)
  {
    if constexpr (is_same_v<decay_t<Sequence>, RollingHashDeque>)
    {
      if (&s == this)
      {
        V a = this->content();
        push_front(a);
        return;
      }
    }
    for (ll i = s.size(); i--;) push_front(elem(s, i));
  }
  template <size_t n, class W = V, enable_if_t<is_same_v<W, string>, int> = 0>
  void push_front(const char (&s)[n]) { push_front(string_view(s, n - 1)); }
  template <class Sequence, enable_if_t<is_rolling_hash_range_for<Sequence, T>::value, int> = 0>
  void push_back(const Sequence &s)
  {
    if constexpr (is_same_v<decay_t<Sequence>, RollingHashDeque>)
    {
      if (&s == this)
      {
        V a = this->content();
        push_back(a);
        return;
      }
    }
    for (ll i = 0; i < (ll)s.size(); i++) push_back(elem(s, i));
  }
  template <size_t n, class W = V, enable_if_t<is_same_v<W, string>, int> = 0>
  void push_back(const char (&s)[n]) { push_back(string_view(s, n - 1)); }
  RollingHashDeque &operator+=(const T &x)
  {
    push_back(x);
    return *this;
  }
  template <class Sequence, enable_if_t<is_rolling_hash_range_for<Sequence, T>::value, int> = 0>
  RollingHashDeque &operator+=(const Sequence &s)
  {
    push_back(s);
    return *this;
  }
  template <size_t n, class W = V, enable_if_t<is_same_v<W, string>, int> = 0>
  RollingHashDeque &operator+=(const char (&s)[n])
  {
    push_back(s);
    return *this;
  }

  Value hash() const
  {
    int len = size();
    return value(Base::add(Base::mul(lh.back(), Base::pow(rg.size())), rh.back()), len);
  }
  Value hash(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    return value(hash_value(l, r), r - l);
  }
  H hash_value(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    int m = lf.size(), len = r - l;
    if (r <= m)
      return Base::mul(Base::sub(lh[m - l], lh[m - r]), Base::inv_pow(m - r));
    if (m <= l)
      return Base::sub(rh[r - m], Base::mul(rh[l - m], Base::pow(len)));
    int nr = r - m;
    return Base::add(Base::mul(lh[m - l], Base::pow(nr)), rh[nr]);
  }
  const T &get(ll i) const
  {
    assert(0 <= i && i < size());
    return i < (ll)lf.size() ? lf[lf.size() - 1 - i] : rg[i - lf.size()];
  }
};

template <size_t n>
RollingHashDeque(const char (&)[n]) -> RollingHashDeque<string>;

template <class V = string, class mint = modint61, int id = INT_MIN, int hash_count = 1>
struct RollingHashConcat : RollingHashSequenceOps<RollingHashConcat<V, mint, id, hash_count>>
{
private:
  using RH = RollingHash<V, mint, id, hash_count>;
  using Value = RollingHashValue<mint, id, hash_count>;
  struct Block
  {
    const RH *src;
    int l, r;
    Value pref;
  };

  vc<Block> blocks;
  vc<shared_ptr<RH>> owned;

  void append_range(const RH &src, ll l, ll r) { append(src, l, r); }

  template <class Sequence>
  void append_range(const RollingHashRange<Sequence> &src, ll l, ll r)
  {
    append_range(*src.seq, src.l + l, src.l + r);
  }

  template <class L, class R>
  void append_range(const RollingHashConcatView<L, R> &src, ll l, ll r)
  {
    ll m = src.lhs.size();
    if (l < m) append_range(src.lhs, l, min(r, m));
    if (m < r) append_range(src.rhs, max(l, m) - m, r - m);
  }

  void append_range(const RollingHashConcat &src, ll l, ll r)
  {
    owned.insert(owned.end(), src.owned.begin(), src.owned.end());
    for (int i = 0; i < (int)src.blocks.size(); i++)
    {
      ll prv = i == 0 ? 0 : src.blocks[i - 1].pref.size();
      ll nxt = src.blocks[i].pref.size();
      if (r <= prv) break;
      if (nxt <= l) continue;
      const Block &b = src.blocks[i];
      append(*b.src, b.l + max(l, prv) - prv, b.l + min(r, nxt) - prv);
    }
  }

  void append(const RH &src, ll l, ll r)
  {
    if (l == r) return;
    Value add = src.hash(l, r);
    if (!blocks.empty() && blocks.back().src == &src && blocks.back().r == l)
    {
      blocks.back().r = int(r);
      blocks.back().pref += add;
      return;
    }
    Value pref = blocks.empty() ? add : blocks.back().pref + add;
    blocks.push_back({&src, int(l), int(r), pref});
  }

  Value prefix_hash(ll r) const
  {
    assert(0 <= r && r <= size());
    if (r == 0) return {};
    auto it = lower_bound(blocks.begin(), blocks.end(), r, [](const Block &block, ll x)
    {
      return block.pref.size() < x;
    });
    ll before = it == blocks.begin() ? 0 : prev(it)->pref.size();
    Value res = it == blocks.begin() ? Value() : prev(it)->pref;
    return res + it->src->hash(it->l, it->l + r - before);
  }

public:
  using element_type = typename V::value_type;

  RollingHashConcat() = default;
  RollingHashConcat(const V &s)
  {
    owned.push_back(make_shared<RH>(V(s)));
    *this += *owned.back();
  }
  template <size_t n, class W = V, enable_if_t<is_same_v<W, string>, int> = 0>
  RollingHashConcat(const char (&s)[n]) : RollingHashConcat(V(s, n - 1)) {}
  RollingHashConcat(const RH &source) { *this += source; }
  RollingHashConcat(const RollingHashRange<RH> &range) { *this += range; }
  template <class L, class R>
  RollingHashConcat(const RollingHashConcatView<L, R> &view) { append_range(view, 0, view.size()); }

  template <class L, class R>
  RollingHashConcat &operator=(const RollingHashConcatView<L, R> &rhs)
  {
    RollingHashConcat res(rhs);
    res.blocks.reserve(blocks.capacity());
    blocks = std::move(res.blocks);
    owned = std::move(res.owned);
    return *this;
  }

  void reserve(int n) { blocks.reserve(n); }
  ll size() const { return blocks.empty() ? 0 : blocks.back().pref.size(); }

  RollingHashConcat &operator+=(const RH &rhs)
  {
    append(rhs, 0, rhs.size());
    return *this;
  }
  RollingHashConcat &operator+=(const RollingHashRange<RH> &rhs)
  {
    append(*rhs.seq, rhs.l, rhs.r);
    return *this;
  }
  RollingHashConcat &operator+=(const V &rhs)
  {
    owned.push_back(make_shared<RH>(V(rhs)));
    append(*owned.back(), 0, owned.back()->size());
    return *this;
  }
  template <size_t n, class W = V, enable_if_t<is_same_v<W, string>, int> = 0>
  RollingHashConcat &operator+=(const char (&rhs)[n]) { return *this += V(rhs, n - 1); }

  Value hash() const { return blocks.empty() ? Value() : blocks.back().pref; }
  Value hash(ll l, ll r) const
  {
    assert(0 <= l && l <= r && r <= size());
    return prefix_hash(r).remove_prefix(prefix_hash(l));
  }
  typename Value::hash_type hash_value(ll l, ll r) const { return hash(l, r).value(); }
  decltype(auto) get(ll i) const
  {
    assert(0 <= i && i < size());
    auto it = lower_bound(blocks.begin(), blocks.end(), i + 1, [](const Block &block, ll x)
    {
      return block.pref.size() < x;
    });
    ll before = it == blocks.begin() ? 0 : prev(it)->pref.size();
    return it->src->get(it->l + i - before);
  }
};

template <size_t n>
RollingHashConcat(const char (&)[n]) -> RollingHashConcat<string>;

template <class mint = modint61, int id = INT_MIN>
using RollingHashValue2 = RollingHashValue<mint, id, 2>;
template <class V = string, class mint = modint61, int id = INT_MIN>
using RollingHash2 = RollingHash<V, mint, id, 2>;
template <class V = string, class mint = modint61, int id = INT_MIN>
using RollingHashDeque2 = RollingHashDeque<V, mint, id, 2>;
template <class V = string, class mint = modint61, int id = INT_MIN>
using RollingHashConcat2 = RollingHashConcat<V, mint, id, 2>;
