## 概要

作用つきモノイドの `mapping` が区間情報だけでは計算できない場合に、作用を子へ伝播して区間情報を再構築する Segment Tree Beats。

実装は [atcoder::lazy_segtree に1行書き足すだけの抽象化 Segment Tree Beats](https://rsm9.hatenablog.com/entry/2021/02/01/220408) の方針に基づく。既存の `LazySegmentTree` と本体を共有し、beats モードでは `mapping` 後の `S::fail` が `true` なら `push` と `update` を行う。

作用つきモノイドは次を満たす必要がある。

- `S` が `bool fail` を持つ
- `mapping(f, x)` を `x` の情報だけで正しく計算できない場合に限り、返り値の `fail` を `true` にする
- `op` と葉への `mapping` は失敗しない
- 全クエリを通した失敗回数に十分小さい上界がある

## 使用例

```cpp
using AM = ActedMonoidChminChmaxAddMinMaxSum<ll>;
using F = AM::F;
SegmentTreeBeats<AM> seg(a);

seg.apply(l, r, F::chmin(x));
seg.apply(l, r, F::chmax(x));
seg.apply(l, r, F::add(x));
auto res = seg.prod(l, r);
cout << res.mn << ' ' << res.mx << ' ' << res.sum << '\n';
```

## 詳細なドキュメント

### SegmentTreeBeats

```cpp
SegmentTreeBeats<AM>
```

`LazySegmentTree<AM>` と同じコンストラクタおよび公開関数を持つ。各関数の仕様は [LazySegmentTree](./lazy_segtree.md) を参照。

`mapping` が失敗した頂点では、その作用を子へ伝播して `op` で再構築する。

##### 制約

- `AM` が概要に記載した失敗規約を満たす

##### 計算量

- `mapping` の失敗回数に依存する
- `ActedMonoidChminChmaxAddMinMaxSum` では、各操作あたり償却 $O(\log^2 n)$
