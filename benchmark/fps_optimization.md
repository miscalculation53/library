## 概要

FPS と周辺処理の高速化をライブラリ本体へ反映した。
`exp`・平方根・逆数のブロック化、多点評価の転置、補間での積木の共有、合成の中間積を扱う。
sparse / dense は、非零項の位置と NTT の変換長・回数を用いて選ぶ。

定数倍の評価には、各変換長の順・逆 NTT 回数、$L\log_2 L$ の総和、NTT 単体の実測を使う。
ブロック方式では変換回数が増えても、短い変換によって高速になる。

## 使用例

```sh
python3 benchmark/fps_constant_factor_generate.py
python3 benchmark/fps_production_generate.py
g++-15 -std=c++20 -O2 -DNDEBUG -I. benchmark/fps_production.cpp -o /tmp/fps-production
/tmp/fps-production adopted 998244353 65536 5
/tmp/fps-production adopted 1000000007 16384 5
/tmp/fps-production threshold 998244353 16384 3
/tmp/fps-production threshold 1000000007 16384 3
g++-15 -std=c++20 -O2 -DNDEBUG -I. benchmark/fps_ntt_cost.cpp -o /tmp/fps-ntt-cost
/tmp/fps-ntt-cost 131072
python3 benchmark/fps_ntt_cost_check.py
python3 benchmark/fps_production_cost.py
```

[本体との比較](fps_production.cpp) は本体から計測用のコピーを生成する。
比較対象の FPS は [変更前の保存ソース](fps_constant_factor_source.txt)、周辺算法は
[保存した実装](fps_baseline.hpp)（`fb6134ff`）を使う。畳み込みの共通カーネルは両者で共有する。
生成物は `/tmp/production-profile` に置き、計測用の変数は生成物に置き、全変換を数える。
`kernels` 列には CRT の各素数も区別して記録する。時間計測中は集計を止める。

## 詳細なドキュメント

#### 採用した変更

| 処理 | 採用した方式 |
| --- | --- |
| `exp` | native NTT は Harvey のブロック方式。小さい入力とブロックの初期値は、短い循環積・最後の分割を使う Bostan–Schost。CRT は逆数を維持する更新 |
| `sqrt` | native NTT は平方根のブロック更新。CRT と小さい入力は高次側だけの Newton 更新 |
| `inv` | Newton 法を基本とし、$n\ge512$, $4n\le3\operatorname{bit\_ceil}(n)$ の場合は必要なブロックだけ求める |
| 多点評価 | $\prod(1-x_i x)$ の積木と中間積による転置方式。積木の NTT 結果を下向きの更新でも使う |
| 補間 | 同じ積木で $G'$ を評価し、分母の積と NTT 結果を共有して分子を構築 |
| 合成 | CRT の畳み込みの転置を中間積に変更。変換長を $8N$ から $4N$ に縮小 |
| 2 変数の逆数 | $y$ 方向の逆数を初期値にし、$\bmod y^w$ で $x$ の精度を倍増 |
| `pow` | 指数 $0,1,-1$ を直接処理。$2\le k\le8$ は打ち切った積の繰り返し |
| 高い付値 | $f=c+O(x^d)$, $3d\ge n$ の逆数・平方根・累乗は一次または二次の展開で求める。`exp` は $2d\ge n$ なら $1+f$ |
| sparse 漸化式 | 積の剰余をまとめ、微分係数を保存し、必要次数で非零リストの走査を止める |
| その他 | 両方が疎な畳み込みの非零項同士を列挙。`div_poly` の逆順コピーを商の長さに限定。長さが揃う `rational_plus` の逆 NTT を 3 回から 2 回へ |

逆数は 2 の冪に揃う長さでブロック化の差が小さく、境界直後に効果が大きかった。
分割数は exp が 4 または 16、sqrt が 16、inv が 8。頻度領域の積和も含めた実測で選んだ。

#### 実測

