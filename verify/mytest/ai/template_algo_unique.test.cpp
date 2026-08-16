#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

struct EqOnly
{
  int x;
  friend bool operator==(const EqOnly &a, const EqOnly &b) { return a.x == b.x; }
};

// Test focus: unique uses operator== by default, custom equality controls adjacent
// deduplication, and sortunique accepts compatible ordering and equality predicates.
int main()
{
  {
    vc<EqOnly> a = {{1}, {1}, {2}};
    unique(a);
    assert(a.size() == 2 && a[0].x == 1 && a[1].x == 2);
  }
  {
    vc<int> a = {1, 3, 2, 4, 6, 7};
    auto same_parity = [](int x, int y) { return x % 2 == y % 2; };
    assert(uniqued(a, same_parity) == (vc<int>{1, 2, 7}));
    unique(a, same_parity);
    assert(a == (vc<int>{1, 2, 7}));
  }
  {
    vc<int> a = {5, 2, 3, 4, 1, 6};
    auto parity_less = [](int x, int y) { return x % 2 < y % 2; };
    auto same_parity = [](int x, int y) { return x % 2 == y % 2; };
    auto b = sortuniqued(a, parity_less, same_parity);
    assert(b.size() == 2 && b[0] % 2 == 0 && b[1] % 2 == 1);
    assert(a == (vc<int>{5, 2, 3, 4, 1, 6}));

    sortunique(a, greater<>());
    assert(a == (vc<int>{6, 5, 4, 3, 2, 1}));
  }
  PRINT("Hello World");
}
