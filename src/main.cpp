#include <iostream>
#include <filesystem>
#include <memory>
#include <string>
#include <cstdlib>
#include "../include/graph.hpp"

namespace fs = std::filesystem;

class GraphManager {
private:
    std::unique_ptr<Graph<int, int, int>> graph_; // Updated template parameters
    const std::string files_dir_ = "files";
    const std::string parameters_file_ = files_dir_ + "/graph_parameters.json";

    void ensure_dir_exists() {
        if (!fs::exists(files_dir_)) {
            fs::create_directory(files_dir_);
        }
    }

    // Menu printing functions remain unchanged
    void print_main_menu() const {
        std::cout << "\nGraph Management System\n"
                  << "=====================\n\n"
                  << "1. Graph Creation and Visualization\n"
                  << "2. Algorithm Selection and Analysis\n"
                  << "3. Exit\n\n"
                  << "Choose option: ";
    }

    void print_creation_menu() const {
        std::cout << "\nGraph Creation Menu\n"
                  << "==================\n\n"
                  << "1. Manual Graph Creation\n"
                  << "2. Generate Graph from Topology\n"
                  << "3. Visualize Current Graph\n"
                  << "4. Edit Current Graph\n"
                  << "5. Back to Main Menu\n\n"
                  << "Choose option: ";
    }

    void print_algorithm_menu() const {
        std::cout << "\nAlgorithm Selection Menu\n"
                  << "======================\n\n"
                  << "1. Traversal Algorithms\n"
                  << "2. Path Finding Algorithms\n"
                  << "3. Connectivity Algorithms\n"
                  << "4. Tree Algorithms\n"
                  << "5. Flow Algorithms\n"
                  << "6. Back to Main Menu\n\n"
                  << "Choose option: ";
    }

    void print_topology_menu() const {
        std::cout << "\nTopology Selection Menu\n"
                  << "=====================\n\n"
                  << "1. Basic Graphs\n"
                  << "   - Complete Graph\n"
                  << "   - Path Graph\n"
                  << "   - Cycle Graph\n"
                  << "2. Special Graphs\n"
                  << "   - Star Graph\n"
                  << "   - Grid Graph\n"
                  << "   - Hypercube Graph\n"
                  << "3. Tree\n"
                  << "   - Random Tree\n"
                  << "4. Bipartite Graphs\n"
                  << "   - Regular Bipartite\n"
                  << "   - Complete Bipartite\n"
                  << "5. Back to Creation Menu\n\n"
                  << "Choose option: ";
    }

    void handle_manual_creation() {
        int vertex_count;
        std::cout << "Enter number of vertices: ";
        std::cin >> vertex_count;

        graph_ = std::make_unique<Graph<int, int, int>>(vertex_count);

        // Initialize vertices first
        for(int i = 0; i < vertex_count; ++i) {
            graph_->add_vertex(i, i); // Using vertex id as both identifier and resource
        }

        std::cout << "Enter edges (format: from to weight, enter -1 to stop):" << std::endl;
        while (true) {
            int from;
            std::cin >> from;
            if (from == -1) break;

            int to, weight;
            std::cin >> to >> weight;

            try {
                graph_->add_edge(from, to, weight);
            } catch (const std::exception& e) {
                std::cout << "\nError adding edge: " << e.what() << std::endl;
            }
        }

        graph_->save_to_json(parameters_file_);
    }

