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

  const string hexadecimal = "0x1p2 -0x1.8p+2 +0X1.4P3 ";
  memcpy(fastio::ibuf, hexadecimal.data(), hexadecimal.size());
  fastio::pil = 0, fastio::pir = uint32_t(hexadecimal.size());
  fastio::ibuf[fastio::pir] = '\x7f';
  float hex_float;
  double hex_double;
  long double hex_long_double;
  fastio::read(hex_float, hex_double, hex_long_double);
  assert(hex_float == 4 && hex_double == -6 && hex_long_double == 10);

  string s(1000, 'x');
  fastio::por = 0;
  fastio::write(s, ":", 1.25);
  string output(fastio::obuf, fastio::obuf + fastio::por);
  assert(output == s + ":1.250000000000000");
  fastio::por = 0;

  PRINT("Hello World");
}
