## 測定条件

2026-09-07、手元の macOS / x86_64、Homebrew GCC 15.2.0。
`-std=c++17 -O2 -Wall -Wextra`、アサーション有効。
各方式を順番に別プロセスで実行した。以下は各一回の値。
実時間は起動・終了を含み、最大 RSS は `wait4` で測定。
オンラインジャッジへの提出結果ではない。

## 通常の乗法的関数

[既存の測定](multiplicative_prefix_sum.md) と同じく、
$f(p^e)=ae+bp$、$a=123456789,b=234567890$、法 $469762049$。
両方式とも `FieldAddSubMulDiv<mint>` を使ったが、環での除算はどちらも使わない。

素数の個数・素数の和の準備は同じ自動版。
以下は **素数上の和から乗法的関数の和へ復元する部分だけ**の時間。

| $N$ | 既存の専用 $O(N^{2/3})$ 版 | 一般の Euler 積版 |
| --- | ---: | ---: |
| $10^9$ | 0.0150 秒 | 0.0178 秒 |
| $10^{10}$ | 0.0606 秒 | 0.0696 秒 |
| $10^{11}$ | 0.2536 秒 | 0.2847 秒 |
| $10^{13}$ | 5.307 秒 | 5.830 秒 |

全方式で答えは一致した。$N=10^{13}$ の答えは `284866429`。
この入力の全プロセスの実時間は専用版 12.064 秒、一般版 12.423 秒。
最大 RSS はそれぞれ 241.6 MiB、374.9 MiB。

復元部分は今回の測定で専用版が約 1〜2 割速く、メモリも少なかった。
そのため、通常の `multiplicative_prefix_sum` の自動選択は既存の専用版を使い続ける。
この比較に使った一般 Euler 積への入口は削除済みで、ここには当時の測定記録を残している。
環や係数が異なる場合の優劣まで保証する測定ではない。

## Totient Bound

