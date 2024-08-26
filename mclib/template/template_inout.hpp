#pragma once

#include "template_types.hpp"
#include "template_rep.hpp"
#include "template_math.hpp"
template <class Tuple> Tuple operator+=(Tuple &a, const Tuple &b) { return tuple_add(a, b, make_index_sequence<tuple_size_v<Tuple>>{}); }
template <class Tuple> Tuple operator+(Tuple a, const Tuple &b) { return a += b; }
template <class T, class U> void offset(vector<T> &v, U add) { for (auto &vi : v) vi += add; }
template <class T, class U> void offset(vector<vector<T>> &v, U add) { for (auto &vi : v) for (auto &vij : vi) vij += add; }
template <class T, const size_t m> array<vector<T>, m> top(const vector<array<T, m>> &a) { const size_t n = a.size(); array<vector<T>, m> b; b.fill(vector<T>(n)); for (size_t i = 0; i < n; i++) for (size_t j = 0; j < m; j++) b[j][i] = a[i][j]; return b; }
template <class T, const size_t n> vector<array<T, n>> top(const array<vector<T>, n> &a) { if (a.empty()) return {}; const size_t m = a[0].size(); vector<array<T, n>> b(m); for (size_t i = 0; i < n; i++) for (size_t j = 0; j < m; j++) b[j][i] = a[i].at(j); return b; }
template <class T, class U> pair<vector<T>, vector<U>> top(const vector<pair<T, U>> &a) { const size_t n = a.size(); vector<T> b(n); vector<U> c(n); for (size_t i = 0; i < n; i++) tie(b[i], c[i]) = a[i]; return make_pair(b, c); }
template <class T, class U> vector<pair<T, U>> top(const pair<vector<T>, vector<U>> &a) { const size_t n = a.first.size(); vector<pair<T, U>> b(n); for (size_t i = 0; i < n; i++) b[i] = make_pair(a.first[i], a.second.at(i)); return b; }
template <class T1, class T2, class T3> tuple<vector<T1>, vector<T2>, vector<T3>> top(const vector<tuple<T1, T2, T3>> &a) { const size_t n = a.size(); vector<T1> b(n); vector<T2> c(n); vector<T3> d(n); for (size_t i = 0; i < n; i++) tie(b[i], c[i], d[i]) = a[i]; return make_tuple(b, c, d); }
template <class T1, class T2, class T3> vector<tuple<T1, T2, T3>> top(const tuple<vector<T1>, vector<T2>, vector<T3>> &a) { const size_t n = get<0>(a).size(); vector<tuple<T1, T2, T3>> b(n); for (size_t i = 0; i < n; i++) b[i] = make_tuple(get<0>(a)[i], get<1>(a).at(i), get<2>(a).at(i)); return b; }
template <class T1, class T2, class T3, class T4> tuple<vector<T1>, vector<T2>, vector<T3>, vector<T4>> top(const vector<tuple<T1, T2, T3, T4>> &a) { const size_t n = a.size(); vector<T1> b(n); vector<T2> c(n); vector<T3> d(n); vector<T4> e(n); for (size_t i = 0; i < n; i++) tie(b[i], c[i], d[i], e[i]) = a[i]; return make_tuple(b, c, d, e); }
template <class T1, class T2, class T3, class T4> vector<tuple<T1, T2, T3, T4>> top(const tuple<vector<T1>, vector<T2>, vector<T3>, vector<T4>> &a) { const size_t n = get<0>(a).size(); vector<tuple<T1, T2, T3, T4>> b(n); for (size_t i = 0; i < n; i++) b[i] = make_tuple(get<0>(a)[i], get<1>(a).at(i), get<2>(a).at(i), get<3>(a).at(i)); return b; }
template <class Tuple, enable_if_t<__is_tuple_like<Tuple>::value == true> * = nullptr> istream &operator>>(istream &is, Tuple &t) { apply([&](auto&... a){ (is >> ... >> a); }, t); return is; }
template <class... T> void input(T&... a) { (cin >> ... >> a); }
template <class T> void inputvec(int n, vector<T> &v) { v.resize(n); rep(i, n) cin >> v[i]; }
template <class T, class... Ts> void inputvec(int n, vector<T>& v, vector<Ts>&... vs) { inputvec(n, v); inputvec(n, vs...); }
template <class T> void inputvec2(int n, int m, vector<vector<T>> &v) { v.assign(n, vector<T>(m)); rep(i, n) rep(j, m) cin >> v[i][j]; }
template <class T, class... Ts> void inputvec2(int n, int m, vector<T>& v, vector<Ts>&... vs) { inputvec2(n, m, v); inputvec2(n, m, vs...); }
#define INT(...) int __VA_ARGS__; input(__VA_ARGS__)
#define LL(...) ll __VA_ARGS__; input(__VA_ARGS__)
#define STR(...) string __VA_ARGS__; input(__VA_ARGS__)
#define ARR(T, n, ...) array<T, n> __VA_ARGS__; input(__VA_ARGS__)
#define VEC(T, n, ...) vector<T> __VA_ARGS__; inputvec(n, __VA_ARGS__)
#define VEC2(T, n, m, ...) vector<vector<T>> __VA_ARGS__; inputvec2(n, m, __VA_ARGS__)
template <class T> void PRINT(const T &a) { cout << a << '\n'; }
template <class T, class... Ts> void PRINT(const T& a, const Ts&... b) { cout << a; (cout << ... << (cout << ' ', b)); cout << '\n'; }
template <class T> void PRINTVEC(const vector<T> &v) { int n = v.size(); rep(i, n) cout << v[i] << (i == n - 1 ? "" : " "); cout << '\n'; }
template <class T> void PRINTVECT(const vector<T> &v) { for (auto &vi : v) cout << vi << '\n';}
template <class T> void PRINTVEC2(const vector<vector<T>> &v) { for (auto &vi : v) PRINTVEC(vi); }
#define PRINTEXIT(...) do { PRINT(__VA_ARGS__); exit(0); } while (false)
#define PRINTRETURN(...) do { PRINT(__VA_ARGS__); return; } while (false)