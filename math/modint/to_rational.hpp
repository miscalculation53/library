#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "../svp2d.hpp"

/**
 * @brief modint を有理数に
 * @docs docs/math/modint/to_rational.md
 */

template <class mint>
pair<decltype(mint(0).val()), decltype(mint(0).val())>
mint_to_rat(const mint &x)
{
  auto [p, q] = svp2d({x.val(), 1}, {mint::mod(), 0});
  if (q < 0)
    p = -p, q = -q;
  return {p, q};
}

#if __cplusplus >= 202302L
namespace cpp_dump
{
  struct mint_to_rat_fn
  {
    template <class T>
    constexpr auto operator()(const T &x) const -> decltype(mint_to_rat(x))
    {
      return mint_to_rat(x);
    }
  };

  struct rat_closure : std::ranges::range_adaptor_closure<rat_closure>
  {
    template <typename T>
    constexpr auto operator()(T &&t) const
    {
      if constexpr (!std::ranges::range<T> && std::invocable<mint_to_rat_fn, decltype(std::forward<T>(t))>)
      {
        return mint_to_rat_fn{}(std::forward<T>(t));
      }
      else if constexpr (std::ranges::range<T>)
      {
        using Ref = std::ranges::range_reference_t<T>;

        if constexpr (std::invocable<mint_to_rat_fn, decltype(std::forward<Ref>(std::declval<Ref>()))>)
        {
          return std::forward<T>(t) | std::views::transform(mint_to_rat_fn{});
        }
        else if constexpr (std::ranges::range<Ref>)
        {
          return std::forward<T>(t) | std::views::transform([this](auto &&inner)
                                                            { return (*this)(std::forward<decltype(inner)>(inner)); });
        }
        else
        {
          static_assert(false);
        }
      }
      else
      {
        static_assert(false);
      }
    }
  };

  template <typename T>
    requires(!std::ranges::range<T> && std::invocable<mint_to_rat_fn, T>)
  constexpr auto operator|(T &&t, const rat_closure &c)
  {
    return c(std::forward<T>(t));
  }

  constexpr rat_closure rat()
  {
    return rat_closure{};
  }
}
#endif
