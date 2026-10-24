/*!
 * SPDX-License-Identifier: CC0-1.0
 * Independent structural and custom-numeric rectangle area fixtures.
 * To the extent possible under law, the authors waive all copyright and related
 * or neighboring rights to this file under CC0 1.0 Universal.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */
// verification-helper: PROBLEM https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "ds/2d/offline/area_of_union_of_rectangles.hpp"

namespace own_area_fixture {
using GridRectangle = std::array<int, 4>;

[[noreturn]] void fail(const std::string &message) {
    throw std::runtime_error(message);
}

// Count covered unit cells directly. No events or maintained interval union.
int grid_oracle(const std::vector<GridRectangle> &rectangles) {
    int area = 0;
    for (int x = -4; x < 8; ++x) {
        for (int y = -4; y < 8; ++y) {
            bool covered = false;
            for (const auto &r : rectangles) {
                if (r[0] <= x && x < r[1] && r[2] <= y && y < r[3]) {
                    covered = true;
                    break;
                }
            }
            if (covered) ++area;
        }
    }
    return area;
}

}

namespace own_area_v3_fixture {
using own_area_fixture::GridRectangle;
std::uint64_t extra_cases = 0;
std::uint64_t extra_calls = 0;

struct Number {
    long long value;
    Number() = delete;
    explicit Number(long long v) : value(v) {}
    friend bool operator<(const Number &a, const Number &b) { return a.value < b.value; }
    friend Number operator+(const Number &a, const Number &b) { return Number(a.value + b.value); }
    friend Number operator-(const Number &a, const Number &b) { return Number(a.value - b.value); }
    friend Number operator*(const Number &a, const Number &b) { return Number(a.value * b.value); }
};

template<class T>
long long value_of(const T &value) {
    if constexpr (std::is_same<T, Number>::value) return value.value;
    else return static_cast<long long>(value);
}

template<class T>
void verify(AreaOfUnionOfRectangles<T> &area, long long expected, const char *context) {
    ++extra_calls;
    if (value_of(area.run()) != expected) own_area_fixture::fail(context);
}

template<class T>
void grid_input(const std::vector<GridRectangle> &rectangles) {
    ++extra_cases;
    AreaOfUnionOfRectangles<T> area;
    for (const auto &r : rectangles) area.rectangle_add(T(r[0]), T(r[1]), T(r[2]), T(r[3]));
    const long long expected = own_area_fixture::grid_oracle(rectangles);
    verify(area, expected, "v3 structural unit cells");
    verify(area, expected, "v3 structural repeated run");
    auto copy = area;
    area.rectangle_add(T(-4), T(7), T(-4), T(7));
    verify(area, 121, "v3 addition after run");
    verify(copy, expected, "v3 independent numeric copy");
    AreaOfUnionOfRectangles<T> assigned;
    assigned = copy;
    auto moved = std::move(copy);
    verify(moved, expected, "v3 numeric move construction");
    verify(assigned, expected, "v3 numeric copy assignment");
    AreaOfUnionOfRectangles<T> move_assigned;
    move_assigned = std::move(assigned);
    verify(move_assigned, expected, "v3 numeric move assignment");
}

template<class T>
void structural_inputs() {
    const std::vector<std::vector<GridRectangle>> patterns = {
        {},
        {{0, 3, 0, 1}},
        {{0, 3, 0, 1}, {0, 3, 1, 2}, {0, 3, 2, 4}, {0, 3, 4, 5}, {0, 3, 5, 7}},
        {{0, 2, 0, 7}, {1, 5, 0, 1}, {1, 4, 2, 4}, {1, 3, 5, 7}},
        {{0, 5, 0, 7}, {1, 2, 0, 1}, {1, 4, 1, 2}, {2, 3, 2, 4}, {3, 4, 4, 5}},
        {{0, 2, 0, 7}, {2, 4, 0, 7}, {0, 2, 2, 4}, {2, 4, 1, 5}, {2, 3, 2, 4}},
        {{0, 0, -4, 7}, {5, 6, 4, 4}, {1, 3, 5, 7}, {0, 2, 0, 1}, {2, 4, 2, 4}, {0, 2, 0, 1}}
    };
    for (auto rectangles : patterns) {
        grid_input<T>(rectangles);
        std::reverse(rectangles.begin(), rectangles.end());
        grid_input<T>(rectangles);
    }
    std::mt19937_64 random(0xA2EA320261023ULL);
    for (int trial = 0; trial < 600; ++trial) {
        std::vector<GridRectangle> rectangles;
        if (trial % 2 == 0) rectangles.push_back({-3, 2, -3, 7});
        const int count = 1 + static_cast<int>(random() % 18);
        for (int i = 0; i < count; ++i) {
            int lx = static_cast<int>(random() % 11) - 4;
            int rx = static_cast<int>(random() % 11) - 4;
            int ly = static_cast<int>(random() % 12) - 4;
            int ry = static_cast<int>(random() % 12) - 4;
            if (rx < lx) std::swap(lx, rx);
            if (ry < ly) std::swap(ly, ry);
            rectangles.push_back({lx, rx, ly, ry});
        }
        grid_input<T>(rectangles);
    }
}

void large_tree() {
    constexpr int count = 200003;
    for (int order = 0; order < 2; ++order) {
        ++extra_cases;
        AreaOfUnionOfRectangles<long long> area;
        for (int i = 0; i < count; ++i) {
            const int y = order == 0 ? i : count - i - 1;
            area.rectangle_add(0, 1, y, y + 1);
        }
        verify(area, count, "v3 large non-power-of-two tree");
        verify(area, count, "v3 large repeated run");
        area.rectangle_add(1, 2, 0, count);
        verify(area, count * 2LL, "v3 large add after run");
    }
}
}

int main() {
    try {
        own_area_v3_fixture::structural_inputs<long long>();
        own_area_v3_fixture::structural_inputs<own_area_v3_fixture::Number>();
        own_area_v3_fixture::large_tree();
        std::cout << "Hello World\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "FAIL v3_extra_after_cases=" << own_area_v3_fixture::extra_cases
                  << " " << error.what() << '\n';
        return 1;
    }
}
