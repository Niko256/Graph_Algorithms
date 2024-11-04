#include <gtest/gtest.h>
#include "../include/graph.hpp"
#include <vector>

class BFSTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!std::filesystem::exists("files")) {
            std::filesystem::create_directory("files");
        }
    }

    void TearDown() override {
        if (std::filesystem::exists("files/bfs_parameters.json")) {
            std::filesystem::remove("files/bfs_parameters.json");
        }
    }
};


TEST_F(BFSTest, SimplePath) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    
    graph.breadth_first_search(0);
    
    auto discovery = graph.get_discovery_time();
    auto finish = graph.get_finish_time();
    
    ASSERT_LT(discovery[0], discovery[1]);
    ASSERT_LT(discovery[1], discovery[2]);
}

TEST_F(BFSTest, CyclicGraph) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 0, 1);
    
    graph.breadth_first_search(0);
    
    auto discovery = graph.get_discovery_time();
    
    for(size_t i = 0; i < 3; ++i) {
        ASSERT_GE(discovery[i], 0);
    }
}

TEST_F(BFSTest, DisconnectedGraph) {
    Graph<int, int> graph(4);
    graph.add_edge(0, 1, 1);
    graph.add_edge(2, 3, 1);
    
    graph.breadth_first_search(0);
    
    auto discovery = graph.get_discovery_time();
    
    ASSERT_GT(discovery[1], 0);
    ASSERT_EQ(discovery[2], 0);
    ASSERT_EQ(discovery[3], 0);
}

TEST_F(BFSTest, JSONOutput) {
    Graph<int, int> graph(2);
    graph.add_edge(0, 1, 1);
    
    graph.breadth_first_search(0);
    
    ASSERT_TRUE(std::filesystem::exists("files/bfs_parameters.json"));
    
    std::ifstream file("files/bfs_parameters.json");
    ASSERT_TRUE(file.is_open());
    
    json parameters;
    file >> parameters;
    
    ASSERT_EQ(parameters["vertex_count"], 2);
    ASSERT_EQ(parameters["start_vertex"], 0);
}