    // Rest of the handle functions remain the same, just updating Graph template parameters
    void handle_topology_selection() {
        while (true) {
            print_topology_menu();
            int choice;
            std::cin >> choice;

            try {
                size_t n, m;
                double prob;
                switch (choice) {
                    case 1: // Basic Graphs
                        std::cout << "Select type (1-Complete, 2-Path, 3-Cycle): ";
                        int type;
                        std::cin >> type;
                        std::cout << "Enter number of vertices: ";
                        std::cin >> n;
                        switch (type) {
                            case 1: graph_->generate_complete_graph(n); break;
                            case 2: graph_->generate_path_graph(n); break;
                            case 3: graph_->generate_cycle_graph(n); break;
                            default: std::cout << "\nInvalid choice\n"; continue;
                        }
                        break;
                    case 2: // Special Graphs
                        std::cout << "Select type (1-Star, 2-Grid, 3-Hypercube): ";
                        std::cin >> type;
                        switch (type) {
                            case 1:
                                std::cout << "Enter number of vertices: ";
                                std::cin >> n;
                                graph_->generate_star_graph(n);
                                break;
                            case 2:
                                std::cout << "Enter grid dimensions (m n): ";
                                std::cin >> m >> n;
                                graph_->generate_grid_graph(m, n);
                                break;
                            case 3:
                                std::cout << "Enter dimension: ";
                                std::cin >> n;
                                graph_->generate_hypercube_graph(n);
                            default: std::cout << "\nInvalid choice\n"; continue;
                        }
                        break;
                    case 3: // Random Graphs
                        std::cout << "Select type (1-Tree, 2-Connected): ";
                        std::cin >> type;
                        std::cout << "Enter number of vertices: ";
                        std::cin >> n;
                        switch (type) {
                            case 1: graph_->generate_tree(n); break;
                            default: std::cout << "\nInvalid choice\n"; continue;
                        }
                        break;
                    case 4: // Bipartite Graphs
                        std::cout << "Select type (1-Regular, 2-Complete): ";
                        std::cin >> type;
                        std::cout << "Enter parts sizes (m n): ";
                        std::cin >> m >> n;
                        switch (type) {
                            case 1:
                                std::cout << "Enter edge probability (0-1): ";
                                std::cin >> prob;
                                graph_->generate_bipartite_graph(m, n, prob);
                                break;
                            case 2:
                                graph_->generate_complete_bipartite_graph(m, n);
                                break;
                            default: std::cout << "\nInvalid choice\n"; continue;
                        }
                        break;
                    case 5: return;
                    default: std::cout << "\nInvalid option!" << std::endl; continue;
                }
                graph_->save_to_json(parameters_file_);
                std::cout << "\nGraph generated successfully!" << std::endl;
                return;
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }

    void handle_creation_menu() {
        while (true) {
            print_creation_menu();
            int choice;
            std::cin >> choice;

            try {
                switch(choice) {
                    case 1: handle_manual_creation(); break;
                    case 2: handle_topology_selection(); break;
                    case 3: run_visualization("graph_visualization.py"); break;
                    case 4: // Edit current graph - TODO
                        std::cout << "\nFeature coming soon!\n";
                        break;
                    case 5: return;
                    default: std::cout << "\nInvalid option!" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }

    void handle_algorithm_menu() {
        while (true) {
            print_algorithm_menu();
            int choice;
            std::cin >> choice;

            try {
                switch(choice) {
                    case 1: handle_traversal_algorithms(); break;
                    case 2: // Path finding - TODO
                    case 3: handle_connectivity_algorithms(); break;
                    case 4: // Tree algorithms - TODO
                    case 5: // Flow algorithms - TODO
                        std::cout << "\nFeature coming soon!\n";
                        break;
                    case 6: return;
                    default: std::cout << "\nInvalid option!" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }

    void handle_traversal_algorithms() {
        std::cout << "\nTraversal Algorithms:\n"
                  << "1. Run BFS\n"
                  << "2. Run DFS\n"
                  << "3. Visualize BFS\n"
                  << "4. Visualize DFS\n"
                  << "Choose option: ";

        int choice;
        std::cin >> choice;

        try {
            switch(choice) {
                case 1: run_bfs(); break;
                case 2: run_dfs(); break;
                case 3: run_visualization("bfs_visualization.py"); break;
                case 4: run_visualization("dfs_visualization.py"); break;
                default: std::cout << "Invalid option!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void handle_connectivity_algorithms() {
        std::cout << "\nConnectivity Algorithms:\n"
                  << "1. Find Connected Components\n"
                  << "2. Visualize Components\n"
                  << "Choose option: ";

        int choice;
        std::cin >> choice;

        try {
            switch(choice) {
                case 1: find_components(); break;
                case 2: run_visualization("find_components.py"); break;
                default: std::cout << "\nInvalid option!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void run_visualization(const std::string& script_name) {
        std::string command = "../visualization/run_manim.sh " + script_name;
        if (std::system(command.c_str()) != 0) {
            std::cerr << "Error running visualization script: " << script_name << std::endl;
        } else {
            std::cout << "\nVisualization completed successfully!" << std::endl;
        }
    }

    void run_bfs() {
        if (!graph_ || graph_->is_empty()) {
            std::cout << "\nPlease create a graph first!" << std::endl;
            return;
        }

        std::cout << "Enter start vertex (0 to " << graph_->vertex_count() - 1 << "): ";
        int start;
        std::cin >> start;

        try {
            graph_->breadth_first_search(start);
            std::cout << "\nBFS completed successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error during BFS: " << e.what() << std::endl;
        }
    }

    void run_dfs() {
        if (!graph_ || graph_->is_empty()) {
            std::cout << "\nPlease create a graph first!" << std::endl;
            return;
        }

        std::cout << "Enter start vertex (0 to " << graph_->vertex_count() - 1 << "): ";
        int start;
        std::cin >> start;

        try {
            graph_->depth_first_search(start);
            std::cout << "\nDFS completed successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error during DFS: " << e.what() << std::endl;
        }
    }

    void find_components() {
        if (!graph_ || graph_->is_empty()) {
            std::cout << "\nPlease create a graph first!" << std::endl;
            return;
        }
        try {
            auto components = graph_->find_connected_components();
            std::cout << "\nFound " << components.size() << " components!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error finding components: " << e.what() << std::endl;
        }
};

public:
    GraphManager() : graph_(std::make_unique<Graph<int, int, int>>()) {
        ensure_dir_exists();
    }

    void run() {
        while (true) {
            print_main_menu();
            int choice;
            std::cin >> choice;

            try {
                switch(choice) {
                    case 1: handle_creation_menu(); break;
                    case 2: handle_algorithm_menu(); break;
                    case 3: return;
                    default: std::cout << "\nInvalid option!" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
};

int main() {
    try {
        GraphManager manager;
        manager.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
