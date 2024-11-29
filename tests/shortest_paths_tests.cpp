#include <gtest/gtest.h>
#include "../include/graph.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ShortestPathsTest : public ::testing::Test {
protected:
    struct TestResource {
        int data;
        TestResource(int d = 0) : data(d) {}
    };
    
    Graph<int, TestResource, int> graph;
    
    json getResultsFromFile() {
        std::ifstream file("files/shortest_paths_unweighted.json");
        EXPECT_TRUE(file.is_open());
        json result;
        file >> result;
        return result;
    }
};

TEST_F(ShortestPathsTest, EmptyGraph) {
    EXPECT_THROW(graph.shortest_paths_unweighted(0), std::runtime_error);
}

TEST_F(ShortestPathsTest, NonExistentStartVertex) {
    graph.add_vertex(0, TestResource(0));
    EXPECT_THROW(graph.shortest_paths_unweighted(1), std::runtime_error);
}

TEST_F(ShortestPathsTest, SingleVertex) {
    graph.add_vertex(0, TestResource(0));
    graph.shortest_paths_unweighted(0);
    
    auto result = getResultsFromFile();
    EXPECT_EQ(result["distances"]["0"], 0);
    EXPECT_EQ(result["paths"]["0"].size(), 1);
    EXPECT_EQ(result["paths"]["0"][0], 0);
}

TEST_F(ShortestPathsTest, SimplePath) {
    graph.add_vertex(0, TestResource(0));
    graph.add_vertex(1, TestResource(1));
    graph.add_vertex(2, TestResource(2));
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    
    graph.shortest_paths_unweighted(0);
    
    auto result = getResultsFromFile();
    EXPECT_EQ(result["distances"]["0"], 0);
    EXPECT_EQ(result["distances"]["1"], 1);
    EXPECT_EQ(result["distances"]["2"], 2);
     
    EXPECT_EQ(result["paths"]["2"].size(), 3);
    EXPECT_EQ(result["paths"]["2"][0], 0);
    EXPECT_EQ(result["paths"]["2"][1], 1);
    EXPECT_EQ(result["paths"]["2"][2], 2);
}

TEST_F(ShortestPathsTest, GraphWithCycle) {
    graph.add_vertex(0, TestResource(0));
    graph.add_vertex(1, TestResource(1));
    graph.add_vertex(2, TestResource(2));
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 0, 1);
     
    graph.shortest_paths_unweighted(0);
     
    auto result = getResultsFromFile();
    EXPECT_EQ(result["distances"]["0"], 0);
    EXPECT_EQ(result["distances"]["1"], 1);
    EXPECT_EQ(result["distances"]["2"], 1);
}

TEST_F(ShortestPathsTest, DisconnectedGraph) {
    graph.add_vertex(0, TestResource(0));
    graph.add_vertex(1, TestResource(1));
    graph.add_vertex(2, TestResource(2));
    graph.add_edge(0, 1, 1);
     
    graph.shortest_paths_unweighted(0);
    
    auto result = getResultsFromFile();
    EXPECT_EQ(result["distances"]["0"], 0);
    EXPECT_EQ(result["distances"]["1"], 1);
    EXPECT_EQ(result["distances"]["2"], -1); 
}

TEST_F(ShortestPathsTest, MultiplePathsGraph) {
    graph.add_vertex(0, TestResource(0));
    graph.add_vertex(1, TestResource(1));
    graph.add_vertex(2, TestResource(2));
    graph.add_vertex(3, TestResource(3));
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 3, 1);
    graph.add_edge(0, 2, 1);
    graph.add_edge(2, 3, 1);
    
    graph.shortest_paths_unweighted(0);
    
    auto result = getResultsFromFile();
    EXPECT_EQ(result["distances"]["0"], 0);
    EXPECT_EQ(result["distances"]["1"], 1);
    EXPECT_EQ(result["distances"]["2"], 1);
    EXPECT_EQ(result["distances"]["3"], 2);
}
