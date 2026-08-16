#include "template/template_all_but_modint.hpp"

#include "ds/hash_map.hpp"

#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;

using GPResize = hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<>, true>;

template <class Key>
using GPHashMap = gp_hash_table<Key, ll, safe_hash, equal_to<Key>, direct_mask_range_hashing<>, linear_probe_fn<>, GPResize>;

template <class Key>
struct StdMap
{
  unordered_map<Key, ll, safe_hash> mp;
  void reserve(size_t n) { mp.reserve(n); }
  ll &operator[](const Key &key) { return mp[key]; }
  const ll *find_ptr(const Key &key) const
  {
    auto it = mp.find(key);
    return it == mp.end() ? nullptr : &it->second;
  }
  bool erase(const Key &key) { return mp.erase(key); }
};

template <class Key>
struct GPMap
{
  GPHashMap<Key> mp;
  void reserve(size_t n) { mp.resize(bit_ceil(max<size_t>(8, 2 * n))); }
  ll &operator[](const Key &key) { return mp[key]; }
  const ll *find_ptr(const Key &key) const
  {
    auto it = mp.find(key);
    return it == mp.end() ? nullptr : &it->second;
  }
  bool erase(const Key &key) { return mp.erase(key); }
};

template <class Key>
struct OwnMap
{
  HashMap<Key, ll> mp;
  void reserve(size_t n) { mp.reserve(n); }
  ll &operator[](const Key &key) { return mp[key]; }
  const ll *find_ptr(const Key &key) const { return mp.find_ptr(key); }
  bool erase(const Key &key) { return mp.erase(key); }
};

struct Result
{
  double reserve_ms, insert_ms, hit_ms, miss_ms, erase_ms;
  ull checksum;
};

volatile ull benchmark_sink = 0;

template <template <class> class Map, class Key>
Result run(const vc<Key> &keys, const vc<int> &hit_indices, const vc<Key> &misses, int erase_count)
{
  Map<Key> mp;
  auto tp = chrono::steady_clock::now();
  mp.reserve(keys.size());
  auto t0 = chrono::steady_clock::now();
  repi(i, keys.size()) mp[keys[i]] = i + 1;
  auto t1 = chrono::steady_clock::now();

  ull checksum = 0;
  for (int i : hit_indices)
  {
    const ll *p = mp.find_ptr(keys[i]);
    assert(p != nullptr);
    checksum += *p;
  }
  auto t2 = chrono::steady_clock::now();

  for (const Key &key : misses)
    checksum += mp.find_ptr(key) != nullptr;
  auto t3 = chrono::steady_clock::now();

  repi(i, erase_count) checksum += mp.erase(keys[i]);
  repi(i, erase_count) assert(mp.find_ptr(keys[i]) == nullptr);
  auto t4 = chrono::steady_clock::now();
  benchmark_sink = benchmark_sink ^ checksum;

  return {
      chrono::duration<double, milli>(t0 - tp).count(),
      chrono::duration<double, milli>(t1 - t0).count(),
      chrono::duration<double, milli>(t2 - t1).count(),
      chrono::duration<double, milli>(t3 - t2).count(),
      chrono::duration<double, milli>(t4 - t3).count(),
      checksum};
}

template <class Key>
void benchmark(string name, const vc<Key> &keys, const vc<int> &hit_indices, const vc<Key> &misses)
{
  constexpr int rounds = 3;
  constexpr int erase_count = 100'000;
  array<vc<Result>, 3> results;
  repi(_, rounds)
  {
    results[0].eb(run<StdMap>(keys, hit_indices, misses, erase_count));
    results[1].eb(run<GPMap>(keys, hit_indices, misses, erase_count));
    results[2].eb(run<OwnMap>(keys, hit_indices, misses, erase_count));
  }
  cout << name << " (" << keys.size() << " insert, " << hit_indices.size() << " hit, "
       << misses.size() << " miss, " << erase_count << " erase)\n";
  const array<string, 3> names = {"unordered_map", "gp_hash_table", "HashMap"};
  repi(i, 3)
  {
    for (const Result &result : results[i])
      if (result.checksum != results[0][0].checksum)
        abort();
    auto median = [&](auto member)
    {
      array<double, rounds> values;
      repi(j, rounds) values[j] = results[i][j].*member;
      sort(ALL(values));
      return values[rounds / 2];
    };
    cout << left << setw(16) << names[i] << right << fixed << setprecision(2)
         << " reserve " << setw(8) << median(&Result::reserve_ms)
         << "  insert " << setw(8) << median(&Result::insert_ms)
         << "  hit " << setw(8) << median(&Result::hit_ms)
         << "  miss " << setw(8) << median(&Result::miss_ms)
         << "  erase " << setw(8) << median(&Result::erase_ms) << " ms\n";
  }
}

int main()
{
  constexpr int n = 500'000, q_hit = 2'000'000, q_miss = 1'000'000;
  mt19937 rng(123456789);
  vc<int> hit_indices(q_hit);
  for (int &i : hit_indices) i = rng() % n;

  vc<ull> keys(n), misses(q_miss);
  repi(i, n) keys[i] = safe_hash::splitmix64(i + 0x12345678ULL);
  repi(i, q_miss) misses[i] = safe_hash::splitmix64(i + 0x9abcdef0ULL);
  benchmark("uint64 key", keys, hit_indices, misses);

  vc<pair<ull, ull>> pair_keys(n), pair_misses(q_miss);
  repi(i, n) pair_keys[i] = {keys[i], safe_hash::splitmix64(keys[i])};
  repi(i, q_miss) pair_misses[i] = {misses[i], safe_hash::splitmix64(misses[i])};
  benchmark("pair<uint64,uint64> key", pair_keys, hit_indices, pair_misses);
}
