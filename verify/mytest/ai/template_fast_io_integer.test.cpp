#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#define FAST_IO

#include "template/template_inout.hpp"

// Test focus: FAST_IO handles the minimum signed 64/128-bit values, generic
// tuple/container input, vector/jagged macros, and exact mixed-output formatting.
int main()
{
  string input = "-9223372036854775808 -170141183460469231731687303715884105728 ";
  memset(fastio::ibuf, ' ', 200);
  memcpy(fastio::ibuf, input.data(), input.size());
  fastio::pil = 0, fastio::pir = 200;
  ll x;
  i128 y;
  fastio::rd1_integer(x);
  fastio::rd1_integer(y);
  assert(x == numeric_limits<ll>::lowest());
  assert(y == numeric_limits<i128>::lowest());

  fastio::por = 0;
  fastio::wt1_integer(x);
  string output(fastio::obuf, fastio::obuf + fastio::por);
  assert(output == "-9223372036854775808");
  fastio::por = 0;

  assert(stoi128(i128tos(y)) == y);

  string input2 = "1 2 3 4 5 6";
  memset(fastio::ibuf, ' ', 100);
  memcpy(fastio::ibuf, input2.data(), input2.size());
  fastio::pil = 0, fastio::pir = 100;
  tuple<int, int> t;
  array<int, 2> a;
  vc<int> v(2);
  fastio::read(t, a, v);
  assert((t == tuple{1, 2}));
  assert((a == array{3, 4}));
  assert(v == vc<int>({5, 6}));

  string input3 = "1 2 3 4 5 6";
  memset(fastio::ibuf, ' ', 100);
  memcpy(fastio::ibuf, input3.data(), input3.size());
  fastio::pil = 0, fastio::pir = 100;
  vc<int> v1, v2;
  internal::READVECnodump(3, v1, v2);
  assert(v1 == vc<int>({1, 2, 3}));
  assert(v2 == vc<int>({4, 5, 6}));

  string input4 = "2 1 2 1 3 1 4 2 5 6";
  memset(fastio::ibuf, ' ', 100);
  memcpy(fastio::ibuf, input4.data(), input4.size());
  fastio::pil = 0, fastio::pir = 100;
  vvc<int> jag1, jag2;
  internal::READJAGnodump(2, jag1, jag2);
  assert(jag1 == vvc<int>({{1, 2}, {3}}));
  assert(jag2 == vvc<int>({{4}, {5, 6}}));

  fastio::por = 0;
  fastio::write(tuple{1, 2}, ":", array{3, 4});
  fastio::print();
  fastio::print(5, "x", pair{6, 7});
  string output2(fastio::obuf, fastio::obuf + fastio::por);
  assert(output2 == "1 2:3 4\n5 x 6 7\n");
  fastio::por = 0;

  PRINT("Hello World");
}
