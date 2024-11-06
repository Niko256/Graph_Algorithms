#include <iostream>
#include <filesystem>
#include <vector>
#include <tuple>
#include <string>
#include "../include/graph.hpp"
#include <fstream>

namespace fs = std::filesystem;

void create_graph_parameters_file(const std::string& filename) {
    int vertex_count;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<bool> is_empty_vertex;

    std::cout << "Enter vertex count: ";
    std::cin >> vertex_count;

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

    Graph<int, int> graph(vertex_count);
    for (const auto& [from, to, weight] : edges) {
        graph.add_edge(from, to, weight);
    }

    graph.save_to_json(filename);
}



int main() {
    std::string directory = "files";
    if (!fs::exists(directory)) {
        fs::create_directory(directory);
    }

    std::string graph_parameters_file = directory + "/graph_parameters.json";

    Graph<int, int> graph;
    bool running = true;

    while (running) {
        std::cout << "\nGraph Operations Menu:" << std::endl;
        std::cout << "1. Create new graph" << std::endl;
        std::cout << "2. Run BFS" << std::endl;
        std::cout << "3. Run DFS" << std::endl;
        std::cout << "4. Find Connected Components" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Choose option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                create_graph_parameters_file(graph_parameters_file);
                graph.load_from_json(graph_parameters_file);
                std::cout << "Graph created successfully!" << std::endl;
                break;
            }
            case 2: {
                if (graph.vertex_count() == 0) {
                    std::cout << "Please create a graph first!" << std::endl;
                    break;
                }
                std::cout << "Enter start vertex (0 to " << graph.vertex_count() - 1 << "): ";
                int start;
                std::cin >> start;
                if (start >= 0 && start < graph.vertex_count()) {
                    graph.breadth_first_search(start);
                    std::cout << "BFS completed successfully!" << std::endl;
                } else {
                    std::cout << "Invalid vertex!" << std::endl;
                }
                break;
            }
            case 3: {
                if (graph.vertex_count() == 0) {
                    std::cout << "Please create a graph first!" << std::endl;
                    break;
                }
                std::cout << "Enter start vertex (0 to " << graph.vertex_count() - 1 << "): ";
                int start;
                std::cin >> start;
                if (start >= 0 && start < graph.vertex_count()) {
                    graph.depth_first_search(start);
                    std::cout << "DFS completed successfully!" << std::endl;
                } else {
                    std::cout << "Invalid vertex!" << std::endl;
                }
                break;
            }
            case 4: {
                try {
                auto components = graph.find_connected_components();
            } catch (const std::runtime_error& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
            case 5: {
                running = false;
                break;
            }
            default: {
                std::cout << "Invalid option!" << std::endl;
                break;
            }
        }
    }

    return 0;
}
