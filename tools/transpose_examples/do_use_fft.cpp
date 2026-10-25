// https://codeforces.com/gym/102978/problem/D
#include "do_use_fft.hpp"

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  using mint = modint998244353;
  int n;
  if (!(std::cin >> n)) return 0;
  std::vector<mint> a(n), b(n), c(n);
  for (auto &x : a) std::cin >> x;
  for (auto &x : b) std::cin >> x;
  for (auto &x : c) std::cin >> x;
  for (auto x : do_use_fft(a, b, c)) std::cout << x << '\n';
}
