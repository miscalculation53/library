#pragma once

#include "../template/template_all_but_modint.hpp"
#include "algebra_base.hpp"

/**
 * @brief 更新作用のモノイド
 * @docs docs/algebra/upd.md
 */

template <class T, T idnty>
struct MonoidUpd
{
  using S = T;
  static constexpr S op(S f, S g) { return f == idnty ? g : f; }
  static constexpr S e() { return idnty; }
};

template <class T>
struct MonoidUpdFlag
{
  struct S
  {
    bool is_id = true;
    T val = T{};
    S() {}
    S(T val) : is_id(false), val(val) {}
    S(bool is_id, T val) : is_id(is_id), val(val) {}
    CPP_DUMP_DEFINE_DATA(is_id, val);
  };
  static constexpr S op(const S &f, const S &g) { return f.is_id ? g : f; }
  static constexpr S e() { return {}; }
};
