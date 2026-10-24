#pragma once

#include "internal_sparse_point_fold.hpp"

/**
 * @brief 必要なところだけ作るセグメント木
 * @docs docs/ds/segtree/sparse_segtree.md
 */

template <class M>
struct SparseSegmentTree {
    using S = typename M::S;
private:
    ll n;
    std::optional<internal_sparse_point_fold::unit_avl<M>> fast;
    std::optional<internal_sparse_point_fold::general_initial_tree<M>> fallback;
public:
    SparseSegmentTree(ll n, const S& initial_value) : n(n) {
        assert(n >= 0);
        if (internal_sparse_point_fold::identity_value<M>::test(initial_value))
            fast.emplace();
        else
            fallback.emplace(n, initial_value);
    }
    void reserve(int count) {
        if (fast) fast->reserve(count);
        else fallback->reserve(count);
    }
    template <class F>
    void modify(ll p, const F& f) {
        if (fast) {
            assert(0 <= p && p < n);
            fast->modify(p, f);
        } else fallback->modify(p, f);
    }
    void set(ll p, const S& value) {
        modify(p, [&](S& current) { current = value; });
    }
    S get(ll p) const {
        if (fast) {
            assert(0 <= p && p < n);
            return fast->get(p);
        }
        return fallback->get(p);
    }
    S prod(ll l, ll r) const {
        if (fast) {
            assert(0 <= l && l <= r && r <= n);
            return fast->prod(l, r);
        }
        return fallback->prod(l, r);
    }
    S all_prod() const { return fast ? fast->all_prod() : fallback->all_prod(); }
    template <class G>
    ll max_right_ok(ll l, const G& g) const {
        if (fast) {
            assert(0 <= l && l <= n);
            assert(g(M::e()));
            return fast->max_right_ok(l, n, g);
        }
        return fallback->max_right_ok(l, g);
    }
    template <class G>
    ll min_left_ok(ll r, const G& g) const {
        if (fast) {
            assert(0 <= r && r <= n);
            assert(g(M::e()));
            return fast->min_left_ok(r, g);
        }
        return fallback->min_left_ok(r, g);
    }
    std::map<ll, S> content() const { return fast ? fast->content() : fallback->content(); }
};
