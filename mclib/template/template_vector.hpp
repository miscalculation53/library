#pragma once

#ifndef INF
#define INF 4'000'000'000'000'000'037LL
#endif

#include "template_types.hpp"
#include "template_rep.hpp"
#include "template_math.hpp"

#define ALL(a) (a).begin(), (a).end()
#define SZ(x) (ll)((x).size())
#define SZI(x) (int)((x).size())

template <class T = ll> vector<T> permid(int n, int base_index = 0) { vector<T> p(n); rep(i, n) p[i] = i + base_index; return p; }
template <class T = ll> vector<T> combid(int n, int k) { vector<T> p(n, 0); fill(p.rbegin(), p.rbegin() + k, 1); return p; }
template <class F> auto gen_vec(int n, const F &f) { using T = decltype(f(0)); vector<T> res(n); rep(i, n) res[i] = f(i); return res; }
// res[i] = op[0, i) for 0 <= i < n+1
template <class T, class F = decltype(plus<>())> vector<T> cuml(vector<T> v, const F &op = plus<>(), const T &e = 0) { v.emplace_back(e); exclusive_scan(v.begin(), v.end(), v.begin(), e, op); return v; }
// res[i] = op[i, n) for 0 <= i < n+1
template <class T, class F = decltype(plus<>())> vector<T> cumr(vector<T> v, const F &op = plus<>(), const T &e = 0) { v.insert(v.begin(), e); exclusive_scan(v.rbegin(), v.rend(), v.rbegin(), e, op); return v; }
// res[i] = v[i] - v[i-1] for 0 <= i < n+1
template <class T> vector<T> adjd(vector<T> v) { v.emplace_back(0); adjacent_difference(v.begin(), v.end(), v.begin()); return v; }
template <class T = ll> struct max_op { T operator()(const T &a, const T &b) const { return max(a, b); } };
template <class T = ll> struct min_op { T operator()(const T &a, const T &b) const { return min(a, b); } };
template <class T, const T val> struct const_fn { T operator()() const { return val; } };
using max_e = const_fn<ll, -INF>;
using min_e = const_fn<ll, INF>;
using zero_fn = const_fn<ll, 0LL>;
template <class T> vector<T> cumlmax(const vector<T> &v) { return cuml(v, max_op<T>(), max_e()()); }
template <class T> vector<T> cumrmax(const vector<T> &v) { return cumr(v, max_op<T>(), max_e()()); }
template <class T> vector<T> cumlmin(const vector<T> &v) { return cuml(v, min_op<T>(), min_e()()); }
template <class T> vector<T> cumrmin(const vector<T> &v) { return cumr(v, min_op<T>(), min_e()()); }
template <class T> vector<T> sorted(vector<T> v) { sort(v.begin(), v.end()); return v; }
template <class T> vector<T> reversed(const vector<T> &v) { return {v.rbegin(), v.rend()}; }
template <class T> void unique(vector<T> &v) { v.erase(unique(v.begin(), v.end()), v.end()); }
template <class T> vector<T> uniqued(vector<T> v) { v.erase(unique(v.begin(), v.end()), v.end()); return v; }
template <class T> void sortunique(vector<T> &v) { sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end()); }
template <class T> vector<T> sortuniqued(vector<T> v) { sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end()); return v; }
template <class T> void rotate(vector<T> &v, int k) { rotate(v.begin(), v.begin() + k, v.end()); }
template <class T> vector<T> rotated(vector<T> v, int k) { rotate(v.begin(), v.begin() + k, v.end()); return v; }
string sorted(string s) { sort(s.begin(), s.end()); return s; }
string reversed(const string &s) { return {s.rbegin(), s.rend()}; }
void unique(string &s) { s.erase(unique(s.begin(), s.end()), s.end()); }
string uniqued(string s) { s.erase(unique(s.begin(), s.end()), s.end()); return s; }
void sortunique(string &s) { sort(s.begin(), s.end()); s.erase(unique(s.begin(), s.end()), s.end()); }
string sortuniqued(string s) { sort(s.begin(), s.end()); s.erase(unique(s.begin(), s.end()), s.end()); return s; }
void rotate(string &s, int k) { rotate(s.begin(), s.begin() + k, s.end()); }
string rotated(string s, int k) { rotate(s.begin(), s.begin() + k, s.end()); return s; }
template <class T> vector<vector<T>> top(const vector<vector<T>> &a) { if (a.empty()) return {}; const size_t n = a.size(), m = a[0].size(); vector<vector<T>> b(m, vector<T>(n)); for (size_t i = 0; i < n; i++) for (size_t j = 0; j < m; j++) b[j][i] = a[i].at(j); return b; }
vector<string> top(const vector<string> &a) { if (a.empty()) return {}; const size_t n = a.size(), m = a[0].size(); vector<string> b(m, string(n, 0)); for (size_t i = 0; i < n; i++) for (size_t j = 0; j < m; j++) b[j][i] = a[i].at(j); return b; }
template <class T> vector<vector<T>> rot90(const vector<vector<T>> &a) { if (a.empty()) return {}; const size_t n = a.size(), m = a[0].size(); vector<vector<T>> b(m, vector<T>(n)); for (size_t i = 0; i < n; i++) for (size_t j = 0; j < m; j++) b[j][n - 1 - i] = a[i][j]; return b; }
vector<string> rot90(const vector<string> &a) { if (a.empty()) return {}; const size_t n = a.size(), m = a[0].size(); vector<string> b(m, string(n, 0)); for (size_t i = 0; i < n; i++) for (size_t j = 0; j < m; j++) b[j][n - 1 - i] = a[i][j]; return b; }
template <class T = ll> vector<T> digitvec(const string &s) { int n = s.size(); vector<T> a(n); rep(i, n) a[i] = s[i] - '0'; return a; }
template <class T, size_t d, size_t i = 0> auto make_vec(const auto (&sz)[d], const T &init) { if constexpr (i < d) return vector(sz[i], make_vec<T, d, i + 1>(sz, init)); else return init; }