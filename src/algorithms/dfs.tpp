#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Stack.hpp"
#include "../../../Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>


namespace fs = std::filesystem;


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::depth_first_search(VertexType start) {
    if (vertex_count_ == 0) throw std::runtime_error("Cannot perform DFS on empty graph");

    if (start >= vertex_count_) throw std::runtime_error("Invalid vertex index");

    reset_timer();

    Stack<VertexType> stack;

    // 0 -> white, 1 -> gray, 2 -> black
    DynamicArray<int> colors(adjacency_list_.size(), 0);

    discovery_time_ = DynamicArray<int>(adjacency_list_.size(), 0);  
    finish_time_ = DynamicArray<int>(adjacency_list_.size(), 0);    

    json parameters;
    parameters["vertex_count"] = vertex_count_;
    parameters["start_vertex"] = start;
    save_json_to_file("bfs_parameters.json", parameters);


    stack.push(start);
    colors[start] = 1; // Mark the starting vertex as gray 
    discovery_time_[start] = traversal_timer_++;

    // for single-vertex graph 
    if (vertex_count_ == 1) {
        colors[start] = 2;
        finish_time_[start] = traversal_timer_++;
        return;
    }


    while (!stack.empty()) {
        VertexType current = stack.top();

        if (colors[current] == 2) { 
            // If the vertex is already black, just pop it from the stack
            stack.pop();
            continue;
        }

        // A flag to check if there are any unvisited neighbors.
        bool has_unvisited_neighbours = false;

        for (auto& neighbour: adjacency_list_[current]) {
            VertexType neighbour_vertex = neighbour.first_;

            if (colors[neighbour_vertex] == 0) {
                stack.push(neighbour_vertex);
                colors[neighbour_vertex] = 1;
                discovery_time_[neighbour_vertex] = traversal_timer_++;
                has_unvisited_neighbours = true;

                break; // Move to processing the new vertex 
            }
        }

        // If all neighbors are already visited
        if (!has_unvisited_neighbours) {
            stack.pop();
            colors[current] = 2;
            finish_time_[current] = traversal_timer_++;
        }
    }
}
