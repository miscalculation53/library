#include "ds/hash_set.hpp"

struct Result
{
  double init_ms, merge_ms;
  size_t size;
};

// UnionFind と同じ形で、隣接する成分を順に2倍ずつ併合する。
template <class Set>
Result run(const vl &keys)
{
  auto t0 = chrono::steady_clock::now();
  vc<Set> sets(keys.size());
  repi(i, keys.size()) sets[i].insert(keys[i]);
  auto t1 = chrono::steady_clock::now();
  for (int step = 1; step < SZ<int>(keys); step *= 2)
    for (int i = 0; i + step < SZ<int>(keys); i += 2 * step)
    {
      auto &a = sets[i], &b = sets[i + step];
      if (a.size() < b.size()) a.swap(b);
      if constexpr (is_same_v<Set, set<ll>>)
        a.merge(b);
      else
        for (ll key : b) a.insert(key);
      b.clear();
    }
  auto t2 = chrono::steady_clock::now();
  return {chrono::duration<double, milli>(t1 - t0).count(),
          chrono::duration<double, milli>(t2 - t1).count(), sets[0].size()};
}

void benchmark(const string &name, const vl &keys)
{
  constexpr int rounds = 3;
  array<vc<Result>, 3> results;
  repi(_, rounds)
  {
    results[0].push_back(run<set<ll>>(keys));
    results[1].push_back(run<unordered_set<ll, safe_hash>>(keys));
    results[2].push_back(run<HashSet<ll>>(keys));
  }
  const array<string, 3> names = {"set", "unordered_set", "HashSet"};
  cout << name << " (" << keys.size() << " vertices)\n";
  repi(i, 3)
  {
    for (const Result &r : results[i])
      assert(r.size == results[0][0].size);
    auto median = [&](auto member)
    {
      array<double, rounds> values;
      repi(j, rounds) values[j] = results[i][j].*member;
      sort(ALL(values));
      return values[rounds / 2];
    };
    cout << left << setw(16) << names[i] << right << fixed << setprecision(2)
         << " init " << setw(8) << median(&Result::init_ms)
         << "  merge " << setw(8) << median(&Result::merge_ms) << " ms\n";
  }
}

int main()
{
  constexpr int n = 200000;
  vl keys(n);
  iota(ALL(keys), 0LL);
  benchmark("distinct keys", keys);
  for (ll &key : keys) key %= 1000;
  benchmark("1000 distinct keys", keys);
}
