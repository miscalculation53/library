#pragma once
// Frozen baseline dependencies from fb6134ffafb4ffdc66daf858831aef1c64532512.
#include "math/modint/inv_many.hpp"
#include "bit/bit_reverse.hpp"
template<class M> using BaselineFPS=AuditFPS<M,0>;
#define ntt fps_algorithm_audit::forward
#define intt fps_algorithm_audit::backward
#define convolution fps_algorithm_audit::convolve






/**
 * @brief 多項式の多点評価
 * @docs docs/math/fps/baseline_multipoint_evaluation.md
 */

template <class mint>
vc<mint> baseline_multipoint_evaluation(const BaselineFPS<mint> &f, const vc<mint> &xs)
{
  using F = BaselineFPS<mint>;
  const int m0 = xs.size();
  if (m0 == 0)
    return {};
  if (m0 == 1)
    return {f.eval(xs[0])};
  const int m = bit_ceil(m0), h = min(6, (int)bit_width(m0) - 1);
  vc<F> node(2 * m, {1});
  repi(i, m0) node[m + i] = {-xs[i], 1};
  repi(i, m - 1, 0, -1) node[i] = convolution(node[2 * i], node[2 * i + 1]);
  node[1] = f % node[1];
  repi(i, 2, m >> (h - 1)) node[i] = node[i / 2] % node[i];
  vc<mint> res(m0);
  repi(i, m0) res[i] = node[(m + i) >> h].eval(xs[i]);
  return res;
}






/**
 * @brief 多項式の総積
 * @docs docs/math/convolution/baseline_convolution_many.md
 */

// F は vc<mint> または fps
// d は返ってくる vector の最大長さ
// 計算量: 次数の総和を n として O(n log^2 n)
template <class F>
F baseline_convolution_many(const vc<F> &fs, int d = -1)
{
  auto pre = [&](F f) -> F
  {
    if (d >= 0 && d < (int)f.size())
      f.resize(d);
    return f;
  };
  auto dc = [&](auto dc, int l, int r) -> F
  {
    if (r - l == 0)
      return pre(F{1});
    if (r - l == 1)
      return pre(fs[l]);
    const int m = (l + r) / 2;
    F f = convolution(dc(dc, l, m), dc(dc, m, r));
    return pre(move(f));
  };
  return dc(dc, 0, fs.size());
}






/**
 * @brief 有理式の総和
 * @docs docs/math/fps/baseline_rational_sum.md
 */

template <class mint>
pair<BaselineFPS<mint>, BaselineFPS<mint>> baseline_rational_plus
(
  const pair<BaselineFPS<mint>, BaselineFPS<mint>> &f,
  const pair<BaselineFPS<mint>, BaselineFPS<mint>> &g
)
{
  using F = BaselineFPS<mint>;
  cauto &[p_, q_] = f;
  cauto &[r_, s_] = g;
  const int k = p_.size(), l = q_.size(), m = r_.size(), n = s_.size();
  const int pz = bit_ceil(k + n - 1), rz = bit_ceil(l + m - 1);
  const int z = bit_ceil(max({k + n - 1, l + m - 1, l + n - 1}));
  // (ps + qr) / qs
  if (!ntt_ok<mint>(z) || min({p_.cnt_nz(), q_.cnt_nz(), r_.cnt_nz(), s_.cnt_nz()}) <= 60)
    return {p_ * s_ + q_ * r_, q_ * s_};
  // NTT を使い回す
  F p = p_, q = q_, r = r_, s = s_;
  p.resize(pz), q.resize(z), r.resize(rz), s.resize(z);
  ntt(p), ntt(q), ntt(r), ntt(s);
  repi(i, pz) p[i] *= s[i];
  repi(i, rz) r[i] *= q[i];
  repi(i, z) q[i] *= s[i];
  intt(p), intt(r), intt(q);
  mint ipz = mint(pz).inv(), irz = mint(rz).inv(), iz = mint(z).inv();
  repi(i, pz) p[i] *= ipz;
  repi(i, rz) r[i] *= irz;
  repi(i, z) q[i] *= iz;
  return {p.resized(k + n - 1) + r.resized(l + m - 1), q.resized(l + n - 1)};
}

template <class mint>
pair<BaselineFPS<mint>, BaselineFPS<mint>> baseline_rational_sum
(const vc<pair<BaselineFPS<mint>, BaselineFPS<mint>>> &fs, int d = -1)
{
  using F = BaselineFPS<mint>;
  using R = pair<F, F>;
  auto dc = [&](auto dc, int l, int r) -> R
  {
    if (r - l == 0)
      return {{}, {1}};
    if (r - l == 1)
      return fs[l];
    const int m = (l + r) / 2;
    R res = baseline_rational_plus(dc(dc, l, m), dc(dc, m, r));
    if (d < 0)
      return res;
    else
      return {res.first.resized(min(d, res.first.sz())), res.second.resized(min(d, res.second.sz()))};
  };
  return dc(dc, 0, fs.size());
}











