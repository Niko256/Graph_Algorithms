#include <gtest/gtest.h>
#include "../include/graph.hpp"

class DijkstraTest : public ::testing::Test {
protected:
    Graph<int, int, int> graph;

    void SetUp() override {
        // Setup will be called before each test
    }
};

// Basic functionality tests
TEST_F(DijkstraTest, EmptyGraphTest) {
    EXPECT_THROW(graph.dijkstra(0), std::runtime_error);
}

TEST_F(DijkstraTest, SingleVertexTest) {
    graph.add_vertex(0);
    EXPECT_NO_THROW(graph.dijkstra(0));
}

TEST_F(DijkstraTest, NonExistentStartVertex) {
    graph.add_vertex(0);
    EXPECT_THROW(graph.dijkstra(1), std::runtime_error);
}

// Simple path tests
TEST_F(DijkstraTest, SimplePathTest) {
    // Create a simple path: 0 -> 1 -> 2 (weights: 1, 2)
    graph.add_vertex(0);
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 2);

    EXPECT_NO_THROW(graph.dijkstra(0));
    
    // Verify distances through the results file
    std::ifstream file("files/dijkstra_results.json");
    json results;
    file >> results;
    
    EXPECT_EQ(results["distances"]["0"], 0);
    EXPECT_EQ(results["distances"]["1"], 1);
    EXPECT_EQ(results["distances"]["2"], 3);
}

// Complex graph tests
TEST_F(DijkstraTest, ComplexGraphTest) {
    // Create a more complex graph with multiple paths
    for(int i = 0; i < 5; i++) {
        graph.add_vertex(i);
    }
    
    graph.add_edge(0, 1, 4);
    graph.add_edge(0, 2, 2);
    graph.add_edge(1, 2, 1);
    graph.add_edge(1, 3, 5);
    graph.add_edge(2, 3, 8);
    graph.add_edge(2, 4, 10);
    graph.add_edge(3, 4, 2);
    
    EXPECT_NO_THROW(graph.dijkstra(0));
    
    std::ifstream file("files/dijkstra_results.json");
    json results;
    file >> results;
    
    EXPECT_EQ(results["distances"]["0"], 0);
    EXPECT_EQ(results["distances"]["1"], 4);
    EXPECT_EQ(results["distances"]["2"], 2);
    EXPECT_EQ(results["distances"]["3"], 9);
    EXPECT_EQ(results["distances"]["4"], 11);
}


// Cycle test
TEST_F(DijkstraTest, CycleTest) {
    for(int i = 0; i < 3; i++) {
        graph.add_vertex(i);
    }
    
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 2);
    graph.add_edge(2, 0, 4);
    
    EXPECT_NO_THROW(graph.dijkstra(0));
    
    std::ifstream file("files/dijkstra_results.json");
    json results;
    file >> results;
    
    EXPECT_EQ(results["distances"]["0"], 0);
    EXPECT_EQ(results["distances"]["1"], 1);
    EXPECT_EQ(results["distances"]["2"], 4);
}

// Large graph performance test
TEST_F(DijkstraTest, LargeGraphPerformanceTest) {
    const int SIZE = 1000;
    for(int i = 0; i < SIZE; i++) {
        graph.add_vertex(i);
    }
    
    // Add edges in a grid-like pattern
    for(int i = 0; i < SIZE; i++) {
        for(int j = i + 1; j < std::min(i + 10, SIZE); j++) {
            graph.add_edge(i, j, j-i);
        }
    }
    
    EXPECT_NO_THROW(graph.dijkstra(0));
}
