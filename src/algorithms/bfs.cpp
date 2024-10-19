
#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Queue.hpp"
#include "../../../Data_Structures/Containers/Dynamic_Array.hpp"

size_t g_timer = 0;


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::breadth_first_search(VertexType start) {
    
    Queue<VertexType> queue;

    // 0 -> white (unvisited)
    // 1 -> gray (discovered but not fully explored)
    // 2 -> black (fully explored)
    DynamicArray<int> colors(adjacency_list_.size(), 0);

    DynamicArray<size_t> discovery_time(adjacency_list_.size(), 0);
    DynamicArray<size_t> finish_time(adjacency_list_.size(), 0);

    queue.enqueue(start);
    colors[start] = 1; // Mark the starting vertex as gray 
    discovery_time[start] = g_timer++;

    while (!queue.empty()) {
        VertexType current = queue.front();
        queue.dequeue();

        for (auto& neighbour : adjacency_list_[current]) {
            VertexType neighbour_vertex = neighbour.first_;

            if (colors[neighbour_vertex] == 0) {
                queue.enqueue(neighbour_vertex);
                colors[neighbour_vertex] = 1;
                discovery_time[neighbour_vertex] = g_timer++;
            }
        }

        colors[current] = 2;
        finish_time[current] = g_timer++;
    }
}


template void Graph<int, int>::breadth_first_search(int start);
