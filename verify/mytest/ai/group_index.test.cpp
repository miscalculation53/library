#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/group_index.hpp"

#if __cplusplus >= 202002L
static_assert(ranges::input_range<const GroupIndex<string>>);
static_assert(ranges::input_range<const GroupIndex<int, false>>);
static_assert(ranges::input_range<const GroupIndexRaw<int>>);
#endif

template <class T, bool compress = true, class I = ll>
void check(const vc<T> &a, const vc<T> &queries)
{
  using Group = conditional_t<compress, GroupIndex<T, true, I>, GroupIndexRaw<T, I>>;
  const Group grp(a);
  const int n = a.size();
  map<T, vc<I>> expected;
  repi(i, n) expected[a[i]].push_back(i);
  assert(grp.size() == int(expected.size()));
  assert(grp.empty() == a.empty());
  assert(distance(grp.begin(), grp.end()) == grp.size());

  auto eit = expected.begin();
  vvc<I> rows;
  if constexpr (!compress) rows.resize(a.empty() ? 0 : int(MAX(a)) + 1);
  for (auto [value, indices] : grp)
  {
    static_assert(is_same_v<decltype(indices[0]), const I &>);
    assert(eit != expected.end() && value == eit->first);
    assert(indices.to_v() == eit->second);
    assert(grp.idxs(value).to_v() == eit->second);
    if constexpr (compress) rows.push_back(eit->second);
    else rows[int(value)] = eit->second;
    ++eit;
  }
  assert(eit == expected.end());
  assert(grp.to_vv() == rows);
  assert(grp.to_csr().to_vv() == rows);

  auto it = grp.begin(), independent = it;
  if (!grp.empty())
  {
    auto old = it++;
    assert(old == independent);
    assert((*old).first == expected.begin()->first);
    assert((*old).second.to_v() == expected.begin()->second);
    ++independent;
    assert(it == independent);
  }

  for (const T &value : queries)
  {
    const auto found = expected.find(value);
    const vc<I> want = found == expected.end() ? vc<I>{} : found->second;
    assert(grp.idxs(value).to_v() == want);
    for (int i = -1; i <= n + 1; ++i)
    {
      I lt = -1, leq = -1, gt = n, geq = n;
      I ltc = 0, leqc = 0, gtc = 0, geqc = 0;
      repi(j, n) if (a[j] == value)
      {
        if (j < i) lt = j, ++ltc;
        if (j <= i) leq = j, ++leqc;
        if (j > i) gt = min(gt, I(j)), ++gtc;
        if (j >= i) geq = min(geq, I(j)), ++geqc;
      }
      assert(grp.lt_max(value, i).value_or(-1) == lt && grp.leq_max(value, i).value_or(-1) == leq);
      assert(grp.gt_min(value, i).value_or(n) == gt && grp.geq_min(value, i).value_or(n) == geq);
      assert(grp.lt_cnt(value, i) == ltc && grp.leq_cnt(value, i) == leqc);
      assert(grp.gt_cnt(value, i) == gtc && grp.geq_cnt(value, i) == geqc);
      for (int r = i; r <= n + 1; ++r)
      {
        I count = 0;
        repi(j, n) if (i <= j && j < r && a[j] == value) ++count;
        assert(grp.in_cnt(value, i, r) == count);
      }
    }
  }
}

template <bool compress>
void check_default()
{
  using Group = conditional_t<compress, GroupIndex<int, true, int>, GroupIndexRaw<int, int>>;
  Group grp;
  assert(grp.empty() && grp.size() == 0);
  assert(grp.begin() == grp.end());
  assert(grp.to_vv().empty() && grp.to_csr().size() == 0);
  assert(grp.idxs(5).empty());
  assert(grp.lt_max(5, 0).value_or(-1) == -1 && !grp.geq_min(5, 0));
  assert(grp.in_cnt(5, 0, 1) == 0);
  grp = Group(vc<int>{2, 0, 2});
  assert((grp.idxs(2).to_v() == vc<int>{0, 2}));
  auto copy = grp;
  grp = {};
  assert((copy.idxs(2).to_v() == vc<int>{0, 2}));
}

template <class T>
void check_large(const vc<T> &a)
{
  const GroupIndex grp(a);
  map<T, vl> expected;
  repi(i, a.size()) expected[a[i]].eb(i);
  assert(grp.size() == int(expected.size()));
  auto it = expected.begin();
  for (auto [value, indices] : grp)
  {
    assert(it != expected.end() && value == it->first && indices.to_v() == it->second);
    assert(grp.idxs(value).to_v() == it->second);
    ++it;
  }
  assert(it == expected.end());
}

