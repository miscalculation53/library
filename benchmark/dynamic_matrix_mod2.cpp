#include "math/linalg/dynamic_matrix_mod2.hpp"
#include "math/linalg/matrix_mod2.hpp"

volatile ull matrix_mod2_sink = 0;

template <bool Slice>
DynamicMatrixMod2 reduce(const DynamicMatrixMod2 &src, bool rref)
{
  int n = src.size(), m = src.m;
  auto a = src;
  for (int i = 0, j = 0; i < n && j < m; j++)
  {
    int pivot = i;
    while (pivot < n && !a[pivot].test(j))
      pivot++;
    if (pivot == n)
      continue;
    swap(a[i], a[pivot]);
    auto add = [&](int k)
    {
      if (a[k].test(j))
      {
        if constexpr (Slice)
          a[k].xor_slice(j, m, a[i], j);
        else
          a[k] ^= a[i];
      }
    };
    if (rref)
      repi(k, i) add(k);
    repi(k, i + 1, n) add(k);
    i++;
  }
  return a;
}

pair<bool, DynamicMatrixMod2> inv_bitwise(const DynamicMatrixMod2 &src)
{
  int n = src.size();
  DynamicMatrixMod2 a(n, 2 * n);
  repi(i, n)
  {
    repi(j, n) if (src[i].test(j)) a[i].set(j);
    a[i].set(n + i);
  }
  auto b = reduce<false>(a, true);
  repi(i, n) if (!b[i].test(i)) return {false, {}};
  DynamicMatrixMod2 res(n, n);
  repi(i, n) repi(j, n) if (b[i].test(n + j)) res[i].set(j);
  return {true, res};
}

tuple<bool, DynamicBitset, vc<DynamicBitset>> solve_bitwise(const DynamicMatrixMod2 &src, const DynamicBitset &b)
{
  int n = src.size(), m = src.m;
  DynamicMatrixMod2 a(n, m + 1);
  repi(i, n)
  {
    repi(j, m) if (src[i].test(j)) a[i].set(j);
    if (b.test(i))
      a[i].set(m);
  }
  a = reduce<false>(a, true);
  int rk = 0;
  fec(row : a) if (row.any()) rk++;
  DynamicBitset sol(m);
  vc<DynamicBitset> basis;
  vc<int> piv(m, -1);
  for (int i = 0, j = 0; i < rk; i++)
  {
    while (j < m && !a[i].test(j))
      j++;
    if (j == m)
      return {false, {}, {}};
    sol.set(j, a[i].test(m));
    piv[j] = i;
  }
  repi(j, m)
  {
    if (piv[j] != -1)
      continue;
    DynamicBitset v(m);
    v.set(j);
    repi(k, j) if (piv[k] != -1 && a[piv[k]].test(j)) v.set(k);
    basis.eb(move(v));
  }
  return {true, sol, basis};
}

ull checksum(const DynamicMatrixMod2 &a)
{
  ull res = 0;
  fec(row : a) res = res * 101 + row.count();
  return res;
}

ull checksum(const pair<bool, DynamicMatrixMod2> &res)
{
  return res.first + checksum(res.second);
}

ull checksum(const tuple<bool, DynamicBitset, vc<DynamicBitset>> &res)
{
  auto &[ok, sol, basis] = res;
  ull value = ok + sol.count();
  fec(row : basis) value = value * 101 + row.count();
  return value;
}

template <size_t W>
ull checksum(const MatrixMod2<W> &a)
{
  ull res = 0;
  fec(row : a) res = res * 101 + row.count();
  return res;
}

template <size_t W>
ull checksum(const pair<bool, MatrixMod2<W>> &res)
{
  return res.first + checksum(res.second);
}

template <size_t W>
ull checksum(const tuple<bool, bitset<W>, vc<bitset<W>>> &res)
{
  auto &[ok, sol, basis] = res;
  ull value = ok + sol.count();
  fec(row : basis) value = value * 101 + row.count();
  return value;
}

template <class F>
double measure(F f)
{
  auto begin = chrono::steady_clock::now();
  auto result = f();
  auto end = chrono::steady_clock::now();
  matrix_mod2_sink = matrix_mod2_sink ^ checksum(result);
  return chrono::duration<double, milli>(end - begin).count();
}

DynamicMatrixMod2 make_matrix(int n, int m, int prefix, int rows, mt19937_64 &rng)
{
  DynamicMatrixMod2 a(n, m);
  repi(i, min(n, rows)) repi(j, prefix, m) if (rng() & 1) a[i].set(j);
  repi(i, rows, n) a[i] = a[rng() % rows];
  return a;
}

void benchmark_reduce(const string &name, const DynamicMatrixMod2 &a)
{
  constexpr int rounds = 7;
  for (bool rref : {false, true})
  {
    auto expected = reduce<false>(a, rref);
    assert(expected == reduce<true>(a, rref));
    assert(expected == a.row_reduction(rref));
    array<vc<double>, 3> times;
    repi(t, rounds) repi(k, 3)
    {
      int mode = (t + k) % 3;
      if (mode == 0)
        times[mode].eb(measure([&] { return reduce<false>(a, rref); }));
      else if (mode == 1)
        times[mode].eb(measure([&] { return reduce<true>(a, rref); }));
      else
        times[mode].eb(measure([&] { return a.row_reduction(rref); }));
    }
    fem(v : times) sort(ALL(v));
    cout << name << ' ' << (rref ? "RREF" : "REF")
         << " full=" << times[0][rounds / 2]
         << " slice=" << times[1][rounds / 2]
         << " library=" << times[2][rounds / 2] << " ms\n";
  }
}

