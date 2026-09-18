## 概要

長さ $n$ の数列から最短線形漸化式を一括で復元する。
[通常の BM](berlekamp_massey.md) と同じ問題を
[多項式 Half-GCD](half_gcd.md) に帰着する。
必要な長さの NTT が使える場合は $O(n\log^2 n)$ 時間、$O(n)$ 空間。

```cpp
vc<modint998244353> a{0, 1, 1, 2, 3, 5};
auto c = berlekamp_massey_fast(a); // {-1,1,1}
```

返り値は `c[0] == -1`、`d = c.size()-1` として
$a_i=\sum_{j=1}^d c_j a_{i-j}$ がすべての $d\le i<n$ で成立する最小の `d` の解。
通常版と異なり、テンプレート引数は `FieldAddSubMulDiv<mint>` ではなく `mint`（推論可能）。
係数型は `FormalPowerSeries<mint>` と同じものを使い、体をなす必要がある。
空列・全零列の返り値は `{-1}`。

有限列の最短漸化式は一意でないことがあり、通常版との係数一致は保証しない。
最小次数と漸化式の成立は保証する。`2*d <= n` なら解は一意なので通常版とも一致する。
末尾の零係数も次数の一部であり、返り値を `shrink()` してはいけない。
例えば最後の項だけが非零の列では、`d=n`、`c={-1,0,...,0}` になる。

長い低次数の列では、$O(n(d+1))$ の通常版が速いことがある。
自動切り替えはせず、別 API として提供する。逐次更新には従来の `BerlekampMassey<F>` を使う。
実測値は [ベンチマーク](../../../benchmark/half_gcd.md) を参照。

## 帰着

入力の係数順を反転し、
$T(x)=a_{n-1}+a_{n-2}x+\cdots+a_0x^{n-1}$ とする。
求める漸化式を monic な特性多項式
$U(x)=x^d-c_1x^{d-1}-\cdots-c_d$ と表すと、条件は

$$ U(x)T(x)\bmod x^n=R(x),\qquad\deg R<\deg U=d $$

になる。そこで $(x^n,T)$ に拡張 Euclid を適用し、
剰余とその係数を $r=vx^n+uT$ と追跡する。
初めて $\deg r<\deg u$ となった `u` を monic にすると、最短漸化式が得られる。

Half-GCD で中間点まで一括処理した後、この停止条件まで通常の除算を追加する。
停止地点は中間点付近なので、追加は高々 1 回。
GCD を最後まで求める必要はない。
この停止条件を使うことで、奇数長・零が続く列・次数が $n/2$ を超える場合も扱える。

帰着と最小性の証明は [AtCoder「線形漸化式の復元」§6](https://info.atcoder.jp/entry/algorithm_lectures/linearly_recurrent_sequence_reconstruction) を参照。
