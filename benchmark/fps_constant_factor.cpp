// g++-15 -std=c++20 -O2 -DNDEBUG -I. benchmark/fps_constant_factor.cpp -o /tmp/fps-audit
#include "fps_constant_factor_variants.hpp"
#include "math/fps/fps.hpp"
#include "math/fps/multipoint_evaluation.hpp"
#include "math/fps/interpolation.hpp"
#include "math/fps/fps_2d.hpp"
#include "fps_algorithm_candidates.hpp"
#include "fps_baseline.hpp"

template <class mint>
vc<mint> baseline_convolution_naive(const vc<mint> &a, const vc<mint> &b)
{
  const int n = a.size(), m = b.size();
  const int cnta = n - count(ALL(a), 0), cntb = m - count(ALL(b), 0);
  vc<mint> c(n + m - 1);
  if constexpr (internal::dot_product_mod32_value<mint>::value)
  {
    // 零を飛ばす方が有利な入力や短い積は、従来のループを使う。
    if (min(n, m) >= 16 && ll(cnta) * 2 >= n && ll(cntb) * 2 >= m)
    {
      repi(p, n + m - 1) c[p] = convolution_point_get(a, b, p);
      return c;
    }
  }
  if ((ll)m * cnta > (ll)n * cntb)
  {
    repi(j, m)
    {
      if (b[j] == 0)
        continue;
      repi(i, n) c[i + j] += a[i] * b[j];
    }
  }
  else
  {
    repi(i, n)
    {
      if (a[i] == 0)
        continue;
      repi(j, m) c[i + j] += a[i] * b[j];
    }
  }
  return c;
}


using Clock = chrono::steady_clock;
volatile ull fps_audit_sink = 0;
int rounds = 5;

template <class F> ull checksum(const F &f)
{
  ull res = 0;
  for (const auto &x : f) res = res * 1000000007 + x.val();
  return res;
}
template <class F, class G> void equal_result(const F &f, const G &g)
{
  if (f.size() != g.size() || !equal(f.begin(), f.end(), g.begin()))
    throw runtime_error("coefficient mismatch");
}
template <class Fn> double elapsed(const Fn &fn, int iterations)
{
  decltype(fn()) result;
  auto start = Clock::now();
  for (int i = 0; i < iterations; ++i) result = fn();
  double us = chrono::duration<double, micro>(Clock::now() - start).count() / iterations;
  fps_audit_sink = checksum(result);
  return us;
}
template <class A, class B>
void measure(string suite, int mod, string op, int n, int s, string layout,
             string name_a, string name_b, string current, const A &a, const B &b)
{
  auto expected = a();
  equal_result(expected, b());
  double warm_a = elapsed(a, 1), warm_b = elapsed(b, 1);
  int iters[2];
  iters[0] = iters[1] = clamp(int(2000 / max({warm_a, warm_b, 1.0})), 1, 256);
  // Special powers can differ by several orders of magnitude. Batch each
  // side separately so the faster result exceeds the clock resolution.
  if (suite == "extra")
  {
    iters[0] = clamp(int(2000 / max(warm_a, 1.0)), 1, 256);
    iters[1] = clamp(int(2000 / max(warm_b, 1.0)), 1, 256);
  }
  vc<double> times[2];
  for (int r = 0; r < rounds; ++r)
  {
    for (int j = 0; j < 2; ++j)
    {
      int who = (r + j) % 2;
      times[who].push_back(who == 0 ? elapsed(a, iters[0]) : elapsed(b, iters[1]));
    }
  }
  for (auto &v : times) sort(v.begin(), v.end());
  cout << suite << ',' << mod << ',' << op << ',' << n << ',' << s << ',' << layout
       << ',' << name_a << ',' << name_b << ',' << current << ',' << iters[0];
  if (iters[0] != iters[1]) cout << '/' << iters[1];
  cout << ',' << rounds;
  for (const auto &v : times) cout << ',' << v[v.size() / 2] << ',' << v.front() << ',' << v.back();
  cout << ',' << times[0][times[0].size()/2] / times[1][times[1].size()/2] << '\n';
  cout.flush();
}

