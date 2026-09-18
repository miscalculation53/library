// g++-15 -std=c++20 -O2 -I. benchmark/bbla_profile.cpp -o /tmp/bbla-profile
// /tmp/bbla-profile
// Krylov 列生成と BM の時間を分離して測る。Half-GCD 自体の計測ではない。
#include "math/linalg/bbla.hpp"
#include "ds/csr.hpp"

using mint = modint998244353;
using F = FieldAddSubMulDiv<mint>;
using V = vc<mint>;
volatile ull bbla_profile_sink = 0;

template <class Map>
void profile(const string &name, int n, const Map &map)
{
  mt.seed(12345);
  V u(n), v(n);
  repi(i, n) u[i] = randrange(1, mint::mod()), v[i] = randrange(1, mint::mod());
  vc<double> sequence_times, bm_times, total_times;
  int degree = 0;
  for (int rep = -1; rep < 5; rep++)
  {
    auto t0 = chrono::steady_clock::now();
    auto a = bbla::krylov_sequence<F>(map, u, v, 2 * n);
    auto t1 = chrono::steady_clock::now();
    auto c = berlekamp_massey<F>(a);
    auto t2 = chrono::steady_clock::now();
    for (auto x : c) bbla_profile_sink = bbla_profile_sink * 1000003 + x.val();
    degree = int(c.size()) - 1;
    if (rep >= 0)
    {
      sequence_times.eb(chrono::duration<double, milli>(t1 - t0).count());
      bm_times.eb(chrono::duration<double, milli>(t2 - t1).count());
      total_times.eb(chrono::duration<double, milli>(t2 - t0).count());
    }
  }
  sort(sequence_times.begin(), sequence_times.end());
  sort(bm_times.begin(), bm_times.end());
  sort(total_times.begin(), total_times.end());
  double s = sequence_times[2], b = bm_times[2];
  cout << name << ',' << n << ',' << degree << ',' << s << ',' << b << ','
       << total_times[2] << ',' << 100 * b / (s + b) << ',' << (s + b) / s << endl;
}

int main()
{
  cout << fixed << setprecision(3)
       << "case,n,degree,sequence_ms,bm_ms,total_ms,bm_percent,max_speedup_if_bm_free\n";
  for (int n : {4000, 8000})
  {
    mt.seed(6789);
    V diag(n), upper(n);
    repi(i, n) diag[i] = randrange(1, mint::mod()), upper[i] = randrange(1, mint::mod());
    auto bidiagonal = [&](V &v)
    {
      repi(i, n) v[i] = diag[i] * v[i] + (i + 1 < n ? upper[i] * v[i + 1] : mint(0));
    };
    profile("bidiagonal", n, bidiagonal);

    mt.seed(23456);
    vc<pair<int, pair<int, mint>>> entries;
    repi(i, n) repi(j, 4)
      entries.eb(i, pair{int(randrange(0, n)), mint(randrange(1, mint::mod()))});
    CSR<pair<int, mint>> a(n, entries);
    V buffer(n);
    auto sparse = [&](V &v)
    {
      repi(i, n)
      {
        mint sum = 0;
        for (auto [j, x] : a[i]) sum += x * v[j];
        buffer[i] = sum;
      }
      v.swap(buffer);
    };
    profile("sparse_4n", n, sparse);

    auto shift = [](V &v) { rotate(v.begin(), v.begin() + 1, v.end()); };
    profile("cyclic_shift", n, shift);
  }
}
