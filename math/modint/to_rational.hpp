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
  template <class T>
  struct rat_value
  {
    T p, q;

    friend ostream &operator<<(ostream &os, const rat_value &x)
    {
      os << x.p;
      if (x.q != 1)
        os << '/' << x.q;
      return os;
    }
  };

  struct mint_to_rat_fn
  {
    template <class T>
      requires requires(const T &x)
      {
        { T::mod() } -> std::convertible_to<ll>;
        { x.val() } -> std::convertible_to<ll>;
      }
    constexpr auto operator()(const T &x) const
        -> rat_value<typename decltype(mint_to_rat(x))::first_type>
    {
      auto [p, q] = mint_to_rat(x);
      return {p, q};
    }
  };

  template <class T>
  struct rat_object
  {
    T value;
  };

  template <class T>
  inline constexpr bool rat_is_map = false;
  template <class... Args>
  inline constexpr bool rat_is_map<std::map<Args...>> = true;
  template <class... Args>
  inline constexpr bool rat_is_map<std::multimap<Args...>> = true;
  template <class... Args>
  inline constexpr bool rat_is_map<std::unordered_map<Args...>> = true;
  template <class... Args>
  inline constexpr bool rat_is_map<std::unordered_multimap<Args...>> = true;

  struct rat_closure : std::ranges::range_adaptor_closure<rat_closure>
  {
    template <class T>
    static constexpr bool can_convert()
    {
#ifdef LOCAL
      if constexpr (_detail::is_value_with_command<T>)
        return can_convert<decltype(std::declval<T>().value)>();
      else
#endif
      if constexpr (std::ranges::range<T>)
      {
        using Ref = std::ranges::range_reference_t<T>;
        if constexpr (std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<Ref>>)
          return false;
        else
          return can_convert<Ref>();
      }
      else if constexpr (requires { std::tuple_size<std::remove_cvref_t<T>>::value; })
      {
        return []<size_t... I>(std::index_sequence<I...>)
        {
          if constexpr ((requires { std::get<I>(std::declval<T>()); } && ...))
            return (can_convert<decltype(std::get<I>(std::declval<T>()))>() || ...);
          else
            return false;
        }(std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<T>>>{});
      }
      else
        return std::invocable<mint_to_rat_fn, T>;
    }

    // 範囲アダプタ同士の合成は標準ライブラリの operator| に任せる。
    template <typename T>
      requires(!requires { std::views::all | std::declval<T>(); })
    constexpr decltype(auto) operator()(T &&t) const
    {
      if constexpr (!std::ranges::range<T> && std::invocable<mint_to_rat_fn, T>)
      {
        return mint_to_rat_fn{}(std::forward<T>(t));
      }
      else if constexpr (can_convert<T>())
      {
#ifdef LOCAL
        // 表示設定の一時オブジェクトを保持し、元の値は参照したままにする。
        if constexpr (_detail::is_value_with_command<T>)
          return rat_object<std::remove_cvref_t<T>>{std::forward<T>(t)};
        else
#endif
        if constexpr (!std::ranges::range<T> || rat_is_map<std::remove_cvref_t<T>>)
          return rat_object<T>{std::forward<T>(t)};
        else
          return std::forward<T>(t) | std::views::transform(*this);
      }
      else
      {
        return std::forward<T>(t);
      }
    }
  };

#ifdef LOCAL
  template <class T>
  string export_object_generic(
      const rat_object<T> &object, const string &indent, size_t last_line_length,
      size_t current_depth, bool fail_on_newline, const _detail::export_command &command)
  {
    using U = std::remove_cvref_t<T>;
    const auto &value = object.value;
    auto render = [&](const auto &x)
    {
      return _detail::export_var(x, indent, last_line_length, current_depth, fail_on_newline, command);
    };
    if constexpr (_detail::is_value_with_command<U>)
      return _detail::export_var(rat_closure{}(value.value), indent, last_line_length,
                                current_depth, fail_on_newline, value.command);
    else if constexpr (!rat_closure::can_convert<const U &>())
      return render(value);
    else if constexpr (_detail::is_map<U>)
      return _detail::export_library_map(value, indent, last_line_length, current_depth,
                                        fail_on_newline, command, rat_closure{});
    else if constexpr (!std::ranges::range<U> && requires { std::tuple_size<U>::value; })
      return std::apply([&](const auto &...member)
      {
        return render(std::tuple{rat_object<const std::remove_reference_t<decltype(member)> &>{member}...});
      }, value);
    else
      return render(rat_closure{}(value));
  }

  // rat() の前後にある表示設定を、cpp-dump と同じ順序で合成する。
  template <class T>
    requires _detail::is_value_with_command<T>
  auto operator|(rat_object<T> object, const _detail::export_command &command)
  {
    return rat_object<T>{object.value | command};
  }

  template <class T>
    requires _detail::is_value_with_command<T>
  auto operator|(rat_object<T> object, _detail::export_command &&command)
  {
    return rat_object<T>{object.value | std::move(command)};
  }
#endif

  template <typename T>
    requires(!requires { std::views::all | std::declval<T>(); })
  constexpr decltype(auto) operator|(T &&t, rat_closure c)
  {
    return c(std::forward<T>(t));
  }

  constexpr rat_closure rat()
  {
    return rat_closure{};
  }
}
#endif