template <class mint>
vc<mint> input(int length, int support_n, int s, string layout, int seed = 1, bool exp = false)
{
  mt19937 rng(seed);
  vc<mint> f(length);
  const int first = exp ? 1 : 0;
  if (!exp) f[0] = 1;
  const int terms = exp ? s : s - 1;
  vc<int> pos;
  for (int j = 1; j < support_n; ++j) pos.push_back(j);
  if (layout == "spread") shuffle(pos.begin(), pos.end(), rng);
  if (layout == "suffix") reverse(pos.begin(), pos.end());
  for (int j = 0; j < terms; ++j) f[pos[j]] = mint(1 + rng() % (mint::mod() - 1));
  (void)first;
  return f;
}

template <class F>
F operation(string op, const F &f, const F &numerator, int n, string path)
{
  if (path == "sparse")
  {
    if (op == "inv") return F{1}.div_sparse(f, n);
    if (op == "div") return numerator.div_sparse(f, n);
    if (op == "exp") return f.exp_sparse(n);
    if (op == "pow") return f.pow_sparse(1234567, n);
    if (op == "sqrt") return f.sqrt_sparse(n).second;
  }
  int saved = 0;
  int *cutoff = nullptr;
  if (op == "inv") cutoff = &F::inv_cutoff;
  if (op == "div") cutoff = &F::div_cutoff;
  if (op == "exp") cutoff = ntt_ok<typename F::value_type>(2 * n) ? &F::exp_ntt_cutoff : &F::exp_crt_cutoff;
  if (op == "pow") cutoff = ntt_ok<typename F::value_type>(2 * n) ? &F::pow_ntt_cutoff : &F::pow_crt_cutoff;
  if (op == "sqrt") cutoff = &F::sqrt_cutoff;
  if (cutoff) saved = *cutoff, *cutoff = path == "dense" ? -1 : saved;
  F res;
  if (op == "inv") res = f.inv(n);
  if (op == "div") res = numerator.div(f, n);
  if (op == "exp") res = f.exp(n);
  if (op == "pow") res = f.pow(1234567, n);
  if (op == "sqrt") res = f.sqrt(n).second;
  if (op == "log") res = f.log(n);
  if (op == "mul") res = f * numerator;
  if (cutoff) *cutoff = saved;
  return res;
}

template <class mint>
int threshold(string op, int n)
{
  if (op == "inv" || op == "div" || op == "sqrt") return 200;
  if (op == "exp") return ntt_ok<mint>(2 * n) ? 320 : 3000;
  return ntt_ok<mint>(2 * n) ? 100 : 1300;
}
template <class mint>
void thresholds(int max_n, bool optimized)
{
  using Ref = AuditFPS<mint, 0>;
  using Opt = AuditFPS<mint, 15>;
  for (int n : {256, 1024, 4096, 16384, 65536})
  {
    if (n > max_n) continue;
    for (string layout : {"prefix", "spread", "suffix"})
    {
      for (int s : {8, 32, 64, 100, 128, 200, 256, 320, 512, 800, 1300, 2000, 3000})
      {
        if (s >= n || (n >= 65536 && s > 512)) continue;
        auto num = input<mint>(n, n, n, "prefix", 19);
        for (string op : {"inv", "div", "exp", "pow", "sqrt"})
        {
          auto v = input<mint>(n, n, s, layout, 7, op == "exp");
          Ref f(v), numerator(num);
          auto current = s <= threshold<mint>(op, n) ? "sparse" : "dense";
          if (!optimized)
            measure("threshold", mint::mod(), op, n, s, layout, "sparse", "dense", current,
              [&]{ return operation(op, f, numerator, n, "sparse"); },
              [&]{ return operation(op, f, numerator, n, "dense"); });
          else
          {
            Opt g(v), numerator2(num);
            measure("threshold_optimized", mint::mod(), op, n, s, layout, "sparse", "dense", current,
              [&]{ return operation(op, g, numerator2, n, "sparse"); },
              [&]{ return operation(op, g, numerator2, n, "dense"); });
          }
        }
      }
    }
  }
}

