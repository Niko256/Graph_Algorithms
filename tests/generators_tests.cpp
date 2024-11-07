#include <gtest/gtest.h>
#include "../include/graph.hpp"

class GraphGeneratorTest : public ::testing::Test {
protected:
    Graph<int, int> graph;
};

// Complete Graph Tests
TEST_F(GraphGeneratorTest, CompleteGraphGeneration) {
    const size_t n = 5;
    graph.generate_complete_graph(n);
    
    // Verify vertex count
    EXPECT_EQ(graph.vertex_count(), n);
    
    // Verify edge count and connections
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            EXPECT_TRUE(graph.has_edge(i, j));
            EXPECT_TRUE(graph.has_edge(j, i));
        }
    }
}

// Path Graph Tests
TEST_F(GraphGeneratorTest, PathGraphGeneration) {
    const size_t n = 5;
    graph.generate_path_graph(n);
    
    EXPECT_EQ(graph.vertex_count(), n);
    
    for (size_t i = 0; i < n - 1; ++i) {
        EXPECT_TRUE(graph.has_edge(i, i + 1));
        EXPECT_TRUE(graph.has_edge(i + 1, i));
    }
}

// Cycle Graph Tests
TEST_F(GraphGeneratorTest, CycleGraphGeneration) {
    const size_t n = 5;
    graph.generate_cycle_graph(n);
    
    EXPECT_EQ(graph.vertex_count(), n);
    
    for (size_t i = 0; i < n; ++i) {
        EXPECT_TRUE(graph.has_edge(i, (i + 1) % n));
        EXPECT_TRUE(graph.has_edge((i + 1) % n, i));
    }
}

// Star Graph Tests
TEST_F(GraphGeneratorTest, StarGraphGeneration) {
    const size_t n = 5;
    graph.generate_star_graph(n);
    
    EXPECT_EQ(graph.vertex_count(), n);
    
    for (size_t i = 1; i < n; ++i) {
        EXPECT_TRUE(graph.has_edge(0, i));
        EXPECT_TRUE(graph.has_edge(i, 0));
    }
}

// Grid Graph Tests
TEST_F(GraphGeneratorTest, GridGraphGeneration) {
    const size_t m = 3, n = 4;
    graph.generate_grid_graph(m, n);
    
    EXPECT_EQ(graph.vertex_count(), m * n);
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            size_t current = i * n + j;
            if (j + 1 < n) {
                EXPECT_TRUE(graph.has_edge(current, current + 1));
            }
            if (i + 1 < m) {
                EXPECT_TRUE(graph.has_edge(current, current + n));
            }
        }
    }
}

// Hypercube Graph Tests
TEST_F(GraphGeneratorTest, HypercubeGraphGeneration) {
    const size_t dimension = 3;
    graph.generate_hypercube_graph(dimension);
    
    size_t expected_vertices = 1 << dimension;
    EXPECT_EQ(graph.vertex_count(), expected_vertices);
    
    for (size_t i = 0; i < expected_vertices; ++i) {
        for (size_t j = 0; j < dimension; ++j) {
            size_t neighbor = i ^ (1 << j);
            if (i < neighbor) {
                EXPECT_TRUE(graph.has_edge(i, neighbor));
                EXPECT_TRUE(graph.has_edge(neighbor, i));
            }
        }
    }
}

// Random Tree Tests
TEST_F(GraphGeneratorTest, RandomTreeGeneration) {
    const size_t n = 5;
    graph.generate_tree(n);
    
    EXPECT_EQ(graph.vertex_count(), n);
    
    // Verify it's connected and has n-1 edges
    size_t edge_count = 0;
    for (size_t i = 0; i < n; ++i) {
        const auto& adj_list = graph.get_adjacency_list(i);
        edge_count += adj_list.size();
    }
    EXPECT_EQ(edge_count, 2 * (n - 1)); // Count is doubled due to undirected edges
}

TEST_F(GraphGeneratorTest, BipartiteGraphGeneration) {
    const size_t m = 3, n = 4;
    const double probability = 0.5;
    
    EXPECT_NO_THROW(graph.generate_bipartite_graph(m, n, probability));
    EXPECT_EQ(graph.vertex_count(), m + n);
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (i != j) EXPECT_FALSE(graph.has_edge(i, j));
        }
    }
    
    for (size_t i = m; i < m + n; ++i) {
        for (size_t j = m; j < m + n; ++j) {
            if (i != j) EXPECT_FALSE(graph.has_edge(i, j));
        }
    }
}

TEST_F(GraphGeneratorTest, CompleteBipartiteGraphGeneration) {
    const size_t m = 3, n = 4;
    graph.generate_complete_bipartite_graph(m, n);
    
    EXPECT_EQ(graph.vertex_count(), m + n);
    
    // Verify all edges between partitions exist
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            EXPECT_TRUE(graph.has_edge(i, m + j));
            EXPECT_TRUE(graph.has_edge(m + j, i));
        }
    }
}

TEST_F(GraphGeneratorTest, ErrorCases) {
    EXPECT_NO_THROW(graph.generate_complete_graph(0));
    EXPECT_THROW(graph.generate_bipartite_graph(1, 1, -0.1), std::invalid_argument);
    EXPECT_THROW(graph.generate_bipartite_graph(1, 1, 1.1), std::invalid_argument);
}
