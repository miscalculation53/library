#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ITP1_1_A"

/*!
 * SPDX-License-Identifier: CC0-1.0
 * Independently written definition-based cycle fixtures.
 * To the extent possible under law, the authors waive all copyright and related
 * or neighboring rights to this file under CC0 1.0 Universal.
 * https://creativecommons.org/publicdomain/zero/1.0/
 */
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "graph/cycle_detection.hpp"

namespace own_cycle_fixture {
using InputEdge = std::pair<int, int>;
using EdgeValue = std::tuple<int, int, int, long long>;
std::uint64_t cases = 0;
std::uint64_t checked_calls = 0;

[[noreturn]] void fail(const std::string &message) {
    throw std::runtime_error(message);
}

// Reachable[subset][last] enumerates simple ordered vertex sequences with one
// chosen start. It accepts exactly when an allowed closing edge exists.
bool definition_oracle(bool directed, int n, const std::vector<InputEdge> &edges) {
    if (n > 10) fail("definition oracle fixture too large");
    std::vector<unsigned int> adjacent(n, 0);
    std::vector<std::vector<int>> multiplicity(n, std::vector<int>(n, 0));
    for (auto edge : edges) {
        const int u = edge.first;
        const int v = edge.second;
        if (u == v) return true;
        adjacent[u] |= 1U << v;
        if (!directed) {
            adjacent[v] |= 1U << u;
            const int a = std::min(u, v);
            const int b = std::max(u, v);
            if (++multiplicity[a][b] >= 2) return true;
        }
    }
    const unsigned int end = 1U << n;
    for (int start = 0; start < n; ++start) {
        std::vector<unsigned int> reachable(end, 0);
        reachable[1U << start] = 1U << start;
        for (unsigned int used = 0; used < end; ++used) {
            int vertex_count = 0;
            for (unsigned int remaining = used; remaining != 0; remaining &= remaining - 1U) {
                ++vertex_count;
            }
            const bool enough_vertices = directed || vertex_count >= 3;
            unsigned int last_set = reachable[used];
            while (last_set != 0) {
                const unsigned int last_bit = last_set & (0U - last_set);
                last_set -= last_bit;
                int last = 0;
                for (unsigned int bit = last_bit; bit > 1U; bit >>= 1) ++last;
                if (enough_vertices && ((adjacent[last] >> start) & 1U)) return true;
                unsigned int choices = adjacent[last] & ~used;
                while (choices != 0) {
                    const unsigned int next_bit = choices & (0U - choices);
                    choices -= next_bit;
                    reachable[used | next_bit] |= next_bit;
                }
            }
        }
    }
    return false;
}

template <class Cost>
EdgeValue value_of(const Edge<Cost> &edge) {
    if constexpr (std::is_void<Cost>::value) {
        return {edge.from, edge.to, edge.index, 1};
    } else {
        return {edge.from, edge.to, edge.index, edge.cost};
    }
}

template <bool Directed, class Cost>
std::vector<std::vector<EdgeValue>> snapshot(const Graph<Directed, Cost> &graph) {
    const int n = graph.template size<int>();
    std::vector<std::vector<EdgeValue>> rows(n);
    for (int v = 0; v < n; ++v) {
        for (const auto &edge : graph.out_edges(v)) rows[v].emplace_back(value_of(edge));
        std::sort(rows[v].begin(), rows[v].end());
    }
    return rows;
}

template <class Cost>
std::vector<EdgeValue> serialize(const optional<vc<Edge<Cost>>> &result) {
    std::vector<EdgeValue> values;
    if (result) {
        for (const auto &edge : *result) values.emplace_back(value_of(edge));
    }
    return values;
}

template <bool Directed, class Cost>
void check(const Graph<Directed, Cost> &graph,
           const std::vector<InputEdge> &edges,
           int expected = -1,
           const std::vector<long long> &weights = {}) {
    ++cases;
    const int n = graph.template size<int>();
    if (expected == -1) expected = definition_oracle(Directed, n, edges) ? 1 : 0;
    const auto before = snapshot(graph);
    for (int repetition = 0; repetition < 2; ++repetition) {
        const auto result = cycle_detection(graph);
        ++checked_calls;
        if (static_cast<bool>(result) != (expected != 0)) fail("cycle existence");
        if (!result) continue;
        if (result->empty()) fail("empty successful cycle");
        std::vector<unsigned char> used_vertex(n, 0);
        std::vector<unsigned char> used_identity(edges.size(), 0);
        for (std::size_t p = 0; p < result->size(); ++p) {
            const auto &edge = (*result)[p];
            if (edge.from < 0 || edge.from >= n || edge.to < 0 || edge.to >= n) fail("vertex range");
            if (edge.index < 0 || static_cast<std::size_t>(edge.index) >= edges.size()) fail("identity range");
            if (used_vertex[edge.from]) fail("repeated cycle vertex");
            if (used_identity[edge.index]) fail("repeated edge identity");
            used_vertex[edge.from] = used_identity[edge.index] = 1;
            const auto input = edges[edge.index];
            const bool forward = edge.from == input.first && edge.to == input.second;
            if constexpr (Directed) {
                if (!forward) fail("directed orientation");
            } else {
                const bool reverse = edge.from == input.second && edge.to == input.first;
                if (!forward && !reverse) fail("undirected endpoints");
            }
            if (edge.to != (*result)[(p + 1) % result->size()].from) fail("traversal does not close");
            if constexpr (std::is_void<Cost>::value) {
                if (edge.cost != 1) fail("unweighted cost");
            } else {
                if (edge.cost != weights[edge.index]) fail("weight changed");
            }
        }
    }
    if (snapshot(graph) != before) fail("graph changed");
}

void exhaustive_directed() {
    for (int n = 0; n <= 4; ++n) {
        const unsigned int end = 1U << (n * n);
        for (unsigned int mask = 0; mask < end; ++mask) {
            std::vector<InputEdge> edges;
            for (int u = 0; u < n; ++u) {
                for (int v = 0; v < n; ++v) {
                    if ((mask >> (u * n + v)) & 1U) edges.emplace_back(u, v);
                }
            }
            const GraphDirected<void> graph(n, edges);
            check(graph, edges);
        }
    }
}

void exhaustive_undirected() {
    for (int n = 0; n <= 4; ++n) {
        std::vector<InputEdge> pairs;
        for (int u = 0; u < n; ++u) {
            for (int v = u; v < n; ++v) pairs.emplace_back(u, v);
        }
        unsigned int end = 1;
        for (std::size_t e = 0; e < pairs.size(); ++e) end *= 3;
        for (unsigned int code = 0; code < end; ++code) {
            unsigned int digits = code;
            std::vector<InputEdge> edges;
            for (auto pair : pairs) {
                const unsigned int count = digits % 3;
                digits /= 3;
                for (unsigned int copy = 0; copy < count; ++copy) edges.emplace_back(pair);
            }
            const GraphUndirected<void> graph(n, edges);
            check(graph, edges);
        }
    }
}

void random_cases() {
    std::mt19937_64 random(0x20261023C1C1EULL);
    const long long weight_choices[] = {
        std::numeric_limits<long long>::min(),
        std::numeric_limits<long long>::max(), -1, 0, 1};
    for (int trial = 0; trial < 1800; ++trial) {
        const int n = static_cast<int>(random() % 10);
        const int count = n == 0 ? 0 : static_cast<int>(random() % 36);
        std::vector<InputEdge> edges;
        std::vector<std::tuple<int, int, long long>> weighted;
        std::vector<long long> weights;
        for (int e = 0; e < count; ++e) {
            const int u = static_cast<int>(random() % n);
            const int v = static_cast<int>(random() % n);
            const long long weight = weight_choices[random() % 5];
            edges.emplace_back(u, v);
            weighted.emplace_back(u, v, weight);
            weights.emplace_back(weight);
        }
        const GraphDirected<void> directed(n, edges);
        const GraphUndirected<void> undirected(n, edges);
        const GraphDirected<long long> weighted_directed(n, weighted);
        const GraphUndirected<long long> weighted_undirected(n, weighted);
        check(directed, edges);
        check(undirected, edges);
        check(weighted_directed, edges, -1, weights);
        check(weighted_undirected, edges, -1, weights);
    }

    // These inputs force the oracle to examine acyclic graphs without loops.
    for (int trial = 0; trial < 600; ++trial) {
        const int n = 1 + static_cast<int>(random() % 9);
        std::vector<InputEdge> dag;
        std::vector<InputEdge> forest;
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                if (random() % 3 == 0) dag.emplace_back(u, v);
            }
            if (u > 0 && random() % 3 != 0) {
                int v = static_cast<int>(random() % u);
                if (random() & 1ULL) forest.emplace_back(u, v);
                else forest.emplace_back(v, u);
            }
        }
        const GraphDirected<void> directed(n, dag);
        const GraphUndirected<void> undirected(n, forest);
        check(directed, dag);
        check(undirected, forest);
    }
}

