#pragma once

#include "template_float128_types.hpp"
#include <cassert>
using namespace std;

#ifndef EPS
#define EPS 1e-11
#endif
using ld = decltype(EPS);

using ll = long long;
using uint = unsigned int;
using ull = unsigned long long;
using pll = pair<ll, ll>;
using tlll = tuple<ll, ll, ll>;
using tllll = tuple<ll, ll, ll, ll>;

#define vc vector
template <class T>
using vvc = vc<vc<T>>;
template <class T>
using vvvc = vc<vc<vc<T>>>;

using vb = vc<bool>;
using vl = vc<ll>;
using vpll = vc<pll>;
using vtlll = vc<tlll>;
using vtllll = vc<tllll>;
using vstr = vc<string>;
using vvb = vvc<bool>;
using vvl = vvc<ll>;

template <class T>
using pql = priority_queue<T, vc<T>, greater<T>>;
template <class T>
using pqg = priority_queue<T>;

#ifdef __SIZEOF_INT128__
using i128 = __int128_t;
using u128 = __uint128_t;
#endif

#define cauto const auto
