#include "../../include/graph.hpp"
#include "../../dependencies/Data_Structures/Containers/Stack.hpp"
#include "../../dependencies/Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::depth_first_search(VertexId start) {
    // Check if graph is empty
    if (vertex_pool_.empty()) {
        throw std::runtime_error("Cannot perform DFS on empty graph");
    }

    // Check if start vertex exists
    if (!has_vertex(start)) {
        throw std::runtime_error("Start vertex does not exist in graph");
    }

    reset_parameters();

    // Create parameters for logging
    json parameters;
    parameters["vertex_count"] = vertex_pool_.size();
    parameters["start_vertex"] = start;
    save_json_to_file("dfs_parameters.json", parameters);

    Stack<VertexId> stack;
    size_t timer = 0;

    // Start DFS from the start vertex
    stack.push(start);
    vertex_pool_[start].set_color(1); // Gray
    vertex_pool_[start].set_discovery_time(timer++);

    // Handle single-vertex case
    if (vertex_pool_.size() == 1) {
        vertex_pool_[start].set_color(2); // Black
        vertex_pool_[start].set_finish_time(timer++);
        return;
    }

    while (!stack.empty()) {
        VertexId current = stack.top();
        bool has_unvisited_neighbors = false;

        // Check all neighbors of current vertex
        for (const auto& [neighbor, edge_ptr] : adjacency_list_[current]) {
            if (vertex_pool_[neighbor].get_color() == 0) { // White vertex
                stack.push(neighbor);
                vertex_pool_[neighbor].set_color(1); // Gray
                vertex_pool_[neighbor].set_discovery_time(timer++);
                has_unvisited_neighbors = true;
                break;
            }
        }

        // If all neighbors are visited, finish current vertex
        if (!has_unvisited_neighbors) {
            stack.pop();
            vertex_pool_[current].set_color(2); // Black
            vertex_pool_[current].set_finish_time(timer++);
        }
    }
}
