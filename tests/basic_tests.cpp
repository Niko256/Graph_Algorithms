#include <gtest/gtest.h>
#include "../include/graph.hpp"

class GraphTest : public ::testing::Test {
protected:
    Graph<int, int> graph;
    
    void SetUp() override {}
};

TEST_F(GraphTest, DefaultConstructor) {
    Graph<int, int> g;
    EXPECT_TRUE(g.is_empty());
    EXPECT_EQ(g.vertex_count(), 0);
    EXPECT_EQ(g.edge_count(), 0);
}

TEST_F(GraphTest, SizeConstructor) {
    Graph<int, int> g(5);
    EXPECT_FALSE(g.is_empty());
    EXPECT_EQ(g.vertex_count(), 5);
    EXPECT_EQ(g.edge_count(), 0);
    
    for(int i = 0; i < 5; ++i) {
        EXPECT_TRUE(g.has_vertex(i));
    }
}

TEST_F(GraphTest, AddVertex) {
    EXPECT_NO_THROW(graph.add_vertex(1));
    EXPECT_TRUE(graph.has_vertex(1));
    EXPECT_EQ(graph.vertex_count(), 1);
    
    EXPECT_THROW(graph.add_vertex(1), std::invalid_argument);
}

TEST_F(GraphTest, RemoveVertex) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 10);
    
    EXPECT_NO_THROW(graph.remove_vertex(1));
    EXPECT_FALSE(graph.has_vertex(1));
    EXPECT_FALSE(graph.has_edge(1, 2));
    EXPECT_EQ(graph.vertex_count(), 1);
    
    EXPECT_THROW(graph.remove_vertex(3), std::invalid_argument);
}

TEST_F(GraphTest, AddEdge) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    
    EXPECT_NO_THROW(graph.add_edge(1, 2, 10));
    EXPECT_TRUE(graph.has_edge(1, 2));
    EXPECT_TRUE(graph.has_edge(2, 1)); 
    
    EXPECT_THROW(graph.add_edge(1, 2, 20), std::invalid_argument);
    
    EXPECT_THROW(graph.add_edge(1, 3, 10), std::invalid_argument);
}

TEST_F(GraphTest, JsonSerialization) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 10);
    
    json j = graph.to_json();
    EXPECT_EQ(j["vertices"].size(), 2);
    EXPECT_EQ(j["edges"].size(), 1);
    
    EXPECT_EQ(j["edges"][0]["from"], 1);
    EXPECT_EQ(j["edges"][0]["to"], 2);
    EXPECT_EQ(j["edges"][0]["weight"], 10);
}

TEST_F(GraphTest, ComparisonOperators) {
    Graph<int, int> g1, g2;
    
    EXPECT_EQ(g1, g2);
    
    g1.add_vertex(1);
    EXPECT_NE(g1, g2);
    
    g2.add_vertex(1);
    EXPECT_EQ(g1, g2);
}



TEST_F(GraphTest, MoveConstructor) {
    Graph<int, int> g1;
    g1.add_vertex(1);
    g1.add_vertex(2);
    g1.add_edge(1, 2, 10);
    
    Graph<int, int> g2(std::move(g1));
    EXPECT_TRUE(g1.is_empty()); 
    EXPECT_TRUE(g2.has_vertex(1));
    EXPECT_TRUE(g2.has_vertex(2));
    EXPECT_TRUE(g2.has_edge(1, 2));
}

TEST_F(GraphTest, EdgeOperations) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 10);
    
    EXPECT_EQ(graph.get_edge(1, 2).get_weight(), 10);
    EXPECT_NO_THROW(graph.set_edge_weight(1, 2, 20));
    EXPECT_EQ(graph.get_edge(1, 2).get_weight(), 20);
    
    EXPECT_NO_THROW(graph.remove_edge(1, 2));
    EXPECT_FALSE(graph.has_edge(1, 2));
    EXPECT_EQ(graph.edge_count(), 0);
}

TEST_F(GraphTest, VertexDegree) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    
    EXPECT_EQ(graph.get_degree(1), 0);
    
    graph.add_edge(1, 2, 10);
    EXPECT_EQ(graph.get_degree(1), 1);
    
    graph.add_edge(1, 3, 20);
    EXPECT_EQ(graph.get_degree(1), 2);
    
    graph.remove_edge(1, 2);
    EXPECT_EQ(graph.get_degree(1), 1);
}

TEST_F(GraphTest, EdgeCases) {
    // Пустой граф
    EXPECT_THROW(graph.remove_vertex(1), std::invalid_argument);
    EXPECT_THROW(graph.get_vertex(1), std::invalid_argument);
    EXPECT_THROW(graph.get_edge(1, 2), std::invalid_argument);
    
    graph.add_vertex(1);
    EXPECT_EQ(graph.vertex_count(), 1);
    EXPECT_EQ(graph.edge_count(), 0);
    EXPECT_THROW(graph.add_edge(1, 1, 10), std::invalid_argument); 
}

TEST_F(GraphTest, LargeGraph) {
    const size_t size = 1000;
    for(size_t i = 0; i < size; ++i) {
        EXPECT_NO_THROW(graph.add_vertex(i));
    }
    EXPECT_EQ(graph.vertex_count(), size);
    
    for(size_t i = 0; i < size-1; ++i) {
        EXPECT_NO_THROW(graph.add_edge(i, i+1, i));
    }
    EXPECT_EQ(graph.edge_count(), size-1);
}

TEST_F(GraphTest, JsonOperations) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 10);
    
    EXPECT_NO_THROW(graph.save_to_json("test_graph.json"));
    
    Graph<int, int> loaded_graph;
    EXPECT_NO_THROW(loaded_graph.load_from_json("test_graph.json"));
    EXPECT_EQ(graph, loaded_graph);
}

TEST_F(GraphTest, Connectivity) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    
    EXPECT_FALSE(graph.is_connected(1, 2));
    
    graph.add_edge(1, 2, 10);
    EXPECT_TRUE(graph.is_connected(1, 2));
    EXPECT_TRUE(graph.is_connected(2, 1));
    EXPECT_FALSE(graph.is_connected(1, 3));
}