| 法 | 処理 | 項数 / 点数 | 変更前 ms | 採用版 ms | 速度比 | NTT 回数（前→後） |
| --- | --- | ---: | ---: | ---: | ---: | ---: |
| 998244353 | `exp` | 65,536 | 16.535 | 10.314 | 1.60 | 192→371 |
| 998244353 | `sqrt` | 65,536 | 23.970 | 7.195 | 3.33 | 533→341 |
| 998244353 | `multipoint` | 65,536 | 212.694 | 71.603 | 2.97 | 38215→6221 |
| 998244353 | `interpolation` | 65,536 | 406.137 | 111.906 | 3.63 | 48445→9290 |
| 998244353 | `inv` | 4,097 | 0.838 | 0.473 | 1.77 | 65→86 |
| 1000000007 | `exp` | 16,384 | 51.581 | 24.002 | 2.15 | 390→204 |
| 1000000007 | `composition` | 16,384 | 492.707 | 304.612 | 1.62 | 252→252 |
| 1000000007 | `multipoint` | 16,384 | 161.160 | 78.076 | 2.06 | 3363→1511 |
| 1000000007 | `interpolation` | 16,384 | 267.912 | 114.021 | 2.35 | 4479→2069 |

2026-10-07、arm64 macOS 上で x86_64 バイナリを実行。GCC 15.2.0、C++20、`-O2 -DNDEBUG`。
固定 seed の入力を全係数照合し、方式の先行順を交代して 5 回測定した中央値。
配列確保・コピー・係数演算を含む。小さい処理は反復し、逆数表の初期化は計測前の実行で済ませる。
環境依存の値なので、CSV の最小・最大値も併せて参照する。

#### NTT を使った定数倍の評価

長さ $L$ の順・逆 NTT の回数を $F_L,I_L$ とする。比較には次の 3 種を使う。

$$C=\sum_L(F_L+I_L),\qquad W=\sum_L(F_L+I_L)L\log_2 L,$$

$$\widehat T=\sum_{p,L}\{F_{p,L}t^F_p(L)+I_{p,L}t^I_p(L)\}.$$

$t^F_p,t^I_p$ は [NTT 単体の実測](fps_ntt_cost.csv)。CRT は各素数の NTT を別々に数える。
$\widehat T$ は NTT 部分の見積もりで、正規化・周波数上の積和・CRT 復元・配列操作は別の費用。
[集計結果](fps_production_cost.csv) に関数全体の時間と並べた。

以下は $n=N=2^K$、一般の dense 入力、$E(L)=L\log_2L$ とした式。
通常の積を $M(N)=3E(2N)+O(N)$ と置くと、主要項は $6E(N)$。
順・逆 NTT の費用を同じとみなす理想化を使い、実際の判断には上の測定値を使う。

| 算法 | 1 段の NTT またはブロック本体 | 主要項 / $M(N)$ |
| --- | --- | ---: |
| Newton 逆数 | $5E(2m)$ | $5/3$ |
| 旧 exp | $4E(m)+8E(2m)$ | $10/3$ |
| 短い循環積の exp | $5E(m)+6E(2m)$ | $17/6$ |
| 同上＋最後の分割 | 最終段を $3E(N)-5E(N/2)$ 削減 | $11/4$ |
| ブロック exp | $(13s-4)E(2m)$、$N=2sm$ | $13/6$（分割数を増やす漸近評価） |
| 旧 sqrt | 各段 $\Phi_{\rm inv}(2m)+3E(4m)$ | $16/3$ |
| 高次側だけの sqrt | 各段 $\Phi_{\rm inv}(m)+5E(2m)$ | $10/3$ |
| ブロック sqrt | $(4r-3)E(2m)$、$N=rm$ | $4/3$（同上） |
| 2 次 Newton のブロック逆数 | $(9s-3)E(2m)$、$N=2sm$ | $3/2$（同上） |

