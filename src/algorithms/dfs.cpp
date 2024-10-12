#include "../../include/graph.hpp"
#include "../../include/logger.hpp"
#include "../../../Data_Structures/Stack.hpp"
#include <string>
#include <unordered_set>

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::depth_first_search(VertexType start) {
    Logger logger("../../visualization/data/graph_data.txt");
    Stack<VertexType> stack;
    std::unordered_set<VertexType> visited;

    stack.push(start);
    logger.log("Start DFS from vertex " + std::to_string(start));

    while (!stack.empty()) {
        VertexType current = stack.top();
        stack.pop();

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            logger.log("Visited vertex " + std::to_string(current));

            for (const auto& neighbor : adjacency_list_[current]) {
                stack.push(neighbor.first_);
                logger.log("Pushed vertex " + std::to_string(neighbor.first_) + " to stack");
            }
        }
    }
}

template void Graph<int, int>::depth_first_search(int start);
