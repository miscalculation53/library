このドキュメントには AI による作成・変更が含まれています。

## 概要

正しい括弧列のランダム生成

## 使用例

```cpp
#include "random/bracket_sequence.hpp"

string s = random_bracket_sequence(4);
int balance = 0;
for (char c : s) {
  balance += c == '(' ? 1 : -1;
  assert(balance >= 0);
}
assert(s.size() == 8 && balance == 0);
```

## 詳細なドキュメント

#### random_bracket_sequence

```cpp
string random_bracket_sequence(int n)
```

長さ $2n$ の正しい括弧列をほぼ一様ランダムに生成する。

各文字を置いた後に正しい括弧列を作れる場合の数から遷移確率を求め、その確率に従って次の文字を選ぶ。確率の計算には `double` を用いる。

返り値は `'('` と `')'` からなる文字列。

##### 制約

- $n \geq 0$

##### 計算量

- $O(n)$
