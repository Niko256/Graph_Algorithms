#include <gtest/gtest.h>
#include "../include/graph.hpp"

TEST(GraphTest, DefaultConstructor) {
    Graph<int, int> g;
    EXPECT_EQ(g.vertex_count(), 0);
    EXPECT_FALSE(g.is_directed());
}

TEST(GraphTest, ParameterizedConstructor) {
    Graph<int, int> g(5, true);
    EXPECT_EQ(g.vertex_count(), 5);
    EXPECT_TRUE(g.is_directed());
}

TEST(GraphTest, AddEdge) {
    Graph<int, int> g(3);
    g.add_edge(0, 1, 10);
    g.add_edge(1, 2, 20);

    EXPECT_TRUE(g.has_edge(0, 1));
    EXPECT_TRUE(g.has_edge(1, 2));
    EXPECT_TRUE(g.has_edge(1, 0)); 
    EXPECT_TRUE(g.has_edge(2, 1)); 
}

TEST(GraphTest, AddVertex) {
    Graph<int, int> g(3);
    g.add_vertex();

    EXPECT_EQ(g.vertex_count(), 4);
}

TEST(GraphTest, RemoveEdge) {
    Graph<int, int> g(3);
    g.add_edge(0, 1, 10);
    g.add_edge(1, 2, 20);

    g.remove_edge(0, 1);
    EXPECT_FALSE(g.has_edge(0, 1));
    EXPECT_FALSE(g.has_edge(1, 0)); 
}


TEST(GraphTest, RemoveVertexNoEdges) {
    Graph<int, int> graph(3);

    graph.remove_vertex(1);

    EXPECT_EQ(graph.vertex_count(), 2);
    EXPECT_EQ(graph.get_adjacency_list(0).size(), 0);
    EXPECT_EQ(graph.get_adjacency_list(1).size(), 0);
}

TEST(GraphTest, RemoveVertexWithEdges) {
    Graph<int, int> graph(3);

    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);
    graph.add_edge(2, 0, 30);

    graph.remove_vertex(1);

    EXPECT_EQ(graph.vertex_count(), 2);
    EXPECT_EQ(graph.get_adjacency_list(0).size(), 1);
    EXPECT_EQ(graph.get_adjacency_list(1).size(), 1);

    EXPECT_EQ(graph.get_adjacency_list(0)[0].first_, 1);
    EXPECT_EQ(graph.get_adjacency_list(0)[0].second_, 30);

    EXPECT_EQ(graph.get_adjacency_list(1)[0].first_, 0);
    EXPECT_EQ(graph.get_adjacency_list(1)[0].second_, 30);
}

TEST(GraphTest, RemoveVertexUndirectedGraph) {
    Graph<int, int> graph(3, false);

    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);
    graph.add_edge(2, 0, 30);

    graph.remove_vertex(1);

    EXPECT_EQ(graph.vertex_count(), 2);
    EXPECT_EQ(graph.get_adjacency_list(0).size(), 1);
    EXPECT_EQ(graph.get_adjacency_list(1).size(), 1);

    EXPECT_EQ(graph.get_adjacency_list(0)[0].first_, 1);
    EXPECT_EQ(graph.get_adjacency_list(0)[0].second_, 30);

    EXPECT_EQ(graph.get_adjacency_list(1)[0].first_, 0);
    EXPECT_EQ(graph.get_adjacency_list(1)[0].second_, 30);
}

TEST(GraphTest, RemoveVertexMultipleEdges) {
    Graph<int, int> graph(4);

    graph.add_edge(0, 1, 10);
    graph.add_edge(0, 2, 20);
    graph.add_edge(0, 3, 30);
    graph.add_edge(1, 2, 40);
    graph.add_edge(1, 3, 50);
    graph.add_edge(2, 3, 60);

    graph.remove_vertex(1);

    EXPECT_EQ(graph.vertex_count(), 3);
    EXPECT_EQ(graph.get_adjacency_list(0).size(), 2);
    EXPECT_EQ(graph.get_adjacency_list(1).size(), 2);
    EXPECT_EQ(graph.get_adjacency_list(2).size(), 2);

    EXPECT_EQ(graph.get_adjacency_list(0)[0].first_, 1);
    EXPECT_EQ(graph.get_adjacency_list(0)[0].second_, 20);
    EXPECT_EQ(graph.get_adjacency_list(0)[1].first_, 2);
    EXPECT_EQ(graph.get_adjacency_list(0)[1].second_, 30);

    EXPECT_EQ(graph.get_adjacency_list(1)[0].first_, 0);
    EXPECT_EQ(graph.get_adjacency_list(1)[0].second_, 20);
    EXPECT_EQ(graph.get_adjacency_list(1)[1].first_, 2);
    EXPECT_EQ(graph.get_adjacency_list(1)[1].second_, 60);

    EXPECT_EQ(graph.get_adjacency_list(2)[0].first_, 0);
    EXPECT_EQ(graph.get_adjacency_list(2)[0].second_, 30);
    EXPECT_EQ(graph.get_adjacency_list(2)[1].first_, 1);
    EXPECT_EQ(graph.get_adjacency_list(2)[1].second_, 60);
}

TEST(GraphTest, RemoveVertexOutOfRange) {
    Graph<int, int> graph(3);

    EXPECT_THROW(graph.remove_vertex(3), std::out_of_range);
}


TEST(GraphTest, GetAdjacencyList) {
    Graph<int, int> g(3);
    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 20);

    const auto& adj_list = g.get_adjacency_list(0);
    EXPECT_EQ(adj_list.size(), 2);
    EXPECT_EQ(adj_list[0].first_, 1);
    EXPECT_EQ(adj_list[0].second_, 10);
    EXPECT_EQ(adj_list[1].first_, 2);
    EXPECT_EQ(adj_list[1].second_, 20);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
