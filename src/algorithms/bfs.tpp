#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <queue>
#include <stdexcept>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::breadth_first_search(VertexType start) {
    if (adjacency_list_.size() == 0) throw std::runtime_error("Cannot perform BFS on empty graph");

    if (start >= vertex_count_ || start < 0) throw std::runtime_error("Invalid start vertex");

    reset_timer();
    
    std::queue<VertexType> queue;

    // 0 -> white, 1 -> gray, 2 -> black
    DynamicArray<int> colors(adjacency_list_.size(), 0);

    discovery_time_ = DynamicArray<int>(adjacency_list_.size(), 0);  
    finish_time_ = DynamicArray<int>(adjacency_list_.size(), 0);    

    json parameters;
    parameters["vertex_count"] = vertex_count_;
    parameters["start_vertex"] = start;
    save_json_to_file("bfs_parameters.json", parameters);

    queue.push(start); 
    colors[start] = 1; // Mark the starting vertex as gray
    discovery_time_[start] = traversal_timer_++;

    // for single-vertex graph 
    if (vertex_count_ == 1) {
        colors[start] = 2;
        finish_time_[start] = traversal_timer_++;
        return;
    }


    while (!queue.empty()) {
        VertexType current = queue.front();
        queue.pop();

        for (auto& neighbour : adjacency_list_[current]) {
            VertexType neighbour_vertex = neighbour.first_;

            if (colors[neighbour_vertex] == 0) {
                queue.push(neighbour_vertex);
                colors[neighbour_vertex] = 1;
                discovery_time_[neighbour_vertex] = traversal_timer_++;
            }
        }

        colors[current] = 2;
        finish_time_[current] = traversal_timer_++;
    }
}
