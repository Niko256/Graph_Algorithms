#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Stack.hpp"
#include "../../../Data_Structures/Containers/Dynamic_Array.hpp"

size_t dfs_timer = 0;


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::reset_timer() const {
    dfs_timer = 0;
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::depth_first_search(VertexType start) {
    reset_timer();

    Stack<VertexType> stack;

    // 0 -> white, 1 -> gray, 2 -> black
    DynamicArray<int> colors(adjacency_list_.size(), 0);

    discovery_time_ = DynamicArray<int>(adjacency_list_.size(), 0);  
    finish_time_ = DynamicArray<int>(adjacency_list_.size(), 0);    



    stack.push(start);
    colors[start] = 1; // Mark the starting vertex as gray 
    discovery_time_[start] = dfs_timer++;

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
                discovery_time_[neighbour_vertex] = dfs_timer++;
                has_unvisited_neighbours = true;


                break; // Move to processing the new vertex 
            }
        }

        // If all neighbors are already visited
        if (!has_unvisited_neighbours) {
            stack.pop();
            colors[current] = 2;
            finish_time_[current] = dfs_timer++;

        }
    }
}
