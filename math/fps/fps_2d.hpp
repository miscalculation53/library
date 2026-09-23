#pragma once

#include "fps.hpp"

/**
 * @brief 2変数形式的冪級数
 * @docs docs/math/fps/fps_2d.md
 */

// f[i][j] = [x^i y^j] f。省略した係数は 0。
// 打ち切り長 (h, w) を指定する演算は mod (x^h, y^w) で計算する。
template <class mint>
struct FormalPowerSeries2D : vvc<mint>
{
  using F = FormalPowerSeries2D;
  using Base = vvc<mint>;
  using Base::vector;
  using Base::operator=;

  FormalPowerSeries2D(const Base &f) : Base(f) {}
  FormalPowerSeries2D(Base &&f) : Base(move(f)) {}
  FormalPowerSeries2D(int h, int w)
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
