#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

#include "graph/cycle_detection.hpp"

namespace own_copy_cost_fixture {
struct Tag {
    long long value = 0;
    Tag() = default;
    explicit Tag(long long initial) : value(initial) {}
    Tag(const Tag &) = default;
    Tag &operator=(const Tag &) = default;
    Tag(Tag &&) = delete;
    Tag &operator=(Tag &&) = delete;
};
using Input = std::array<long long, 3>;
using Value = std::tuple<int, int, int, long long>;
std::uint64_t cases = 0;
std::uint64_t calls = 0;

[[noreturn]] void fail(const char *message) {
    throw std::runtime_error(message);
}

template <bool Directed>
std::vector<std::vector<Value>> snapshot(const Graph<Directed, Tag> &graph) {
    const int n = graph.template size<int>();
    std::vector<std::vector<Value>> values(n);
    for (int v = 0; v < n; ++v) {
        for (const auto &edge : graph.out_edges(v)) {
            values[v].emplace_back(edge.from, edge.to, edge.index, edge.cost.value);
        }
        std::sort(values[v].begin(), values[v].end());
    }
    return values;
}

template <bool Directed>
void run_case(int n, const std::vector<Input> &input, bool expected) {
    ++cases;
    const auto validate = [&](const optional<vc<Edge<Tag>>> &result) {
        if (static_cast<bool>(result) != expected) fail("opaque cycle existence");
        if (!result) return;
        if (result->empty()) fail("opaque empty success");
        std::vector<unsigned char> vertices(n, 0);
        std::vector<unsigned char> identities(input.size(), 0);
        for (std::size_t p = 0; p < result->size(); ++p) {
            const auto &edge = (*result)[p];
            if (edge.from < 0 || edge.from >= n || edge.to < 0 || edge.to >= n) fail("opaque endpoints");
            if (edge.index < 0 || static_cast<std::size_t>(edge.index) >= input.size()) fail("opaque identity");
            if (vertices[edge.from] || identities[edge.index]) fail("opaque repeated value");
            vertices[edge.from] = identities[edge.index] = 1;
            const auto &model = input[edge.index];
            if (edge.cost.value != model[2]) fail("opaque cost changed");
            const bool forward = edge.from == model[0] && edge.to == model[1];
            if constexpr (Directed) {
                if (!forward) fail("opaque directed orientation");
            } else {
                const bool reverse = edge.from == model[1] && edge.to == model[0];
                if (!forward && !reverse) fail("opaque undirected orientation");
            }
            if (edge.to != (*result)[(p + 1) % result->size()].from) fail("opaque traversal");
        }
    };
    optional<vc<Edge<Tag>>> saved;
    {
        std::vector<std::tuple<int, int, Tag>> weighted;
        weighted.reserve(input.size());
        for (const auto &edge : input) {
            const Tag cost(edge[2]);
            weighted.emplace_back(static_cast<int>(edge[0]), static_cast<int>(edge[1]), cost);
        }
        const Graph<Directed, Tag> graph(n, weighted);
        const auto before = snapshot(graph);
        for (int repetition = 0; repetition < 2; ++repetition) {
            auto result = cycle_detection(graph);
            ++calls;
            validate(result);
            saved = std::move(result);
        }
        if (snapshot(graph) != before) fail("opaque input changed");
    }
    validate(saved);
    auto copied = saved;
    validate(copied);
    auto moved = std::move(copied);
    validate(moved);
}

template <bool Directed>
void cases_for_orientation() {
    run_case<Directed>(0, {}, false);
    run_case<Directed>(4, {{0, 1, 42}, {1, 2, -7}, {1, 3, 99}}, false);
    run_case<Directed>(3, {{1, 1, -123}}, true);
    run_case<Directed>(2, {{0, 1, 3}, {0, 1, 5}}, !Directed);
    run_case<Directed>(2, {{0, 1, 3}, {1, 0, 5}}, true);
    run_case<Directed>(6, {{0, 1, 9}, {3, 4, -4}, {4, 5, 7}, {5, 3, 21}}, true);
    const int n = 20000;
    std::vector<Input> path;
    path.reserve(n);
    for (int v = 0; v + 1 < n; ++v) path.push_back({v, v + 1, 3LL * v - 10000});
    run_case<Directed>(n, path, false);
    path.push_back({n - 1, 0, -987654321});
    run_case<Directed>(n, path, true);
}
}  // namespace own_copy_cost_fixture

int main() {
    try {
        own_copy_cost_fixture::cases_for_orientation<true>();
        own_copy_cost_fixture::cases_for_orientation<false>();
        std::cout << "Hello World\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "FAIL opaque_after_cases=" << own_copy_cost_fixture::cases
                  << " " << error.what() << '\n';
        return 1;
    }
}
