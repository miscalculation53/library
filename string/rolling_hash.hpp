#pragma once

#include "../template/template_all_but_modint.hpp"

#include "../math/modint/modint.hpp"
#include "../math/modint/power_table.hpp"

#include "lcp_compare.hpp"

/**
 * @brief Rolling Hash
 * @docs docs/string/rolling_hash.md
 */

template <class mint = modint61, int id = INT_MIN>
struct RollingHashBase
{
  static mint &base()
  {
    static mint val = local_oj(1000, mt());
    return val;
  }
  static mint pow(int n)
  {
    static PowerTable<mint> pw = PowerTable<mint>(base());
    return pw.pow(n);
  }
};

template <class mint = modint61, int id = INT_MIN>
struct RollingHash
{
private:
  mint val;
  int len;
  static mint pow(int i) { return RollingHashBase<mint, id>::pow(i); }

public:
  static mint base() { return RollingHashBase<mint, id>::base(); }

  RollingHash() : val(0), len(0) {}
  RollingHash(const mint &val_, int len_) : val(val_), len(len_) {}
  template <class V>
  RollingHash(const V &s) : len(s.size())
  {
    val = 0;
    fec(c : s) val = val * base() + c;
  }

  mint hash() const { return val; }
  template <class I = ll>
  I size() const { return len; }

  using RH = RollingHash<mint, id>;
  RH &operator+=(const RH &rhs)
  {
    val = val * pow(rhs.len) + rhs.val;
    len += rhs.len;
    return *this;
  }
  friend RH operator+(const RH &lhs, const RH &rhs) { return RH(lhs) += rhs; }
  friend bool operator==(const RH &lhs, const RH &rhs) { return lhs.len == rhs.len && lhs.val == rhs.val; }
  friend bool operator!=(const RH &lhs, const RH &rhs) { return lhs.len != rhs.len || lhs.val != rhs.val; }
  friend auto safe_hash_key(const RH &x) { return pair{x.val, x.len}; }
};

template <class mint = modint61, int id = INT_MIN>
struct RollingHashSubstring
{
private:
  vc<mint> sm;
  static mint pow(int i) { return RollingHashBase<mint, id>::pow(i); }

public:
  static mint base() { return RollingHashBase<mint, id>::base(); }

  RollingHashSubstring() : sm(1) {}
  template <class V>
  RollingHashSubstring(const V &s)
  {
    const int n = s.size();
    sm.resize(n + 1);
    repi(i, n) sm[i + 1] = sm[i] * base() + s[i];
  }

  template <class I = ll>
  I size() const { return sm.size() - 1; }
  RollingHash<mint, id> hash(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= size());
    mint val = sm[r] - sm[l] * pow(r - l);
    return {val, r - l};
  }

  // 列を追加する
  template <class V>
  void push_back(const V &s) { fec(c : s) sm.eb(sm.back() * base() + c); }

  void pop_back()
  {
    assert(size() > 0);
    sm.pop_back();
  }
};

// s[i, ...) と t[j, ...) の LCP の長さ
template <class I = ll, class mint, int id>
I calc_lcp_rh(const RollingHashSubstring<mint, id> &rh_s, int i, const RollingHashSubstring<mint, id> &rh_t, int j)
{
  assert(0 <= i && i <= rh_s.size());
  assert(0 <= j && j <= rh_t.size());
  auto judge = [&](int l1, int r1, int l2, int r2)
  {
    if (r1 > rh_s.size() || r2 > rh_t.size())
      return false;
    return rh_s.hash(l1, r1) == rh_t.hash(l2, r2);
  };
  return calc_lcp(i, j, judge);
}

// s[l1, r1) と t[l2, r2) の辞書順比較
// < なら -1, == なら 0, > なら 1
template <class V, class mint, int id>
int compare_substr_rh(
  const V &s, const RollingHashSubstring<mint, id> &rh_s, int l1, int r1,
  const V &t, const RollingHashSubstring<mint, id> &rh_t, int l2, int r2
)
{
  assert(0 <= l1 && l1 <= r1 && r1 <= SZ(s));
  assert(0 <= l2 && l2 <= r2 && r2 <= SZ(t));
  auto [i, j] = compare_substr(l1, r1, l2, r2, calc_lcp_rh(rh_s, l1, rh_t, l2));
  if (i >= SZ(s) && j >= SZ(t))
    return 0;
  else if (i >= SZ(s))
    return -1;
  else if (j >= SZ(t))
    return 1;
  else
    return s[i] < t[j] ? -1 : s[i] > t[j] ? 1 : 0;
}

#ifdef LOCAL
CPP_DUMP_DEFINE_EXPORT_OBJECT_GENERIC(size(), hash());
#endif