template <class I = ll>
void check_search_boundaries(int n, int k)
{
  vc<int> a(n);
  map<int, vc<I>> expected;
  for (int i = 0; i < n; ++i)
  {
    a[i] = (i * 17 + i / 3) % k;
    expected[a[i]].push_back(I(i));
  }
  const GroupIndex<int, true, I> grp(a);
  static_assert(is_same_v<decltype(grp.in_cnt(0, 0, 1)), I>);
  const vc<int> boundaries{INT_MIN, -1, 0, 1, 63, 64, 65, n / 2, n - 1, n, n + 1, INT_MAX};
  for (int value = -1; value <= k; ++value)
  {
    const vc<I> &is = expected[value];
    assert(grp.idxs(value).to_v() == is);
    assert(grp.in_cnt(value, 2, 1) == 0);
    for (int p : boundaries)
    {
      I lo = lower_bound(ALL(is), p) - is.begin();
      I hi = upper_bound(ALL(is), p) - is.begin();
      assert(grp.lt_cnt(value, p) == lo && grp.leq_cnt(value, p) == hi);
      assert(grp.geq_cnt(value, p) == I(is.size()) - lo && grp.gt_cnt(value, p) == I(is.size()) - hi);
      assert(grp.lt_max(value, p).value_or(-1) == (lo == 0 ? I(-1) : is[lo - 1]));
      assert(grp.leq_max(value, p).value_or(-1) == (hi == 0 ? I(-1) : is[hi - 1]));
      assert(grp.geq_min(value, p).value_or(n) == (lo == I(is.size()) ? I(n) : is[lo]));
      assert(grp.gt_min(value, p).value_or(n) == (hi == I(is.size()) ? I(n) : is[hi]));
      for (int q : boundaries) if (p <= q)
        assert(grp.in_cnt(value, p, q) == lower_bound(ALL(is), q) - (is.begin() + lo));
    }
  }
}

int main()
{
  for (int n : {1023, 1024, 1025, 1087, 1088})
    for (int k : {1, 2, 8, 16, 17}) check_search_boundaries(n, k);
  check_search_boundaries(2048, 512);
  check_search_boundaries<signed char>(100, 3);
  check_search_boundaries<short>(1024, 3);
  check_search_boundaries<i128>(1024, 3);

  GroupIndex inferred(vstr{"b", "a", "b"});
  static_assert(is_same_v<decltype(inferred), GroupIndex<string>>);
  static_assert(is_same_v<decltype(inferred.lt_max("b", 1)), optional<ll>>);
  GroupIndex<int, true, int> int_indices(vc<int>{2});
  static_assert(is_same_v<decltype(int_indices.lt_max(2, 1)), optional<int>>);

  const vc<int> ids{0, 2, 2, 5};
  GroupIndexRaw raw(ids);
  static_assert(is_same_v<decltype(raw), GroupIndexRaw<int>>);
  static_assert(is_same_v<decltype(raw.lt_max(2, 1)), optional<ll>>);
  assert(raw.size() == 3 && raw.to_csr().size() == 6);
  assert((raw.idxs(2).to_v() == vl{1, 2}));
  GroupIndexRaw raw_ll(vl{2, 0, 2});
  static_assert(is_same_v<decltype(raw_ll), GroupIndexRaw<ll>>);
  assert((raw_ll.idxs(2).to_v() == vl{0, 2}));
  GroupIndexRaw raw_empty(vc<int>{});
  static_assert(is_same_v<decltype(raw_empty), GroupIndexRaw<int>>);
  assert(raw_empty.empty() && raw_empty.begin() == raw_empty.end());

  check_default<true>();
  check_default<false>();
  check<int>({}, {-1, 0, 5});
  check<int, false>({}, {-1, 0, 5});
  check<int>({0, 2, 2, 5}, {-1, 0, 1, 2, 3, 5, 6});
  check<int, false, int>({0, 2, 2, 5}, {-1, 0, 1, 2, 3, 5, 6});
  check<ll>({LLONG_MAX, -3, LLONG_MIN, LLONG_MAX, 10000000000LL},
            {LLONG_MIN, -4, -3, 0, 10000000000LL, LLONG_MAX - 1, LLONG_MAX});
  check<ull>({ULLONG_MAX, 0, ULLONG_MAX}, {0, 1, ULLONG_MAX - 1, ULLONG_MAX});
  check<ull, false>({2, 0, 2, 5}, {0, 1, 2, 5, ULLONG_MAX});
  check<unsigned char, false>({255, 0, 255}, {0, 1, 255});
  check<signed char, false>({127, 0, 127}, {-1, 0, 1, 127});
  check<string>({"pear", "apple", "pear", ""}, {"", "apple", "orange", "pear", "z"});
  check<pair<int, int>>({{2, 3}, {-1, 4}, {2, 3}}, {{-1, 4}, {0, 0}, {2, 3}, {3, 0}});
  check<bool>({true, false, true}, {false, true});

  mt19937 rng(20260930);
  repi(trial, 200)
  {
    vc<int> a(rng() % 13);
    for (auto &value : a) value = rng() % 8;
    const vc<int> queries{-1, 0, 1, 2, 3, 4, 5, 6, 7, 8};
    check<int, true, int>(a, queries);
    check<int, false, int>(a, queries);
    for (auto &value : a) value = value * 100000000 - 500000000;
    check<int>(a, {-500000001, -500000000, -200000000, 0, 200000000, 200000001});
  }

  for (int n : {33, 257, 511, 512, 513, 65535, 65536, 65537})
  {
    vl a(n);
    vc<i128> wide(n);
    repi(i, n)
    {
      a[i] = i % 3 == 0 ? LLONG_MIN : i % 3 == 1 ? LLONG_MAX : ll(rng());
      wide[i] = i128(rng()) * (i128(1) << 80) - (i128(1) << 120);
    }
    check_large(a);
    check_large(wide);
  }
  vc<string> words(2048);
  for (auto &word : words) word = string(30, 'a') + to_string(rng() % 300);
  check_large(words);

  cout << "Hello World" << endl;
}
