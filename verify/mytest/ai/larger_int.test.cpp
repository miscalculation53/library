#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"
#include "utils/larger_int.hpp"
#include "math/modint/modint.hpp"

// Test focus: widening rules for integers and identity rules for real and modint types.
static_assert(is_same_v<larger_int_t<int>, long long>);
static_assert(is_same_v<larger_int_t<long long>, __int128_t>);
static_assert(is_same_v<larger_int_t<unsigned long long>, __uint128_t>);
static_assert(is_same_v<larger_int_t<__int128_t>, __int128_t>);
static_assert(is_same_v<larger_int_t<__uint128_t>, __uint128_t>);
static_assert(is_same_v<larger_int_t<float>, float>);
static_assert(is_same_v<larger_int_t<double>, double>);
static_assert(is_same_v<larger_int_t<long double>, long double>);
static_assert(is_same_v<larger_int_t<modint998244353>, modint998244353>);

int main()
{
  cout << "Hello World" << endl;
}
