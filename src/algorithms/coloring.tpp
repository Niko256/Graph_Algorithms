#include "../../include/graph.hpp"
#include "../../dependencies/Data_Structures/Containers/Stack.hpp"
#include "../../dependencies/Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::greedy_coloring(VertexId start) {
    if (vertex_pool_.empty()) { 
        throw std::runtime_error("Cannot perform coloring on empty graph"); 
    }

    if (!has_vertex(start)) { 
        throw std::runtime_error("Start vertex doesn't exist in graph"); 
    }

    reset_parameters();

    json parameters;
    parameters["vertex_count"] = vertex_pool_.size();
    parameters["start_vertex"] = start;
    save_json_to_file("greedy_algorithms_parameters.json", parameters);

    std::vector<bool> used_colors(vertex_count_, false);
    
    std::vector<VertexId> sorted_vertices;
    sorted_vertices.reserve(vertex_pool_.size());
    for (const auto& [vertex_id, _] : vertex_pool_) {
        if (vertex_id != start) {
            sorted_vertices.push_back(vertex_id);
        }
    }
    
    std::sort(sorted_vertices.begin(), sorted_vertices.end(),
        [this](const VertexId& a, const VertexId& b) {
            return adjacency_list_[a].size() > adjacency_list_[b].size();
        });

    vertex_pool_[start].set_color(0);
    sorted_vertices.insert(sorted_vertices.begin(), start);

    for (const VertexId& vertex_id : sorted_vertices) {
        std::fill(used_colors.begin(), used_colors.end(), false);

        for (const auto& [neighbour, _] : adjacency_list_[vertex_id]) {
            size_t neighbour_color = vertex_pool_[neighbour].get_color();
            if (neighbour_color < vertex_count_) {
                used_colors[neighbour_color] = true;
            }
        }

        size_t new_color = 0;
        while (new_color < vertex_count_ && used_colors[new_color]) {
            ++new_color;
        }

        vertex_pool_[vertex_id].set_color(new_color);
    }

    json result;
    result["coloring"] = json::object();
    
    for (const auto& [vertex_id, vertex] : vertex_pool_) {
        result["coloring"][std::to_string(vertex_id)] = vertex.get_color();
    }

    save_json_to_file("greedy_coloring_results.json", result);
}
