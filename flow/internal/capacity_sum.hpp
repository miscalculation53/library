#pragma once

#include "../../utils/make_unsigned_ext.hpp"

namespace flow_internal
{
// blocks * 2^bits + low。blocks は加えた項数以下の桁上がりの回数。
// 係数型を拡張せず、容量の総和を同じ幅の容量へ分割して使う。
template <class Unsigned>
struct CapacitySum
{
  Unsigned low = 0;
  int blocks = 0;

  void add(Unsigned x)
  {
    const Unsigned previous = low;
    low += x;
    blocks += low < previous;
  }

  template <class F>
  void for_each_chunk(F f) const
  {
    Unsigned rest = low;
    int high = blocks;
    while (high > 0)
    {
      f(numeric_limits<Unsigned>::max());
      high--;
      if (++rest == 0) high++;
    }
    if (rest != 0) f(rest);
  }
};

template <class Signed>
struct SignedCapacitySum
{
  using Unsigned = make_unsigned_ext_t<Signed>;
  Unsigned low = 0;
  int blocks = 0;

  void add(Signed value)
  {
    const Unsigned x = Unsigned(value), previous = low;
    low += x;
    blocks += int(low < previous) - int(value < 0);
  }
  void subtract(Signed value)
  {
    const Unsigned x = Unsigned(value), previous = low;
    low -= x;
    blocks += int(value < 0) - int(previous < x);
  }
  bool negative() const { return blocks < 0; }
  CapacitySum<Unsigned> magnitude() const
  {
    if (!negative()) return {low, blocks};
    return {Unsigned(Unsigned(0) - low), -blocks - int(low != 0)};
  }
};
}