/**
 * @brief 多項式補間
 * @docs docs/math/fps/baseline_interpolation.md
 */

template <class mint>
BaselineFPS<mint> baseline_interpolation(const vc<mint> &xs, const vc<mint> &ys)
{
  using F = BaselineFPS<mint>;
  assert(xs.size() == ys.size());
  const int n = xs.size();
  vc<F> fs(n);
  repi(i, n) fs[i] = {-xs[i], 1};
  F g = baseline_convolution_many(fs);
  vc<mint> a = baseline_multipoint_evaluation(g.diff(), xs);
  vc<mint> ia = inv_many<FieldAddSubMulDiv<mint>>(a);
  vc<pair<F, F>> rs(n);
  repi(i, n) rs[i] = {{ys[i] * ia[i]}, fs[i]};
  return baseline_rational_sum(rs, n).first;
}







/**
 * @brief FPS 合成
 * @docs docs/math/fps/baseline_composition.md
 */

// f(g(x)) mod x^n。g の定数項は 0。
// power_projection(N-1, N, g, w.rev()) の w に関する転置。
template <class mint>
BaselineFPS<mint> baseline_composition
(
  const BaselineFPS<mint> &f,
  const BaselineFPS<mint> &g,
  int n
)
{
  using F = BaselineFPS<mint>;
  assert(n >= 0);
  assert(g.get(0) == 0);
  if (n == 0 || f.empty())
    return F(n);
  if (n == 1)
    return {f[0]};

  const int N = bit_ceil(n);
  F ipw;
  mint iz = 1;
  if constexpr (is_static_modint_v<mint>)
  {
    if (ntt_ok<mint>(4 * N))
    {
      static const internal::fft_info<mint> info;
      iz = mint(4 * N).inv();
      const mint iroot = info.iroot[bit_width(2 * N)];
      ipw.resize(2 * N);
      mint pw = 1;
      repi(i, 2 * N) ipw[bitrev(2 * N, i)] = pw, pw *= iroot;
    }
  }
  const bool use_ntt = !ipw.empty();

  // power_projection と同じ配置：x 方向に l 項、y 方向に k 項。
  // y 方向を反転した分母の最高次 y^k は、配列の外で扱う。
  auto rec = [&](auto &&self, F q, int l, int k) -> F
  {
    // g(0)=0 より終端の分母は 1。p.rev() の転置も反転。
    if (l == 1)
      return f.resized(n).resized(N).rev();

    F q2(use_ntt ? 4 * N : 2 * N), r, qq;
    repi(j, k) repi(i, l) q2[j * (2 * l) + i] = q[j * l + i];
    if (use_ntt)
    {
      ntt(q2);
      qq.resize(2 * N);
      repi(i, 2 * N) qq[i] = 2 * q2[2 * i] * q2[2 * i + 1];
      intt(qq);
      fem(a : qq) a *= iz;
    }
    else
    {
      r = q2;
      repi(i, 1, 2 * N, 2) r[i] = -r[i];
      qq = q2 * r;
    }
    F next_q(N);
    repi(j, 2 * k) repi(i, l / 2)
      next_q[j * (l / 2) + i] = qq[j * (use_ntt ? l : 2 * l) + (use_ntt ? i : 2 * i)];
    repi(j, k) repi(i, l / 2)
      next_q[(j + k) * (l / 2) + i] += 2 * q[j * l + 2 * i];

    // 戻り道では固定した q2 / r だけを使う。
    F().swap(q), F().swap(qq);
    if (!use_ntt) F().swap(q2);
    F p = self(self, move(next_q), l / 2, 2 * k), res(N);
    // 省略した y^k による加算の転置。
    repi(j, k) repi(i, l / 2)
      res[j * l + 2 * i + 1] = p[(j + k) * (l / 2) + i];

    if (use_ntt)
    {
      F p2(2 * N), p3(4 * N);
      // 係数の取り出しの転置は、同じ位置への 0 埋め。
      repi(j, 2 * k) repi(i, l / 2) p2[j * l + i] = p[j * (l / 2) + i];
      // intt^T = ntt o reverse_except_zero（intt は正規化前）。
      reverse(p2.begin() + 1, p2.end());
      ntt(p2);
      repi(i, 2 * N)
      {
        const mint a = p2[i] * ipw[i] * iz;
        p3[2 * i] = a * q2[2 * i + 1];
        p3[2 * i + 1] = -a * q2[2 * i];
      }
      // ntt^T = reverse_except_zero o intt。
      intt(p3);
      reverse(p3.begin() + 1, p3.end());
      repi(j, k) repi(i, l) res[j * l + i] += p3[j * (2 * l) + i];
    }
    else
    {
      F p2(4 * N);
      repi(j, 2 * k) repi(i, l / 2)
        p2[j * (2 * l) + 2 * i + 1] = p[j * (l / 2) + i];
      // 畳み込みの転置：a_i = sum_j r_j p2_{i+j}。
      p2 = (p2 * r.rev()) >> (2 * N - 1);
      repi(j, k) repi(i, l) res[j * l + i] += p2[j * (2 * l) + i];
    }
    return res;
  };
  return rec(rec, -g.resized(N), N, 1).rev().resized(n);
}




