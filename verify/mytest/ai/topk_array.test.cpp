#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "ds/topk_array.hpp"

template <class Compare>
// Test focus: incremental insertion retains the best K values in comparator order.
void test1()
{
  vc<ll> vec;
  TopKArray<ll, 10, Compare> arr;
  repi(_, 100)
  {
    ll x = randint(-ipow(10, 6), ipow(10, 6));
    arr.add(x);
    vec.eb(x);
    sort(ALL(vec), Compare());
    dump(arr, vec);
    repi(i, SZ(arr)) assert(arr.get(i) == vec[i]);
  }
}

template <class Compare>
// Test focus: destructive merging matches sorting the union for both comparator orders.
void test2()
{
  repi(a, 20) repi(b, 20)
  {
    vc<ll> vec;
    TopKArray<ll, 10, Compare> arr1, arr2;
    repi(_, a)
    {
      ll x = randint(-ipow(10, 6), ipow(10, 6));
      arr1.add(x);
      vec.eb(x);
    }
    repi(_, b)
    {
      ll x = randint(-ipow(10, 6), ipow(10, 6));
      arr2.add(x);
      vec.eb(x);
    }
    dump(arr1, arr2);
    arr1.merge(arr2);
    dump(arr1);
    sort(ALL(vec), Compare());
    repi(i, SZ(arr1)) assert(arr1.get(i) == vec[i]);
  }
}

// Test focus: nonmutating merge, empty operands, and the monoid wrapper share semantics.
void test3()
{
  TopKArray<int, 4> a = {1};
  TopKArray<int, 4> b = {2, 3};
  assert(a.merged(b).content() == vc<int>({1, 2, 3}));

  TopKArray<int, 4> empty;
  assert(empty.merged(b).content() == vc<int>({2, 3}));
  assert(b.merged(empty).content() == vc<int>({2, 3}));

  using M = TopKArrayMonoid<int, 3, greater<int>>;
  M::S x = {1, 5};
  M::S y = {3, 4};
  assert(M::op(x, y).content() == vc<int>({5, 4, 3}));
}

int main()
{
  test1<less<ll>>();
  test1<greater<ll>>();
  test2<less<ll>>();
  test2<greater<ll>>();
  test3();

  cout << "Hello World" << endl;
}