template <bool Directed>
void rebuild_case() {
    const int n = 8;
    std::vector<InputEdge> edges;
    for (int v = 0; v + 1 < n; ++v) edges.emplace_back(v, v + 1);
    Graph<Directed, void> graph(n, edges);
    check(graph, edges);
    edges.emplace_back(n - 1, 0);
    graph = Graph<Directed, void>(n, edges);
    check(graph, edges);
    const auto saved = cycle_detection(graph);
    const auto before_saved = serialize(saved);
    edges = {{3, 3}};
    graph = Graph<Directed, void>(n, edges);
    check(graph, edges);
    if (serialize(saved) != before_saved) fail("earlier owned cycle changed");

    std::vector<std::tuple<int, int, long long>> weighted{{1, 2, -7}, {2, 1, 9}};
    const std::vector<InputEdge> weighted_edges{{1, 2}, {2, 1}};
    const std::vector<long long> weights{-7, 9};
    Graph<Directed, long long> weighted_graph(n, weighted);
    check(weighted_graph, weighted_edges, 1, weights);
    const auto saved_weighted = cycle_detection(weighted_graph);
    const auto before_weighted = serialize(saved_weighted);
    weighted = {{0, 0, std::numeric_limits<long long>::max()}};
    weighted_graph = Graph<Directed, long long>(n, weighted);
    check(weighted_graph, {{0, 0}}, 1, {std::numeric_limits<long long>::max()});
    if (serialize(saved_weighted) != before_weighted) fail("earlier weight changed");
}

