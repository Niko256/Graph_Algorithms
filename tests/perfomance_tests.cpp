#include <gtest/gtest.h>
#include <chrono>
#include <fstream>
#include "../include/graph.hpp"

struct Resource {
    std::string name;
    int age;
};

class GraphPerformanceTest : public ::testing::Test {
protected:
    void SaveBenchmarkResult(const std::string& algorithm_name, 
                             size_t vertices, 
                             double duration_ms) {
        std::ofstream file("benchmark_results.csv", std::ios::app);
        file << algorithm_name << "," << vertices << "," << duration_ms << "\n";
    }

    template<typename Func>
    double BenchmarkOperation(Func operation) {
        auto start = std::chrono::high_resolution_clock::now();
        operation();
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};

TEST_F(GraphPerformanceTest, LargeGraphOperations) {
    const std::vector<size_t> sizes = {1000, 5000, 10000, 50000, 100000};
    
    for (size_t size : sizes) {
        Graph<int, Resource, double> graph;
        
        // Test graph generation
        double gen_time = BenchmarkOperation([&]() {
            // Add vertices
            for (size_t i = 0; i < size; ++i) {
                graph.add_vertex(i, Resource{"User" + std::to_string(i), static_cast<int>(i % 50)});
            }
            // Add edges
            for (size_t i = 0; i < size; ++i) {
                for (size_t j = i + 1; j < std::min(i + 10, size); ++j) {  // Adding limited edges per vertex
                    graph.add_edge(i, j, 1.0);
                }
            }
        });
        SaveBenchmarkResult("generate_graph", size, gen_time);

        // Test BFS
        double bfs_time = BenchmarkOperation([&]() {
            graph.breadth_first_search(0);
        });
        SaveBenchmarkResult("bfs", size, bfs_time);

        // Test DFS
        double dfs_time = BenchmarkOperation([&]() {
            graph.depth_first_search(0);
        });
        SaveBenchmarkResult("dfs", size, dfs_time);

        // Test Connected Components
        double cc_time = BenchmarkOperation([&]() {
            graph.find_connected_components();
        });
        SaveBenchmarkResult("connected_components", size, cc_time);

        // Test coloring
        double coloring_time = BenchmarkOperation([&]() {
            graph.greedy_coloring(0);
        });
        SaveBenchmarkResult("greedy_coloring", size, coloring_time);
    }
}
