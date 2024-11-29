#include <gtest/gtest.h>
#include "../include/graph.hpp"

class GreedyColoringTest : public ::testing::Test {
protected:
    struct TestResource {
        int data;
        TestResource(int d = 0) : data(d) {}
    };
};

TEST_F(GreedyColoringTest, EmptyGraphTest) {
    Graph<int, TestResource, double> graph;
    EXPECT_THROW(graph.greedy_coloring(0), std::runtime_error);
}

TEST_F(GreedyColoringTest, SingleVertexTest) {
    Graph<int, TestResource, double> graph;
    graph.add_vertex(0, TestResource(1));
    
    EXPECT_NO_THROW(graph.greedy_coloring(0));
    EXPECT_EQ(graph.get_vertex(0).get_color(), 0);
}

TEST_F(GreedyColoringTest, CompleteGraphTest) {
    Graph<int, TestResource, double> graph;
    const int n = 4;
    
    // Create complete graph
    for(int i = 0; i < n; ++i) {
        graph.add_vertex(i, TestResource(i));
    }
    
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            graph.add_edge(i, j, 1.0);
        }
    }
    
    EXPECT_NO_THROW(graph.greedy_coloring(0));
    
    // Check that all vertices have different colors
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            EXPECT_NE(graph.get_vertex(i).get_color(), 
                     graph.get_vertex(j).get_color());
        }
    }
}

TEST_F(GreedyColoringTest, BipartiteGraphTest) {
    Graph<int, TestResource, double> graph;
    
    // Create bipartite graph
    graph.add_vertex(0, TestResource(0));
    graph.add_vertex(1, TestResource(1));
    graph.add_vertex(2, TestResource(2));
    graph.add_vertex(3, TestResource(3));
    
    // Add edges between partites
    graph.add_edge(0, 2, 1.0);
    graph.add_edge(0, 3, 1.0);
    graph.add_edge(1, 2, 1.0);
    graph.add_edge(1, 3, 1.0);
    
    EXPECT_NO_THROW(graph.greedy_coloring(0));
    
    // Check that bipartite graph uses only 2 colors
    std::set<size_t> used_colors;
    for(int i = 0; i < 4; ++i) {
        used_colors.insert(graph.get_vertex(i).get_color());
    }
    EXPECT_LE(used_colors.size(), 2);
}

TEST_F(GreedyColoringTest, PathGraphTest) {
    Graph<int, TestResource, double> graph;
    const int n = 5;
    
    // Create path graph
    for(int i = 0; i < n; ++i) {
        graph.add_vertex(i, TestResource(i));
    }
    for(int i = 0; i < n-1; ++i) {
        graph.add_edge(i, i+1, 1.0);
    }
    
    EXPECT_NO_THROW(graph.greedy_coloring(0));
    
    // Check that path uses only 2 colors
    std::set<size_t> used_colors;
    for(int i = 0; i < n; ++i) {
        used_colors.insert(graph.get_vertex(i).get_color());
    }
    EXPECT_LE(used_colors.size(), 2);
}

TEST_F(GreedyColoringTest, InvalidStartVertexTest) {
    Graph<int, TestResource, double> graph;
    graph.add_vertex(0, TestResource(0));
    
    EXPECT_THROW(graph.greedy_coloring(1), std::runtime_error);
}
