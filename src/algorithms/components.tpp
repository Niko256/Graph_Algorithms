#include "../../include/graph.hpp"
#include "../../dependencies/Data_Structures/Containers/Stack.hpp"
#include "../../dependencies/Data_Structures/Containers/Dynamic_Array.hpp"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;


template <typename VertexId, typename Resource, typename WeightType>
DynamicArray<DynamicArray<VertexId>> Graph<VertexId, Resource, WeightType>::find_connected_components() {
    if (vertex_count_ == 0) {
        throw std::runtime_error("Cannot find components in empty graph");
    }

    if (vertex_pool_.empty()) {
        throw std::runtime_error("Vertex pool is not initialized");
    }

    DynamicArray<DynamicArray<VertexId>> components;
    std::unordered_map<VertexId, bool> visited;

    // Initialize visited map for all vertices
    for (const auto& [id, _] : vertex_pool_) {
        visited[id] = false;
    }

    // Process each unvisited vertex
    for (const auto& [vertex_id, _] : vertex_pool_) {
        if (!visited[vertex_id]) {
            DynamicArray<VertexId> current_component;
            Stack<VertexId> stack;
            
            // Process current component
            stack.push(vertex_id);
            visited[vertex_id] = true;
            current_component.push_back(vertex_id);

            while (!stack.empty()) {
                VertexId current = stack.top();
                stack.pop();

                // Check if current vertex exists in adjacency list
                auto adj_it = adjacency_list_.find(current);
                if (adj_it != adjacency_list_.end()) {
                    // Process all neighbors

                    auto adjacencies = (*adj_it).second;

                    for (const auto& [neighbor_id, edge] : adjacencies) {
                        if (!visited[neighbor_id]) {
                            stack.push(neighbor_id);
                            visited[neighbor_id] = true;
                            current_component.push_back(neighbor_id);
                        }
                    }
                }
            }
            
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
        for (const auto& vertex : components[i]) {
            component["vertices"].push_back(vertex);
        }
        components_data["components"].push_back(component);
    }

    // Save to file with directory creation
    std::string directory = "files";
    if (!std::filesystem::exists(directory)) {
        std::filesystem::create_directory(directory);
    }
    save_json_to_file("components.json", components_data);

    return components;
}
