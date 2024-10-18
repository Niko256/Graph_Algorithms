#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Stack.hpp"
#include <string>
#include <unordered_set>

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::depth_first_search(VertexType start) {

    Stack<VertexType> stack;
    std::unordered_set<VertexType> visited;

    stack.push(start);

    while (!stack.empty()) {
        VertexType current = stack.top();
        stack.pop();

        if (visited.find(current) == visited.end()) {
            visited.insert(current);

            for (const auto& neighbor : adjacency_list_[current]) {
                stack.push(neighbor.first_);
            }
        }
    }

}

template void Graph<int, int>::depth_first_search(int start);
