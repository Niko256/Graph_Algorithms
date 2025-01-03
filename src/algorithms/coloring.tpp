#include "../../include/graph.hpp"
#include "../../dependencies/Data_Structures/Containers/Stack.hpp"
#include "../../dependencies/Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <string>


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
    
    std::vector<VertexId> vertices;
    vertices.reserve(vertex_pool_.size());
    
    for (const auto& [vertex_id, _] : vertex_pool_) {
        if (vertex_id != start) {
            vertices.push_back(vertex_id);
        }
    }
    
    std::sort(vertices.begin(), vertices.end(),
        [this](const VertexId& a, const VertexId& b) {
            return get_degree(a) > get_degree(b);
        });
    
    vertices.insert(vertices.begin(), start);

    vertex_pool_[start].set_color(0);

    for (const auto& current : vertices) {
        std::fill(used_colors.begin(), used_colors.end(), false);
        
        for (const auto& [neighbor, _] : adjacency_list_[current]) {
            int neighbor_color = vertex_pool_[neighbor].get_color();
            if (neighbor_color >= 0) {
                used_colors[neighbor_color] = true;
            }
        }
        
        int color = 0;
        while (color < vertex_count_ && used_colors[color]) {
            ++color;
        }
        
        vertex_pool_[current].set_color(color);
    }

    json result;
    result["coloring"] = json::object();
    
    for (const auto& [vertex_id, vertex] : vertex_pool_) {
        result["coloring"][std::to_string(vertex_id)] = vertex.get_color();
    }

    save_json_to_file("greedy_coloring_results.json", result);
}
