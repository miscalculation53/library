#pragma once

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>

// Static orthogonal range aggregation in a commutative group.

namespace internal_sparse_2d {

// Group supplies value_type, identity(), combine(a, b), and inverse(a).
// Coordinates have a strict weak ordering. Group values are copyable.
template <class X, class Y, class Group>
class static_rectangle_group {
public:
    using value_type = typename Group::value_type;

    struct point {
        X x;
        Y y;
        value_type weight;
    };

private:
    struct layer {
        std::vector<std::uint64_t> branch;
        std::vector<std::uint32_t> earlier_ones;
        std::vector<value_type> zero_prefix;
        int zeros = 0;

        int zero_before(int position) const {
            const unsigned word = static_cast<unsigned>(position) >> 6;
            const unsigned offset = static_cast<unsigned>(position) & 63;
            const std::uint64_t low = (std::uint64_t(1) << offset) - 1;
            const unsigned ones = earlier_ones[word]
                + static_cast<unsigned>(__builtin_popcountll(branch[word] & low));
            return position - static_cast<int>(ones);
        }
    };

    Group group_;
    std::vector<X> xs_;
    std::vector<Y> ys_;
    std::vector<value_type> x_prefix_;
    std::vector<layer> layers_;
    int height_ = 0;

    value_type difference(const value_type& right, const value_type& left) const {
        return group_.combine(right, group_.inverse(left));
    }

    value_type zero_sum(const layer& data, int left, int right) const {
        return difference(data.zero_prefix[right], data.zero_prefix[left]);
    }

    value_type below(int first_layer, int left, int right, unsigned bound) const {
        value_type answer = group_.identity();
        if (bound == 0) return answer;
        const int last_bit = __builtin_ctz(bound);
        const int last_layer = height_ - 1 - last_bit;
        for (int d = first_layer; d <= last_layer && left < right; ++d) {
            const layer& data = layers_[d];
            const int zl = data.zero_before(left);
            const int zr = data.zero_before(right);
            const int bit = height_ - 1 - d;
            if ((bound >> bit) & 1U) {
                answer = group_.combine(answer, zero_sum(data, zl, zr));
                left += data.zeros - zl;
                right += data.zeros - zr;
            } else {
                left = zl;
                right = zr;
            }
        }
        return answer;
    }

