#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "ds/2d/offline/area_of_union_of_rectangles.hpp"
#if defined(__SIZEOF_INT128__) && (defined(__clang__) || !defined(__STRICT_ANSI__))
#define OWN_AREA_ENABLE_INT128
#endif
#if defined(LIBRARY_HAS_FLOAT128) && LIBRARY_HAS_FLOAT128
#define OWN_AREA_ENABLE_QUAD
#endif

namespace own_area_fixture {
using GridRectangle = std::array<int, 4>;
std::uint64_t cases = 0;
std::uint64_t run_calls = 0;

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

template <class T>
T checked_run(AreaOfUnionOfRectangles<T> &area) {
    ++run_calls;
    return area.run();
}

template <class T>
void require_equal(T actual, T expected, const char *context) {
    if (!(actual == expected)) fail(context);
}

template <class T>
void check_grid(const std::vector<GridRectangle> &rectangles) {
    ++cases;
    AreaOfUnionOfRectangles<T> area;
    for (const auto &r : rectangles) area.rectangle_add(T(r[0]), T(r[1]), T(r[2]), T(r[3]));
    const T expected = T(grid_oracle(rectangles));
    require_equal(checked_run(area), expected, "grid definition");
    require_equal(checked_run(area), expected, "repeat run");
    AreaOfUnionOfRectangles<T> reversed;
    for (auto it = rectangles.rbegin(); it != rectangles.rend(); ++it) {
        reversed.rectangle_add(T((*it)[0]), T((*it)[1]), T((*it)[2]), T((*it)[3]));
    }
    require_equal(checked_run(reversed), expected, "reverse insertion");
    auto copy = area;
    area.rectangle_add(T(0), T(0), T(0), T(1));
    require_equal(checked_run(area), expected, "zero after run");
    require_equal(checked_run(copy), expected, "independent copy");
}

template <class T>
void grid_cases(bool allow_negative) {
    std::vector<GridRectangle> choices;
    const int shift = allow_negative ? -1 : 0;
    for (int lx = 0; lx < 2; ++lx) {
        for (int rx = lx + 1; rx <= 2; ++rx) {
            for (int ly = 0; ly < 2; ++ly) {
                for (int ry = ly + 1; ry <= 2; ++ry) {
                    choices.push_back({lx + shift, rx + shift, ly + shift, ry + shift});
                }
            }
        }
    }
    for (unsigned int mask = 0; mask < (1U << choices.size()); ++mask) {
        std::vector<GridRectangle> rectangles;
        for (std::size_t i = 0; i < choices.size(); ++i) {
            if ((mask >> i) & 1U) rectangles.emplace_back(choices[i]);
        }
        check_grid<T>(rectangles);
    }
    std::mt19937_64 random(0x20261023A2EAULL);
    const int random_shift = allow_negative ? -3 : 0;
    for (int trial = 0; trial < 420; ++trial) {
        std::vector<GridRectangle> rectangles;
        const int count = static_cast<int>(random() % 12);
        for (int i = 0; i < count; ++i) {
            int lx = static_cast<int>(random() % 7) + random_shift;
            int rx = static_cast<int>(random() % 7) + random_shift;
            int ly = static_cast<int>(random() % 7) + random_shift;
            int ry = static_cast<int>(random() % 7) + random_shift;
            if (lx > rx) std::swap(lx, rx);
            if (ly > ry) std::swap(ly, ry);
            rectangles.push_back({lx, rx, ly, ry});
            if (i % 4 == 0) rectangles.push_back(rectangles.back());
        }
        check_grid<T>(rectangles);
    }
    check_grid<T>({{0, 0, 0, 2}, {1, 2, 1, 1}, {0, 0, 0, 0}});
    check_grid<T>({{0, 4, 0, 4}, {1, 3, 1, 3}, {0, 4, 0, 4}});
    check_grid<T>({{0, 1, 0, 2}, {1, 2, 0, 2}, {2, 3, 0, 2}});
}

template <class T>
void limits_and_ownership(T maximum, bool allow_negative) {
    ++cases;
    AreaOfUnionOfRectangles<T> edge;
    edge.rectangle_add(T(0), maximum, T(0), T(1));
    require_equal(checked_run(edge), maximum, "maximum representable area");
    require_equal(checked_run(edge), maximum, "maximum repeated run");
    AreaOfUnionOfRectangles<T> split;
    const T half = maximum / T(2);
    split.rectangle_add(T(0), T(1), T(0), half);
    split.rectangle_add(T(1), T(2), T(0), maximum - half);
    require_equal(checked_run(split), maximum, "accumulate to maximum area");
    if constexpr (!std::is_floating_point<T>::value) {
        AreaOfUnionOfRectangles<T> high_coordinate;
        high_coordinate.rectangle_add(maximum - T(1), maximum, T(0), T(1));
        require_equal(checked_run(high_coordinate), T(1), "highest integer coordinate");
    }
    if (allow_negative) {
        AreaOfUnionOfRectangles<T> negative;
        negative.rectangle_add(T(0) - maximum, T(0), T(0), T(1));
        require_equal(checked_run(negative), maximum, "negative coordinate boundary");
        if constexpr (!std::is_floating_point<T>::value) {
            const T minimum = T(T(0) - maximum - T(1));
            AreaOfUnionOfRectangles<T> low_coordinate;
            low_coordinate.rectangle_add(minimum, minimum + T(1), T(0), T(1));
            require_equal(checked_run(low_coordinate), T(1), "lowest integer coordinate");
        }
    }

    ++cases;
    const T side = maximum / T(4);
    AreaOfUnionOfRectangles<T> sparse;
    sparse.rectangle_add(T(0), T(1), T(0), side);
    sparse.rectangle_add(side - T(1), side, T(0), T(1));
    require_equal(checked_run(sparse), T(side + T(1)), "bounding box product exceeds answer range");

    ++cases;
    AreaOfUnionOfRectangles<T> original;
    original.rectangle_add(T(0), T(2), T(0), T(2));
    require_equal(checked_run(original), T(4), "before additions");
    auto copy = original;
    original.rectangle_add(T(2), T(3), T(0), T(2));
    require_equal(checked_run(original), T(6), "addition after run");
    require_equal(checked_run(copy), T(4), "copy remains independent");
    copy.rectangle_add(T(0), T(2), T(2), T(3));
    require_equal(checked_run(copy), T(6), "addition to copy");
    AreaOfUnionOfRectangles<T> assigned;
    assigned = original;
    require_equal(checked_run(assigned), T(6), "copy assignment");
    AreaOfUnionOfRectangles<T> moved(std::move(copy));
    require_equal(checked_run(moved), T(6), "move construction");
    AreaOfUnionOfRectangles<T> move_assigned;
    move_assigned = std::move(assigned);
    require_equal(checked_run(move_assigned), T(6), "move assignment");
    assigned = AreaOfUnionOfRectangles<T>();
    assigned.rectangle_add(T(0), T(1), T(0), T(1));
    require_equal(checked_run(assigned), T(1), "reuse after assignment");
    require_equal(checked_run(original), T(6), "original after moves");
}

template <class T>
long double elementary_cell_oracle(const std::vector<std::array<T, 4>> &rectangles) {
    std::vector<long double> xs;
    std::vector<long double> ys;
    for (const auto &r : rectangles) {
        xs.push_back(static_cast<long double>(r[0]));
        xs.push_back(static_cast<long double>(r[1]));
        ys.push_back(static_cast<long double>(r[2]));
        ys.push_back(static_cast<long double>(r[3]));
    }
    std::sort(xs.begin(), xs.end());
    std::sort(ys.begin(), ys.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
    long double answer = 0;
    for (std::size_t x = 1; x < xs.size(); ++x) {
        for (std::size_t y = 1; y < ys.size(); ++y) {
            bool covered = false;
            for (const auto &r : rectangles) {
                if (static_cast<long double>(r[0]) <= xs[x - 1]
                    && xs[x] <= static_cast<long double>(r[1])
                    && static_cast<long double>(r[2]) <= ys[y - 1]
                    && ys[y] <= static_cast<long double>(r[3])) {
                    covered = true;
                    break;
                }
            }
            if (covered) answer += (xs[x] - xs[x - 1]) * (ys[y] - ys[y - 1]);
        }
    }
    return answer;
}

template <class T>
void fractional_cases() {
    std::mt19937_64 random(0xF10A710ULL);
    for (int trial = 0; trial < 240; ++trial) {
        ++cases;
        AreaOfUnionOfRectangles<T> area;
        std::vector<std::array<T, 4>> rectangles;
        const int count = static_cast<int>(random() % 10);
        for (int i = 0; i < count; ++i) {
            int lx = static_cast<int>(random() % 41) - 20;
            int rx = static_cast<int>(random() % 41) - 20;
            int ly = static_cast<int>(random() % 41) - 20;
            int ry = static_cast<int>(random() % 41) - 20;
            if (lx > rx) std::swap(lx, rx);
            if (ly > ry) std::swap(ly, ry);
            rectangles.push_back({T(lx) / T(10), T(rx) / T(10), T(ly) / T(10), T(ry) / T(10)});
            const auto &r = rectangles.back();
            area.rectangle_add(r[0], r[1], r[2], r[3]);
        }
        const long double expected = elementary_cell_oracle(rectangles);
        const long double tolerance = static_cast<long double>(std::numeric_limits<T>::epsilon())
                                    * 128 * std::max(1.0L, std::fabs(expected));
        for (int repetition = 0; repetition < 2; ++repetition) {
            const long double actual = static_cast<long double>(checked_run(area));
            if (!std::isfinite(actual) || std::fabs(actual - expected) > tolerance) fail("floating elementary cells");
        }
    }
}

void many_rectangles() {
    ++cases;
    AreaOfUnionOfRectangles<long long> duplicates;
    for (int i = 0; i < 20000; ++i) duplicates.rectangle_add(-4, 7, -9, 2);
    require_equal(checked_run(duplicates), 121LL, "many identical rectangles");
    ++cases;
    AreaOfUnionOfRectangles<long long> touching;
    for (int i = 0; i < 20000; ++i) touching.rectangle_add(i, i + 1, 0, 3);
    require_equal(checked_run(touching), 60000LL, "many touching rectangles");
}

#ifdef OWN_AREA_ENABLE_QUAD
void quad_precision_and_range() {
    using Quad = f128;
    Quad large = Quad(1);
    Quad tiny = Quad(1);
    for (int bit = 0; bit < 100; ++bit) {
        large = large * Quad(2);
        tiny = tiny / Quad(2);
    }
    if (!(large < large + Quad(1)) || !(Quad(1) < Quad(1) + tiny)) {
        fail("quad fixture requires binary128 precision");
    }

    ++cases;
    AreaOfUnionOfRectangles<Quad> unit;
    unit.rectangle_add(large, large + Quad(1), Quad(0), Quad(1));
    require_equal(checked_run(unit), Quad(1), "quad large-coordinate unit width");
    require_equal(checked_run(unit), Quad(1), "quad unit repeated run");
    auto unit_copy = unit;
    unit.rectangle_add(large + Quad(1), large + Quad(2), Quad(0), Quad(1));
    require_equal(checked_run(unit), Quad(2), "quad unit addition after run");
    require_equal(checked_run(unit_copy), Quad(1), "quad independent copy");

    ++cases;
    const Quad fractional_width = Quad(1) + tiny;
    AreaOfUnionOfRectangles<Quad> fraction;
    fraction.rectangle_add(Quad(0), fractional_width, Quad(0), Quad(1));
    require_equal(checked_run(fraction), fractional_width, "quad fractional low bits");
    require_equal(checked_run(fraction), fractional_width, "quad fraction repeated run");
    AreaOfUnionOfRectangles<Quad> assigned;
    assigned = fraction;
    fraction.rectangle_add(fractional_width, fractional_width + tiny, Quad(0), Quad(1));
    require_equal(checked_run(fraction), Quad(1) + tiny + tiny, "quad fractional addition");
    require_equal(checked_run(assigned), fractional_width, "quad independent assignment");

    ++cases;
    AreaOfUnionOfRectangles<Quad> mixed;
    mixed.rectangle_add(large, large + Quad(1), Quad(0), Quad(1));
    mixed.rectangle_add(Quad(0), fractional_width, Quad(2), Quad(3));
    require_equal(checked_run(mixed), Quad(2) + tiny, "quad mixed scale area");

    Quad huge = Quad(1);
    Quad small = Quad(1);
    for (int bit = 0; bit < 2000; ++bit) {
        huge = huge * Quad(2);
        small = small / Quad(2);
    }
    ++cases;
    AreaOfUnionOfRectangles<Quad> wide_range;
    wide_range.rectangle_add(Quad(0), huge, Quad(0), Quad(1));
    require_equal(checked_run(wide_range), huge, "quad exponent range");
    ++cases;
    AreaOfUnionOfRectangles<Quad> narrow_range;
    narrow_range.rectangle_add(Quad(0), small, Quad(0), Quad(1));
    require_equal(checked_run(narrow_range), small, "quad negative exponent range");
}
#endif
}  // namespace own_area_fixture

static_assert(std::is_default_constructible<AreaOfUnionOfRectangles<>>::value, "default construction");
static_assert(std::is_copy_constructible<AreaOfUnionOfRectangles<>>::value, "copy construction");
static_assert(std::is_move_constructible<AreaOfUnionOfRectangles<>>::value, "move construction");
static_assert(std::is_copy_assignable<AreaOfUnionOfRectangles<>>::value, "copy assignment");
static_assert(std::is_move_assignable<AreaOfUnionOfRectangles<>>::value, "move assignment");
static_assert(std::is_same<decltype(std::declval<AreaOfUnionOfRectangles<int> &>().run()), int>::value,
              "owned T result");
static_assert(std::is_same<decltype(std::declval<AreaOfUnionOfRectangles<int> &>().rectangle_add(0, 1, 0, 1)), void>::value,
              "public add type");

int main() {
    try {
        own_area_fixture::grid_cases<signed char>(true);
        own_area_fixture::grid_cases<unsigned char>(false);
        own_area_fixture::grid_cases<int>(true);
        own_area_fixture::grid_cases<unsigned int>(false);
        own_area_fixture::grid_cases<long long>(true);
        own_area_fixture::grid_cases<unsigned long long>(false);
        own_area_fixture::grid_cases<float>(true);
        own_area_fixture::grid_cases<double>(true);
        own_area_fixture::grid_cases<long double>(true);
        own_area_fixture::limits_and_ownership<signed char>(std::numeric_limits<signed char>::max(), true);
        own_area_fixture::limits_and_ownership<unsigned char>(std::numeric_limits<unsigned char>::max(), false);
        own_area_fixture::limits_and_ownership<int>(std::numeric_limits<int>::max(), true);
        own_area_fixture::limits_and_ownership<unsigned int>(std::numeric_limits<unsigned int>::max(), false);
        own_area_fixture::limits_and_ownership<long long>(std::numeric_limits<long long>::max(), true);
        own_area_fixture::limits_and_ownership<unsigned long long>(std::numeric_limits<unsigned long long>::max(), false);
        own_area_fixture::limits_and_ownership<float>(std::numeric_limits<float>::max(), true);
        own_area_fixture::limits_and_ownership<double>(std::numeric_limits<double>::max(), true);
        own_area_fixture::limits_and_ownership<long double>(std::numeric_limits<long double>::max(), true);
        own_area_fixture::fractional_cases<float>();
        own_area_fixture::fractional_cases<double>();
        own_area_fixture::fractional_cases<long double>();
#ifdef OWN_AREA_ENABLE_INT128
        using Signed128 = __int128;
        using Unsigned128 = unsigned __int128;
        const Unsigned128 unsigned_maximum = ~Unsigned128(0);
        const Signed128 signed_maximum = Signed128(unsigned_maximum >> 1);
        own_area_fixture::grid_cases<Signed128>(true);
        own_area_fixture::grid_cases<Unsigned128>(false);
        own_area_fixture::limits_and_ownership<Signed128>(signed_maximum, true);
        own_area_fixture::limits_and_ownership<Unsigned128>(unsigned_maximum, false);
#endif
        own_area_fixture::many_rectangles();
#ifdef OWN_AREA_ENABLE_QUAD
        own_area_fixture::quad_precision_and_range();
#endif
        std::cout << "Hello World\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "FAIL after_cases=" << own_area_fixture::cases << " " << error.what() << '\n';
        return 1;
    }
}
