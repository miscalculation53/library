#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#define FAST_IO

#include "template/template_inout.hpp"

// Test focus: FAST_IO reads ASCII-whitespace-delimited strings and reals, and
// writes strings in bulk while preserving fixed 15-digit real formatting.
int main()
{
  string input = " \talpha\nbeta -1.25 2.5 ";
  memset(fastio::ibuf, ' ', 100);
  memcpy(fastio::ibuf, input.data(), input.size());
  fastio::pil = 0, fastio::pir = 100;

  string a, b;
  double x;
  long double y;
  fastio::read(a, b, x, y);
  assert(a == "alpha");
  assert(b == "beta");
  assert(x == -1.25);
  assert(y == 2.5L);

  string s(1000, 'x');
  fastio::por = 0;
  fastio::write(s, ":", 1.25);
  string output(fastio::obuf, fastio::obuf + fastio::por);
  assert(output == s + ":1.250000000000000");
  fastio::por = 0;

  PRINT("Hello World");
}
