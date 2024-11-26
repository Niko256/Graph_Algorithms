#include "../../include/graph.hpp"
#include "../../dependencies/Data_Structures/Containers/Queue.hpp"
#include "../../dependencies/Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>

template <typename VertexType, typename Resource, typename WeightType>
void Graph<VertexType, Resource, WeightType>::breadth_first_search(VertexType start) {
    // Check if graph is empty
    if (vertex_pool_.empty()) {
        throw std::runtime_error("Cannot perform BFS on empty graph");
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
    save_json_to_file("bfs_parameters.json", parameters);

    Queue<VertexType> queue;
    size_t timer = 0;
    
    queue.enqueue(start);
    vertex_pool_[start].set_color(1); // Gray
    vertex_pool_[start].set_discovery_time(timer++);

    while (!queue.empty()) {
        VertexType current = queue.front();
        queue.dequeue();

        // Check all neighbors of current vertex
        for (const auto& [neighbor, edge] : adjacency_list_[current]) {
            if (vertex_pool_[neighbor].get_color() == 0) { // White vertex
                queue.enqueue(neighbor);
                vertex_pool_[neighbor].set_color(1); // Gray
                vertex_pool_[neighbor].set_discovery_time(timer++);
            }
        }

        // Finish current vertex
        vertex_pool_[current].set_color(2); // Black
        vertex_pool_[current].set_finish_time(timer++);
    }
}