template <class mint, int bits>
void optimization(string name, string op, int n, int s, string layout, string path = "auto", int length = -1)
{
  if (length < 0) length = n;
  auto v = input<mint>(length, n, s, layout, 7, op == "exp");
  auto num = input<mint>(n, n, n, "prefix", 19);
  AuditFPS<mint, 0> f(v), numerator(num);
  AuditFPS<mint, bits> g(v), numerator2(num);
  measure("optimization", mint::mod(), op, n, s, layout, "reference", name, path,
    [&]{ return operation(op, f, numerator, n, path); },
    [&]{ return operation(op, g, numerator2, n, path); });
}
template <class mint>
void optimizations(int max_n)
{
  for (int n : {256, 1024, 4096, 16384, 65536})
  {
    if (n > max_n) continue;
    for (string op : {"inv", "div", "exp", "pow", "sqrt", "log", "mul"})
    {
      optimization<mint, 1>("move", op, n, n - 1, "prefix");
      optimization<mint, 8>("arithmetic_return", op, n, n - 1, "prefix");
      optimization<mint, 15>("combined", op, n, n - 1, "prefix");
    }
    optimization<mint, 2>("exp_prefix", "exp", n, n - 1, "prefix", "dense");
    optimization<mint, 128>("exp_reuse_ntt", "exp", n, n - 1, "prefix", "dense");
    optimization<mint, 131>("exp_prefix_reuse_move", "exp", n, n - 1, "prefix", "dense");
    optimization<mint, 64>("sqrt_high_half", "sqrt", n, n - 1, "prefix", "dense");
    optimization<mint, 32>("inv_seed32", "inv", n, n - 1, "prefix", "dense");
    for (string layout : {"prefix", "spread", "suffix"})
      for (int s : {32, 128, 320})
      {
        if (s >= n) continue;
        for (string op : {"inv", "div", "exp", "pow", "sqrt"})
          optimization<mint, 4>("sparse_reduction", op, n, s, layout, "sparse");
        optimization<mint, 16>("sparse_log", "log", n, min(s, 200), layout);
      }
  }
  // n can be far smaller than the stored input length.
  if (max_n >= 65536)
    optimization<mint, 2>("exp_prefix_long_input", "exp", 1024, 1023, "prefix", "dense", 1 << 20);
}

template <class mint>
void convolutions(int max_n)
{
  for (int n : {64, 128, 256, 1024, 4096, 16384, 65536})
  {
    if (n > max_n) continue;
    for (int m : {64, n})
      for (int s : {8, 32, 60, 100, 200, 300, 512})
      {
        if (s > min(n, m)) continue;
        auto a = input<mint>(n, n, s, "spread", 7);
        auto b = input<mint>(m, m, m, "prefix", 19);
        auto dense = [&]() -> vc<mint>
        {
          if (ntt_ok<mint>(n + m - 1)) return internal::convolution_ntt(a, b);
          vc<ll> aa(n), bb(m);
          repi(i, n) aa[i] = a[i].val();
          repi(i, m) bb[i] = b[i].val();
          return internal::convolution_crt_mod<mint, 469762049, 1811939329, 2013265921>(aa, bb);
        };
        measure("convolution", mint::mod(), "mul", n, s, to_string(m), "naive", "ntt_crt",
          s <= (ntt_ok<mint>(n + m - 1) ? 60 : 300) ? "naive" : "ntt_crt",
          [&]{ return internal::convolution_naive(a, b); }, dense);
      }
  }
}