[問題](https://yukicoder.me/problems/no/1322) の制約は $N\le10^{10}$、5 秒・512 MB。
一般の Euler 積に局所因子 $1+(p-1)^{-s}-p^{-s}$ を渡す
[verify](../verify/yukicoder/totient_bound.test.cpp) をそのまま計測した。
素数判定による大きい因子の集約と、最後の $\zeta$ との一点の Dirichlet 積も含む。

| $N$ | 答え | 実時間 | CPU 時間 | 最大 RSS |
| --- | ---: | ---: | ---: | ---: |
| $10^8$ | 194361790 | 0.043 秒 | 0.029 秒 | 7.3 MiB |
| $10^{10}$ | 19435969728 | 0.133 秒 | 0.125 秒 | 18.9 MiB |

サンプル 3 件はすべて一致した。
最大ケースも、変形を使わず最小素因数で分ける独立した DFS と答えが一致した。
この DFS と直接の $\varphi(p^e)$ を使う構築との照合も
[単体テスト](../verify/mytest/ai/euler_product_prefix_sum.test.cpp) に含めている。

### 式変形前後の比較

2026-09-10、同じ macOS / x86_64、GCC 15.2.0、`-std=c++17 -O2 -Wall -Wextra`。
[比較コード](totient_bound.cpp) を使い、同じ Euler 積エンジンで次の二方式を測定した。

- `direct`：局所因子に $(\varphi(p^e),1)$ を列挙し、大素数側を素数計数で準備する。
- `transformed`：局所因子に $(p-1,1),(p,-1)$ を入れ、大素数側を素数判定で準備する。
  最後の $\zeta$ との一点の積も含む。

各回は別プロセスで、両方式を交互に3回ずつ実行した。
実時間・CPU 時間は中央値、最大 RSS は3回の最大値。
いずれも初期化・前計算・入出力を含む全処理の値。

| $N$ | 方式 | 実時間 | CPU 時間 | 最大 RSS |
| --- | --- | ---: | ---: | ---: |
| $10^8$ | 式変形前 | 0.0447 秒 | 0.0326 秒 | 7.60 MiB |
| $10^8$ | 式変形後 | 0.0437 秒 | 0.0330 秒 | 7.56 MiB |
| $10^{10}$ | 式変形前 | 0.1351 秒 | 0.1268 秒 | 22.23 MiB |
| $10^{10}$ | 式変形後 | 0.1514 秒 | 0.1413 秒 | 20.73 MiB |

答えは全実行で一致し、$N=1$ とサンプル3件も両方式で一致した。
式変形前も全体で $O(N^{2/3})$ であり、最大制約で十分速い。
今回の測定では式変形後の方が速いとはならなかった。
オンラインジャッジへの提出結果ではない。

## seed を渡す場合と後から掛ける場合

2026-09-11、macOS / x86_64、GCC 15.2.0、`-std=c++17 -O2 -Wall -Wextra`。
[比較コード](euler_product_seed.cpp) の `seed` は既知の累積和を直接渡し、
`after` は単位元から局所因子の積を作った後、通常の Dirichlet 積で同じ数列を掛ける。
両方式を交互に別プロセスで3回ずつ実行した中央値。
計測は全商位置の累積和を作る部分のみで、素数・因子・`seed` の準備と答えの照合を含まない。

| 対象 | $N$ | `seed` | `after` |
| --- | ---: | ---: | ---: |
| Totient Bound の変形後の積 | $10^{10}$ | 0.088819 秒 | 0.211006 秒 |
| Totient Bound の変形後の積 | $10^{11}$ | 0.370564 秒 | 0.969651 秒 |
| powerful number の個数 | $10^{10}$ | 0.003765 秒 | 0.041383 秒 |
| powerful number の個数 | $10^{12}$ | 0.039205 秒 | 0.470034 秒 |

Totient の行は $\prod_p(1+(p-1)^{-s}-p^{-s})$ の累積和であり、$\zeta(s)$ は掛けていない。
`seed` は $p>\lfloor\sqrt N\rfloor+1$ の部分をまとめたもの。
一つ上の節にある、前処理・最後の一点の積を含む Totient Bound 全体の測定とは計測範囲が異なる。

powerful number の行は $\zeta(2s)\prod_p(1+p^{-3s})$。
`seed` は平方数の指示関数で、明示する因子は $p^3\le N$ の $(p^3,1)$。
直接渡す方式は、準備を含めて $O(\sqrt N\log N)$ 時間という上界が得られる。
後から掛ける方式も積側の疎さを利用するため、常に $N^{2/3}$ に比例する時間を使うわけではない。

計測時には全商位置のチェックサムが一致した。追加の照合では、両方式の全保持値を直接比較して一致した。
powerful number は $a^2b^3$（$b$ は squarefree）を独立に列挙し、全保持位置で一致した。
$N=10^{10},10^{12}$ の個数はそれぞれ `214122`, `2158391`。

再現例（照合は時間計測の外で行う）：

```sh
g++-15 -std=c++17 -O2 -Wall -Wextra -I. benchmark/euler_product_seed.cpp -o /tmp/euler-product-seed
/tmp/euler-product-seed totient seed 10000000000
/tmp/euler-product-seed totient after 10000000000
/tmp/euler-product-seed powerful seed 1000000000000
/tmp/euler-product-seed powerful after 1000000000000
```

## 正当性の確認

C++17・20 で次を確認した。

- 任意の局所因子と任意の初期数列を、密な配列の愚直な積と比較。
- コスト 1、重複コスト、負の重み、打ち消し、空の因子、$K>\sqrt N$。
- 整数、合成数法、XOR/AND、通常の算術演算子を持たない要素型、多項式の環。
- 小さい Totient Bound を $x\le2N^2$ の全探索と比較。$\varphi(x)\ge\sqrt{x/2}$ により全解を含む。
- 全商位置の個数、重み $w(x)=x$、$\omega(x)$ ごとの個数を数える多項式。
- 大素数をまとめる場合と全因子を明示する場合の一致。
- 通常の乗法的関数の既存テストと一般エンジンの一致。

## 再現方法

Totient Bound の測定は以下で再現できる。

```sh
g++-15 -std=c++17 -O2 -Wall -Wextra -I. verify/yukicoder/totient_bound.test.cpp -o /tmp/totient-bound
g++-15 -std=c++17 -O2 -Wall -Wextra -I. benchmark/totient_bound.cpp -o /tmp/totient-bound-compare
```

Totient Bound の時間と RSS も、同じ測定関数で記録できる。

```sh
python3 - <<'PY'
import json, runpy
measure = runpy.run_path('benchmark/multiplicative_prefix_sum.py')['measure']
print(json.dumps(measure(['/tmp/totient-bound'], b'10000000000\n', 60)))
for repeat in range(3):
    for method in ['direct', 'transformed']:
        print(json.dumps(measure(['/tmp/totient-bound-compare', method], b'10000000000\n', 60)))
PY
```
