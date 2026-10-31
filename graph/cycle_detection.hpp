#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include "graph.hpp"

/**
 * @brief サイクル検出
 * @docs docs/graph/cycle_detection.md
 */

template <bool is_directed, class Cost>
optional<vc<Edge<Cost>>> cycle_detection(const Graph<is_directed, Cost>& g) {
    const int n = g.template size<int>();
    std::vector<int> position(n, -1);
    struct Frame {
        int vertex;
        int next;
    };
    std::vector<Frame> frames;
    std::vector<Edge<Cost>> path_edges;
    frames.reserve(n);
    path_edges.reserve(n);

    for (int root = 0; root < n; ++root) {
        if (position[root] != -1) continue;
        frames.push_back({root, 0});
        position[root] = 0;
        while (!frames.empty()) {
            Frame &frame = frames.back();
            const int vertex = frame.vertex;
            if (static_cast<std::size_t>(frame.next) == g.out_edges(vertex).size()) {
                position[vertex] = -2;
                frames.pop_back();
                if (!path_edges.empty()) path_edges.pop_back();
                continue;
            }

            const Edge<Cost> edge = g.out_edges(vertex)[frame.next++];
            if constexpr (!is_directed) {
                if (!path_edges.empty() && edge.index == path_edges.back().index) continue;
            }
            const int target_position = position[edge.to];
            if (target_position == -1) {
                position[edge.to] = static_cast<int>(frames.size());
                path_edges.emplace_back(edge);
                frames.push_back({edge.to, 0});
                continue;
            }
            if (target_position >= 0) {
                vc<Edge<Cost>> cycle;
                cycle.reserve(path_edges.size() - static_cast<std::size_t>(target_position) + 1);
                for (std::size_t p = static_cast<std::size_t>(target_position);
                     p < path_edges.size(); ++p) {
                    cycle.emplace_back(path_edges[p]);
                }
                cycle.emplace_back(edge);
                return optional<vc<Edge<Cost>>>(std::move(cycle));
            }
        }
    }
    return nullopt;
}
