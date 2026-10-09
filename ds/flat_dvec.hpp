#pragma once

#include "../template/template_vector.hpp"

/**
 * @brief 一次元で保持する多次元配列
 * @docs docs/ds/flat_dvec.md
 */

template <class T, size_t D = 0>
struct FlatDvec
{
  using shape_type = conditional_t<D == 0, vc<size_t>, array<size_t, D>>;
  using reference = typename vc<T>::reference;
  using const_reference = typename vc<T>::const_reference;

private:
  shape_type dims{}, strides{};
  vc<T> dat;

  template <class I, size_t R>
  void init_shape(const I *sz, const T &init)
  {
    static_assert(is_integral_v<I>);
    static_assert(R >= 1);
    static_assert(D == 0 || D == R);
    if constexpr (D == 0)
    {
      dims.resize(R);
      strides.resize(R);
    }
    bool empty_shape = false;
    for (size_t d = 0; d < R; d++)
    {
      if constexpr (is_signed_v<I>)
        assert(sz[d] >= 0);
      dims[d] = size_t(sz[d]);
      empty_shape |= dims[d] == 0;
    }
    if (empty_shape)
    {
      strides.back() = 1;
      return;
    }
    // 最後の添字が連続する通常の配置。
    size_t n = 1;
    for (size_t d = R; d-- > 0;)
    {
      strides[d] = n;
      n *= dims[d];
    }
    dat.assign(n, init);
  }

  template <bool checked, class... I>
  size_t offset(I... is) const
  {
    static_assert(sizeof...(I) >= 1);
    if constexpr (D == 0)
      assert(sizeof...(I) == dims.size());
    else
      static_assert(sizeof...(I) == D);
    static_assert((is_integral_v<I> && ...));
    array<size_t, sizeof...(I)> ids{size_t(is)...};
    size_t pos = 0;
    for (size_t d = 0; d < sizeof...(I); d++)
    {
      if constexpr (checked)
        assert(ids[d] < dims[d]);
      pos += ids[d] * strides[d];
    }
    return pos;
  }

public:
  FlatDvec()
  {
    if constexpr (D > 0)
      strides.back() = 1;
  }
  // ll の配列として受け取り、{N, M, 2} のような混在も扱う。
  template <size_t R>
  FlatDvec(const ll (&sz)[R], const T &init = T()) { init_shape<ll, R>(sz, init); }
  template <class I, size_t R>
  FlatDvec(const I (&sz)[R], const T &init = T()) { init_shape<I, R>(sz, init); }
  template <class I, size_t R>
  FlatDvec(const array<I, R> &sz, const T &init = T()) { init_shape<I, R>(sz.data(), init); }

  size_t ndim() const { return dims.size(); }
  size_t size() const { return dat.size(); }
  size_t size(size_t d) const
  {
    assert(d < ndim());
    return dims[d];
  }
  bool empty() const { return dat.empty(); }
  const shape_type &shape() const { return dims; }
  size_t stride(size_t d) const
  {
    assert(d < ndim());
    return strides[d];
  }

  template <class... I>
  reference operator()(I... is) { return dat[offset<false>(is...)]; }
  template <class... I>
  const_reference operator()(I... is) const { return dat[offset<false>(is...)]; }
  template <class... I>
  reference at(I... is) { return dat[offset<true>(is...)]; }
  template <class... I>
  const_reference at(I... is) const { return dat[offset<true>(is...)]; }

  void fill(const T &value) { std::fill(dat.begin(), dat.end(), value); }
  auto begin() { return dat.begin(); }
  auto end() { return dat.end(); }
  auto begin() const { return dat.begin(); }
  auto end() const { return dat.end(); }
  const vc<T> &content() const { return dat; }
};

template <class T, size_t D>
FlatDvec(const ll (&)[D], const T &) -> FlatDvec<T, D>;
template <class I, class T, size_t D>
FlatDvec(const I (&)[D], const T &) -> FlatDvec<T, D>;
template <class I, class T, size_t D>
FlatDvec(const array<I, D> &, const T &) -> FlatDvec<T, D>;
