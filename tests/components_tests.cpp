#include <gtest/gtest.h>
#include "../include/graph.hpp"

class ComponentsTest : public ::testing::Test {
protected:
    Graph<int, int> graph;

    void SetUp() override {}
};

TEST_F(ComponentsTest, EmptyGraph) {
    EXPECT_THROW(graph.find_connected_components(), std::runtime_error);
}

TEST_F(ComponentsTest, SingleVertex) {
    graph.add_vertex(1);
    auto components = graph.find_connected_components();
    
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 1);
    EXPECT_EQ(components[0][0], 1);
}

TEST_F(ComponentsTest, IsolatedVertices) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 3);
    for (const auto& component : components) {
        EXPECT_EQ(component.size(), 1);
    }
}

TEST_F(ComponentsTest, ConnectedGraph) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 3, 1);
    
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 3);
}

TEST_F(ComponentsTest, TwoComponents) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 1);
    
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_edge(3, 4, 1);
    
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 2);
    EXPECT_EQ(components[0].size(), 2);
    EXPECT_EQ(components[1].size(), 2);
}

TEST_F(ComponentsTest, ComplexGraph) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_edge(1, 2, 1);
    graph.add_edge(1, 3, 1);
    graph.add_edge(1, 4, 1);
    
    graph.add_vertex(5);
    graph.add_vertex(6);
    graph.add_edge(5, 6, 1);
    
    graph.add_vertex(7);
    
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 3);
    
    bool found_size_4 = false;
    bool found_size_2 = false;
    bool found_size_1 = false;
    
    for (const auto& component : components) {
        if (component.size() == 4) found_size_4 = true;
        if (component.size() == 2) found_size_2 = true;
        if (component.size() == 1) found_size_1 = true;
    }
    
    EXPECT_TRUE(found_size_4);
    EXPECT_TRUE(found_size_2);
    EXPECT_TRUE(found_size_1);
}

TEST_F(ComponentsTest, JsonOutput) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 1);
    
    graph.add_vertex(3);
    
    graph.find_connected_components();
    
    EXPECT_TRUE(std::filesystem::exists("files/components.json"));
    
    std::ifstream file("files/components.json");
    ASSERT_TRUE(file.is_open());
    
    json j;
    file >> j;
    
    EXPECT_EQ(j["components_count"], 2);
    EXPECT_EQ(j["components"].size(), 2);
}
