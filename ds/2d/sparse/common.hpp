#pragma once

namespace sparse_2d
{
template <class DS>
auto range_sum_impl(const DS &ds, int l, int r, int)
    -> decltype(ds.sum(l, r))
{
  return ds.sum(l, r);
}

template <class DS>
auto range_sum_impl(const DS &ds, int l, int r, long)
    -> decltype(ds.prod(l, r))
{
  return ds.prod(l, r);
}

template <class DS>
auto range_sum(const DS &ds, int l, int r)
    -> decltype(range_sum_impl(ds, l, r, 0))
{
  return range_sum_impl(ds, l, r, 0);
}

template <class DS>
auto prefix_sum_impl(const DS &ds, int r, int)
    -> decltype(ds.sum(r))
{
  return ds.sum(r);
}

template <class DS>
auto prefix_sum_impl(const DS &ds, int r, long)
    -> decltype(range_sum(ds, 0, r))
{
  return range_sum(ds, 0, r);
}

template <class DS>
auto prefix_sum(const DS &ds, int r)
    -> decltype(prefix_sum_impl(ds, r, 0))
{
  return prefix_sum_impl(ds, r, 0);
}

template <class M, class DS>
auto point_add_impl(DS &ds, int p, const typename M::S &x, int)
    -> decltype(ds.add(p, x), void())
{
  ds.add(p, x);
}

template <class M, class DS>
auto point_add_impl(DS &ds, int p, const typename M::S &x, long)
    -> decltype(ds.set(p, M::op(ds.get(p), x)), void())
{
  ds.set(p, M::op(ds.get(p), x));
}

template <class M, class DS>
void point_add(DS &ds, int p, const typename M::S &x)
{
  point_add_impl<M>(ds, p, x, 0);
}
} // namespace sparse_2d
