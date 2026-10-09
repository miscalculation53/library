"""Create benchmark-only FPS variants from the frozen original implementation."""
from pathlib import Path
import hashlib

root = Path(__file__).resolve().parents[1]
source = (root / "benchmark/fps_constant_factor_source.txt").read_text()
s = source.replace('../../template/', 'template/').replace('../modint/', 'math/modint/').replace('../convolution/', 'math/convolution/')
s = s.replace('template <class mint>\nstruct FormalPowerSeries', 'template <class mint, int optimization = 0>\nstruct AuditFPS')
s = s.replace('FormalPowerSeries', 'AuditFPS')
s = s.replace('  using F = AuditFPS;', '''  using F = AuditFPS;
  // 1: move vector results; 2: limit exp temporaries; 4: sparse reduction;
  // 8: arithmetic result copies; 16: sparse log; 32: naive seed for inv;
  // 64: high-half sqrt update; 128: reuse exp transform prefix.
  inline static int inv_cutoff = 200, div_cutoff = 200, sqrt_cutoff = 200;
  inline static int exp_ntt_cutoff = 320, exp_crt_cutoff = 3000;
  inline static int pow_ntt_cutoff = 100, pow_crt_cutoff = 1300;''')
s = s.replace('  AuditFPS(const vc<mint> &f) : vc<mint>(f) {}', '''  AuditFPS(const vc<mint> &f) : vc<mint>(f) {}
  AuditFPS(vc<mint> &&f) requires ((optimization & 1) != 0) : vc<mint>(std::move(f)) {}''')
s = s.replace('if (cnt_nz() <= 200)\n      return F{1}', 'if (cnt_nz() <= inv_cutoff)\n      return F{1}')
s = s.replace('if (g.cnt_nz() <= 200)', 'if (g.cnt_nz() <= div_cutoff)')
s = s.replace('if (cnt_nz() <= 320)', 'if (cnt_nz() <= exp_ntt_cutoff)')
s = s.replace('if (cnt_nz() <= 3000)', 'if (cnt_nz() <= exp_crt_cutoff)')
s = s.replace('if (cnt_nz() <= 100)', 'if (cnt_nz() <= pow_ntt_cutoff)')
s = s.replace('if (cnt_nz() <= 1300)', 'if (cnt_nz() <= pow_crt_cutoff)')
s = s.replace('if (cnt_nz() <= 200)\n      return sqrt_sparse', 'if (cnt_nz() <= sqrt_cutoff)\n      return sqrt_sparse')
s = s.replace('q = diff(), q.resize(2 * m), fill(q.begin() + m - 1, q.end(), 0);', '''if constexpr (optimization & 2)
        {
          q.assign(2 * m, 0);
          repi(i, 1, min(m, sz())) q[i - 1] = (*this)[i] * i;
        }
        else q = diff(), q.resize(2 * m), fill(q.begin() + m - 1, q.end(), 0);''')
s = s.replace('h = *this, h.resize(2 * m), s.resize(2 * m);', '''if constexpr (optimization & 2) h = resized(2 * m);
        else h = *this, h.resize(2 * m);
        s.resize(2 * m);''')
s = s.replace('f3 = f, ntt(f3);', '''if constexpr (optimization & 128) f3.assign(f2.begin(), f2.begin() + m);
        else f3 = f, ntt(f3);''')
s = s.replace('''    for (int m = 1; m < n - d0 / 2; m *= 2)
      g = (g + f.resized(2 * m) * g.inv(2 * m)).resized(2 * m) * i2;''', '''    for (int m = 1; m < n - d0 / 2; m *= 2)
    {
      if constexpr (optimization & 64)
      {
        F error = (f.resized(2 * m) - (g * g).resized(2 * m)) >> m;
        F correction = (error * g.inv(m)).resized(m) * i2;
        g.insert(g.end(), correction.begin(), correction.end());
      }
      else g = (g + f.resized(2 * m) * g.inv(2 * m)).resized(2 * m) * i2;
    }''')
