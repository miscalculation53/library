#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_algo.hpp"
#include "math/algebra/algebra_basic_ops.hpp"
#include "math/algebra/bit.hpp"
#include "math/algebra/min_count.hpp"

struct MonoidMulWithoutPow
{
  using S = ll;
  static S op(S a, S b) { return a * b; }
  static S e() { return 1; }
};

struct MonoidWithPow
{
  using S = ll;
  static S op(S, S)
  {
    assert(false);
    return 0;
  }
  static S e()
  {
    assert(false);
    return 0;
  }
  static S pow(S a, ll k) { return a * k; }
};

struct AddOnly
{
  ll val;
  friend AddOnly operator+(AddOnly a, AddOnly b) { return {a.val + b.val}; }
};

// Test focus: custom pow dispatch, generic fallback, additive/XOR closed forms,
// negative group powers, and monoids whose values only provide operator+.
int main()
{
  assert(pow_monoid<MonoidWithPow>(7LL, 6LL) == 42);
  assert(pow_monoid<MonoidMulWithoutPow>(3LL, 4LL) == 81);
  assert(pow_monoid<MonoidAdd<ll>>(7LL, 6LL) == 42);
  assert(pow_monoid<MonoidAdd<AddOnly>>({7}, 6).val == 42);
  using Min = MonoidMin<int, 1'000'000'000>;
  assert(pow_monoid<Min>(7, 0) == Min::e());
  assert(pow_monoid<Min>(7, 100) == 7);
  assert(pow_monoid<GroupXor<unsigned>>(13U, 8) == 0);
  assert(pow_monoid<GroupXor<unsigned>>(13U, 9) == 13);
  assert(pow_group<GroupAddSub<ll>>(7LL, -6LL) == -42);
  assert(pow_group<GroupAddSub<ll>>(1LL, numeric_limits<ll>::min()) == numeric_limits<ll>::min());
  assert(pow_group<GroupAddSub<double>>(2.5, -3.0) == -7.5);

  using M = MonoidMinCount<int, ll, 1'000'000'000>;
  auto x = pow_monoid<M>({3, 2}, 5);
  assert(x.mn == 3 && x.cnt == 10);

  cout << "Hello World" << endl;
}
