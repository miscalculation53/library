#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief unordered_map の安全なハッシュ関数
 * @docs docs/ds/safe_unordered_map.md
 */

// https://xuzijian629.hatenablog.com/entry/2019/03/31/130708

struct safe_hash
{
private:
  static ull fixed_random()
  {
    static const ull seed = []
    {
      random_device rd;
      ull x = chrono::steady_clock::now().time_since_epoch().count();
      x ^= ull(rd()) << 32, x ^= ull(rd());
      x ^= ull(reinterpret_cast<uintptr_t>(&rd));
      return splitmix64(x);
    }();
    return seed;
  }

  static ull combine(ull h, ull x)
  {
    return splitmix64(h ^ (x + 0x9e3779b97f4a7c15ULL));
  }

  template <class Iter>
  size_t hash_range(Iter first, Iter last, size_t size) const
  {
    ull h = splitmix64(fixed_random() ^ size);
    for (; first != last; ++first)
      h = combine(h, operator()(*first));
    return h;
  }

public:
  // splitmix64 で x を攪拌する
  static ull splitmix64(ull x)
  {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  // std::hash<T> が定義された値をハッシュ化する
  template <class T>
  auto operator()(const T &x) const -> decltype(std::hash<T>{}(x), size_t{})
  {
    return splitmix64(ull(std::hash<T>{}(x)) ^ fixed_random());
  }

  // 128 bit 整数をハッシュ化する
  size_t operator()(u128 x) const
  {
    return combine(splitmix64(ull(x) ^ fixed_random()), ull(x >> 64));
  }
  size_t operator()(i128 x) const { return operator()(u128(x)); }

  // pair の各要素を順にハッシュ化する
  template <class T, class U>
  size_t operator()(const pair<T, U> &x) const
  {
    ull h = splitmix64(fixed_random() ^ 2);
    h = combine(h, operator()(x.first));
    return combine(h, operator()(x.second));
  }

  // tuple の各要素を順にハッシュ化する
  template <class... Ts>
  size_t operator()(const tuple<Ts...> &x) const
  {
    ull h = splitmix64(fixed_random() ^ sizeof...(Ts));
    apply([&](const auto &...xs) { ((h = combine(h, operator()(xs))), ...); }, x);
    return h;
  }

  // array の各要素を順にハッシュ化する
  template <class T, size_t n>
  size_t operator()(const array<T, n> &x) const
  {
    return hash_range(x.begin(), x.end(), n);
  }

  // vector の各要素を順にハッシュ化する
  template <class T, class Alloc>
  size_t operator()(const vector<T, Alloc> &x) const
  {
    return hash_range(x.begin(), x.end(), x.size());
  }

  // basic_string の文字列内容を秘密 base でハッシュ化する
  template <class Char, class Traits, class Alloc>
  size_t operator()(const basic_string<Char, Traits, Alloc> &x) const
  {
    ull h = splitmix64(fixed_random() ^ x.size());
    const ull base = splitmix64(fixed_random() ^ 0x243f6a8885a308d3ULL) | 1;
    for (Char c : x)
      h = h * base + ull(make_unsigned_t<Char>(c)) + 1;
    return splitmix64(h);
  }

  // basic_string_view の文字列内容を秘密 base でハッシュ化する
  template <class Char, class Traits>
  size_t operator()(basic_string_view<Char, Traits> x) const
  {
    ull h = splitmix64(fixed_random() ^ x.size());
    const ull base = splitmix64(fixed_random() ^ 0x243f6a8885a308d3ULL) | 1;
    for (Char c : x)
      h = h * base + ull(make_unsigned_t<Char>(c)) + 1;
    return splitmix64(h);
  }

  // safe_hash_key(x) が返す代表値をハッシュ化する
  template <class T>
  auto operator()(const T &x) const -> decltype(safe_hash_key(x), size_t{})
  {
    return operator()(safe_hash_key(x));
  }
};
