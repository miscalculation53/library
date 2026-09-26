#include "math/linalg/linear_transpose_cpp.hpp"
#include "ds/fenwick_tree/fenwick_tree.hpp"

// 既存の FenwickTree とテンプレートを使う順方向。
template <class S>
vc<S> range_sum_cpp(const vc<S> &x, const vc<int> &left, const vc<int> &right)
{
  FenwickTree<GroupAddSub<S>> tree(x);
  vc<S> y(left.size());
  repi(i, left.size()) y[i] = tree.sum(left[i], right[i]);
  return y;
}

int main(int argc, char **argv)
{
  using mint = modint998244353;
  vc<int> left{0, 1}, right{2, 3};
  auto forward = [&](const auto &x) { return range_sum_cpp(x, left, right); };
  auto program = linear_transpose::record<mint>(3, forward);
  if (argc > 1)
  {
    std::ofstream output(argv[1]);
    if (!output) return 1;
    program.write_cpp(output, "generated_range_add", "modint998244353");
    return !output;
  }
  for (auto x : program.transpose({10, 20})) std::cout << x << ' ';
  std::cout << '\n'; // 10 30 20
}
