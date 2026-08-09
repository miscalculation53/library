#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/csr.hpp"

// Test focus: sortunique rebuilds offsets, erasable rows remain valid, and const access is const.
int main()
{
  CSR<int> csr(vc<int>{4, 0, 5});
  int a[] = {3, 1, 3, 2, 5, 4, 5, 4, 5};
  repi(i, 9) csr.get_elist()[i] = a[i];
  assert(csr.offset(0) == 0);
  assert(csr.offset(1) == 4);
  assert(csr.offset(2) == 4);
  assert(csr.offset(3) == 9);

  csr.sortunique();
  assert((csr[0].to_v() == vc<int>{1, 2, 3}));
  assert(csr[1].empty());
  assert((csr[2].to_v() == vc<int>{4, 5}));
  assert(csr.offset(0) == 0);
  assert(csr.offset(1) == 3);
  assert(csr.offset(2) == 3);
  assert(csr.offset(3) == 5);

  CSR<int, true> erasable(vvc<int>{{3, 1, 3}, {2, 2, 1}});
  erasable.pop_back(0);
  erasable.sortunique();
  assert((erasable[0].to_v() == vc<int>{1, 3}));
  assert((erasable[1].to_v() == vc<int>{1, 2}));
  erasable.pop_back(1);
  assert((erasable[1].to_v() == vc<int>{1}));

  const CSR<int> const_csr(vvc<int>{{1}});
  static_assert(is_const_v<remove_reference_t<decltype(const_csr[0][0])>>);
  assert(const_csr[0][0] == 1);

  cout << "Hello World" << endl;
}