/**
 * @brief 2変数形式的冪級数
 * @docs docs/math/fps/fps_2d.md
 */

// f[i][j] = [x^i y^j] f。省略した係数は 0。
// 打ち切り長 (h, w) を指定する演算は mod (x^h, y^w) で計算する。
template <class mint>
struct BaselineFPS2D : vvc<mint>
{
  using F = BaselineFPS2D;
  using Base = vvc<mint>;
  using Base::vector;
  using Base::operator=;

  BaselineFPS2D(const Base &f) : Base(f) {}
  BaselineFPS2D(Base &&f) : Base(move(f)) {}
  BaselineFPS2D(int h, int w)
  {
    assert(h >= 0 && w >= 0);
    this->assign(h, vc<mint>(w));
  }

  pair<int, int> shape() const
  {
    int w = 0;
    for (const auto &row : *this) chmax(w, int(row.size()));
    return {this->size(), w};
  }
  mint get(int i, int j) const
  {
    return 0 <= i && i < SZ(*this) && 0 <= j && j < SZ((*this)[i]) ? (*this)[i][j] : mint(0);
  }
  F resized(int h, int w) const
  {
    F res(h, w);
    repi(i, min<int>(h, this->size()))
      copy_n((*this)[i].begin(), min<int>(w, (*this)[i].size()), res[i].begin());
    return res;
  }
  void shrink()
  {
    for (auto &row : *this) while (!row.empty() && row.back() == 0) row.pop_back();
    while (!this->empty() && this->back().empty()) this->pop_back();
  }
  F transposed() const
  {
    auto [h, w] = shape();
    F res(w, h);
    repi(i, h) repi(j, (*this)[i].size()) res[j][i] = (*this)[i][j];
    return res;
  }
  mint eval(mint x, mint y) const
  {
    mint res = 0;
    repi(i, int(this->size()) - 1, -1, -1)
    {
      mint row = 0;
      repi(j, int((*this)[i].size()) - 1, -1, -1) row = row * y + (*this)[i][j];
      res = res * x + row;
    }
    return res;
  }

  F operator-() const
  {
    F res(*this);
    for (auto &row : res) for (auto &x : row) x = -x;
    return res;
  }
  F &operator+=(const F &g)
  {
    this->resize(max(this->size(), g.size()));
    repi(i, g.size())
    {
      (*this)[i].resize(max((*this)[i].size(), g[i].size()));
      repi(j, g[i].size()) (*this)[i][j] += g[i][j];
    }
    return *this;
  }
  F &operator-=(const F &g)
  {
    this->resize(max(this->size(), g.size()));
    repi(i, g.size())
    {
      (*this)[i].resize(max((*this)[i].size(), g[i].size()));
      repi(j, g[i].size()) (*this)[i][j] -= g[i][j];
    }
    return *this;
  }
  F operator+(const F &g) const { return F(*this) += g; }
  F operator-(const F &g) const { return F(*this) -= g; }
  F &operator*=(mint c)
  {
    for (auto &row : *this) for (auto &x : row) x *= c;
    return *this;
  }
  F &operator/=(mint c)
  {
    assert(c != 0);
    return *this *= c.inv();
  }
  F operator*(mint c) const { return F(*this) *= c; }
  friend F operator*(mint c, const F &f) { return f * c; }
  F operator/(mint c) const { return F(*this) /= c; }

  // y の次数が次の x の係数へ繰り上がらない間隔で、1変数の畳み込みに埋め込む。
  F mul(const F &g, int h, int w) const
  {
    F res(h, w);
    if (h == 0 || w == 0) return res;
    const int ah = min<int>(h, this->size()), bh = min<int>(h, g.size());
    int aw = 0, bw = 0;
    repi(i, ah) chmax(aw, min<int>(w, (*this)[i].size()));
    repi(i, bh) chmax(bw, min<int>(w, g[i].size()));
    if (aw == 0 || bw == 0) return res;
    const int stride = aw + bw - 1;
    vc<mint> a((ah - 1) * stride + aw), b((bh - 1) * stride + bw);
    repi(i, ah) copy_n((*this)[i].begin(), min<int>(aw, (*this)[i].size()), a.begin() + i * stride);
    repi(i, bh) copy_n(g[i].begin(), min<int>(bw, g[i].size()), b.begin() + i * stride);
    auto c = convolution(a, b);
    repi(i, min(h, ah + bh - 1)) repi(j, min(w, stride)) res[i][j] = c[i * stride + j];
    return res;
  }
  F operator*(const F &g) const
  {
    auto [h, w] = shape();
    auto [gh, gw] = g.shape();
    if (h == 0 || w == 0 || gh == 0 || gw == 0) return {};
    return mul(g, h + gh - 1, w + gw - 1);
  }
  F &operator*=(const F &g) { return *this = *this * g; }

