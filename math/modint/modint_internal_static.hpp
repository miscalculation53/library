#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../../utils/larger_int.hpp"
#include "modint_internal_isprime.hpp"

namespace internal
{

template <auto M>
struct policy_static
{
  using mod_type = decltype(M);
  using value_type = make_unsigned_t<mod_type>;
  using calc_type = conditional_t<(M <= UINT_MAX), ull, larger_int_t<value_type>>;

  static constexpr bool is_prime = isprime_constexpr(M);

  static constexpr mod_type mod() { return M; }
  static constexpr value_type umod() { return M; }

  static constexpr value_type init(value_type v) { return v; }
  static constexpr mod_type val(value_type v) { return v; }
  static constexpr value_type mul(value_type a, value_type b)
  {
    const calc_type product = calc_type(a) * b;
    if constexpr (M == (1LL << 61) - 1)
    {
      const value_type reduced = (value_type(product) & umod()) + value_type(product >> 61);
      return reduced >= umod() ? reduced - umod() : reduced;
    }
    else
      return value_type(product % M);
  }
};

};
