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

TEST(GraphTest, RemoveVertex) {
    Graph<int, int> g(3);
    g.add_edge(0, 1, 10);
    g.add_edge(1, 2, 20);

    g.remove_vertex(1);
    std::cout << g.has_edge(0, 1) << std::endl;
    EXPECT_EQ(g.vertex_count(), 2);
    EXPECT_FALSE(g.has_edge(0, 1));
    EXPECT_FALSE(g.has_edge(1, 2));
}

TEST(GraphTest, HasEdge) {
    Graph<int, int> g(3);
    g.add_edge(0, 1, 10);

    EXPECT_TRUE(g.has_edge(0, 1));
    EXPECT_FALSE(g.has_edge(0, 2));
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

TEST(GraphTest, Clear) {
    Graph<int, int> g(3);
    g.add_edge(0, 1, 10);
    g.add_edge(1, 2, 20);

    g.clear();
    EXPECT_EQ(g.vertex_count(), 0);
    EXPECT_FALSE(g.has_edge(0, 1));
    EXPECT_FALSE(g.has_edge(1, 2));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