$\Phi$ は初期値を求める NTT 費用。ブロック exp は $\Phi_{\rm exp}(m)+\Phi_{\rm inv}(m)$、
sqrt は $\Phi_{\rm sqrt}(m)+\Phi_{\rm inv}(m)$、inv は $\Phi_{\rm inv}(m)$ を本体に加える。
各方式には $O(s^2m)$ の周波数上の積和もある。固定の分割数 16 の実装では、表の漸近定数と有限長の費用を分けて考える。

65,536 項の初期比較では、BS の最後の分割が 167 回、逆数更新を $3E(m)+7E(2m)$ に変える版が 152 回だった。後者の $W$ は約 0.8% 大きく、時間も 12.064→12.281 ms だった。総回数の少なさと変換長の費用を分けて評価する。採用したブロック版は短い NTT を増やし、実際の $W$ を旧方式から約 46% 減らした。

必要な高次ブロックだけ求める場合、exp は $h=\lceil n/m\rceil-s$ として $(7s+6h-4)E(2m)$、
inv は $r=\lceil n/m\rceil$ として $(4r+s-3)E(2m)$、sqrt は同じ $r$ で $(4r-3)E(2m)$。
これが 4,097 項などの境界直後で効く。

exp の係数と平方根のブロック算法はそれぞれ
[Harvey: exp](https://arxiv.org/pdf/0911.3110) と
[Harvey: sqrt / reciprocal](https://arxiv.org/pdf/0910.1926) に基づく。
[Bostan–Schost](https://arxiv.org/abs/1301.5804) の $11/4$ も、今回の変換長の集計から確認できた。

#### 多点評価・補間の NTT の主要項

点数と係数数がともに $N=2^K$ の場合を比較する。native NTT、畳み込みの閾値 60、
旧逆数の閾値 200、旧剰余方式の末端 64 点を前提に、$K\ge8$ では次の式が CSV と一致する。

| 多点評価 | $W$ |
| --- | --- |
| 旧剰余方式 | $14NK^2-(614-5/64)N-20$ |
| 転置・NTT 保持なし | $6NK^2+28NK-292N+10$ |
| 転置・NTT 保持あり | $3NK^2+19NK-130N+10$ |

積木の 1 ノードを、次数 $k$ の子 2 個から作る。
旧方式は長さ $4k$ の NTT 3 回、採用版は循環積を使う長さ $2k$ の NTT 3 回。
下向きの更新も、剰余に伴う各子の逆数・積から、保持した変換と親の NTT 1 回・逆 NTT 2 回に置き換わる。
$NK^2$ の係数は $14\to3$。関数全体の速度には係数操作と保持配列の費用も含まれる。

補間の旧方式は積木を別々に構築し、有理式の和でも分母を更新する。
共有すると追加処理は分子だけの併合になる。

$$W_{\rm old}=22NK^2+20NK-(1022-5/64)N-20,$$
$$W_{\rm shared}=\tfrac92NK^2+\tfrac{41}2NK-193N+10.$$

[変換ヒストグラムとの照合](fps_ntt_cost_check.py) で式を検証した。
転置原理と中間積の背景は [Bostan–Lecerf–Schost](https://mathexp.eu/bostan/publications/BoLeSc03.pdf) を参照。
NTT 結果の保持で作業配列が増える。65,536 点の過去の単独プロセス測定では最大 RSS が
剰余方式 24.91 MiB、転置・保持版 36.36 MiB だった（[記録](fps_algorithms_memory_cache.txt)）。

#### sparse / dense の選択

必要な正次数の非零項だけで

$$W_s(f,n)=\sum_{1\le j<n,\ f_j\ne0}(n-j)$$

を求める。inv/div/exp はこの積和回数、pow/sqrt は $2W_s$ を基本にする。
先頭の零・シフト・入力の不要な末尾は除く。走査中に dense の予算を超えたら判定を終える。

`internal_ntt.hpp` の予算は、採用した算法の NTT 回数と変換長から作る。
小さい積が素朴法に変わる部分を含む近似なので、演算ごとの実測で係数を補正する。
native NTT は inv/div/pow が 1.4、exp が 1.8、sqrt が 1.7、CRT は 2.5 を使う。
通常表現の 32 bit modint 以外は疎な剰余集約を使えないため係数を半分にする。

表は 16,384 項・先頭配置で sparse / dense の時間比が 1 をまたいだ正次数の非零項数。

| 演算 | native NTT | CRT |
| --- | --- | --- |
| `inv` | 128〜160 | 1600〜2000 |
| `div` | 256〜320 | 2000〜2500 |
| `exp` | 200〜256 | 2500〜3000 |
| `pow` | 256〜320 | 2500〜3000 |
| `sqrt` | 64〜100 | 800〜1300 |

720 条件の自動選択時間 / 速い方の時間の中央値は、native 1.005、CRT 1.000。95 パーセンタイルは 1.133、1.023。
直接の一次・二次展開も dense 比較側に含め、内部の sparse 判定を強制的に無効化する比較を用いた。

これは測定環境での調整値。固定の「200 非零項」より、法・長さ・配置を反映できる。
高い付値の一次・二次展開を先に処理すると、CRT の内部で積が急に軽くなるケースも扱える。

#### さらに算法を変える候補

- 逆数には 3 次 Newton のブロック方式で $13M(n)/9$ を得る方法もある。2 進の NTT で $3sm$ に丸めると余分な精度の費用が増えるため、今回は測定で効いた 2 次の部分更新を採用した。
- 合成・Power Projection はすでに Graeffe 型の更新と転置を使っている。今回の CRT の中間積では、一般 dense 入力の主要 NTT 費用が $108N(\log N)^2\to72N(\log N)^2$ になる。native の経路は同じ算法を保つ。[近線形時間の合成](https://arxiv.org/abs/2404.05177) は既存方式を理解する参照先。
- Bostan–Mori、Half-GCD、Berlekamp–Massey の高速版には NTT の共有・高速行列演算・高速内積がすでに入っていた。共有する FPS 基本演算の改善が波及する。
- ブロック単位の直接除算や逆平方根との同時更新も候補になる。今回採用した範囲で係数検証と測定を揃え、追加方式は別の比較対象にできる。

#### 検証とデータ

- [追加テスト](../verify/mytest/ai/fps_algorithms.test.cpp) は逆数・exp/log・平方根の恒等式、累乗の独立した疎な漸化式、多点評価の Horner 法、補間、中間積の直接積和と照合。
- 2 の冪の前後、末尾の切り詰め、先頭の零、高い付値、評価点の重複と 0、空入力を検証。
- 静的法 998244353・1811939329・1000000007、動的法 998244353、法 17 を検証。
- FPS、長さ、2 変数 FPS、合成、Power Projection、Half-GCD、Bostan–Mori、有理式、畳み込みなど 14 種の既存・追加テストが通過。追加テストは Apple Clang の AddressSanitizer / UndefinedBehaviorSanitizer も通過。

| データ | 内容 |
| --- | --- |
| [production_ntt](fps_production_ntt.csv) / [production_crt](fps_production_crt.csv) | 本体に採用した方式と変更前の比較 |
| [threshold_ntt](fps_production_threshold_ntt.csv) / [threshold_crt](fps_production_threshold_crt.csv) | 3 配置、3 出力長、5 演算の sparse / dense / 自動選択 |
| [NTT 単体](fps_ntt_cost.csv) / [コスト集計](fps_production_cost.csv) | 素数・変換長・順逆別の時間と関数全体の時間 |
| [その他の方式](fps_more_algorithms_ntt.csv) | inv/sqrt の分割数、exp の部分更新、補間の共有 |
| [最初の調査](fps_constant_factor.md) / [exp・多点評価の比較](fps_algorithms.md) | 実装前の候補、初期の閾値、比較データ |