void long_cases() {
    const int n = 200000;
    for (int reverse_input = 0; reverse_input < 2; ++reverse_input) {
        std::vector<InputEdge> edges;
        edges.reserve(n);
        for (int v = 0; v + 1 < n; ++v) edges.emplace_back(v, v + 1);
        if (reverse_input) std::reverse(edges.begin(), edges.end());
        const GraphDirected<void> directed_path(n, edges);
        const GraphUndirected<void> undirected_path(n, edges);
        check(directed_path, edges, 0);
        check(undirected_path, edges, 0);
        if (reverse_input) std::reverse(edges.begin(), edges.end());
        edges.emplace_back(n - 1, 0);
        if (reverse_input) std::reverse(edges.begin(), edges.end());
        const GraphDirected<void> directed_cycle(n, edges);
        const GraphUndirected<void> undirected_cycle(n, edges);
        check(directed_cycle, edges, 1);
        check(undirected_cycle, edges, 1);
    }

    std::vector<InputEdge> star;
    for (int v = 1; v < n; ++v) star.emplace_back(0, v);
    const GraphDirected<void> directed_star(n, star);
    const GraphUndirected<void> undirected_star(n, star);
    check(directed_star, star, 0);
    check(undirected_star, star, 0);

    std::vector<InputEdge> disconnected{{0, 1}, {2, 3}, {4, 5}, {6, 7}, {7, 8}, {8, 6}};
    const GraphDirected<void> directed_disconnected(n, disconnected);
    const GraphUndirected<void> undirected_disconnected(n, disconnected);
    check(directed_disconnected, disconnected, 1);
    check(undirected_disconnected, disconnected, 1);
}
}  // namespace own_cycle_fixture

using PublicDirected = Graph<true, void>;
using PublicUndirected = Graph<false, long long>;
static_assert(std::is_same<
    decltype(cycle_detection(std::declval<const PublicDirected &>())),
    optional<vc<Edge<void>>>>::value, "directed public type");
static_assert(std::is_same<
    decltype(cycle_detection(std::declval<const PublicUndirected &>())),
    optional<vc<Edge<long long>>>>::value, "undirected public type");
static_assert(!noexcept(cycle_detection(std::declval<const PublicDirected &>())),
              "public exception specification");

int main() {
    try {
        own_cycle_fixture::exhaustive_directed();
        own_cycle_fixture::exhaustive_undirected();
        own_cycle_fixture::random_cases();
        own_cycle_fixture::rebuild_case<true>();
        own_cycle_fixture::rebuild_case<false>();
        own_cycle_fixture::long_cases();
        std::cout << "Hello World\n";
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "FAIL after_cases=" << own_cycle_fixture::cases << " " << error.what() << '\n';
        return 1;
    }
}
