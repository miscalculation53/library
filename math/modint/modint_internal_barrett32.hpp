#pragma once

#include "../../template/template_all_but_modint.hpp"

namespace internal
{

struct barrett32
{
  uint m;
  ull im;
  explicit barrett32(uint m) : m(m), im((ull)(-1) / m + 1) {}
  uint umod() const { return m; }
  uint mul(uint a, uint b) const
  {
    ull z = a;
    z *= b;
    ull x = ull((u128(z) * im) >> 64);
    ull y = x * m;
    return uint(z - y + (z < y ? m : 0));
  }
};

template <int id>
struct policy_barrett32
{
  using value_type = uint;
  using calc_type = ull;
  using mod_type = int;
  
  static constexpr bool is_prime = false;
  static inline barrett32 reducer{998244353};
  static void set_mod(mod_type m) { reducer = barrett32(m); }
  static mod_type mod() { return reducer.umod(); }
  static value_type umod() { return reducer.umod(); }
  static value_type init(value_type v) { return v; }
  static mod_type val(value_type v) { return v; }
  static value_type mul(value_type a, value_type b) { return reducer.mul(a, b); }
};

};
