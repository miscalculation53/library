/*!
 * SPDX-License-Identifier: CC0-1.0
 * Independently written from the public API contract and mathematical definitions.
 * To the extent possible under law, the authors waive all copyright and related
 * or neighboring rights to this file under CC0 1.0 Universal.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <vector>

#include "../../../template/template_all_but_modint.hpp"
#include "../../segtree/lazy_segtree.hpp"
#include "../../../algebra/acted_monoid/add_min_count.hpp"

/**
 * @brief 長方形の和集合の面積
 * @docs docs/ds/2d/offline/area_of_union_of_rectangles.md
 */

template<class T = ll> struct AreaOfUnionOfRectangles {
private:
    struct Rectangle {
        T lx, rx, ly, ry;
    };
    std::vector<Rectangle> rectangles;

public:
    void rectangle_add(T lx, T rx, T ly, T ry) {
        rectangles.push_back({lx, rx, ly, ry});
    }

    T run() {
        using Work = typename std::conditional<std::is_same<T, float>::value || std::is_same<T, double>::value,
                                               long double, T>::type;
        std::vector<T> ys;
        ys.reserve(rectangles.size() * 2);
        std::size_t active_count = 0;
        for (const auto &rectangle : rectangles) {
            if (!(rectangle.lx < rectangle.rx) || !(rectangle.ly < rectangle.ry)) continue;
            ys.emplace_back(rectangle.ly);
            ys.emplace_back(rectangle.ry);
            ++active_count;
        }
        if (active_count == 0) return T(0);
        const auto equal_coordinate = [](const T &a, const T &b) {
            return !(a < b) && !(b < a);
        };
        std::sort(ys.begin(), ys.end());
        ys.erase(std::unique(ys.begin(), ys.end(), equal_coordinate), ys.end());

        struct Event {
            T x;
            int lower, upper, delta;
        };
        std::vector<Event> events;
        events.reserve(active_count * 2);
        for (const auto &rectangle : rectangles) {
            if (!(rectangle.lx < rectangle.rx) || !(rectangle.ly < rectangle.ry)) continue;
            const int lower = static_cast<int>(std::lower_bound(ys.begin(), ys.end(), rectangle.ly) - ys.begin());
            const int upper = static_cast<int>(std::lower_bound(ys.begin(), ys.end(), rectangle.ry) - ys.begin());
            events.push_back({rectangle.lx, lower, upper, 1});
            events.push_back({rectangle.rx, lower, upper, -1});
        }
        std::sort(events.begin(), events.end(), [](const Event &a, const Event &b) {
            if (a.x < b.x) return true;
            if (b.x < a.x) return false;
            if (a.lower != b.lower) return a.lower < b.lower;
            return a.upper < b.upper;
        });

        const int segments = static_cast<int>(ys.size()) - 1;
        int base = 1;
        while (base < segments) base *= 2;
        std::vector<int> count(base * 2, 0);
        std::vector<Work> span(base * 2, Work(0));
        std::vector<Work> length(base * 2, Work(0));
        for (int first = base, width = 1; first > 0; first /= 2, width *= 2) {
            for (int node = first; node < first * 2; ++node) {
                const int lower = (node - first) * width;
                if (lower >= segments) continue;
                const int upper = std::min(lower + width, segments);
                span[node] = Work(ys[upper]) - Work(ys[lower]);
            }
        }

        const auto pull = [&](int node) {
            if (count[node] > 0) length[node] = span[node];
            else if (node >= base) length[node] = Work(0);
            else {
                const Work sum = length[node * 2] + length[node * 2 + 1];
                if constexpr (std::is_floating_point<T>::value) {
                    length[node] = span[node] < sum ? span[node] : sum;
                } else {
                    length[node] = sum;
                }
            }
        };
        const auto update = [&](int lower, int upper, int delta) {
            int left = lower + base;
            int right = upper + base;
            const int left_leaf = left;
            const int right_leaf = right - 1;
            while (left < right) {
                if (left & 1) {
                    count[left] += delta;
                    pull(left);
                    ++left;
                }
                if (right & 1) {
                    --right;
                    count[right] += delta;
                    pull(right);
                }
                left /= 2;
                right /= 2;
            }
            left = left_leaf / 2;
            right = right_leaf / 2;
            while (left > 0) {
                pull(left);
                if (left != right) pull(right);
                left /= 2;
                right /= 2;
            }
        };

        Work area = Work(0);
        T previous = events.front().x;
        std::size_t current = 0;
        while (current < events.size()) {
            const T x = events[current].x;
            const Work width = Work(x) - Work(previous);
            area = area + width * length[1];
            while (current < events.size() && equal_coordinate(events[current].x, x)) {
                const int lower = events[current].lower;
                const int upper = events[current].upper;
                int delta = 0;
                do {
                    delta += events[current].delta;
                    ++current;
                } while (current < events.size()
                         && equal_coordinate(events[current].x, x)
                         && events[current].lower == lower
                         && events[current].upper == upper);
                if (delta != 0) update(lower, upper, delta);
            }
            previous = x;
        }
        return T(area);
    }
};
