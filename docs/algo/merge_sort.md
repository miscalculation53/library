## マージソート

比較回数がたかだか $n\lceil\log_2 n \rceil$ 回のソート（インタラクティブで役立つ）。

正確な比較回数はたぶんこれ？ https://oeis.org/A003071

`std::stable_sort` を使うというテクがあるが小さいときにマージソートじゃないとかで比較回数が多くなることがあるらしい（？）（よくわかってない）

#### merge_sort

```cpp
void merge_sort(vc<T> v)
```