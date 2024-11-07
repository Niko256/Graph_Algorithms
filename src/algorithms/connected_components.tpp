#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Stack.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;


template <typename VertexType, typename WeightType>
DynamicArray<DynamicArray<VertexType>> Graph<VertexType, WeightType>::find_connected_components() {
    if (vertex_count_ == 0) throw std::runtime_error("Cannot find components in empty graph");

    if (adjacency_list_.size() == 0) {
        throw std::runtime_error("Adjacency list is not initialized");
    }

    DynamicArray<DynamicArray<VertexType>> components;
    DynamicArray<bool> visited(vertex_count_, false);

    for (VertexType vrt = 0; vrt < vertex_count_; ++vrt) {
        if (!visited[vrt]) {
            DynamicArray<VertexType> current_component;
            Stack<VertexType> stack;
            
            stack.push(vrt);
            visited[vrt] = true;
            current_component.push_back(vrt);

            while(!stack.empty()) {
                VertexType current = stack.top();
                stack.pop();

                for (const auto& neighbour: adjacency_list_[current]) {
                    VertexType neighbour_vertex = neighbour.first_;
                    if (!visited[neighbour_vertex]) {
                        stack.push(neighbour_vertex);
                        visited[neighbour_vertex] = true;
                        current_component.push_back(neighbour_vertex);
                    }
                }
            }
            components.push_back(std::move(current_component));
        }
    }


    json components_data;
    components_data["components_count"] = components.size();
    components_data["components"] = json::array();

    for (size_t i = 0; i < components.size(); ++i) {
        json component;
        component["vertices"] = json::array();

        for (const auto& vertex: components[i]) {
            component["vertices"].push_back(vertex);
        }
        components_data["components"].push_back(component);
    }

    save_json_to_file("components.json", components_data);
    return components;
}
