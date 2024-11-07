#include <iostream>
#include <filesystem>
#include <memory>
#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <cstdlib>
#include "../include/graph.hpp"

namespace fs = std::filesystem;

class GraphManager {
private:
    std::unique_ptr<Graph<int, int>> graph_;
    const std::string files_dir_ = "files";
    const std::string parameters_file_ = files_dir_ + "/graph_parameters.json";

    void ensure_dir_exists() {
        if (!fs::exists(files_dir_)) {
            fs::create_directory(files_dir_);
        }
    }

    void print_main_menu() const {
        std::cout << "\nGraph Operations Menu:\n" << std::endl
                  << "  1. Create new graph" << std::endl
                  << "  2. Generate graph from chosen topology" << std::endl
                  << "  3. Run BFS" << std::endl
                  << "  4. Run DFS" << std::endl
                  << "  5. Find Connected Components" << std::endl
                  << "  6. Visualize BFS" << std::endl
                  << "  7. Visualize DFS" << std::endl
                  << "  8. Visualize Connected Components" << std::endl
                  << "  9. Exit" << std::endl
                  << "\nChoose option: ";
    }

    void print_generator_menu() const {
        std::cout << "\nGraph Generator Menu: \n" << std::endl;
        std::cout << "  1. Complete Graph" << std::endl;
        std::cout << "  2. Cycle Graph" << std::endl;
        std::cout << "  3. Path Graph" << std::endl;
        std::cout << "  4. Star Graph" << std::endl;
        std::cout << "  5. Grid Graph" << std::endl;
        std::cout << "  6. Hypercube Graph" << std::endl;
        std::cout << "  7. Random Tree" << std::endl;
        std::cout << "  8. Bipartite Graph" << std::endl;
        std::cout << "  9. Complete Bipartite Graph" << std::endl;
        std::cout << "  0. Back to Main Menu" << std::endl;
        std::cout << "\nChoose option: \n";
    }

    void handle_graph_generation() {
        int choice;
        size_t n, m, k;
        double prob;

        print_generator_menu();
        std::cin >> choice;

        try {
            switch (choice) {
                case 1:
                    std::cout << "Enter number of vertices: ";
                    std::cin >> n;
                    graph_->generate_complete_graph(n);
                    break;
                case 2:
                    std::cout << "Enter number of vertices: ";
                    std::cin >> n;
                    graph_->generate_cycle_graph(n);
                    break;
                case 3:
                    std::cout << "Enter number of vertices: ";
                    std::cin >> n;
                    graph_->generate_path_graph(n);
                    break;
                case 4:
                    std::cout << "Enter number of vertices: ";
                    std::cin >> n;
                    graph_->generate_star_graph(n);
                    break;
                case 5:
                    std::cout << "Enter grid dimensions (m n): ";
                    std::cin >> m >> n;
                    graph_->generate_grid_graph(m, n);
                    break;
                case 6:
                    std::cout << "Enter dimension: ";
                    std::cin >> n;
                    graph_->generate_hypercube_graph(n);
                    break;
                case 7:
                    std::cout << "Enter number of vertices: ";
                    std::cin >> n;
                    graph_->generate_tree(n);
                    break;
                case 8:
                    std::cout << "Enter sizes of parts (m n) and edge probability: ";
                    std::cin >> m >> n >> prob;
                    graph_->generate_bipartite_graph(m, n, prob);
                    break;
                case 9:
                    std::cout << "Enter sizes of parts (m n): ";
                    std::cin >> m >> n;
                    graph_->generate_complete_bipartite_graph(m, n);
                    break;
                case 0:
                    return;
                default:
                    std::cout << "Invalid option!" << std::endl;
                    return;
            }
            std::cout << "Graph generated successfully!" << std::endl;
            graph_->save_to_json(parameters_file_);
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    void run_visualization(const std::string& script_name) {
        std::string command = "../visualization/run_manim.sh " + script_name;
        int result = std::system(command.c_str());
        if (result != 0) {
            std::cerr << "Error running visualization script: " << script_name << std::endl;
        } else {
            std::cout << "Visualization completed successfully!" << std::endl;
        }
    }

public:
    GraphManager() : graph_(std::make_unique<Graph<int, int>>()) {
        ensure_dir_exists();
    }

    void create_new_graph() {
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

        graph_ = std::make_unique<Graph<int, int>>(vertex_count);
        for (const auto& [from, to, weight] : edges) {
            graph_->add_edge(from, to, weight);
        }

        graph_->save_to_json(parameters_file_);
        std::cout << "Graph created successfully!" << std::endl;
    }

    void run_bfs() {
        if (graph_->vertex_count() == 0) {
            std::cout << "Please create a graph first!" << std::endl;
            return;
        }
        std::cout << "Enter start vertex (0 to " << graph_->vertex_count() - 1 << "): ";
        int start;
        std::cin >> start;
        if (start >= 0 && start < graph_->vertex_count()) {
            graph_->breadth_first_search(start);
            std::cout << "BFS completed successfully!" << std::endl;
        } else {
            std::cout << "Invalid vertex!" << std::endl;
        }
    }

    void run_dfs() {
        if (graph_->vertex_count() == 0) {
            std::cout << "Please create a graph first!" << std::endl;
            return;
        }
        std::cout << "Enter start vertex (0 to " << graph_->vertex_count() - 1 << "): ";
        int start;
        std::cin >> start;
        if (start >= 0 && start < graph_->vertex_count()) {
            graph_->depth_first_search(start);
            std::cout << "DFS completed successfully!" << std::endl;
        } else {
            std::cout << "Invalid vertex!" << std::endl;
        }
    }

    void find_connected_components() {
        try {
            auto components = graph_->find_connected_components();
            std::cout << "Connected components found successfully!" << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    void run() {
        bool running = true;
        while (running) {
            print_main_menu();
            int choice;
            std::cin >> choice;

            try {
                switch(choice) {
                    case 1:
                        create_new_graph();
                        break;
                    case 2:
                        handle_graph_generation();
                        break;
                    case 3:
                        run_bfs();
                        break;
                    case 4:
                        run_dfs();
                        break;
                    case 5:
                        find_connected_components();
                        break;
                    case 6:
                        run_visualization("bfs_visualization.py");
                        break;
                    case 7:
                        run_visualization("dfs_visualization.py");
                        break;
                    case 8:
                        run_visualization("find_components.py");
                        break;
                    case 9:
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid option!" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
};

int main() {
    GraphManager manager;
    manager.run();
    return 0;
}
