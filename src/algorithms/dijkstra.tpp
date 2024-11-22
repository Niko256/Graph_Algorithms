#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Dynamic_Array.hpp"
#include "../../../Data_Structures/Containers/Pair.hpp"
#include "../../../Data_Structures/Containers/Priority_Queue.hpp"
#include "../../../Data_Structures/Containers/Queue.hpp"
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::dijkstra(VertexId start) {
    if (vertex_pool_.empty()) {
        throw std::runtime_error("Cannot perform Dijkstra on empty graph");
    }
    if (!has_vertex(start)) {
        throw std::runtime_error("Start vertex does not exist in graph");
    }

    
    // Check for negative weights which Dijkstra cannot handle
    for (const auto& [vertex_id, _] : vertex_pool_) {
        for (const auto& [neighbor, edge_ptr] : adjacency_list_[vertex_id]) {
            if (edge_ptr->get_weight() < 0) {
                throw std::runtime_error("Dijkstra's algorithm cannot handle negative weights");
            }
        }
    }


    // Reset graph state
    reset_parameters();

    json parameters;
    parameters["vertex_count"] = vertex_pool_.size();
    parameters["start_vertex"] = start;
    save_json_to_file("dijkstra_parameters.json", parameters);

    std::unordered_map<VertexId, WeightType> distances;
    std::unordered_map<VertexId, VertexId> previous;
    std::unordered_map<VertexId, size_t> visit_count;
    PriorityQueue<Pair<WeightType, VertexId>> pq;

    // Initialize distances
    for (const auto& [vertex_id, _] : vertex_pool_) {
        distances[vertex_id] = std::numeric_limits<WeightType>::max();
        previous[vertex_id] = vertex_id; // Initialize each vertex as its own predecessor
    }

    // Set start vertex
    distances[start] = 0;
    pq.push(0, {0, start}); // Pass priority and item
    vertex_pool_[start].set_color(1); // Mark as in progress

    // Main Dijkstra loop
    while (!pq.empty()) {
        auto current_node = pq.top();
        pq.pop();
        
        auto current = current_node.get_item(); // Extract Pair from PriorityNode
        VertexId current_vertex = current.second_;
        WeightType current_distance = current.first_;

        // Skip if we've found a better path already
        if (current_distance > distances[current_vertex]) {
            continue;
        }

        // Mark as processed
        vertex_pool_[current_vertex].set_color(2);

        // Process neighbors
        for (const auto& [neighbor, edge_ptr] : adjacency_list_[current_vertex]) {
            // Skip processed vertices
            if (vertex_pool_[neighbor].get_color() == 2) {
                continue;
            }

            WeightType edge_weight = edge_ptr->get_weight();
            WeightType new_distance = distances[current_vertex] + edge_weight;

            // Update if we found a shorter path
            if (new_distance < distances[neighbor]) {
                distances[neighbor] = new_distance;
                previous[neighbor] = current_vertex;
                pq.push(new_distance, {new_distance, neighbor}); // Pass priority and item
                vertex_pool_[neighbor].set_color(1);
            }
        }
    }

    // Save results
    json result;
    result["distances"] = json::object();
    result["paths"] = json::object();

    // Store distances and reconstruct paths
    for (const auto& [vertex_id, distance] : distances) {
        result["distances"][std::to_string(vertex_id)] = distance;
        
        // Reconstruct path
        std::vector<VertexId> path;
        VertexId current = vertex_id;
        while (current != start) {
            path.push_back(current);
            current = previous[current];
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        
        result["paths"][std::to_string(vertex_id)] = path;
    }

    save_json_to_file("dijkstra_results.json", result);
}