    value_type by_rank(int left, int right, unsigned lower, unsigned upper) const {
        if (left == right || lower == upper) return group_.identity();
        if (upper == ys_.size()) {
            value_type total = difference(x_prefix_[right], x_prefix_[left]);
            return group_.combine(total, group_.inverse(below(0, left, right, lower)));
        }
        if (lower == 0) return below(0, left, right, upper);

        const unsigned disagreement = lower ^ upper;
        const int split_bit = 31 - __builtin_clz(disagreement);
        const int split_layer = height_ - 1 - split_bit;
        for (int d = 0; d < split_layer; ++d) {
            const layer& data = layers_[d];
            const int zl = data.zero_before(left);
            const int zr = data.zero_before(right);
            const int bit = height_ - 1 - d;
            if ((lower >> bit) & 1U) {
                left += data.zeros - zl;
                right += data.zeros - zr;
            } else {
                left = zl;
                right = zr;
            }
            if (left == right) return group_.identity();
        }

        const layer& data = layers_[split_layer];
        const int zl = data.zero_before(left);
        const int zr = data.zero_before(right);
        value_type answer = zero_sum(data, zl, zr);
        const value_type excluded = below(split_layer + 1, zl, zr, lower);
        answer = group_.combine(answer, group_.inverse(excluded));
        const value_type included = below(split_layer + 1,
            left + data.zeros - zl, right + data.zeros - zr, upper);
        return group_.combine(answer, included);
    }

public:
    explicit static_rectangle_group(std::vector<point> points, Group group = Group())
        : group_(std::move(group)) {
        const int n = static_cast<int>(points.size());
        std::sort(points.begin(), points.end(), [](const point& a, const point& b) {
            return a.x < b.x;
        });
        xs_.reserve(n);
        x_prefix_.reserve(n + 1);
        x_prefix_.push_back(group_.identity());
        std::vector<value_type> weights;
        weights.reserve(n);
        for (point& p : points) {
            xs_.push_back(p.x);
            x_prefix_.push_back(group_.combine(x_prefix_.back(), p.weight));
            weights.push_back(std::move(p.weight));
        }

        std::vector<int> order(n), ranks(n);
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            return points[a].y < points[b].y;
        });
        ys_.reserve(n);
        for (int index : order) {
            if (ys_.empty() || ys_.back() < points[index].y) {
                ys_.push_back(points[index].y);
            }
            ranks[index] = static_cast<int>(ys_.size()) - 1;
        }
        std::vector<point>().swap(points);
        std::vector<int>().swap(order);
        for (unsigned last = static_cast<unsigned>(ys_.size()); last > 1;) {
            last = (last + 1) >> 1;
            ++height_;
        }
        layers_.reserve(height_);
        std::vector<int> next_ranks(n);
        std::vector<value_type> next_weights(n, group_.identity());
        for (int bit = height_ - 1; bit >= 0; --bit) {
            layers_.emplace_back();
            layer& data = layers_.back();
            const int words = (n >> 6) + 1;
            data.branch.assign(words, 0);
            data.earlier_ones.resize(words);
            int ones = 0;
            for (int i = 0; i < n; ++i) {
                if ((static_cast<unsigned>(ranks[i]) >> bit) & 1U) {
                    data.branch[static_cast<unsigned>(i) >> 6]
                        |= std::uint64_t(1) << (static_cast<unsigned>(i) & 63);
                    ++ones;
                }
            }
            for (int word = 1; word < words; ++word) {
                data.earlier_ones[word] = data.earlier_ones[word - 1]
                    + static_cast<unsigned>(__builtin_popcountll(data.branch[word - 1]));
            }
            data.zeros = n - ones;
            data.zero_prefix.reserve(data.zeros + 1);
            data.zero_prefix.push_back(group_.identity());
            int z = 0, o = data.zeros;
            for (int i = 0; i < n; ++i) {
                if ((static_cast<unsigned>(ranks[i]) >> bit) & 1U) {
                    next_ranks[o] = ranks[i];
                    next_weights[o++] = std::move(weights[i]);
                } else {
                    data.zero_prefix.push_back(
                        group_.combine(data.zero_prefix.back(), weights[i]));
                    next_ranks[z] = ranks[i];
                    next_weights[z++] = std::move(weights[i]);
                }
            }
            ranks.swap(next_ranks);
            weights.swap(next_weights);
        }
    }

    value_type sum(const X& xl, const X& xr, const Y& yl, const Y& yr) const {
        if (!(xl < xr) || !(yl < yr) || xs_.empty()) return group_.identity();
        const int left = static_cast<int>(
            std::lower_bound(xs_.begin(), xs_.end(), xl) - xs_.begin());
        const int right = static_cast<int>(
            std::lower_bound(xs_.begin(), xs_.end(), xr) - xs_.begin());
        const unsigned lower = static_cast<unsigned>(
            std::lower_bound(ys_.begin(), ys_.end(), yl) - ys_.begin());
        const unsigned upper = static_cast<unsigned>(
            std::lower_bound(ys_.begin(), ys_.end(), yr) - ys_.begin());
        return by_rank(left, right, lower, upper);
    }

    value_type sum_x(const X& x, const Y& yl, const Y& yr) const {
        if (!(yl < yr) || xs_.empty()) return group_.identity();
        const int left = static_cast<int>(
            std::lower_bound(xs_.begin(), xs_.end(), x) - xs_.begin());
        const int right = static_cast<int>(
            std::upper_bound(xs_.begin(), xs_.end(), x) - xs_.begin());
        if (left == right) return group_.identity();
        const unsigned lower = static_cast<unsigned>(
            std::lower_bound(ys_.begin(), ys_.end(), yl) - ys_.begin());
        const unsigned upper = static_cast<unsigned>(
            std::lower_bound(ys_.begin(), ys_.end(), yr) - ys_.begin());
        return by_rank(left, right, lower, upper);
    }

    value_type all_sum() const {
        return x_prefix_.back();
    }

};

} // namespace internal_sparse_2d

namespace internal_sparse_2d {

template <class M>
struct group_bridge {
    using value_type = typename M::S;
    value_type identity() const { return M::e(); }
    value_type combine(const value_type& a, const value_type& b) const {
        return M::op(a, b);
    }
    value_type inverse(const value_type& a) const { return M::inv(a); }
};

template <class M, class I, bool Enabled>
struct static_rectangle_storage {
    static_rectangle_storage() = default;
    static_rectangle_storage(const std::vector<std::tuple<I, I, typename M::S>>&) {}
};

template <class M, class I>
struct static_rectangle_storage<M, I, true> {
    using S = typename M::S;
    using core_type = static_rectangle_group<I, I, group_bridge<M>>;
    core_type core;

    static std::vector<typename core_type::point> points_from(
            const std::vector<std::tuple<I, I, S>>& xyws) {
        std::vector<typename core_type::point> result;
        result.reserve(xyws.size());
        for (const auto& p : xyws) {
            result.push_back({std::get<0>(p), std::get<1>(p), std::get<2>(p)});
        }
        return result;
    }

    static_rectangle_storage() : core(std::vector<typename core_type::point>{}) {}
    static_rectangle_storage(const std::vector<std::tuple<I, I, S>>& xyws)
        : core(points_from(xyws)) {}
};

} // namespace internal_sparse_2d
