#include <iostream>
#include "../include/graph.hpp"
#include <string>
#include <cstdlib>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

void create_graph_parameters_file(const std::string& filename) {
    int vertex_count;
    bool directed;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<bool> is_empty_vertex;

    std::cout << "Enter vertex count: ";
    std::cin >> vertex_count;

    std::cout << "Is the graph directed? (1 for yes, 0 for no): ";
    std::cin >> directed;

    is_empty_vertex.resize(vertex_count, false);
    std::cout << "Enter indices of empty vertices (enter -1 to stop):" << std::endl;
    int empty_vertex_index;
    while (true) {
        std::cin >> empty_vertex_index;
        if (empty_vertex_index == -1) break;
        if (empty_vertex_index >= 0 && empty_vertex_index < vertex_count) {
            is_empty_vertex[empty_vertex_index] = true;
        } else {
            std::cout << "Invalid vertex index. Please enter a valid index." << std::endl;
        }
    }

    int from, to, weight;
    std::cout << "Enter edges (format: from to weight, enter -1 to stop):" << std::endl;
    while (true) {
        std::cin >> from;
        if (from == -1) break;
        std::cin >> to >> weight;
        if (from >= 0 && from < vertex_count && to >= 0 && to < vertex_count) {
            edges.emplace_back(from, to, weight);
        } else {
            std::cout << "Invalid vertex index. Please enter valid indices." << std::endl;
        }
    }

    Graph<int, int> graph(vertex_count, directed);
    for (const auto& [from, to, weight] : edges) {
        graph.add_edge(from, to, weight);
    }

    for (int i = 0; i < vertex_count; ++i) {
        if (is_empty_vertex[i]) {
            graph.add_vertex();
        }
    }

    graph.save_to_json(filename);
}

int main() {
    std::string directory = "files";
    if (!fs::exists(directory)) {
        fs::create_directory(directory);
    }

    std::string graph_parameters_file = directory + "/graph_parameters.json";

    std::cout << "Welcome to the Graph Parameters Input Tool!" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    create_graph_parameters_file(graph_parameters_file);

    std::cout << "Graph parameters have been saved to " << graph_parameters_file << std::endl;

    return 0;
}
