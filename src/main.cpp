#include <exception>
#include <iostream>
#include <filesystem>
#include <memory>
#include <string>
#include <cstdlib>
#include "../include/graph.hpp"

namespace fs = std::filesystem;

class GraphManager {
private:
    std::unique_ptr<Graph<int, int, int>> graph_; 
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
                  << "5. Coloring algorithms\n"
                  << "6. Back to Main Menu\n\n"
                  << "Choose option: ";
    }

    void print_topology_menu() const {
        std::cout << "\nTopology Selection Menu\n"
                  << "=====================\n\n"
                  << "1. Complete Graph\n"
                  << "2. Path Graph\n"
                  << "3. Cycle Graph\n"
                  << "4. Star Graph\n"
                  << "5. Random Tree\n"
                  << "6. Regular Bipartite Graph\n"
                  << "7. Complete Bipartite Graph\n"
                  << "8. Back to Creation Menu\n\n"
                  << "Choose option: ";
    }

    void print_edit_menu() const {
        std::cout << "\nEdit Graph Menu\n"
                  << "===============\n\n"
                  << "1. Add Vertex\n"
                  << "2. Remove Vertex\n"
                  << "3. Add Edge\n"
                  << "4. Remove Edge\n"
                  << "5. Back to Creation Menu\n\n"
                  << "Choose option: ";
    }

    void handle_manual_creation() {
        int vertex_count;
        std::cout << "Enter number of vertices: ";
        std::cin >> vertex_count;

        // Validate vertex count
        if (vertex_count <= 0) {
            std::cout << "\nError: Number of vertices must be positive." << std::endl;
            return;
        }

        // Create a new graph instance with the specified vertex count
        graph_ = std::make_unique<Graph<int, int, int>>();
        graph_->initialize_graph(vertex_count); // This will clear any existing graph and set up a new one

        std::cout << "Enter edges (format: from to weight, enter -1 to stop):" << std::endl;
        while (true) {
            int from;
            std::cin >> from;
            if (from == -1) break;
        
            int to, weight;
            std::cin >> to >> weight;
        
            try {
                // Validate vertex indices
                if (from >= vertex_count || to >= vertex_count) {
                    std::cout << "\nError: Vertex index out of range" << std::endl;
                    continue;
                }
            
                graph_->add_edge(from, to, weight);
            } catch (const std::exception& e) {
                std::cout << "\nError adding edge: " << e.what() << std::endl;
            }
        }
    
        try {
            graph_->save_to_json(parameters_file_);
            std::cout << "\nGraph created successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error saving graph: " << e.what() << std::endl;
        }
    }

    void handle_topology_selection() {
        while (true) {
            print_topology_menu();
            int choice;
            std::cin >> choice;

            try {
                size_t n, m;
                double prob;
                switch (choice) {
                    case 1: // Complete Graph
                        std::cout << "Enter number of vertices: ";
                        std::cin >> n;
                        graph_->generate_complete_graph(n);
                        break;
                    case 2: // Path Graph
                        std::cout << "Enter number of vertices: ";
                        std::cin >> n;
                        graph_->generate_path_graph(n);
                        break;
                    case 3: // Cycle Graph
                        std::cout << "Enter number of vertices: ";
                        std::cin >> n;
                        graph_->generate_cycle_graph(n);
                        break;
                    case 4: // Star Graph
                        std::cout << "Enter number of vertices: ";
                        std::cin >> n;
                        graph_->generate_star_graph(n);
                        break;
                    case 5: // Random Tree
                        std::cout << "Enter number of vertices: ";
                        std::cin >> n;
                        graph_->generate_tree(n);
                        break;
                    case 6: // Regular Bipartite Graph
                        std::cout << "Enter parts sizes (m n): ";
                        std::cin >> m >> n;
                        std::cout << "Enter edge probability (0-1): ";
                        std::cin >> prob;
                        graph_->generate_bipartite_graph(m, n, prob);
                        break;
                    case 7: // Complete Bipartite Graph
                        std::cout << "Enter parts sizes (m n): ";
                        std::cin >> m >> n;
                        graph_->generate_complete_bipartite_graph(m, n);
                        break;
                    case 8: return;
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
                    case 3: run_visualization("graph_visualization_static.py"); break;
                    case 4: handle_edit_graph(); break;
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
                    case 2: handle_shortest_path_algorithms(); break; 
                    case 3: handle_connectivity_algorithms(); break;
                    case 4: handle_tree_algorithms(); break;
                    case 5: handle_coloring_algorithms(); break;
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
                  << "1. Run and Visualize BFS\n"
                  << "2. Run and Visualize DFS\n"
                  << "3. Back to Algorithm Menu\n"
                  << "Choose option: ";

        int choice;
        std::cin >> choice;

        try {
            switch(choice) {
                case 1: run_bfs_and_visualize(); break;
                case 2: run_dfs_and_visualize(); break;
                case 3: return;
                default: std::cout << "Invalid option!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void handle_connectivity_algorithms() {
        std::cout << "\nConnectivity Algorithms:\n"
                  << "1. Find and Visualize Connected Components\n"
                  << "2. Back to Algorithm Menu\n"
                  << "Choose option: ";

        int choice;
        std::cin >> choice;

        try {
            switch(choice) {
                case 1: find_components_and_visualize(); break;
                case 2: return;
                default: std::cout << "\nInvalid option!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void handle_shortest_path_algorithms() {
        std::cout << "\nShortest Path Algorithms: \n"
                  << "1. Run and Visualize Dijkstra\n"
                  << "2. Run and Visualize Unweighted Shorted Paths\n"
                  << "3. Back to Algorithm Menu\n"
                  << "Choose option: ";
        int choice;
        std::cin >> choice;

        try {
            switch (choice) {
                case 1: run_dijkstra_and_visualize(); break;
                case 2: run_shortest_paths_and_visualize(); break;
                case 3: return;
                default: std::cout << "Invalid option!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void handle_tree_algorithms() {
        std::cout << "\nTree Algorithms:\n"
                  << "This feature coming soon!\n"
                  << "1. Back to Algorithm Menu\n"
                  << "Choose option: ";

        int choice;
        std::cin >> choice;

        try {
            switch (choice) {
                case 1: return;
                default: std::cout << "Invalid option!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void handle_coloring_algorithms() {
        std::cout << "\nColoring algorithms:\n"
                  << "1. Greedy Coloring\n"
                  << "2. Back to Algorithm Menu\n"
                  << "Choose option: ";

        int choice;
        std::cin >> choice;

        try {
            switch(choice) {
                case 1: run_greedy_coloring_and_visualize(); break;
                case 2: return;
                default: std::cout << "\nInvalid option!" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void run_greedy_coloring_and_visualize() {
        if (!graph_ || graph_->is_empty()) {
            std::cout << "\nPlease create graph first!" << std::endl;
            return;
        }

        std::cout << "Enter start vertex (0 to " << graph_->vertex_count() - 1 << "): ";
        int start;
        std::cin >> start;

        try {
            graph_->greedy_coloring(start);
            std::cout << "\nGreedy coloring completed successfully!" << std::endl;
            run_visualization("greedy_coloring_visualization.py");
        } catch (const std::exception& e) {
            std::cerr << "Error during greedy coloring: " << e.what() << std::endl;
        }
    }

    void run_dijkstra_and_visualize() {
        if (!graph_ || graph_->is_empty()) {
            std::cout << "\nPlease create graph first!" << std::endl;
            return;
        }

        std::cout << "Enter start vertex (0 to " << graph_->vertex_count() - 1 << "): ";
        int start;
        std::cin >> start;

        try {
            graph_->dijkstra(start);
            std::cout << "\nDijkstra's algorithm completed successfully!" << std::endl;
            run_visualization("dijkstra_visualization.py");
        } catch (const std::exception& e) {
            std::cerr << "Error during Dijkstra algorithm: " << e.what() << std::endl;
        }
    }

    void run_shortest_paths_and_visualize() {
        if (!graph_ || graph_->is_empty()) {
            std::cout << "\nPlease create graph first!" << std::endl;
            return;
        }

        std::cout << "Enter start vertex (0 to " << graph_->vertex_count() - 1 << "): ";
        int start;
        std::cin >> start;

        try {
            graph_->shortest_paths_unweighted(start);
            std::cout << "\nShortest paths calculation completed successfully!" << std::endl;
            run_visualization("shortest_paths_visualization.py");
        } catch (const std::exception& e) {
            std::cerr << "Error during shortest paths calculation: " << e.what() << std::endl;
        }
    }


    void run_visualization(const std::string& script_name) {

        if (script_name == "graph_visualization.py") {
            std::string base = "../visualization/run_manim.sh";
            std::string quality_params = "-s -qk";

            std::string command = base + script_name + " " + quality_params;
            if (std::system(command.c_str()) != 0) {
                std::cerr << "Error running visualization script" << std::endl;
            } else {
                std::cout << "\nGraph structure image saved succesfully!" << std::endl;
            }
        } else {
            std::string command = "../visualization/run_manim.sh " + script_name;
            if (std::system(command.c_str()) != 0) {
                std::cerr << "Error running visualization script: " << script_name << std::endl;
            } else {
                std::cout << "\nVisualization completed successfully!" << std::endl;
            }
        }
    }

    void run_bfs_and_visualize() {
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
            run_visualization("bfs_visualization.py");
        } catch (const std::exception& e) {
            std::cerr << "Error during BFS: " << e.what() << std::endl;
        }
    }

    void run_dfs_and_visualize() {
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
            run_visualization("dfs_visualization.py");
        } catch (const std::exception& e) {
            std::cerr << "Error during DFS: " << e.what() << std::endl;
        }
    }

    void find_components_and_visualize() {
        if (!graph_ || graph_->is_empty()) {
            std::cout << "\nPlease create a graph first!" << std::endl;
            return;
        }
        try {
            auto components = graph_->find_connected_components();
            std::cout << "\nFound " << components.size() << " components!" << std::endl;
            run_visualization("find_components.py");
        } catch (const std::exception& e) {
            std::cerr << "Error finding components: " << e.what() << std::endl;
        }
    }

    void handle_edit_graph() {
        while (true) {
            print_edit_menu();
            int choice;
            std::cin >> choice;

            try {
                switch(choice) {
                    case 1: // Add Vertex
                        int vertex_id;
                        std::cout << "Enter vertex ID: ";
                        std::cin >> vertex_id;
                        graph_->add_vertex(vertex_id);
                        std::cout << "\nVertex added successfully!" << std::endl;
                        break;
                    case 2: // Remove Vertex
                        std::cout << "Enter vertex ID to remove: ";
                        std::cin >> vertex_id;
                        graph_->remove_vertex(vertex_id);
                        std::cout << "\nVertex removed successfully!" << std::endl;
                        break;
                    case 3: // Add Edge
                        int from, to, weight;
                        std::cout << "Enter edge (from to weight): ";
                        std::cin >> from >> to >> weight;
                        graph_->add_edge(from, to, weight);
                        std::cout << "\nEdge added successfully!" << std::endl;
                        break;
                    case 4: // Remove Edge
                        std::cout << "Enter edge (from to): ";
                        std::cin >> from >> to;
                        graph_->remove_edge(from, to);
                        std::cout << "\nEdge removed successfully!" << std::endl;
                        break;
                    case 5: return;
                    default: std::cout << "\nInvalid option!" << std::endl;
                }
                graph_->save_to_json(parameters_file_);
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }

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
