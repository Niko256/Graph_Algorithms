#include "../../include/graph.hpp"
#include "../../../Data_Structures/Containers/Stack.hpp"
#include "../../../Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;


template <typename VertexType, typename WeightType>
DynamicArray<DynamicArray<VertexType>> Graph<VertexType, WeightType>::find_connected_components() {
    if (vertex_count_ == 0) throw std::runtime_error("Cannot find components in empty graph");

    if (adjacency_list_.size() == 0) {
        throw std::runtime_error("Adjacency list is not initialized");
    }

    DynamicArray<DynamicArray<VertexType>> components;
    
    // Reset all vertices to unvisited (color 0)
    for (auto& [vertex_id, vertex] : vertices_) {
        vertex.set_color(0);
    }

    // Process each unvisited vertex
    for (auto& [start_vertex, vertex] : vertices_) {
        if (vertex.get_color() == 0) { // Unvisited vertex
            // Start new component
            DynamicArray<VertexType> current_component;
            Stack<VertexType> stack;
            
            // Initialize DFS from current vertex
            stack.push(start_vertex);
            vertices_[start_vertex].set_color(1); // Mark as visited
            current_component.push_back(start_vertex);

            while (!stack.empty()) {
                VertexType current = stack.top();
                stack.pop();

                // Check all neighbors in adjacency list
                for (const auto& [neighbor, edge] : adjacency_list_[current]) {
                    if (vertices_[neighbor].get_color() == 0) { // Unvisited neighbor
                        stack.push(neighbor);
                        vertices_[neighbor].set_color(1); // Mark as visited
                        current_component.push_back(neighbor);
                    }
                }
            }
            
            // Add completed component to results
            components.push_back(std::move(current_component));
        }
    }

    // Create JSON output
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