template <class F, class G>
array<double, 2> compare(F f, G g)
{
  assert(f() == g());
  measure(f);
  measure(g);
  array<vc<double>, 2> times;
  repi(t, 7) repi(k, 2)
  {
    int mode = (t + k) % 2;
    times[mode].eb(mode == 0 ? measure(f) : measure(g));
  }
  fem(v : times) sort(ALL(v));
  return {times[0][3], times[1][3]};
}

template <class F, class G, class H>
array<double, 3> compare_three(F f, G g, H h)
{
  measure(f);
  measure(g);
  measure(h);
  array<vc<double>, 3> times;
  repi(t, 7) repi(k, 3)
  {
    int mode = (t + k) % 3;
    times[mode].eb(mode == 0 ? measure(f) : mode == 1 ? measure(g) : measure(h));
  }
  fem(v : times) sort(ALL(v));
  return {times[0][3], times[1][3], times[2][3]};
}

template <int N, int Capacity = N>
void benchmark_fixed(mt19937_64 &rng)
{
  auto a = make_matrix(N, N, 0, N, rng);
  while (!a.det())
    a = make_matrix(N, N, 0, N, rng);
  MatrixMod2<Capacity> fixed(N, N);
  repi(i, N) repi(j, N) fixed[i][j] = a[i].test(j);
  DynamicBitset b(N);
  bitset<N> fixed_b;
  repi(i, N) if (rng() & 1) b.set(i), fixed_b.set(i);

  auto fi = fixed.inv();
  auto di = a.inv();
  assert(fi.first == di.first && fi.second.template to_vvi<int>() == di.second.to_vvi<int>());
  assert(di == inv_bitwise(a));
  auto [fo, fs, fb] = fixed.solve(fixed_b);
  auto [ok, sol, basis] = a.solve(b);
  assert(fo == ok && fb.size() == basis.size());
  repi(j, N) assert(fs[j] == sol.test(j));
  repi(i, fb.size()) repi(j, N) assert(fb[i][j] == basis[i].test(j));
  assert(a.solve(b) == solve_bitwise(a, b));

  auto print = [&](const string &op, const array<double, 3> &t)
  {
    cout << N << 'x' << N << " capacity=" << Capacity << ' ' << op
         << " fixed=" << t[0] << " dynamic_before=" << t[1]
         << " dynamic_after=" << t[2] << " ms\n";
  };
  print("inv", compare_three([&] { return fixed.inv(); },
                             [&] { return inv_bitwise(a); }, [&] { return a.inv(); }));
  print("solve", compare_three([&] { return fixed.solve(fixed_b); },
                               [&] { return solve_bitwise(a, b); }, [&] { return a.solve(b); }));
  for (bool rref : {false, true})
  {
    assert(fixed.row_reduction(rref).template to_vvi<int>() == a.row_reduction(rref).to_vvi<int>());
    print(rref ? "RREF" : "REF", compare_three([&] { return fixed.row_reduction(rref); },
                                                [&] { return reduce<false>(a, rref); },
                                                [&] { return a.row_reduction(rref); }));
  }
}

int main(int argc, char **argv)
{
  cout << fixed << setprecision(3);
  mt19937_64 rng(20260831);
  if (argc == 2 && string(argv[1]) == "--fixed")
  {
    benchmark_fixed<65>(rng);
    benchmark_fixed<256>(rng);
    benchmark_fixed<1024>(rng);
    benchmark_fixed<2048>(rng);
    benchmark_fixed<4096>(rng);
    rng.seed(20260831);
    benchmark_fixed<65, 4096>(rng);
    benchmark_fixed<256, 4096>(rng);
    benchmark_fixed<1024, 4096>(rng);
    benchmark_fixed<2048, 4096>(rng);
    cerr << "checksum=" << matrix_mod2_sink << '\n';
    return 0;
  }
  for (int n : {256, 1024, 2048})
    benchmark_reduce(to_string(n) + "x" + to_string(n), make_matrix(n, n, 0, n, rng));
  benchmark_reduce("1024x4096", make_matrix(1024, 4096, 0, 1024, rng));
  benchmark_reduce("4096x1024", make_matrix(4096, 1024, 0, 4096, rng));
  benchmark_reduce("1024x1024-prefix512", make_matrix(1024, 1024, 512, 1024, rng));
  benchmark_reduce("1024x1024-rank32", make_matrix(1024, 1024, 0, 32, rng));

  for (int n : {256, 1024, 2048})
  {
    auto a = make_matrix(n, n, 0, n, rng);
    while (!a.det())
      a = make_matrix(n, n, 0, n, rng);
    DynamicBitset b(n);
    repi(i, n) if (rng() & 1) b.set(i);
    auto ti = compare([&] { return inv_bitwise(a); }, [&] { return a.inv(); });
    auto ts = compare([&] { return solve_bitwise(a, b); }, [&] { return a.solve(b); });
    cout << n << "x" << n << " inv bitwise=" << ti[0] << " slice=" << ti[1]
         << " solve bitwise=" << ts[0] << " slice=" << ts[1] << " ms\n";
  }
  cerr << "checksum=" << matrix_mod2_sink << '\n';
}
