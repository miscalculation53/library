#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "ds/default_map.hpp"

vl dflt() { return {100, 200}; }

// Test focus: const lookup returns the generated default without inserting a key.
int main()
{
  const DefaultMap<ll, vl, dflt> mp;
  vl a = mp[0];
  vl b = mp.at(0);
  assert((a == vl{100, 200}));
  assert((b == vl{100, 200}));

  const DefaultUnorderedMap<ll, vl, dflt> ump;
  assert((ump[0] == vl{100, 200}));

  PRINT("Hello World");
}
