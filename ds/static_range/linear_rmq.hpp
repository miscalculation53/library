#pragma once

#include "../../template/template_all_but_modint.hpp"

#include "block_sparse_table.hpp"

/**
 * @brief 線形 RMQ
 * @docs docs/ds/static_range/linear_rmq.md
 */

// M は min, max などのモノイド
template <class M, class Word = uint64_t>
struct LinearRMQ : BlockSparseTable<M, 8 * sizeof(Word)>
{
  static_assert(is_unsigned_v<Word>);
  static const int B = 8 * sizeof(Word);
  using S = typename M::S;
  using BST = BlockSparseTable<M, B>;

protected:
  using BST::n, BST::v, BST::cl, BST::cr, BST::st;
  // bit[j] の i' ビット目は、
  // そのブロックにある i ≡ i' (mod B) な i について
  // [i, j] の答えが i かどうか
  vc<Word> bit;

public:
  LinearRMQ() {}
  LinearRMQ(const vc<S> &v) : BST(v), bit(n)
  {
    vc<int> sta;
    Word cur = 0;
    repi(i, n)
    {
      if (i % B == 0)
      {
        sta.clear();
        cur = 0;
      }
      while (!sta.empty())
      {
        int j = sta.back();
        if (M::op(v[j], v[i]) == v[i])
        {
          sta.pop_back();
          bset(cur, j % B, 0);
        }
        else
          break;
      }
      sta.eb(i);
      bset(cur, i % B, 1);
      bit[i] = cur;
    }
  }

  S get(int p) const
  {
    assert(0 <= p && p < n);
    return v[p];
  }
  S prod(int l, int r) const
  {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return M::e();
    const int i = l / B, j = r == n ? (n - 1) / B + 1 : r / B;
    if (i < j)
      return M::op(M::op(cr[l], st.prod(i + 1, j)), cl[r]);
    else
      return v[i * B + lsb_pos(bit[r - 1] & ~((Word(1) << (l % B)) - 1))];
  }
};
