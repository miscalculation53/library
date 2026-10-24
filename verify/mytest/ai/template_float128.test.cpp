#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#define FAST_IO

#include "template/template_inout.hpp"
#include "template/template_math.hpp"

// 同じソースを C++17 / C++20 / C++23 で検証する。
static_assert(is_same_v<decltype(sqrt(f128(2))), f128>);
static_assert(is_same_v<decltype(abs(f128(-2))), f128>);
static_assert(is_same_v<decltype(pow(f128(2), f128(3))), f128>);
static_assert(numeric_limits<f128>::digits == 113);
static_assert(!is_same_v<f128, long double>);
static_assert(is_same_v<decltype(pow(f128(2), 3)), f128>);
static_assert(is_same_v<decltype(pow(2, f128(3))), f128>);
static_assert(is_same_v<decltype(fma(f128(1), 2, 3)), f128>);
static_assert(is_same_v<decltype(fma(1, f128(2), 3)), f128>);
static_assert(is_same_v<decltype(fma(1, 2, f128(3))), f128>);

void stream_test()
{
  f128 a, b, c, d;
  istringstream input("+1.25 -2.5e+4 -0 0.125");
  input >> a >> b >> c >> d;
  assert(!input.fail());
  assert(a == f128(5) / 4 && b == -25000 && c == 0 && signbit(c));
  assert(d == f128(1) / 8);

  ostringstream output;
  output << fixed << setprecision(30) << a;
  assert(output.str() == "1.250000000000000000000000000000");
  output.str("");
  output << defaultfloat << setprecision(6) << a;
  assert(output.str() == "1.25");
  output.str("");
  output << scientific << setprecision(2) << f128(125);
  assert(output.str() == "1.25e+02");
  output.str("");
  output << showpos << uppercase << a;
  assert(output.str() == "+1.25E+00");

  // 固定小数点で 128 文字を超える値も、精度を維持して出力する。
  const f128 large = numeric_limits<f128>::max();
  output.str("");
  output << noshowpos << fixed << setprecision(0) << large;
  assert(output.str().size() == size_t(numeric_limits<f128>::max_exponent10 + 1));
  istringstream large_input(output.str());
  large_input >> a;
  assert(!large_input.fail() && a == large);

  a = 7;
  istringstream empty_input;
  empty_input >> a;
  assert(empty_input.fail() && a == 7);
}

void fastio_test()
{
  const string input = "+1.25 -2.5e+4 0.125 8 ";
  memcpy(fastio::ibuf, input.data(), input.size());
  fastio::pil = 0, fastio::pir = uint32_t(input.size());
  fastio::ibuf[fastio::pir] = '\x7f';
  vector<f128> a(2);
  pair<f128, f128> b;
  READ(a, b);
  assert(a[0] == f128(5) / 4 && a[1] == -25000);
  assert(b.first == f128(1) / 8 && b.second == 8);

  fastio::por = 0;
  WRITE(a[0]);
  const int precision = 35;
  assert(string(fastio::obuf, fastio::por) == "1.25" + string(precision - 2, '0'));
  fastio::por = 0;
  WRITE(numeric_limits<f128>::max());
  assert(fastio::por == uint32_t(numeric_limits<f128>::max_exponent10 + 2 + precision));
  fastio::por = 0;
}

void precision_test()
{
  const f128 large = ldexp(f128(1), 100);
  assert((large + 1) - large == 1);
  const f128 root = sqrt(f128(2));
  assert(abs(root * root - 2) <= 4 * numeric_limits<f128>::epsilon());
  assert(floor(root) == 1);

  f128 x;
  istringstream input("1.000000000000000000000000000001");
  input >> x;
  assert(x > 1 && x - 1 < f128(1e-29L));
  ostringstream output;
  output << setprecision(numeric_limits<f128>::max_digits10) << x;
  f128 y;
  istringstream roundtrip(output.str());
  roundtrip >> y;
  assert(y == x);

  fastio::por = 0;
  WRITE(x);
  istringstream fastio_output(string(fastio::obuf, fastio::por));
  fastio_output >> y;
  assert(y == x);
  fastio::por = 0;

  const string precise_input = "1.000000000000000000000000000001 1e-4000 ";
  memcpy(fastio::ibuf, precise_input.data(), precise_input.size());
  fastio::pil = 0, fastio::pir = uint32_t(precise_input.size());
  fastio::ibuf[fastio::pir] = '\x7f';
  f128 a, b;
  READ(a, b);
  assert(a == x && b > 0 && b < f128(1e-300L));

  input.clear();
  input.str("1e-4000");
  input >> x;
  assert(!input.fail() && x > 0);
  for (const char *s : {"bad", "1.2x", "1e99999", "+-1"})
  {
    input.clear();
    input.str(s);
    x = 7;
    input >> x;
    assert(input.fail() && x == 7);
  }

  const f128 subnormal = numeric_limits<f128>::denorm_min();
  output.str("");
  output << scientific << setprecision(numeric_limits<f128>::max_digits10) << subnormal;
  input.clear();
  input.str(output.str());
  input >> x;
  assert(!input.fail() && x == subnormal);
}

