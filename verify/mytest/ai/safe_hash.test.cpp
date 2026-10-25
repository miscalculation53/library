#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#include "ds/hash_map.hpp"
#include "ds/safe_unordered_map.hpp"
#include "math/bigint.hpp"
#include "math/modint/modint.hpp"
#include "math/modint/modint_div.hpp"
#include "math/rational.hpp"
#include "string/rolling_hash.hpp"

// Test focus: safe_hash supports nested standard composite keys and adversarial integer patterns.
void test()
{
  unordered_map<pair<ll, ll>, int, safe_hash> pairs;
  unordered_map<tuple<int, string, pair<ll, ll>>, int, safe_hash> tuples;
  unordered_map<array<int, 4>, int, safe_hash> arrays;
  unordered_map<vector<pair<int, int>>, int, safe_hash> vectors;
  unordered_map<string, int, safe_hash> strings;
  unordered_map<string_view, int, safe_hash> string_views;
  unordered_map<i128, int, safe_hash> int128s;

  repi(i, 1000)
  {
    pairs[{i, i * i}] = i;
    tuples[{i, to_string(i), {i + 1, i - 1}}] = i;
    arrays[{{i, i + 1, i + 2, i + 3}}] = i;
    vectors[{{i, -i}, {i + 1, i * 2}}] = i;
    strings[to_string(i) + "#safe-hash"] = i;
    int128s[i128(i) << 100] = i;
  }
  repi(i, 1000)
  {
    assert(pairs.at({i, i * i}) == i);
    assert(tuples.at({i, to_string(i), {i + 1, i - 1}}) == i);
    assert((arrays.at({{i, i + 1, i + 2, i + 3}}) == i));
    assert((vectors.at({{i, -i}, {i + 1, i * 2}}) == i));
    assert(strings.at(to_string(i) + "#safe-hash") == i);
    assert(int128s.at(i128(i) << 100) == i);
  }

  const string text = "string-view-safe-hash";
  string_views[string_view(text)] = 42;
  assert(string_views.at(string_view(text)) == 42);

  unordered_map<ull, int, safe_hash> adversarial;
  adversarial.reserve(10000);
  repi(i, 10000) adversarial[ull(i) * 107897] = i;
  repi(i, 10000) assert(adversarial.at(ull(i) * 107897) == i);
}

// Test focus: library value types use equality-compatible canonical hash keys.
void test_library_types()
{
  using mint = modint998244353;

  unordered_map<mint, int, safe_hash> mints;
  mints[mint(-1)] = 1;
  assert(mints.at(mint(998244352)) == 1);

  unordered_map<Rational<ll>, int, safe_hash> rationals;
  rationals[Rational<ll>(2, 4)] = 2;
  assert(rationals.at(Rational<ll>(-3, -6)) == 2);

  unordered_map<Rational<mint>, int, safe_hash> rational_mints;
  rational_mints[Rational<mint>(2, 4)] = 3;
  assert(rational_mints.at(Rational<mint>(3, 6)) == 3);

  unordered_map<RollingHashValue<mint, 0>, int, safe_hash> rolling_hashes;
  rolling_hashes[RollingHashValue<mint, 0>(string("abc"))] = 4;
  assert(rolling_hashes.at(RollingHashValue<mint, 0>(string("abc"))) == 4);

  unordered_map<ModintDiv<mint>, int, safe_hash> modint_divs;
  modint_divs[ModintDiv<mint>(mint(5), -2)] = 5;
  assert(modint_divs.at(ModintDiv<mint>(mint(5), -2)) == 5);

  unordered_map<BigInteger<>, int, safe_hash> big_integers;
  big_integers[BigInteger<>("-123456789012345678901234567890")] = 6;
  assert(big_integers.at(BigInteger<>("-123456789012345678901234567890")) == 6);

  HashMap<Rational<mint>, int> hash_map;
  hash_map[Rational<mint>(5, 10)] = 7;
  assert(hash_map.at(Rational<mint>(1, 2)) == 7);
}

int main()
{
  test();
  test_library_types();
  cout << "Hello World" << endl;
}