template <class mint>
void validation()
{
  using Real = FormalPowerSeries<mint>;
  for (int n : {1, 7, 127, 384, 1025})
  {
    auto v = input<mint>(n, n, n, "prefix", 7);
    auto e = v; e[0] = 0;
    Real real(v), real_e(e);
    AuditFPS<mint, 0> ref(v), ref_e(e);
    equal_result(real.inv(n), ref.inv(n));
    equal_result(real.log(n), ref.log(n));
    equal_result(real.pow(1234567, n), ref.pow(1234567, n));
    equal_result(real.sqrt(n).second, ref.sqrt(n).second);
    equal_result(real_e.exp(n), ref_e.exp(n));
  }
}

template <class mint>
void extras(int max_n)
{
  using F = BaselineFPS<mint>;
  auto flat = [](const auto &rows)
  {
    vc<mint> v;
    for (const auto &row : rows) v.insert(v.end(), row.begin(), row.end());
    return v;
  };
  for (int n : {1024, 4096, 16384, 65536})
  {
    if (n > max_n) continue;
    F f(input<mint>(n, n, n, "prefix", 7));
    for (int s : {32, n})
    {
      F base(input<mint>(n, n, s, "prefix", 7));
      for (ll k : {-1, 0, 1, 2})
        measure("extra", mint::mod(), "pow_special", n, s, to_string(k), "reference", "special_case", "auto",
          [&]{ return base.pow(k, n); }, [&]() -> F
          {
            if (k == -1) return base.inv(n);
            if (k == 0) return F{1}.resized(n);
            if (k == 1) return base.resized(n);
            return (base * base).resized(n);
          });
    }
    for (int k : {1, 8, 64})
    {
      F g(input<mint>(n - k + 1, n - k + 1, n - k + 1, "prefix", 19));
      measure("extra", mint::mod(), "div_poly", n, k, "dense", "reference", "reverse_prefix", "auto",
        [&]{ return f.div_poly(g); }, [&]() -> F
        {
          F a(k), b(min(k, g.sz()));
          copy_n(f.rbegin(), k, a.begin());
          copy_n(g.rbegin(), b.sz(), b.begin());
          return (a * b.inv(k)).resized(k).rev();
        });
    }
    for (int s : {8, 32, 60})
    {
      auto a = input<mint>(n, n, s, "spread", 7);
      auto b = input<mint>(n, n, s, "spread", 19);
      measure("extra", mint::mod(), "mul_both_sparse", n, s, "spread", "reference", "nonzero_pairs", "auto",
        [&]{ return baseline_convolution_naive(a, b); }, [&]
        {
          vc<pair<int, mint>> an, bn;
          repi(i, n) { if (a[i] != 0) an.eb(i, a[i]); if (b[i] != 0) bn.eb(i, b[i]); }
          vc<mint> c(2 * n - 1);
          for (const auto &[i, x] : an) for (const auto &[j, y] : bn) c[i + j] += x * y;
          return c;
        });
    }
    if (ntt_ok<mint>(2 * n))
    {
      F p = f, q(input<mint>(n, n, n, "prefix", 19));
      F r(input<mint>(n, n, n, "prefix", 23)), s(input<mint>(n, n, n, "prefix", 29));
      measure("extra", mint::mod(), "rational_plus", n, n, "equal_lengths", "reference", "sum_in_frequency", "auto",
        [&]
        {
          auto [a, b] = baseline_rational_plus<mint>({p, q}, {r, s});
          a.insert(a.end(), b.begin(), b.end());
          return a;
        }, [&]
        {
          int z = bit_ceil(2 * n - 1);
          F pp = p, qq = q, rr = r, ss = s;
          pp.resize(z), qq.resize(z), rr.resize(z), ss.resize(z);
          ntt(pp), ntt(qq), ntt(rr), ntt(ss);
          repi(i, z) pp[i] = pp[i] * ss[i] + qq[i] * rr[i], qq[i] *= ss[i];
          intt(pp), intt(qq);
          mint iz = mint(z).inv();
          pp *= iz, qq *= iz;
          pp.resize(2 * n - 1), qq.resize(2 * n - 1);
          pp.insert(pp.end(), qq.begin(), qq.end());
          return pp;
        });
    }
  }
  for (int n : {4095, 4096, 4097})
  {
    if (n > max_n) continue;
    using Ref = AuditFPS<mint, 0>;
    Ref num(input<mint>(n, n, n, "prefix", 19));
    for (string op : {"inv", "exp", "pow", "sqrt"})
    {
      int s = threshold<mint>(op, n);
      if (s >= n) continue;
      Ref f(input<mint>(n, n, s, "prefix", 7, op == "exp"));
      measure("extra", mint::mod(), "boundary_" + op, n, s, "prefix", "sparse", "dense", "sparse",
        [&]{ return operation(op, f, num, n, "sparse"); },
        [&]{ return operation(op, f, num, n, "dense"); });
    }
  }
  for (int n : {512, 4096, 16384})
  {
    if (n > max_n) continue;
    F f(input<mint>(n, n, n, "prefix", 7));
    vc<mint> xs(n);
    repi(i, n) xs[i] = i + 2;
    for (int group_log : {3, 4, 5, 7, 8})
      measure("extra", mint::mod(), "multipoint", n, 1 << group_log, "dense", "group64", "group" + to_string(1 << group_log), "auto",
        [&]{ return baseline_multipoint_evaluation(f, xs); }, [&]
        {
          int m = bit_ceil(n), h = min(group_log, int(bit_width(n)) - 1);
          vc<F> node(2 * m, {1});
          repi(i, n) node[m + i] = {-xs[i], 1};
          repi(i, m - 1, 0, -1) node[i] = convolution(node[2 * i], node[2 * i + 1]);
          node[1] = f % node[1];
          repi(i, 2, m >> (h - 1)) node[i] = node[i / 2] % node[i];
          vc<mint> res(n);
          repi(i, n) res[i] = node[(m + i) >> h].eval(xs[i]);
          return res;
        });
  }
  using F2 = BaselineFPS2D<mint>;
  for (auto [h, w] : vc<pair<int,int>>{{16,16}, {32,32}, {64,64}, {128,128}, {8,128}, {128,8}})
  {
    if (h * w > max_n) continue;
    F2 f(h, w);
    mt19937 rng(7);
    repi(i, h) repi(j, w) f[i][j] = 1 + rng() % (mint::mod() - 1);
    measure("extra", mint::mod(), "inv_2d", h, w, "dense", "total_degree", "double_x", "auto",
      [&]{ return flat(f.inv(h, w)); }, [&]
      {
        F2 g{F(f[0]).inv(w)};
        for (int m = 1; m < h; m *= 2)
        {
          int nh = min(h, 2 * m);
          F2 error = -f.mul(g, nh, w);
          error[0][0] += 2;
          g = g.mul(error, nh, w);
        }
        return flat(g.resized(h, w));
      });
  }
}

