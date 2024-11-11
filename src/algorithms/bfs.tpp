#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Queue.hpp"
#include "../../../Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::breadth_first_search(VertexType start) {
    // Check if graph is empty
    if (vertices_.empty()) {
        throw std::runtime_error("Cannot perform BFS on empty graph");
    }

    // Check if start vertex exists
    if (!has_vertex(start)) {
        throw std::runtime_error("Start vertex does not exist in graph");
    }

    // Reset all vertices
    for (auto& [id, vertex] : vertices_) {
        vertex.set_color(0);
        vertex.set_discovery_time(-1);
        vertex.set_finish_time(-1);
    }

    // Create parameters for logging
    json parameters;
    parameters["vertex_count"] = vertices_.size();
    parameters["start_vertex"] = start;
    save_json_to_file("bfs_parameters.json", parameters);

    Queue<VertexType> queue;
    size_t timer = 0;
    
    queue.enqueue(start);
    vertices_[start].set_color(1); // Gray
    vertices_[start].set_discovery_time(timer++);

    while (!queue.empty()) {
        VertexType current = queue.front();
        queue.dequeue();

        // Check all neighbors of current vertex
        for (const auto& [neighbor, edge] : adjacency_list_[current]) {
            if (vertices_[neighbor].get_color() == 0) { // White vertex
                queue.enqueue(neighbor);
                vertices_[neighbor].set_color(1); // Gray
                vertices_[neighbor].set_discovery_time(timer++);
            }
        }

        // Finish current vertex
        vertices_[current].set_color(2); // Black
        vertices_[current].set_finish_time(timer++);
    }
}
