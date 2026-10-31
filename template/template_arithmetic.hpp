#pragma once

#include "template_types.hpp"

/**
 * @brief pair・tuple・array の成分ごとの加算・減算
 * @docs docs/template/template_arithmetic.md
 */

template <class T, class U, class P>
constexpr pair<T, U> &operator+=(pair<T, U> &a, const P &b);
template <class T, class U, class P>
constexpr pair<T, U> operator+(pair<T, U> a, const P &b);
template <class T, class U, class P>
constexpr pair<T, U> &operator-=(pair<T, U> &a, const P &b);
template <class T, class U, class P>
constexpr pair<T, U> operator-(pair<T, U> a, const P &b);
template <class T, class U>
constexpr pair<T, U> operator-(pair<T, U> a);

template <class T, size_t n, class A>
constexpr array<T, n> &operator+=(array<T, n> &a, const A &b);
template <class T, size_t n, class A>
constexpr array<T, n> operator+(array<T, n> a, const A &b);
template <class T, size_t n, class A>
constexpr array<T, n> &operator-=(array<T, n> &a, const A &b);
template <class T, size_t n, class A>
constexpr array<T, n> operator-(array<T, n> a, const A &b);
template <class T, size_t n>
constexpr array<T, n> operator-(array<T, n> a);

template <class... Ts, class Tp>
constexpr tuple<Ts...> &operator+=(tuple<Ts...> &a, const Tp &b);
template <class... Ts, class Tp>
constexpr tuple<Ts...> operator+(tuple<Ts...> a, const Tp &b);
template <class... Ts, class Tp>
constexpr tuple<Ts...> &operator-=(tuple<Ts...> &a, const Tp &b);
template <class... Ts, class Tp>
constexpr tuple<Ts...> operator-(tuple<Ts...> a, const Tp &b);
template <class... Ts>
constexpr tuple<Ts...> operator-(tuple<Ts...> a);

template <class T, class U, class P>
constexpr pair<T, U> &operator+=(pair<T, U> &a, const P &b)
{
  a.first += b.first;
  a.second += b.second;
  return a;
}
template <class T, class U, class P>
constexpr pair<T, U> operator+(pair<T, U> a, const P &b) { return a += b; }
template <class T, class U, class P>
constexpr pair<T, U> &operator-=(pair<T, U> &a, const P &b)
{
  a.first -= b.first;
  a.second -= b.second;
  return a;
}
template <class T, class U, class P>
constexpr pair<T, U> operator-(pair<T, U> a, const P &b) { return a -= b; }
template <class T, class U>
constexpr pair<T, U> operator-(pair<T, U> a)
{
  a.first = -a.first;
  a.second = -a.second;
  return a;
}

template <class T, size_t n, class A>
constexpr array<T, n> &operator+=(array<T, n> &a, const A &b)
{
  for (size_t i = 0; i < n; i++)
    a[i] += b[i];
  return a;
}
template <class T, size_t n, class A>
constexpr array<T, n> operator+(array<T, n> a, const A &b) { return a += b; }
template <class T, size_t n, class A>
constexpr array<T, n> &operator-=(array<T, n> &a, const A &b)
{
  for (size_t i = 0; i < n; i++)
    a[i] -= b[i];
  return a;
}
template <class T, size_t n, class A>
constexpr array<T, n> operator-(array<T, n> a, const A &b) { return a -= b; }
template <class T, size_t n>
constexpr array<T, n> operator-(array<T, n> a)
{
  for (auto &ai : a)
    ai = -ai;
  return a;
}

namespace internal
{

template <size_t... I, class A, class B>
constexpr auto &tuple_add_impl(A &a, const B &b, const index_sequence<I...>)
{
  ((get<I>(a) += get<I>(b)), ...);
  return a;
}
template <size_t... I, class A, class B>
constexpr auto &tuple_sub_impl(A &a, const B &b, const index_sequence<I...>)
{
  ((get<I>(a) -= get<I>(b)), ...);
  return a;
}
template <size_t... I, class A>
constexpr auto &tuple_neg_impl(A &a, const index_sequence<I...>)
{
  ((get<I>(a) = -get<I>(a)), ...);
  return a;
}

}; // namespace internal

template <class... Ts, class Tp>
constexpr tuple<Ts...> &operator+=(tuple<Ts...> &a, const Tp &b)
{ return internal::tuple_add_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{}); }
template <class... Ts, class Tp>
constexpr tuple<Ts...> operator+(tuple<Ts...> a, const Tp &b) { return a += b; }
template <class... Ts, class Tp>
constexpr tuple<Ts...> &operator-=(tuple<Ts...> &a, const Tp &b)
{ return internal::tuple_sub_impl(a, b, make_index_sequence<tuple_size_v<tuple<Ts...>>>{}); }
template <class... Ts, class Tp>
constexpr tuple<Ts...> operator-(tuple<Ts...> a, const Tp &b) { return a -= b; }
template <class... Ts>
constexpr tuple<Ts...> operator-(tuple<Ts...> a)
{
  internal::tuple_neg_impl(a, make_index_sequence<sizeof...(Ts)>{});
  return a;
}

