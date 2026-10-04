#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/sqrt_decomposition/sqrt_decomposition.hpp"
#include "math/modint/modint.hpp"

using mint = modint998244353;

// Library Checker: Range Affine Range Sum
// 部分ブロックごとに push / rebuild を一度ずつ行う。
template <int B>
struct AffineSum
{
  int n, pushes = 0, rebuilds = 0;
  vc<mint> a, sums, mul, add;
  AffineSum(const vc<mint> &v) : n(v.size()), a(v), sums((n + B - 1) / B),
      mul(sums.size(), 1), add(sums.size())
  {
    for (auto s : sqrt_decomposition_blocks<B>(n, 0, n)) rebuild(s);
    rebuilds = 0;
  }
  void push(const SqrtDecompositionSegment &s)
  {
    pushes++;
    for (int i = s.block_l; i < s.block_r; i++) a[i] = mul[s.b] * a[i] + add[s.b];
    mul[s.b] = 1;
    add[s.b] = 0;
  }
  void rebuild(const SqrtDecompositionSegment &s)
  {
    rebuilds++;
    sums[s.b] = 0;
    for (int i = s.block_l; i < s.block_r; i++) sums[s.b] += a[i];
  }
  void apply(int l, int r, mint x, mint y)
  {
    for (auto s : sqrt_decomposition_blocks<B>(n, l, r))
    {
      if (s.full())
      {
        sums[s.b] = x * sums[s.b] + y * (s.r - s.l);
        mul[s.b] = x * mul[s.b];
        add[s.b] = x * add[s.b] + y;
      }
      else
      {
        push(s);
        for (int i = s.l; i < s.r; i++) a[i] = x * a[i] + y;
        rebuild(s);
      }
    }
  }
  mint sum(int l, int r) const
  {
    mint result = 0;
    for (auto s : sqrt_decomposition_blocks<B>(n, l, r))
    {
      if (s.full()) result += sums[s.b];
      else
        for (int i = s.l; i < s.r; i++) result += mul[s.b] * a[i] + add[s.b];
    }
    return result;
  }
};

// Library Checker: Point Set Range Frequency / AtCoder practice2 J
template <int B>
struct SortedBlocks
{
  int n;
  vc<int> a;
  vvc<int> sorted;
  SortedBlocks(const vc<int> &v) : n(v.size()), a(v), sorted((n + B - 1) / B)
  {
    for (auto s : sqrt_decomposition_blocks<B>(n, 0, n))
    {
      sorted[s.b].assign(a.begin() + s.l, a.begin() + s.r);
      sort(sorted[s.b].begin(), sorted[s.b].end());
    }
  }
  void set(int p, int x)
  {
    auto &v = sorted[p / B];
    v.erase(lower_bound(v.begin(), v.end(), a[p]));
    v.insert(lower_bound(v.begin(), v.end(), x), x);
    a[p] = x;
  }
  int frequency(int l, int r, int x) const
  {
    int result = 0;
    for (auto s : sqrt_decomposition_blocks<B>(n, l, r))
    {
      if (s.full())
      {
        const auto &v = sorted[s.b];
        auto [lo, hi] = equal_range(v.begin(), v.end(), x);
        result += hi - lo;
      }
      else
        for (int i = s.l; i < s.r; i++) result += a[i] == x;
    }
    return result;
  }
  int range_max(int l, int r) const
  {
    int result = numeric_limits<int>::min();
    for (auto s : sqrt_decomposition_blocks<B>(n, l, r))
    {
      if (s.full()) result = max(result, sorted[s.b].back());
      else
        for (int i = s.l; i < s.r; i++) result = max(result, a[i]);
    }
    return result;
  }
  int first_ge(int l, int x) const
  {
    for (auto s : sqrt_decomposition_blocks<B>(n, l, n))
    {
      if (s.full() && sorted[s.b].back() < x) continue;
      // 候補を含む完全ブロックも、必要に応じて要素を走査する。
      for (int i = s.l; i < s.r; i++)
        if (a[i] >= x) return i;
    }
    return n;
  }
};

