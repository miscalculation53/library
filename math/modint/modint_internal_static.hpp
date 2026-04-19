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
  using calc_type = larger_int_t<value_type>;

  static constexpr bool is_prime = isprime_constexpr(M);

  static constexpr mod_type mod() { return M; }
  static constexpr value_type umod() { return M; }

  static constexpr value_type init(value_type v) { return v; }
  static constexpr mod_type val(value_type v) { return v; }
  static constexpr value_type mul(value_type a, value_type b)
  {
    return (value_type)((calc_type(a) * b) % M);
  }
};

};
