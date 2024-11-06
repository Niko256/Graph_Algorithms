#include <gtest/gtest.h>
#include "../include/graph.hpp"

class ConnectedComponentsTest : public ::testing::Test {
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(ConnectedComponentsTest, EmptyGraph) {
    Graph<int, int> graph;
    EXPECT_THROW(graph.find_connected_components(), std::runtime_error);
}

TEST_F(ConnectedComponentsTest, SingleVertex) {
    Graph<int, int> graph(1);
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 1);
    EXPECT_EQ(components[0][0], 0);
}

TEST_F(ConnectedComponentsTest, TwoDisconnectedVertices) {
    Graph<int, int> graph(2);
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 2);
    EXPECT_EQ(components[0].size(), 1);
    EXPECT_EQ(components[1].size(), 1);
}

TEST_F(ConnectedComponentsTest, TwoConnectedVertices) {
    Graph<int, int> graph(2);
    graph.add_edge(0, 1, 1);
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 2);
}

TEST_F(ConnectedComponentsTest, MultipleComponents) {
    Graph<int, int> graph(6);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(3, 4, 1);
    graph.add_edge(4, 5, 1);
    
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 2);
    EXPECT_EQ(components[0].size(), 3);
    EXPECT_EQ(components[1].size(), 3);
}

TEST_F(ConnectedComponentsTest, CyclicGraph) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 0, 1);
    
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 3);
}

TEST_F(ConnectedComponentsTest, JsonOutput) {
    Graph<int, int> graph(4);
    graph.add_edge(0, 1, 1);
    graph.add_edge(2, 3, 1);
    
    graph.find_connected_components();
    
    std::ifstream file("files/components.json");
    ASSERT_TRUE(file.is_open());
    
    json j;
    file >> j;
    EXPECT_EQ(j["components_count"], 2);
    EXPECT_EQ(j["components"].size(), 2);
}