// Codeforces 455D: Serega and Fun
// 値をブロック内の deque に持ち、右シフトの繰越しを左から右へ渡す。
template <int B>
struct RotateFrequency
{
  int n;
  vc<deque<int>> a;
  vc<unordered_map<int, int>> counts;
  RotateFrequency(const vc<int> &v) : n(v.size()), a((n + B - 1) / B), counts(a.size())
  {
    for (int i = 0; i < n; i++) a[i / B].push_back(v[i]), counts[i / B][v[i]]++;
  }
  void rotate(int l, int r)
  {
    if (l == r) return;
    int carry = a[(r - 1) / B][(r - 1) % B];
    for (auto s : sqrt_decomposition_blocks<B>(n, l, r))
    {
      auto &v = a[s.b];
      auto &freq = counts[s.b];
      if (s.full())
      {
        int last = v.back();
        v.pop_back();
        v.push_front(carry);
        if (--freq[last] == 0) freq.erase(last);
        freq[carry]++;
        carry = last;
      }
      else
      {
        for (int i = s.l; i < s.r; i++) swap(carry, v[i - s.block_l]);
        freq.clear();
        for (int x : v) freq[x]++;
      }
    }
  }
  int frequency(int l, int r, int x) const
  {
    int result = 0;
    for (auto s : sqrt_decomposition_blocks<B>(n, l, r))
    {
      if (s.full())
      {
        auto it = counts[s.b].find(x);
        if (it != counts[s.b].end()) result += it->second;
      }
      else
        for (int i = s.l; i < s.r; i++) result += a[s.b][i - s.block_l] == x;
    }
    return result;
  }
};

// Codeforces 13E: Holes
// 一点の変更でも、その点に依存するブロック内の情報を右から再計算する。
template <int B>
struct Holes
{
  int n;
  vc<int> a, to, last, count;
  Holes(const vc<int> &v) : n(v.size()), a(v), to(n), last(n), count(n)
  {
    for (auto s : sqrt_decomposition_blocks<B>(n, 0, n)) rebuild(s);
  }
  void rebuild(const SqrtDecompositionSegment &s)
  {
    for (int i = s.block_r - 1; i >= s.block_l; i--)
    {
      int j = min(n, i + a[i]);
      if (j >= s.block_r) to[i] = j, last[i] = i, count[i] = 1;
      else to[i] = to[j], last[i] = last[j], count[i] = count[j] + 1;
    }
  }
  void set(int p, int x)
  {
    a[p] = x;
    for (auto s : sqrt_decomposition_blocks<B>(n, p, p + 1)) rebuild(s);
  }
  pair<int, int> query(int p) const
  {
    int end = p, steps = 0;
    while (p < n) end = last[p], steps += count[p], p = to[p];
    return {end, steps};
  }
};

template <int B>
void check_examples(int n)
{
  mt19937 rng(20260929 + n);
  vc<int> a(n), rotated(n), jumps(n);
  vc<mint> affine(n);
  for (int i = 0; i < n; i++)
  {
    a[i] = rotated[i] = rng() % 10;
    affine[i] = rng() % 100;
    jumps[i] = rng() % n + 1;
  }
  AffineSum<B> af(affine);
  SortedBlocks<B> sorted(a);
  RotateFrequency<B> rot(rotated);
  Holes<B> holes(jumps);
  for (int q = 0; q < 1000; q++)
  {
    int l = rng() % (n + 1), r = rng() % (n + 1), x = rng() % 10;
    if (l > r) swap(l, r);
    mint m = rng() % 5, c = rng() % 100;
    const int pushes = af.pushes, rebuilds = af.rebuilds;
    af.apply(l, r, m, c);
    int partials = 0;
    for (auto s : sqrt_decomposition_blocks<B>(n, l, r)) partials += !s.full();
    assert(af.pushes - pushes == partials && af.rebuilds - rebuilds == partials);
    for (int i = l; i < r; i++) affine[i] = m * affine[i] + c;
    rot.rotate(l, r);
    if (l < r) std::rotate(rotated.begin() + l, rotated.begin() + r - 1, rotated.begin() + r);
    if (n > 0)
    {
      int p = rng() % n;
      sorted.set(p, x);
      a[p] = x;
      int jump = rng() % n + 1;
      holes.set(p, jump);
      jumps[p] = jump;
      int start = rng() % n;
      p = start;
      int end = p, steps = 0;
      while (p < n) end = p, steps++, p += jumps[p];
      assert(holes.query(start) == make_pair(end, steps));
    }
    l = rng() % (n + 1), r = rng() % (n + 1);
    if (l > r) swap(l, r);
    mint sum = 0;
    int freq = 0, freq_rot = 0, mx = numeric_limits<int>::min(), first = n;
    for (int i = l; i < r; i++)
    {
      sum += affine[i];
      freq += a[i] == x;
      freq_rot += rotated[i] == x;
      mx = max(mx, a[i]);
    }
    for (int i = l; i < n; i++) if (a[i] >= x) { first = i; break; }
    assert(af.sum(l, r) == sum);
    assert(sorted.frequency(l, r, x) == freq);
    assert(rot.frequency(l, r, x) == freq_rot);
    assert(sorted.range_max(l, r) == mx);
    assert(sorted.first_ge(l, x) == first);
  }
}

