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

#include "template_arithmetic_impl.hpp"
