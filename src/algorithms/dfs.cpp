#include "../../include/graph.hpp"
#include "../../../Data_Structures/Stack.hpp"
#include <string>
#include <unordered_set>

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::depth_first_search(VertexType start) {
    std::string log_file = "algorithm_log.txt";
    std::string algorithm_name = "Depth First Search (DFS)";

    log_state(log_file, algorithm_name + " - Initial state");

    Stack<VertexType> stack;
    std::unordered_set<VertexType> visited;

    stack.push(start);
    log_state(log_file, algorithm_name + " - Pushed start vertex " + std::to_string(start) + " onto the stack");

    while (!stack.empty()) {
        VertexType current = stack.top();
        stack.pop();
        log_state(log_file, algorithm_name + " - Popped vertex " + std::to_string(current) + " from the stack");

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            log_state(log_file, algorithm_name + " - Visited vertex " + std::to_string(current));

            for (const auto& neighbor : adjacency_list_[current]) {
                stack.push(neighbor.first_);
                log_state(log_file, algorithm_name + " - Pushed neighbor " + std::to_string(neighbor.first_) + " of vertex " + std::to_string(current) + " onto the stack");
            }
        }
    }

    log_state(log_file, algorithm_name + " - DFS completed");
}

template void Graph<int, int>::depth_first_search(int start);
