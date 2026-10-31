#pragma once

#include "../../template/template_all_but_modint.hpp"

// Matrix storage and views over contiguous ranges.

namespace internal
{
template <class Iterator> struct matrix_row
{
  using value_type = typename iterator_traits<Iterator>::value_type;
  using reference = typename iterator_traits<Iterator>::reference;
  Iterator first, last;
  Iterator begin() const { return first; }
  Iterator end() const { return last; }
  size_t size() const { return last - first; }
  bool empty() const { return first == last; }
  reference operator[](size_t j) const { return first[j]; }
  reference at(size_t j) const
  {
    assert(j < size());
    return first[j];
  }
  reference front() const { return *first; }
  reference back() const { return *prev(last); }
  operator vc<value_type>() const { return {first, last}; }
  matrix_row &operator=(const matrix_row &b)
  {
    assert(size() == b.size());
    if (first != b.first)
      copy(b.begin(), b.end(), first);
    return *this;
  }
  template <class Range> matrix_row &operator=(const Range &b)
  {
    assert(size() == b.size());
    copy(b.begin(), b.end(), first);
    return *this;
  }
  matrix_row &operator=(initializer_list<value_type> b)
  {
    assert(size() == b.size());
    copy(b.begin(), b.end(), first);
    return *this;
  }
  template <class Range> bool operator==(const Range &b) const
  {
    return size() == b.size() && equal(first, last, b.begin());
  }
  template <class Range> bool operator!=(const Range &b) const { return !(*this == b); }
};
template <class T, class A, class It>
bool operator==(const vector<T, A> &a, const matrix_row<It> &b)
{
  return b == a;
}
template <class T, size_t N, class It>
bool operator==(const array<T, N> &a, const matrix_row<It> &b)
{
  return b == a;
}

template <class Owner> struct matrix_row_iterator
{
  using value_type = decltype(declval<Owner &>()[0]);
  using reference = value_type;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = random_access_iterator_tag;
  Owner *owner = nullptr;
  difference_type index = 0;
  reference operator*() const { return (*owner)[index]; }
  reference operator[](difference_type k) const { return (*owner)[index + k]; }
  matrix_row_iterator &operator++()
  {
    ++index;
    return *this;
  }
  matrix_row_iterator operator++(int)
  {
    auto old = *this;
    ++*this;
    return old;
  }
  matrix_row_iterator &operator--()
  {
    --index;
    return *this;
  }
  matrix_row_iterator operator--(int)
  {
    auto old = *this;
    --*this;
    return old;
  }
  matrix_row_iterator &operator+=(difference_type k)
  {
    index += k;
    return *this;
  }
  matrix_row_iterator &operator-=(difference_type k)
  {
    index -= k;
    return *this;
  }
  friend matrix_row_iterator operator+(matrix_row_iterator a, difference_type k) { return a += k; }
  friend matrix_row_iterator operator+(difference_type k, matrix_row_iterator a) { return a += k; }
  friend matrix_row_iterator operator-(matrix_row_iterator a, difference_type k) { return a -= k; }
  friend difference_type operator-(matrix_row_iterator a, matrix_row_iterator b)
  {
    return a.index - b.index;
  }
  friend bool operator==(matrix_row_iterator a, matrix_row_iterator b)
  {
    return a.owner == b.owner && a.index == b.index;
  }
  friend bool operator!=(matrix_row_iterator a, matrix_row_iterator b) { return !(a == b); }
  friend bool operator<(matrix_row_iterator a, matrix_row_iterator b) { return a.index < b.index; }
  friend bool operator>(matrix_row_iterator a, matrix_row_iterator b) { return b < a; }
  friend bool operator<=(matrix_row_iterator a, matrix_row_iterator b) { return !(b < a); }
  friend bool operator>=(matrix_row_iterator a, matrix_row_iterator b) { return !(a < b); }
};

template <class T> struct matrix_storage
{
  using reference = typename vc<T>::reference;
  using const_reference = typename vc<T>::const_reference;

private:
  int rows_ = 0, columns_ = 0;
  vc<T> values_;

public:
  matrix_storage() = default;
  matrix_storage(int n, int m, const T &value)
      : rows_(n), columns_(m), values_(size_t(n) * m, value)
  {
    assert(n >= 0 && m >= 0);
  }
  matrix_storage(const vvc<T> &a) : rows_(a.size()), columns_(a.empty() ? 0 : a[0].size())
  {
    values_.reserve(size_t(rows_) * columns_);
    for (const auto &r : a)
    {
      assert(SZ(r) == columns_);
      values_.insert(values_.end(), r.begin(), r.end());
    }
  }
  matrix_storage(const matrix_storage &) = default;
  matrix_storage &operator=(const matrix_storage &) = default;
  matrix_storage(matrix_storage &&a) noexcept
      : rows_(exchange(a.rows_, 0)), columns_(exchange(a.columns_, 0)), values_(move(a.values_))
  {
  }
  matrix_storage &operator=(matrix_storage &&a) noexcept
  {
    if (this != &a)
    {
      rows_ = exchange(a.rows_, 0);
      columns_ = exchange(a.columns_, 0);
      values_ = move(a.values_);
    }
    return *this;
  }
  size_t size() const { return rows_; }
  bool empty() const { return rows_ == 0; }
  template <class I = ll> pair<I, I> shape() const { return {I(rows_), I(columns_)}; }
  reference operator()(int i, int j) { return values_[size_t(i) * columns_ + j]; }
  const_reference operator()(int i, int j) const { return values_[size_t(i) * columns_ + j]; }
  reference at(int i, int j)
  {
    assert(0 <= i && i < rows_ && 0 <= j && j < columns_);
    return (*this)(i, j);
  }
  const_reference at(int i, int j) const
  {
    assert(0 <= i && i < rows_ && 0 <= j && j < columns_);
    return (*this)(i, j);
  }
  auto row(int i)
  {
    auto b = values_.begin() + size_t(i) * columns_;
    return matrix_row<decltype(b)>{b, b + columns_};
  }
  auto row(int i) const
  {
    auto b = values_.begin() + size_t(i) * columns_;
    return matrix_row<decltype(b)>{b, b + columns_};
  }
  auto operator[](int i) { return row(i); }
  auto operator[](int i) const { return row(i); }
  auto front() { return row(0); }
  auto front() const { return row(0); }
  auto back() { return row(rows_ - 1); }
  auto back() const { return row(rows_ - 1); }
  auto at(int i)
  {
    assert(0 <= i && i < rows_);
    return row(i);
  }
  auto at(int i) const
  {
    assert(0 <= i && i < rows_);
    return row(i);
  }
  auto begin() { return matrix_row_iterator<matrix_storage>{this, 0}; }
  auto end() { return matrix_row_iterator<matrix_storage>{this, rows_}; }
  auto begin() const { return matrix_row_iterator<const matrix_storage>{this, 0}; }
  auto end() const { return matrix_row_iterator<const matrix_storage>{this, rows_}; }
  auto cbegin() const { return begin(); }
  auto cend() const { return end(); }
  void swap_rows(int i, int j)
  {
    if (i != j)
      swap_ranges(row(i).begin(), row(i).end(), row(j).begin());
  }
  const vc<T> &content() const { return values_; }
  template <class U = T> vvc<U> to_vvi() const
  {
    vvc<U> a(rows_, vc<U>(columns_));
    repi(i, rows_) repi(j, columns_) a[i][j] = (*this)(i, j);
    return a;
  }
  operator vvc<T>() const { return to_vvi(); }
  friend bool operator==(const matrix_storage &a, const matrix_storage &b)
  {
    return a.shape() == b.shape() && a.values_ == b.values_;
  }
  friend bool operator!=(const matrix_storage &a, const matrix_storage &b) { return !(a == b); }
};
} // namespace internal
