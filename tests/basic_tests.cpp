#include "../include/graph.hpp"
#include <gtest/gtest.h>

TEST(GraphTest, DefaultConstructor) {
    Graph<int, int> graph;
    EXPECT_EQ(graph.vertex_count(), 0);
}

TEST(GraphTest, ParameterizedConstructor) {
    Graph<int, int> graph(5);
    EXPECT_EQ(graph.vertex_count(), 5);
}

TEST(GraphTest, AddEdge) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    EXPECT_TRUE(graph.has_edge(0, 1));
    EXPECT_TRUE(graph.has_edge(1, 0)); 
    EXPECT_TRUE(graph.has_edge(1, 2));
    EXPECT_TRUE(graph.has_edge(2, 1)); 
}


TEST(GraphTest, AddVertex) {
    Graph<int, int> graph;
    graph.add_vertex();
    graph.add_vertex();

    EXPECT_EQ(graph.vertex_count(), 2);
}

TEST(GraphTest, RemoveEdge) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    graph.remove_edge(0, 1);
    EXPECT_FALSE(graph.has_edge(0, 1));
    EXPECT_FALSE(graph.has_edge(1, 0)); 
}


TEST(GraphTest, RemoveVertex) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    graph.remove_vertex(1);
    EXPECT_EQ(graph.vertex_count(), 2);
    /*EXPECT_FALSE(graph.has_edge(0, 1));*/
    /*EXPECT_FALSE(graph.has_edge(1, 2));*/
}


TEST(GraphTest, HasEdge) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);

    EXPECT_TRUE(graph.has_edge(0, 1));
    EXPECT_TRUE(graph.has_edge(1, 0)); 
    EXPECT_FALSE(graph.has_edge(0, 2));
}


TEST(GraphTest, ToJson) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    json j = graph.to_json();
    EXPECT_EQ(j["vertex_count"], 3);
    EXPECT_EQ(j["edges"].size(), 4); 
}

TEST(GraphTest, SaveToJson) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    graph.save_to_json("test_graph.json");

    std::ifstream file("test_graph.json");
    json j;
    file >> j;
    file.close();

    EXPECT_EQ(j["vertex_count"], 3);
    EXPECT_EQ(j["edges"].size(), 4); 
}

TEST(GraphTest, GetAdjacencyList) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    const auto& adj_list = graph.get_adjacency_list(0);
    EXPECT_EQ(adj_list.size(), 1);
    EXPECT_EQ(adj_list[0].first_, 1);
    EXPECT_EQ(adj_list[0].second_, 10);
}


TEST(GraphTest, Clear) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    graph.clear();
    EXPECT_EQ(graph.vertex_count(), 0);
}

TEST(GraphTest, GetDiscoveryAndFinishTime) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    const auto& discovery_time = graph.get_discovery_time();
    const auto& finish_time = graph.get_finish_time();

    EXPECT_EQ(discovery_time.size(), 3);
    EXPECT_EQ(finish_time.size(), 3);
}

TEST(GraphTest, Iterators) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);

    auto it = graph.begin();
    EXPECT_EQ(it->size(), 1);
    ++it;
    EXPECT_EQ(it->size(), 2);
    ++it;
    EXPECT_EQ(it->size(), 1);
    ++it;
    EXPECT_EQ(it, graph.end());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
