#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Stack.hpp"
#include "../../../Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::depth_first_search(VertexType start) {
    // Check if graph is empty
    if (vertices_.empty()) {
        throw std::runtime_error("Cannot perform DFS on empty graph");
    }

    // Check if start vertex exists
    if (!has_vertex(start)) {
        throw std::runtime_error("Start vertex does not exist in graph");
    }

    reset_parameters();

    // Create parameters for logging
    json parameters;
    parameters["vertex_count"] = vertices_.size();
    parameters["start_vertex"] = start;
    save_json_to_file("dfs_parameters.json", parameters);

    Stack<VertexType> stack;
    size_t timer = 0;

    // Start DFS from the start vertex
    stack.push(start);
    vertices_[start].set_color(1); // Gray
    vertices_[start].set_discovery_time(timer++);

    // Handle single-vertex case
    if (vertices_.size() == 1) {
        vertices_[start].set_color(2); // Black
        vertices_[start].set_finish_time(timer++);
        return;
    }

    while (!stack.empty()) {
        VertexType current = stack.top();
        bool has_unvisited_neighbors = false;

        // Check all neighbors of current vertex
        for (const auto& [neighbor, edge] : adjacency_list_[current]) {
            if (vertices_[neighbor].get_color() == 0) { // White vertex
                stack.push(neighbor);
                vertices_[neighbor].set_color(1); // Gray
                vertices_[neighbor].set_discovery_time(timer++);
                has_unvisited_neighbors = true;
                break;
            }
        }

        // If all neighbors are visited, finish current vertex
        if (!has_unvisited_neighbors) {
            stack.pop();
            vertices_[current].set_color(2); // Black
            vertices_[current].set_finish_time(timer++);
        }
    }
}
