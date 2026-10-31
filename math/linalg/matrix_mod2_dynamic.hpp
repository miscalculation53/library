#pragma once

#include "internal_matrix_mod2.hpp"
#include "internal_matrix_mod2_row.hpp"
#include "internal_matrix_storage.hpp"

/**
 * @brief 実行時サイズの行列（$\mathbb{F}_2$）
 * @docs docs/math/linalg/matrix_mod2_dynamic.md
 */
struct MatrixMod2Dynamic
{
  using M = MatrixMod2Dynamic;
  using V = DynamicBitset;
  using Word = V::Word;

private:
  int n_ = 0;
  vc<Word> values_;
  int width() const { return m / 64 + (m % 64 != 0); }

public:
  int m = 0;
  MatrixMod2Dynamic() = default;
  MatrixMod2Dynamic(int n, int m, int diag = 0, int non_diag = 0)
      : n_(n), values_(size_t(n) * (m / 64 + (m % 64 != 0)), non_diag ? ~Word(0) : 0), m(m)
  {
    assert(n >= 0 && m >= 0 && (diag == 0 || diag == 1) && (non_diag == 0 || non_diag == 1));
    if (m % 64)
      for (int i = 0; i < n_; ++i)
        values_[size_t(i + 1) * width() - 1] &= (Word(1) << (m % 64)) - 1;
    if (diag != non_diag)
      for (int i = 0; i < min(n, m); ++i)
        values_[size_t(i) * width() + i / 64] ^= Word(1) << (i % 64);
  }
  template <class T>
  MatrixMod2Dynamic(const vvc<T> &a) : MatrixMod2Dynamic(a.size(), a.empty() ? 0 : a[0].size())
  {
    repi(i, n_)
    {
      assert(SZ(a[i]) == m);
      repi(j, m)
      {
        assert(a[i][j] == T(0) || a[i][j] == T(1));
        set(i, j, a[i][j] == T(1));
      }
    }
  }
  MatrixMod2Dynamic(const vc<string> &a) : MatrixMod2Dynamic(a.size(), a.empty() ? 0 : a[0].size())
  {
    repi(i, n_)
    {
      assert(SZ(a[i]) == m);
      repi(j, m)
      {
        assert(a[i][j] == '0' || a[i][j] == '1');
        set(i, j, a[i][j] == '1');
      }
    }
  }
  MatrixMod2Dynamic(const M &) = default;
  M &operator=(const M &) = default;
  MatrixMod2Dynamic(M &&b) noexcept
      : n_(exchange(b.n_, 0)), values_(move(b.values_)), m(exchange(b.m, 0))
  {
  }
  M &operator=(M &&b) noexcept
  {
    if (this != &b)
    {
      n_ = exchange(b.n_, 0);
      m = exchange(b.m, 0);
      values_ = move(b.values_);
    }
    return *this;
  }
  size_t size() const { return n_; }
  bool empty() const { return n_ == 0; }
  template <class I = ll> pair<I, I> shape() const { return {I(n_), I(m)}; }
  auto row(int i)
  {
    return internal::matrix_mod2_row<false>{
        values_.empty() ? values_.data() : values_.data() + size_t(i) * width(), m};
  }
  auto row(int i) const
  {
    return internal::matrix_mod2_row<true>{
        values_.empty() ? values_.data() : values_.data() + size_t(i) * width(), m};
  }
  auto operator[](int i) { return row(i); }
  auto operator[](int i) const { return row(i); }
  auto operator()(int i, int j) { return row(i)[j]; }
  bool operator()(int i, int j) const { return row(i).test(j); }
  auto at(int i, int j)
  {
    assert(0 <= i && i < n_);
    return row(i)[j];
  }
  bool at(int i, int j) const
  {
    assert(0 <= i && i < n_);
    return row(i).test(j);
  }
  auto begin() { return internal::matrix_row_iterator<M>{this, 0}; }
  auto end() { return internal::matrix_row_iterator<M>{this, n_}; }
  auto begin() const { return internal::matrix_row_iterator<const M>{this, 0}; }
  auto end() const { return internal::matrix_row_iterator<const M>{this, n_}; }
  bool test(int i, int j) const { return at(i, j); }
  M &set(int i, int j, bool value = true)
  {
    at(i, j) = value;
    return *this;
  }
  M &flip(int i, int j)
  {
    at(i, j).flip();
    return *this;
  }
  void swap_rows(int i, int j)
  {
    if (i != j)
      for (int w = 0; w < width(); ++w)
        swap(values_[size_t(i) * width() + w], values_[size_t(j) * width() + w]);
  }
  template <class T = ll> vvc<T> to_vvi() const
  {
    vvc<T> a(n_, vc<T>(m));
    repi(i, n_) repi(j, m) a[i][j] = test(i, j);
    return a;
  }
  vc<string> to_vstr() const
  {
    vc<string> a(n_, string(m, '0'));
    repi(i, n_) repi(j, m) if (test(i, j)) a[i][j] = '1';
    return a;
  }
  friend bool operator==(const M &a, const M &b)
  {
    return a.shape() == b.shape() && a.values_ == b.values_;
  }
  friend bool operator!=(const M &a, const M &b) { return !(a == b); }
  M operator-() const { return *this; }
  M &operator+=(const M &b)
  {
    assert(shape() == b.shape());
    for (size_t i = 0; i < values_.size(); ++i)
      values_[i] ^= b.values_[i];
    return *this;
  }
  M &operator-=(const M &b) { return *this += b; }
  M operator+(const M &b) const { return M(*this) += b; }
  M operator-(const M &b) const { return M(*this) -= b; }
  V operator*(const V &v) const
  {
    assert(v.size() == m);
    V result(n_);
    for (int i = 0; i < n_; ++i)
    {
      Word parity = 0;
      for (int w = 0; w < width(); ++w)
        parity ^= row(i).word(w) & v.word(w);
      result.set(i, __builtin_parityll(parity));
    }
    return result;
  }
  friend V operator*(const V &v, const M &a)
  {
    assert(v.size() == a.n_);
    V result(a.m);
    for (int i = v.find_first(); i < v.size(); i = v.find_next(i))
      for (int w = 0; w < a.width(); ++w)
        result.set_word(w, result.word(w) ^ a.row(i).word(w));
    return result;
  }
  M operator*(const M &b) const
  {
    assert(m == b.n_);
    M c(n_, b.m);
    for (int i = 0; i < n_; ++i)
    {
      auto dest = c[i];
      for (int w = 0; w < width(); ++w)
      {
        Word bits = row(i).word(w);
        while (bits)
        {
          int k = w * 64 + __builtin_ctzll(bits);
          bits &= bits - 1;
          dest ^= b[k];
        }
      }
    }
    return c;
  }
  M &operator*=(const M &b) { return *this = *this * b; }
  template <class T> M pow(T k) const
  {
    assert(n_ == m && k >= 0);
    M result(n_, m, 1), a(*this);
    while (k > 0)
    {
      if (k & 1)
        result *= a;
      k >>= 1;
      if (k > 0)
        a *= a;
    }
    return result;
  }
  M row_reduction(bool rref = false) const
  {
    M a(*this);
    for (int i = 0, j = 0; i < n_ && j < m; ++j)
    {
      int pivot = i;
      while (pivot < n_ && !a.test(pivot, j))
        ++pivot;
      if (pivot == n_)
        continue;
      a.swap_rows(i, pivot);
      for (int k = rref ? 0 : i + 1; k < n_; ++k)
        if (k != i && a.test(k, j))
          for (int w = j / 64; w < width(); ++w)
            a.values_[size_t(k) * width() + w] ^= a.values_[size_t(i) * width() + w];
      ++i;
    }
    return a;
  }
  template <class I = ll> I rank() const
  {
    return I(matrix_mod2_detail::rank(n_, m, [&](int i, int w) { return row(i).word(w); }));
  }
  template <class I = ll> I det() const
  {
    assert(n_ == m);
    if (!n_)
      return I(1);
    auto get = [&](int i, int w) { return row(i).word(w); };
    if (n_ <= 64)
      return I(matrix_mod2_detail::rank_narrow(n_, m, get) == n_);
    matrix_mod2_detail::elimination work(n_, m, false, get);
    work.forward(true);
    return I(int(work.pivots.size()) == n_);
  }
  optional<M> inv() const
  {
    assert(n_ == m);
    auto result = matrix_mod2_detail::inverse(n_, [&](int i, int w) { return row(i).word(w); });
    if (!result)
      return nullopt;
    M a(n_, m);
    for (size_t i = 0; i < a.values_.size(); ++i)
      a.values_[i] = (*result)[i];
    return a;
  }
  optional<pair<V, vc<V>>> solve(const V &b) const
  {
    assert(b.size() == n_);
    M a(n_, m + 1);
    for (int i = 0; i < n_; ++i)
    {
      for (int w = 0; w < width(); ++w)
        a.row(i).set_word(w, row(i).word(w));
      a.set(i, m, b[i]);
    }
    a = a.row_reduction(true);
    V sol(m);
    vc<V> basis;
    vc<int> pivot(m, -1);
    for (int i = 0, j = 0; i < n_ && a[i].any(); ++i)
    {
      while (j < m && !a.test(i, j))
        ++j;
      if (j == m)
        return nullopt;
      sol.set(j, a.test(i, m));
      pivot[j] = i;
    }
    for (int j = 0; j < m; ++j)
      if (pivot[j] < 0)
      {
        V v(m);
        v.set(j);
        for (int k = 0; k < j; ++k)
          if (pivot[k] >= 0 && a.test(pivot[k], j))
            v.set(k);
        basis.push_back(move(v));
      }
    return pair{move(sol), move(basis)};
  }
};