void math_test()
{
  const f128 eps = 128 * numeric_limits<f128>::epsilon();
  const f128 pi = acos(f128(-1));
  assert(abs(sin(pi / 6) - f128(0.5L)) < eps);
  assert(abs(cos(pi / 3) - f128(0.5L)) < eps);
  assert(abs(tan(pi / 4) - 1) < eps);
  assert(abs(asin(f128(0.5L)) - pi / 6) < eps);
  assert(abs(atan(f128(1)) - pi / 4) < eps);
  assert(abs(atan2(f128(1), 1) - pi / 4) < eps);
  assert(abs(asinh(sinh(f128(1))) - 1) < eps);
  assert(abs(acosh(cosh(f128(1))) - 1) < eps);
  assert(abs(atanh(tanh(f128(1))) - 1) < eps);
  assert(abs(exp(log(f128(3))) - 3) < eps);
  assert(abs(exp2(log2(f128(3))) - 3) < eps);
  assert(abs(log10(f128(1000)) - 3) < eps);
  const f128 tiny = ldexp(f128(1), -100);
  assert(abs(expm1(log1p(tiny)) - tiny) <= tiny * eps);
  assert(cbrt(f128(27)) == 3 && pow(f128(3), 4) == 81 && pow(3, f128(4)) == 81);
  assert(abs(erf(f128(1)) + erfc(f128(1)) - 1) < eps);
  assert(tgamma(f128(5)) == 24 && abs(lgamma(f128(5)) - log(f128(24))) < eps);
  assert(hypot(f128(3), 4) == 5);
  assert(hypot(f128(2), f128(3), f128(6)) == 7);
  assert(hypot(f128(2), 3, 6) == 7 && hypot(2, f128(3), 6) == 7 && hypot(2, 3, f128(6)) == 7);
  assert(fma(f128(2), 3, 4) == 10 && fma(2, f128(3), 4) == 10 && fma(2, 3, f128(4)) == 10);

  const f128 x = 1 + ldexp(f128(1), -60);
  assert(fma(x, x, -(1 + ldexp(f128(1), -59))) == ldexp(f128(1), -120));
  assert(floor(f128(-1.25L)) == -2 && ceil(f128(-1.25L)) == -1);
  assert(trunc(f128(-1.25L)) == -1 && round(f128(-1.5L)) == -2);
  assert(lround(f128(-1.5L)) == -2 && llround(f128(1.5L)) == 2);
  assert(nearbyint(f128(1.25L)) == 1 && rint(f128(1.25L)) == 1);
  assert(lrint(f128(1.25L)) == 1 && llrint(f128(1.25L)) == 1);
  int exponent;
  assert(frexp(ldexp(f128(0.75L), 10000), &exponent) == f128(0.75L) && exponent == 10000);
  assert(scalbn(f128(0.75L), 10000) == ldexp(f128(0.75L), 10000));
  assert(scalbln(f128(0.75L), 10000) == ldexp(f128(0.75L), 10000));
  assert(ilogb(f128(8)) == 3 && logb(f128(8)) == 3);
  f128 integer;
  assert(modf(f128(-1.25L), &integer) == f128(-0.25L) && integer == -1);
  int quotient;
  assert(remquo(f128(7), f128(2), &quotient) == -1 && (quotient & 7) == 4);
  assert(remainder(f128(7), 2) == -1 && fmod(f128(7), 2) == 1);
  assert(fdim(f128(7), 2) == 5 && fmax(f128(7), 2) == 7 && fmin(f128(7), 2) == 2);
  assert(copysign(f128(7), -1) == -7);
  assert(nextafter(f128(1), 2) - 1 == numeric_limits<f128>::epsilon());
  assert(nexttoward(f128(1), 2.0L) == nextafter(f128(1), 2));
  const f128 inf = numeric_limits<f128>::infinity(), nan = numeric_limits<f128>::quiet_NaN();
  assert(isfinite(f128(1)) && !isfinite(inf) && isinf(inf) && isnan(nan));
  assert(isnormal(f128(1)) && !isnormal(numeric_limits<f128>::denorm_min()));
  assert(fpclassify(numeric_limits<f128>::denorm_min()) == FP_SUBNORMAL);
}

int main()
{
  stream_test();
  fastio_test();
  precision_test();
  math_test();
  PRINT("Hello World");
}
