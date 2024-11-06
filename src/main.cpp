#include <iostream>
#include <filesystem>
#include <vector>
#include <tuple>
#include <string>
#include "../include/graph.hpp"
#include <fstream>
#include <thread>
#include <future>
#include <cstdlib>

namespace fs = std::filesystem;

const std::string VISUALIZATION_DIR = "../visualization/";


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


// Function to execute Python script asynchronously
std::future<int> execute_python_script_async(const std::string& script_path, const std::string& params) {
    return std::async(std::launch::async, [script_path, params]() {
        // Запуск рендеринга
        std::string render_command = "python3 " + script_path + " " + params;
        int result = std::system(render_command.c_str());
        
        if (result == 0) {
            // Открытие видео с помощью системного видеоплеера
            #ifdef __linux__
                std::system("xdg-open ../media/videos/1080p60/Main.mp4");
            #endif
        }
        
        return result;
    });
}

void render_visualization(const std::string& algorithm, const std::string& graph_file) {
    std::string script_path = VISUALIZATION_DIR;
    if (algorithm == "bfs") {
        script_path += "bfs_visualization.py";
    } else if (algorithm == "dfs") {
        script_path += "dfs_visualization.py";
    } else if (algorithm == "components") {
        script_path += "find_components.py";
    } else {
        script_path += "graph_visualization.py";
    }

    // Проверка существования файла скрипта
    if (!fs::exists(script_path)) {
        std::cout << "Error: Visualization script not found at: " << script_path << std::endl;
        return;
    }

    auto future = execute_python_script_async(script_path, graph_file);
    
    std::cout << "Rendering visualization... " << std::endl;
    
    future.wait();
    
    if (future.get() == 0) {
        std::cout << "Visualization completed successfully!" << std::endl;
    } else {
        std::cout << "Error occurred during visualization." << std::endl;
    }
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
        std::cout << "2. Run BFS with visualization" << std::endl;
        std::cout << "3. Run DFS with visualization" << std::endl;
        std::cout << "4. Find Connected Components with visualization" << std::endl;
        std::cout << "5. Show graph visualization" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "Choose option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                create_graph_parameters_file(graph_parameters_file);
                graph.load_from_json(graph_parameters_file);
                std::cout << "Graph created successfully!" << std::endl;
                // Show initial graph visualization
                render_visualization("graph", graph_parameters_file);
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
                    render_visualization("bfs", graph_parameters_file);
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
                    render_visualization("dfs", graph_parameters_file);
                } else {
                    std::cout << "Invalid vertex!" << std::endl;
                }
                break;
            }
            case 4: {
                try {
                    auto components = graph.find_connected_components();
                    render_visualization("components", graph_parameters_file);
                } catch (const std::runtime_error& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 5: {
                if (graph.vertex_count() == 0) {
                    std::cout << "Please create a graph first!" << std::endl;
                    break;
                }
                render_visualization("graph", graph_parameters_file);
                break;
            }
            case 6: {
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