void check_samples()
{
  // AtCoder practice2 J のサンプル。
  SortedBlocks<2> sorted({1, 2, 3, 2, 1});
  assert(sorted.range_max(0, 5) == 3);
  assert(sorted.first_ge(1, 3) == 2);
  sorted.set(2, 1);
  assert(sorted.range_max(1, 4) == 2);
  assert(sorted.first_ge(0, 3) == 5);

  // Codeforces 13E のサンプル。
  Holes<3> holes({1, 1, 1, 1, 1, 2, 8, 2});
  assert(holes.query(0) == make_pair(7, 7));
  holes.set(0, 3);
  assert(holes.query(0) == make_pair(7, 5));
  holes.set(2, 4);
  assert(holes.query(1) == make_pair(6, 3));

  // Codeforces 455D のサンプル 1。直前の答えによる復号も含む。
  RotateFrequency<3> rot({6, 6, 2, 7, 4, 2, 5});
  vc<array<int, 4>> qs{{1, 3, 6, 0}, {2, 2, 4, 2}, {2, 2, 4, 7},
      {2, 2, 2, 5}, {1, 2, 6, 0}, {1, 1, 4, 0}, {2, 1, 7, 3}};
  vc<int> answers;
  int last = 0;
  for (auto [t, l, r, k] : qs)
  {
    l = (l + last - 1) % 7;
    r = (r + last - 1) % 7;
    k = (k + last - 1) % 7 + 1;
    if (l > r) swap(l, r);
    if (t == 1) rot.rotate(l, r + 1);
    else answers.push_back(last = rot.frequency(l, r + 1, k));
  }
  assert((answers == vc<int>{2, 1, 0, 0}));
}

// 引数を与えると、保存済み Library Checker データの検証にも使える。
int main(int argc, char **argv)
{
  if (argc == 2)
  {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    if (string(argv[1]) == "affine")
    {
      vc<mint> a(n);
      for (auto &x : a) { int v; cin >> v; x = v; }
      AffineSum<512> ds(a);
      while (q--)
      {
        int t, l, r;
        cin >> t >> l >> r;
        if (t == 0) { int b, c; cin >> b >> c; ds.apply(l, r, b, c); }
        else cout << ds.sum(l, r).val() << '\n';
      }
    }
    else
    {
      assert(string(argv[1]) == "frequency");
      vc<int> a(n);
      for (auto &x : a) cin >> x;
      SortedBlocks<512> ds(a);
      while (q--)
      {
        int t, l, r;
        cin >> t >> l >> r;
        if (t == 0) ds.set(l, r);
        else { int x; cin >> x; cout << ds.frequency(l, r, x) << '\n'; }
      }
    }
    return 0;
  }
  check_samples();
  for (int n : {0, 1, 2, 3, 7, 8, 9, 23, 65, 129})
  {
    check_examples<1>(n);
    check_examples<3>(n);
    check_examples<8>(n);
    check_examples<64>(n);
  }
  PRINT("Hello World");
}
