#pragma once

#include "../template/template_all_but_modint.hpp"

/**
 * @brief pb_ds の set/multiset
 * @docs docs/ds/pbds_set.md
 */

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template <class T, class Compare = less<T>>
struct pbds_set : tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>
{
  using base = tree<T, null_type, Compare, rb_tree_tag, tree_order_statistics_node_update>;

  pbds_set() {}
  pbds_set(initializer_list<T> il) : base(il) {}
  template <class Iterator>
  pbds_set(Iterator begi, Iterator endi) : base(begi, endi) {}

  inline bool contains(const T &x) const
  {
    auto it = this->find(x);
    return it != this->end() && *it == x;
  }

  // x 未満の要素の個数
  template <class I = ll>
  inline I lt_cnt(const T &x) const { return this->order_of_key(x); }
  // x 以下の要素の個数
  template <class I = ll>
  inline I leq_cnt(const T &x) const { return lt_cnt<I>(x) + contains(x); }
  // x 以上の要素の個数
  template <class I = ll>
  inline I geq_cnt(const T &x) const { return (I)this->size() - lt_cnt<I>(x); }
  // x 超過の要素の個数
  template <class I = ll>
  inline I gt_cnt(const T &x) const { return (I)this->size() - leq_cnt<I>(x); }

  // x 未満で最大の要素のイテレータ (なければ end())
  inline auto lt_max_it(const T &x) const
  {
    auto it = geq_min_it(x);
    return it == this->begin() ? this->end() : prev(it);
  }
  // x 以下で最大の要素のイテレータ (なければ end())
  inline auto leq_max_it(const T &x) const
  {
    auto it = gt_min_it(x);
    return it == this->begin() ? this->end() : prev(it);
  }
  // x 以上で最小の要素のイテレータ (なければ end())
  inline auto geq_min_it(const T &x) const { return this->lower_bound(x); }
  // x 超過で最小の要素のイテレータ (なければ end())
  inline auto gt_min_it(const T &x) const { return this->upper_bound(x); }

  // x 未満で最大の要素の番号 (なければ -1)
  template <class I = ll>
  inline I lt_max_id(const T &x) const { return lt_cnt<I>(x) - 1; }
  // x 以下で最大の要素の番号 (なければ -1)
  template <class I = ll>
  inline I leq_max_id(const T &x) const { return leq_cnt<I>(x) - 1; }

  // x 以上で最小の要素の番号 (なければ size())
  template <class I = ll>
  inline I geq_min_id(const T &x) const { return lt_cnt<I>(x); }
  // x 超過で最小の要素の番号 (なければ size())
  template <class I = ll>
  inline I gt_min_id(const T &x) const { return leq_cnt<I>(x); }
};

template <class T, class Compare>
struct ComparePair
{
  bool operator()(const pair<T, int> &a, const pair<T, int> &b) const
  {
    if (a.first != b.first)
      return Compare()(a.first, b.first);
    return a.second < b.second;
  }
};

template <class T, class Compare = less<T>>
struct pbds_multiset : tree<pair<T, int>, null_type, ComparePair<T, Compare>, rb_tree_tag, tree_order_statistics_node_update>
{
  using base = tree<pair<T, int>, null_type, ComparePair<T, Compare>, rb_tree_tag, tree_order_statistics_node_update>;
  
private:
  int id = 0;

public:
  pbds_multiset() {}
  pbds_multiset(initializer_list<T> il) { fec(x : il) insert(x); }
  template <class Iterator>
  pbds_multiset(Iterator begi, Iterator endi)
  {
    for (auto it = begi; it != endi; it = next(it))
      insert(*it);
  }

  multiset<T> content() const
  {
    multiset<T> res;
    fec(e : *this) res.insert(e.first);
    return res;
  }

  inline void insert(const T &x)
  {
    base::insert({x, id++});
  }

  inline auto find(const T &x) const
  {
    auto it = this->lower_bound({x, 0});
    if (it == this->end() || it->first != x)
      return this->end();
    return it;
  }
  inline bool contains(const T &x) const
  {
    auto it = find(x);
    return it != this->end() && it->first == x;
  }
  // x 未満の要素の個数
  template <class I = ll>
  inline I lt_cnt(const T &x) const { return this->order_of_key({x, 0}); }
  // x 以下の要素の個数
  template <class I = ll>
  inline I leq_cnt(const T &x) const { return this->order_of_key({x, (int)1e9}); }
  // x 以上の要素の個数
  template <class I = ll>
  inline I geq_cnt(const T &x) const { return (I)this->size() - lt_cnt<I>(x); }
  // x 超過の要素の個数
  template <class I = ll>
  inline I gt_cnt(const T &x) const { return (I)this->size() - leq_cnt<I>(x); }
  
  // x 未満で最大の要素のイテレータ (なければ end())
  inline auto lt_max_it(const T &x) const
  {
    auto it = geq_min_it(x);
    return it == this->begin() ? this->end() : prev(it);
  }
  // x 以下で最大の要素のイテレータ (なければ end())
  inline auto leq_max_it(const T &x) const
  {
    auto it = gt_min_it(x);
    return it == this->begin() ? this->end() : prev(it);
  }
  // x 以上で最小の要素のイテレータ (なければ end())
  inline auto geq_min_it(const T &x) const { return this->lower_bound({x, 0}); }
  // x 超過で最小の要素のイテレータ (なければ end())
  inline auto gt_min_it(const T &x) const { return this->upper_bound({x, (int)1e9}); }

  // x 未満で最大の要素の番号 (なければ -1)
  template <class I = ll>
  inline I lt_max_id(const T &x) const { return lt_cnt<I>(x) - 1; }
  // x 以下で最大の要素の番号 (なければ -1)
  template <class I = ll>
  inline I leq_max_id(const T &x) const { return leq_cnt<I>(x) - 1; }
  // x 以上で最小の要素の番号 (なければ size())
  template <class I = ll>
  inline I geq_min_id(const T &x) const { return lt_cnt<I>(x); }
  // x 超過で最小の要素の番号 (なければ size())
  template <class I = ll>
  inline I gt_min_id(const T &x) const { return leq_cnt<I>(x); }
};
