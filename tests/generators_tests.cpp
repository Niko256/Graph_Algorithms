#include <gtest/gtest.h>
#include "../include/graph.hpp"

class GraphGeneratorsTest : public ::testing::Test {
protected:
    Graph<size_t, int> graph;
};

TEST_F(GraphGeneratorsTest, CompleteGraph) {
    const size_t n = 5;
    graph.generate_complete_graph(n);
    
    EXPECT_EQ(graph.vertex_count(), n);
    EXPECT_EQ(graph.edge_count(), (n * (n - 1)) / 2);
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            EXPECT_TRUE(graph.has_edge(i, j));
        }
    }
}

TEST_F(GraphGeneratorsTest, CycleGraph) {
    const size_t n = 5;
    graph.generate_cycle_graph(n);
    
    EXPECT_EQ(graph.vertex_count(), n);
    EXPECT_EQ(graph.edge_count(), n);
    
    for (size_t i = 0; i < n - 1; ++i) {
        EXPECT_TRUE(graph.has_edge(i, i + 1));
    }
    EXPECT_TRUE(graph.has_edge(n - 1, 0));
    
    EXPECT_THROW(graph.generate_cycle_graph(2), std::invalid_argument);
}

TEST_F(GraphGeneratorsTest, PathGraph) {
    const size_t n = 5;
    graph.generate_path_graph(n);
    
    EXPECT_EQ(graph.vertex_count(), n);
    EXPECT_EQ(graph.edge_count(), n - 1);
    
    for (size_t i = 0; i < n - 1; ++i) {
        EXPECT_TRUE(graph.has_edge(i, i + 1));
    }
    
    EXPECT_THROW(graph.generate_path_graph(1), std::invalid_argument);
}

TEST_F(GraphGeneratorsTest, StarGraph) {
    const size_t n = 5;
    graph.generate_star_graph(n);
    
    EXPECT_EQ(graph.vertex_count(), n);
    EXPECT_EQ(graph.edge_count(), n - 1);
    
    for (size_t i = 1; i < n; ++i) {
        EXPECT_TRUE(graph.has_edge(0, i));
    }
    
    EXPECT_THROW(graph.generate_star_graph(1), std::invalid_argument);
}

TEST_F(GraphGeneratorsTest, GridGraph) {
    const size_t m = 3, n = 4;
    graph.generate_grid_graph(m, n);
    
    EXPECT_EQ(graph.vertex_count(), m * n);
    EXPECT_EQ(graph.edge_count(), (m - 1) * n + m * (n - 1));
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n - 1; ++j) {
            EXPECT_TRUE(graph.has_edge(i * n + j, i * n + j + 1));
        }
    }
    
    EXPECT_THROW(graph.generate_grid_graph(1, 1), std::invalid_argument);
}

TEST_F(GraphGeneratorsTest, HypercubeGraph) {
    const size_t dimension = 3;
    graph.generate_hypercube_graph(dimension);
    
    size_t expected_vertices = 1 << dimension;
    size_t expected_edges = (expected_vertices * dimension) / 2;
    
    EXPECT_EQ(graph.vertex_count(), expected_vertices);
    EXPECT_EQ(graph.edge_count(), expected_edges);
}

TEST_F(GraphGeneratorsTest, BipartiteGraph) {
    const size_t m = 3, n = 4;
    graph.generate_complete_bipartite_graph(m, n);
    
    EXPECT_EQ(graph.vertex_count(), m + n);
    EXPECT_EQ(graph.edge_count(), m * n);
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = i + 1; j < m; ++j) {
            EXPECT_FALSE(graph.has_edge(i, j));
        }
    }
    
    for (size_t i = m; i < m + n; ++i) {
        for (size_t j = i + 1; j < m + n; ++j) {
            EXPECT_FALSE(graph.has_edge(i, j));
        }
    }
}
