#include "../../include/graph.hpp"
#include "../../dependencies/Data_Structures/Containers/Dynamic_Array.hpp"
#include "../../dependencies/Data_Structures/Containers/Priority_Queue.hpp"
#include "../../dependencies/Data_Structures/Containers/Queue.hpp"
#include <limits>
#include <stdexcept>
#include <string>


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::shortest_paths_unweighted(VertexId start) {
    if (vertex_pool_.empty()) {
        throw std::runtime_error("Cannot find shortest paths in empty graph");
    }
    if (!has_vertex(start)) {
        throw std::runtime_error("Start vertex does not exist");
    }

    reset_parameters();

    HashTable<VertexId, int> distances;
    HashTable<VertexId, VertexId> previous;
    
    for (const auto& [vertex_id, _] : vertex_pool_) {
        distances[vertex_id] = -1; // -1 means infinity 
        previous[vertex_id] = vertex_id;
    }
    
    Queue<VertexId> queue;
    queue.enqueue(start);
    distances[start] = 0;
    vertex_pool_[start].set_color(1); // 1 -> Gray 

    while (!queue.empty()) {
        VertexId current = queue.front();
        queue.dequeue();

        for (const auto& [neighbor, _] : adjacency_list_[current]) {
            if (distances[neighbor] == -1) { // unvisited vertex 
                distances[neighbor] = distances[current] + 1;
                previous[neighbor] = current;
                queue.enqueue(neighbor);
                vertex_pool_[neighbor].set_color(1);
            }
        }
        vertex_pool_[current].set_color(2); // 2 -> Black 
    }

    json result;
    result["distances"] = json::object();
    result["paths"] = json::object();

    for (const auto& [vertex_id, distance] : distances) {
        result["distances"][std::to_string(vertex_id)] = distance;
    
        size_t path_length = 0;
        VertexId current = vertex_id;
        
        while (current != start && distances[current] != -1) {
            path_length++;
            current = previous[current];
        }
        
        if (distances[vertex_id] != -1) {
            path_length++; // adding start vertex 
        }

        DynamicArray<VertexId> path(path_length);
    
        current = vertex_id;
        size_t index = path_length - 1;
        
        while (current != start && distances[current] != -1) {
            path[index--] = current;
            current = previous[current];
        }
        
        if (distances[vertex_id] != -1) {
            path[0] = start;
        }
    
        json path_array = json::array();
        
        for (size_t i = 0; i < path_length; ++i) {
            path_array.push_back(path[i]);
        }
        result["paths"][std::to_string(vertex_id)] = path_array;
    }

    save_json_to_file("shortest_paths_unweighted.json", result);
}