int main(int argc, char **argv)
{
  string suite = argc > 1 ? argv[1] : "optimization";
  int mod = argc > 2 ? stoi(argv[2]) : 998244353;
  int max_n = argc > 3 ? stoi(argv[3]) : 65536;
  rounds = argc > 4 ? stoi(argv[4]) : 5;
  if (rounds < 1) throw runtime_error("rounds must be positive");
  cout << fixed << setprecision(3);
  cout << "suite,mod,op,n,s,layout,a,b,current,iterations,rounds,a_median_us,a_min_us,a_max_us,b_median_us,b_min_us,b_max_us,a_over_b\n";
  auto run = [&]<class mint>()
  {
    validation<mint>();
    // Warm caches; cache creation is excluded from steady-state comparisons.
    Binomial<mint>::reserve(max(max_n, 2048));
    if (suite == "threshold") thresholds<mint>(max_n, false);
    else if (suite == "threshold_optimized") thresholds<mint>(max_n, true);
    else if (suite == "convolution") convolutions<mint>(max_n);
    else if (suite == "extra") extras<mint>(max_n);
    else optimizations<mint>(max_n);
  };
  if (mod == 998244353) run.template operator()<modint998244353>();
  else if (mod == 1000000007) run.template operator()<modint1000000007>();
  else throw runtime_error("unsupported modulus");
}
