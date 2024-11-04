#include <gtest/gtest.h>
#include "../include/graph.hpp"

class DFSTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!std::filesystem::exists("files")) {
            std::filesystem::create_directory("files");
        }
    }
};



TEST_F(DFSTest, SimplePath) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    
    graph.depth_first_search(0);
    
    auto discovery = graph.get_discovery_time();
    auto finish = graph.get_finish_time();
    
    EXPECT_LT(discovery[0], discovery[1]);
    EXPECT_LT(discovery[1], discovery[2]);
    EXPECT_GT(finish[0], finish[1]);
    EXPECT_GT(finish[1], finish[2]);
}

TEST_F(DFSTest, CyclicGraph) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 0, 1);
    
    graph.depth_first_search(0);
    
    auto discovery = graph.get_discovery_time();
    auto finish = graph.get_finish_time();
    
    for (int i = 0; i < 3; ++i) {
        EXPECT_GE(discovery[i], 0);
        EXPECT_GE(finish[i], 0);
    }
}

TEST_F(DFSTest, DisconnectedGraph) {
    Graph<int, int> graph(4);
    graph.add_edge(0, 1, 1);
    
    graph.depth_first_search(0);
    
    auto discovery = graph.get_discovery_time();
    
    EXPECT_GE(discovery[0], 0);
    EXPECT_GE(discovery[1], 0);
    EXPECT_EQ(discovery[2], 0); 
    EXPECT_EQ(discovery[3], 0); 
}


TEST_F(DFSTest, ParametersFileCreation) {
    Graph<int, int> graph(2);
    graph.add_edge(0, 1, 1);
    
    graph.depth_first_search(0);
    
    EXPECT_TRUE(std::filesystem::exists("files/dfs_parameters.json"));
}