for operator in ('*', '+', '-'):
    arg = 'const mint &k' if operator == '*' else 'const F &g'
    var = 'k' if operator == '*' else 'g'
    old = f'F operator{operator}({arg}) const {{ return F(*this) {operator}= {var}; }}'
    new = f'''F operator{operator}({arg}) const
  {{
    if constexpr (optimization & 8) {{ F res(*this); res {operator}= {var}; return res; }}
    else return F(*this) {operator}= {var};
  }}'''
    assert old in s
    s = s.replace(old, new)

s = s.replace('    auto gnz = g.nz();\n    resize(n);', '''    auto gnz = g.nz();
    resize(n);
    if constexpr ((optimization & 4) && internal::ordinary_mod32<mint>::value)
    {
      vc<pair<int, mint>> terms;
      for (auto [j, b] : gnz) if (j > 0 && j < n) terms.eb(j, b);
      const ull mod = mint::mod();
      const int block = mod <= (1U << 30) ? 16 : 4;
      repi(i, n)
      {
        ull sum = 0;
        int used = 0;
        for (const auto &[j, b] : terms)
        {
          if (j > i) break;
          sum += ull((*this)[i - j].val()) * b.val();
          if (++used == block) sum %= mod, used = 0;
        }
        (*this)[i] = ((*this)[i] - mint::raw(sum % mod)) * iv;
      }
      return *this;
    }''')
s = s.replace('    auto anz = a.nz(), bnz = b.nz();\n    repi(k, d - 1)', '''    auto anz = a.nz(), bnz = b.nz();
    if constexpr ((optimization & 4) && internal::ordinary_mod32<mint>::value)
    {
      vc<mint> df(d);
      const ull mod = mint::mod();
      const int block = mod <= (1U << 30) ? 16 : 4;
      Binomial<mint>::reserve(d - 1);
      repi(t, 1, d)
      {
        ull sum = 0;
        int used = 0;
        for (const auto &[i, ai] : anz)
        {
          if (i == 0) continue;
          if (i > t) break;
          sum += ull(ai.val()) * df[t - i].val();
          if (++used == block) sum %= mod, used = 0;
        }
        for (const auto &[j, bj] : bnz)
        {
          if (j >= t) break;
          sum += ull(bj.val()) * f[t - 1 - j].val();
          if (++used == block) sum %= mod, used = 0;
        }
        f[t] = -mint::raw(sum % mod) * Binomial<mint>::inv_[t];
        df[t] = mint(t) * f[t];
      }
      return f;
    }
    repi(k, d - 1)''')
s = s.replace('    F f = resized(n);\n    return (f.diff()', '''    F f = resized(n);
    if constexpr (optimization & 16)
      if (f.cnt_nz() <= 200) return f.diff().div_sparse(f, n - 1).integ();
    return (f.diff()''')
s = s.replace('    F f, g2, g{front().inv()};\n    for (int m = 1;', '''    F f, g2, g{front().inv()};
    if constexpr (optimization & 32)
      g = F{1}.div_sparse(*this, min(n, 32));
    for (int m = g.sz();''')
(root / 'benchmark/fps_constant_factor_variants.hpp').write_text(
    '// Benchmark snapshot SHA-256: ' + hashlib.sha256(source.encode()).hexdigest() + '\n'
    '// Regenerate using benchmark/fps_constant_factor_generate.py.\n' + s)

test = (root / 'verify/mytest/ai/fps.test.cpp').read_text()
test = test.replace('#include "math/fps/fps.hpp"', '#include "fps_constant_factor_variants.hpp"')
test = test.replace('FormalPowerSeries<mint>', 'AuditFPS<mint, 207>')
test = test.replace('  cout << "Hello World"', '''  test_common_operations<static_modint32<1811939329>>(512, 256);
  using dynamic_mint = dynamic_modint32<33209>;
  dynamic_mint::set_mod(998244353);
  test_common_operations<dynamic_mint>(3072, 1344);
  cout << "Hello World"''')
(root / 'benchmark/fps_constant_factor_check.cpp').write_text(
    '// Reuses verify/mytest/ai/fps.test.cpp with the combined experimental variant.\n'
    '// Generated using fps_constant_factor_generate.py.\n' + test)
