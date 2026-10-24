#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"
#define FAST_IO

#include "template/template_inout.hpp"
#include <unistd.h>

// 実ファイルを使い、長い空白・各桁でのバッファ境界・EOF・大量出力を確認する。
string decimal(u128 x)
{
  string s;
  do {
    s += char('0' + x % 10);
    x /= 10;
  } while (x);
  reverse(s.begin(), s.end());
  return s;
}

int main()
{
  string input;
  vector<function<void()>> checks;
  auto add = [&](auto expected, const string &token)
  {
    input += token;
    input += ' ';
    checks.push_back([expected]
    {
      decltype(expected) actual;
      fastio::read(actual);
      assert(actual == expected);
    });
  };
  input = "1" + string(2 * fastio::SIZ + 137, ' ') + "2 ";
  checks.push_back([]
  {
    int a, b;
    fastio::read(a, b);
    assert(a == 1 && b == 2);
  });
  const i128 imin = numeric_limits<i128>::lowest();
  const u128 umax = numeric_limits<u128>::max();
  for (uint k = 0; k <= 40; ++k)
  {
    // 符号や数字のどの位置で補充が起きても同じ値を得る。
    input += string((fastio::SIZ - k + fastio::SIZ - input.size() % fastio::SIZ) % fastio::SIZ, ' ');
    add(imin, "-170141183460469231731687303715884105728");
    input += string((fastio::SIZ - k + fastio::SIZ - input.size() % fastio::SIZ) % fastio::SIZ, ' ');
    add(umax, "340282366920938463463374607431768211455");
  }
  add(numeric_limits<int>::lowest(), "-2147483648");
  add(numeric_limits<int>::max(), "+2147483647");
  add(numeric_limits<ll>::lowest(), "-9223372036854775808");
  add(numeric_limits<ll>::max(), "+9223372036854775807");
  add(numeric_limits<ull>::max(), "18446744073709551615");
  add(short(-32768), "-32768");
  add((unsigned short)65535, "65535");
  add((signed char)-128, "-128");
  add((unsigned char)255, "255");
  add(false, "0");
  add(true, "1");
  mt19937_64 rng(781244);
  for (int i = 0; i < 20000; ++i)
  {
    const u128 x = u128(rng()) << 64 | rng();
    add(x, decimal(x));
    const i128 y = i128(x & u128(numeric_limits<i128>::max()));
    add(-y, "-" + decimal(u128(y)));
  }
  input += "12+345 0.5 ";
  checks.push_back([]
  {
    int a, b;
    char op;
    float x;
    fastio::read(a, op, b, x);
    assert(a == 12 && op == '+' && b == 345 && x == 0.5f);
  });
  const string long_token(2 * fastio::SIZ + 17, 'x');
  const string utf8 = "日本語";
  input += long_token + "\t" + utf8 + "\r\n42";
  checks.push_back([&]
  {
    string a, b;
    int x;
    fastio::read(a, b, x);
    assert(a == long_token && b == utf8 && x == 42);
  });

  FILE *in = tmpfile();
  assert(in);
  assert(fwrite(input.data(), 1, input.size(), in) == input.size());
  rewind(in);
  const int saved_in = dup(fileno(stdin));
  assert(saved_in >= 0 && dup2(fileno(in), fileno(stdin)) >= 0);
  clearerr(stdin);
  for (const auto &check : checks) check();
  assert(fastio::pil == fastio::pir);
  assert(dup2(saved_in, fileno(stdin)) >= 0);
  close(saved_in);
  fclose(in);
  clearerr(stdin);

  // 番兵を含む満杯のバッファと、重なる範囲の移動を確認する。
  for (uint32_t i = 0; i < fastio::SIZ; ++i) fastio::ibuf[i] = char('a' + i % 26);
  const string tail(fastio::ibuf + 1, fastio::ibuf + fastio::SIZ);
  fastio::pil = 1, fastio::pir = fastio::SIZ;
  // 読み足す分も tmpfile から与える。
  in = tmpfile();
  assert(in && fputc('z', in) != EOF);
  const string final_token(fastio::SIZ, 'x');
  assert(fwrite(final_token.data(), 1, final_token.size(), in) == final_token.size());
  rewind(in);
  const int saved_in2 = dup(fileno(stdin));
  assert(saved_in2 >= 0 && dup2(fileno(in), fileno(stdin)) >= 0);
  fastio::load();
  assert(fastio::pir == fastio::SIZ);
  assert(string(fastio::ibuf, fastio::ibuf + fastio::SIZ - 1) == tail);
  assert(fastio::ibuf[fastio::SIZ - 1] == 'z');
  assert(fastio::ibuf[fastio::SIZ] == '\x7f');
  fastio::pil = fastio::pir;
  string final_actual;
  fastio::read(final_actual);
  assert(final_actual == final_token && fastio::pil == fastio::pir);
  assert(dup2(saved_in2, fileno(stdin)) >= 0);
  close(saved_in2);
  fclose(in);
  clearerr(stdin);

  FILE *out_file = tmpfile();
  assert(out_file);
  fflush(stdout);
  const int saved_out = dup(fileno(stdout));
  assert(saved_out >= 0 && dup2(fileno(out_file), fileno(stdout)) >= 0);
  string expected;
  fastio::print((signed char)-128, (unsigned char)255, short(-32768), (unsigned short)65535);
  expected += "-128 255 -32768 65535\n";
  for (u128 x = 1;; x *= 10)
  {
    for (u128 y : {x - 1, x, x + 1})
    {
      fastio::print(y);
      expected += decimal(y) + '\n';
    }
    fastio::print(-i128(x));
    expected += '-' + decimal(x) + '\n';
    if (x > umax / 10) break;
  }
  for (int x = 0; x < 10000; ++x)
  {
    fastio::print(x, -x);
    expected += to_string(x) + ' ' + to_string(-x) + '\n';
  }
  for (int i = 0; i < 10000; ++i)
  {
    const u128 x = u128(rng()) << 64 | rng();
    fastio::print(x, imin, true, false);
    expected += decimal(x) + " -170141183460469231731687303715884105728 1 0\n";
    const ll y = ll(rng() >> 1);
    fastio::print(y, -y);
    expected += to_string(y) + ' ' + to_string(-y) + '\n';
  }
  fastio::write(string_view{}, long_token, utf8, numeric_limits<ll>::lowest(), imin, umax);
  expected += long_token + utf8 + "-9223372036854775808-170141183460469231731687303715884105728340282366920938463463374607431768211455";
  fastio::flush();
  fflush(stdout);
  rewind(out_file);
  string actual(expected.size() + 1, '\0');
  const size_t bytes = fread(actual.data(), 1, actual.size(), out_file);
  actual.resize(bytes);
  assert(actual == expected);
  assert(dup2(saved_out, fileno(stdout)) >= 0);
  close(saved_out);
  fclose(out_file);
  PRINT("Hello World");
}