  F inv(int h, int w) const
  {
    assert(h >= 0 && w >= 0 && get(0, 0) != 0);
    if (h == 0 || w == 0) return F(h, w);
    F g{{get(0, 0).inv()}};
    // 全次数 < m の精度を倍増。長方形全体には h+w-1 の精度が必要。
    const int need = h + w - 1;
    for (int m = 1; m < need;)
    {
      m += min(m, need - m);
      const int nh = min(h, m), nw = min(w, m);
      F error = -mul(g, nh, nw);
      error[0][0] += 2;
      g = g.mul(error, nh, nw);
    }
    return g.resized(h, w);
  }
  F div(const F &g, int h, int w) const { return mul(g.inv(h, w), h, w); }

  F diff_x() const
  {
    auto [h, w] = shape();
    F res(max(0, h - 1), w);
    repi(i, 1, h) repi(j, (*this)[i].size()) res[i - 1][j] = (*this)[i][j] * i;
    return res;
  }
  F diff_y() const
  {
    auto [h, w] = shape();
    F res(h, max(0, w - 1));
    repi(i, h) repi(j, 1, (*this)[i].size()) res[i][j - 1] = (*this)[i][j] * j;
    return res;
  }
  F integ_x() const
  {
    auto [h, w] = shape();
    assert(ll(h) < mint::mod());
    F res(h + 1, w);
    repi(i, h)
    {
      const mint iv = Binomial<mint>::inv(i + 1);
      repi(j, (*this)[i].size()) res[i + 1][j] = (*this)[i][j] * iv;
    }
    return res;
  }
  F integ_y() const
  {
    auto [h, w] = shape();
    assert(ll(w) < mint::mod());
    F res(h, w + 1);
    repi(j, w)
    {
      const mint iv = Binomial<mint>::inv(j + 1);
      repi(i, h) res[i][j + 1] = get(i, j) * iv;
    }
    return res;
  }

  // 定数項は 1。保持する全次数 h+w-2 が標数より小さいことが必要。
  F log(int h, int w) const
  {
    assert(h >= 0 && w >= 0 && get(0, 0) == 1);
    if (h == 0 || w == 0) return F(h, w);
    assert(ll(h) + w - 2 < mint::mod());
    F df = resized(h, w);
    repi(i, h) repi(j, w) df[i][j] *= i + j;
    F res = df.mul(inv(h, w), h, w);
    // (x d/dx + y d/dy) log(f) = (x f_x + y f_y) / f。
    repi(i, h) repi(j, w) if (i || j) res[i][j] *= Binomial<mint>::inv(i + j);
    return res;
  }
  // 定数項は 0。保持する全次数 h+w-2 が標数より小さいことが必要。
  F exp(int h, int w) const
  {
    assert(h >= 0 && w >= 0 && get(0, 0) == 0);
    if (h == 0 || w == 0) return F(h, w);
    assert(ll(h) + w - 2 < mint::mod());
    F g{{1}};
    const int need = h + w - 1;
    for (int m = 1; m < need;)
    {
      m += min(m, need - m);
      const int nh = min(h, m), nw = min(w, m);
      F correction = resized(nh, nw) - g.log(nh, nw);
      correction[0][0] += 1;
      g = g.mul(correction, nh, nw);
    }
    return g.resized(h, w);
  }
  // 整数乗。k < 0 のときは定数項が非零。0^0 = 1。
  F pow(ll k, int h, int w) const
  {
    assert(h >= 0 && w >= 0 && (k >= 0 || get(0, 0) != 0));
    F res(h, w);
    if (h == 0 || w == 0) return res;
    res[0][0] = 1;
    if (k == 0) return res;
    F base = k < 0 ? inv(h, w) : resized(h, w);
    ull e = k < 0 ? ull(-(k + 1)) + 1 : ull(k);
    while (e)
    {
      if (e & 1) res = res.mul(base, h, w);
      e >>= 1;
      if (e) base = base.mul(base, h, w);
    }
    return res;
  }
};

#undef convolution
#undef intt
#undef ntt
