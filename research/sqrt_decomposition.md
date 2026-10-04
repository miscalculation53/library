# 列の平方分割の API

## 結論

固定したブロック分割に対して、**各ブロックと対象区間の交わりを列挙する**ことを基本操作にする。
利用側は、その区間を受け取って処理の前後を制御する。

```cpp
for (auto s : sqrt_decomposition_blocks<B>(n, l, r))
{
  if (s.full()) apply_whole_block(s.b);
  else
  {
    push(s.b);
    for (int i = s.l; i < s.r; i++) apply_point(i);
    rebuild(s.b);
  }
}
```

`point(i)` / `block(b)` の呼び分けは、この列挙の使い方の一つ。
それだけを API にすると、部分ブロックの最初と最後を利用側で検出する必要がある。
点ごとに O(B) の再構築を行えば、端の処理が O(B²) になる。
ブロックごとの交差区間を渡せば、展開と再構築を各一度にできる。

## 確認した問題例

問題の操作は下記の公式問題文で確認した。平方分割での持ち方と API の使い方は、この検討で構成したもの。

| 問題 | ブロックに持つ情報 | 必要な処理 |
| --- | --- | --- |
| [Range Affine Range Sum](https://raw.githubusercontent.com/yosupo06/library-checker-problems/master/data_structure/range_affine_range_sum/task.md) | 和と遅延アフィン変換 | 完全ブロックは遅延変換の合成。部分ブロックは展開・対象部分の更新・和の再計算を一度ずつ行う |
| [Point Set Range Frequency](https://raw.githubusercontent.com/yosupo06/library-checker-problems/master/data_structure/point_set_range_frequency/task.md) | 各ブロックのソート済み列 | 一点更新は該当ブロックを修正。頻度は完全ブロックで二分探索、部分ブロックで走査 |
| [AtCoder practice2 J](https://atcoder.jp/contests/practice2/tasks/practice2_j) | ブロックの最大値 | 条件を満たす要素を含む最初のブロックで要素走査へ進み、答えが見つかれば終了 |
| [Codeforces 455D — Serega and Fun](https://codeforces.com/problemset/problem/455/D) | 固定長の deque と頻度表 | 右巡回シフトの繰越しを左から右へ渡す。完全ブロックは末尾を出して先頭に挿入し、部分ブロックは内部を動かす |
| [Codeforces 13E — Holes](https://codeforces.com/problemset/problem/13/E) | 各点からブロックを抜けるまでの跳躍数・出口・最後の点 | 一点更新でもブロック内の依存情報を右から再計算する。問い合わせでは出口をたどる |

頻度のような集計は従来の簡便版でも自然に書ける。
アフィン変換では前後処理、探索では途中終了と完全ブロック内への走査、巡回シフトではブロック間の状態の受け渡しが加わる。
これらを通常の `for` / `if` / `break` / `return` で記述できる形を選んだ。

Holes の問い合わせは、値によって次の訪問先が決まる。独自のジャンプループで処理する。
区間の列挙 API は初期化と更新で使い、ブロック内の再計算範囲を得る役割を持つ。

## 境界と計算量

各要素はブロック $b=\lfloor i/B\rfloor$ に属し、ブロックの実際の範囲は $[bB,\min((b+1)B,n))$。
返す情報は、ブロック番号・ブロック全体の範囲・対象区間との交わり。
空の交わりは省き、各ブロックを一度だけ列挙する。完全ブロックかどうかは実際の末尾を含めて判定する。

列挙器自体の時間は O(1 + (r-l)/B)、作業領域は O(1)。
部分ブロックの処理が O(B)、完全ブロックの処理が O(1) なら全体で O(B + (r-l)/B)。
ソート済み列を二分探索する頻度クエリは O(B + ((r-l)/B) log B)。
データ構造ごとの処理時間は利用側で評価する。

`full()` は対象範囲についての判定であり、処理方法は利用側で選ぶ。
探索では、完全ブロックの最大値を調べてからそのブロック内を走査できる。
全体に対する更新が集計情報だけで処理できるかどうかも、利用側で判断する。

## 可変長列との境界

[Dynamic Sequence Range Affine Range Sum](https://raw.githubusercontent.com/yosupo06/library-checker-problems/master/data_structure/dynamic_sequence_range_affine_range_sum/task.md)
には挿入・削除・区間反転がある。
この種類まで効率よく扱うには、ブロックの分割・結合・並べ替えと、位置からブロックを探す仕組みが必要になる。

今回は固定した位置のブロックを列挙するライブラリとして定義する。
可変長列には、可変長ブロックの列を管理する別の構造を用意するのが自然。
その構造では、端のブロックを分割して対象区間を完全ブロックの列にし、更新や反転を行う。
単に列長 `n` を変えて呼び直すだけでは、既存の集計値と要素の所属の対応が保たれない。

## 実装と検証

- [実装](../ds/sqrt_decomposition/sqrt_decomposition.hpp)：列挙 API を追加し、従来の point / block 版をその簡便版とした。
- [API ドキュメント](../docs/ds/sqrt_decomposition/sqrt_decomposition.md)
- [境界テスト](../verify/mytest/ai/sqrt_decomposition.test.cpp)：全区間の被覆、部分ブロック数、空区間、末尾、巨大な添字、途中終了、従来 API を検査。
- [問題例の実装とテスト](../verify/mytest/ai/sqrt_decomposition_examples.test.cpp)：上の 5 系統を直接計算と比較。アフィン変換では展開・再構築の回数も検査。

問題例のテストは引数なしで乱数テストとサンプルを実行する。
`affine` / `frequency` を引数にすると、それぞれ Library Checker の入力形式で実行できる。

2026-09-29 に GCC 15 の C++17 / C++20 と、Clang の AddressSanitizer / UndefinedBehaviorSanitizer で通過。
保存済みの Range Affine Range Sum 19 ケース、Point Set Range Frequency 25 ケースでも出力の一致を確認した。
