#include <gtest/gtest.h>
#include "../include/graph.hpp"
#include <cmath>

class GraphGeneratorTest : public ::testing::Test {
protected:
    Graph<size_t, int, int> graph;
    
    bool is_complete(size_t n) {
        if (graph.vertex_count() != n) return false;
        size_t expected_edges = n * (n - 1) / 2;
        if (graph.edge_count() != expected_edges) return false;
        
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                if (!graph.has_edge(i, j)) return false;
            }
        }
        return true;
    }
    
    bool is_cycle(size_t n) {
        if (graph.vertex_count() != n) return false;
        if (graph.edge_count() != n) return false;
        
        for (size_t i = 0; i < n; ++i) {
            if (!graph.has_edge(i, (i + 1) % n)) return false;
            if (graph.get_degree(i) != 2) return false;
        }
        return true;
    }
    
    bool is_path(size_t n) {
        if (graph.vertex_count() != n) return false;
        if (graph.edge_count() != n - 1) return false;
        
        for (size_t i = 0; i < n - 1; ++i) {
            if (!graph.has_edge(i, i + 1)) return false;
        }
        return graph.get_degree(0) == 1 && graph.get_degree(n-1) == 1;
    }
    
    bool is_star(size_t n) {
        if (graph.vertex_count() != n) return false;
        if (graph.edge_count() != n - 1) return false;
        
        if (graph.get_degree(0) != n - 1) return false;
        for (size_t i = 1; i < n; ++i) {
            if (graph.get_degree(i) != 1) return false;
            if (!graph.has_edge(0, i)) return false;
        }
        return true;
    }
};

TEST_F(GraphGeneratorTest, CompleteGraph) {
    EXPECT_NO_THROW(graph.generate_complete_graph(5));
    EXPECT_TRUE(is_complete(5));
    
    EXPECT_NO_THROW(graph.generate_complete_graph(0));
    EXPECT_TRUE(graph.is_empty());
    
    EXPECT_NO_THROW(graph.generate_complete_graph(1));
    EXPECT_EQ(graph.vertex_count(), 1);
    EXPECT_EQ(graph.edge_count(), 0);
}

TEST_F(GraphGeneratorTest, CycleGraph) {
    EXPECT_NO_THROW(graph.generate_cycle_graph(5));
    EXPECT_TRUE(is_cycle(5));
    
    EXPECT_NO_THROW(graph.generate_cycle_graph(3));
    EXPECT_TRUE(is_cycle(3));
    
    EXPECT_THROW(graph.generate_cycle_graph(2), std::invalid_argument);
    EXPECT_THROW(graph.generate_cycle_graph(0), std::invalid_argument);
}

TEST_F(GraphGeneratorTest, PathGraph) {
    EXPECT_NO_THROW(graph.generate_path_graph(5));
    EXPECT_TRUE(is_path(5));
    
    EXPECT_NO_THROW(graph.generate_path_graph(2));
    EXPECT_TRUE(is_path(2));
    
    EXPECT_THROW(graph.generate_path_graph(1), std::invalid_argument);
    EXPECT_THROW(graph.generate_path_graph(0), std::invalid_argument);
}

TEST_F(GraphGeneratorTest, StarGraph) {
    EXPECT_NO_THROW(graph.generate_star_graph(5));
    EXPECT_TRUE(is_star(5));
    
    EXPECT_NO_THROW(graph.generate_star_graph(2));
    EXPECT_TRUE(is_star(2));
    
    EXPECT_THROW(graph.generate_star_graph(1), std::invalid_argument);
    EXPECT_THROW(graph.generate_star_graph(0), std::invalid_argument);
}

TEST_F(GraphGeneratorTest, GridGraph) {
    EXPECT_NO_THROW(graph.generate_grid_graph(3, 3));
    EXPECT_EQ(graph.vertex_count(), 9);
    EXPECT_EQ(graph.edge_count(), 12);
    
    EXPECT_NO_THROW(graph.generate_grid_graph(2, 3));
    EXPECT_EQ(graph.vertex_count(), 6);
    EXPECT_EQ(graph.edge_count(), 7);
    
    EXPECT_THROW(graph.generate_grid_graph(0, 3), std::invalid_argument);
    EXPECT_THROW(graph.generate_grid_graph(3, 0), std::invalid_argument);
}

TEST_F(GraphGeneratorTest, HypercubeGraph) {
    EXPECT_NO_THROW(graph.generate_hypercube_graph(3));
    EXPECT_EQ(graph.vertex_count(), 8);
    EXPECT_EQ(graph.edge_count(), 12);
    
    EXPECT_NO_THROW(graph.generate_hypercube_graph(2));
    EXPECT_EQ(graph.vertex_count(), 4);
    EXPECT_EQ(graph.edge_count(), 4);
    
    EXPECT_NO_THROW(graph.generate_hypercube_graph(1));
    EXPECT_EQ(graph.vertex_count(), 2);
    EXPECT_EQ(graph.edge_count(), 1);
}

TEST_F(GraphGeneratorTest, RandomTree) {
    size_t n = 10;
    EXPECT_NO_THROW(graph.generate_tree(n));
    
    EXPECT_EQ(graph.vertex_count(), n);
    EXPECT_EQ(graph.edge_count(), n - 1);
    
    EXPECT_NO_THROW(graph.generate_tree(1));
    EXPECT_EQ(graph.vertex_count(), 1);
    EXPECT_EQ(graph.edge_count(), 0);
}

TEST_F(GraphGeneratorTest, BipartiteGraph) {
    EXPECT_NO_THROW(graph.generate_bipartite_graph(3, 2, 1.0));
    EXPECT_EQ(graph.vertex_count(), 5);
    EXPECT_EQ(graph.edge_count(), 6);
    
    EXPECT_NO_THROW(graph.generate_bipartite_graph(3, 2, 0.0));
    EXPECT_EQ(graph.vertex_count(), 5);
    EXPECT_EQ(graph.edge_count(), 0);
    
    EXPECT_THROW(graph.generate_bipartite_graph(3, 2, -0.1), std::invalid_argument);
    EXPECT_THROW(graph.generate_bipartite_graph(3, 2, 1.1), std::invalid_argument);
}

TEST_F(GraphGeneratorTest, CompleteBipartiteGraph) {
    EXPECT_NO_THROW(graph.generate_complete_bipartite_graph(3, 2));
    EXPECT_EQ(graph.vertex_count(), 5);
    EXPECT_EQ(graph.edge_count(), 6);
    
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            EXPECT_TRUE(graph.has_edge(i, 3 + j));
        }
    }
}
