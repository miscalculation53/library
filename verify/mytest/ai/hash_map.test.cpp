#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "template/template_all_but_modint.hpp"

#include "ds/hash_map.hpp"

struct IdentityHash
{
  size_t operator()(int x) const { return x; }
};

// Test focus: insertion, lookup, overwrite, reserve, and backward-shift deletion match std::unordered_map.
void test()
{
  mt19937_64 rng(123456789);
  HashMap<pair<ll, ll>, ll> mp;
  unordered_map<pair<ll, ll>, ll, safe_hash> expected;
  mp.reserve(1000), expected.reserve(1000);

  repi(iter, 200000)
  {
    pair<ll, ll> key = {ll(rng() % 128), ll(rng() % 128)};
    int type = rng() % 5;
    if (type <= 1)
    {
      ll value = rng();
      bool a = mp.insert_or_assign(key, value);
      bool b = expected.insert_or_assign(key, value).second;
      assert(a == b);
    }
    else if (type == 2)
    {
      assert(mp.erase(key) == (expected.erase(key) != 0));
    }
    else if (type == 3)
    {
      mp[key]++;
      expected[key]++;
    }
    else
    {
      auto p = mp.find_ptr(key);
      auto it = expected.find(key);
      assert((p == nullptr) == (it == expected.end()));
      if (p)
        assert(*p == it->second);
    }
    assert(mp.size() == expected.size());
  }

  for (const auto &[key, value] : expected)
    assert(mp.at(key) == value);
  mp.clear();
  assert(mp.empty());

  HashMap<int, string, IdentityHash> wrap;
  wrap.reserve(4);
  wrap[6] = "six", wrap[14] = "fourteen", wrap[22] = "twenty-two";
  assert(wrap.erase(14));
  assert(wrap.at(6) == "six" && wrap.at(22) == "twenty-two");
  assert(!wrap.contains(14));
}

int main()
{
  test();
  cout << "Hello World" << endl;
}
