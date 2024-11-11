#include <gtest/gtest.h>
#include "../include/graph.hpp"

class BFSTest : public ::testing::Test {
protected:
    Graph<int, int> graph;

    void SetUp() override {}
};

TEST_F(BFSTest, EmptyGraph) {
    EXPECT_THROW(graph.breadth_first_search(1), std::runtime_error);
}

TEST_F(BFSTest, NonexistentStartVertex) {
    graph.add_vertex(1);
    EXPECT_THROW(graph.breadth_first_search(2), std::runtime_error);
}

TEST_F(BFSTest, SingleVertex) {
    graph.add_vertex(1);
    EXPECT_NO_THROW(graph.breadth_first_search(1));
    EXPECT_EQ(graph.get_vertex(1).get_color(), 2); 
    EXPECT_EQ(graph.get_vertex(1).get_discovery_time(), 0);
    EXPECT_EQ(graph.get_vertex(1).get_finish_time(), 1);
}

TEST_F(BFSTest, SimplePath) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 3, 1);

    graph.breadth_first_search(1);

    EXPECT_LT(graph.get_vertex(1).get_discovery_time(), 
              graph.get_vertex(2).get_discovery_time());
    EXPECT_LT(graph.get_vertex(2).get_discovery_time(), 
              graph.get_vertex(3).get_discovery_time());
}

TEST_F(BFSTest, CycleGraph) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 3, 1);
    graph.add_edge(3, 1, 1);

    graph.breadth_first_search(1);

    EXPECT_EQ(graph.get_vertex(1).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(2).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(3).get_color(), 2);
}

TEST_F(BFSTest, DisconnectedGraph) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_edge(1, 2, 1);

    graph.breadth_first_search(1);

    EXPECT_EQ(graph.get_vertex(1).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(2).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(3).get_color(), 0);
}

TEST_F(BFSTest, CompleteGraph) {
    for(int i = 1; i <= 4; ++i) {
        graph.add_vertex(i);
    }
    for(int i = 1; i <= 4; ++i) {
        for(int j = i + 1; j <= 4; ++j) {
            graph.add_edge(i, j, 1);
        }
    }

    graph.breadth_first_search(1);

    for(int i = 1; i <= 4; ++i) {
        EXPECT_EQ(graph.get_vertex(i).get_color(), 2);
        EXPECT_GE(graph.get_vertex(i).get_discovery_time(), 0);
        EXPECT_GE(graph.get_vertex(i).get_finish_time(), 0);
    }
}

TEST_F(BFSTest, VertexReset) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 1);

    graph.breadth_first_search(1);
    
    graph.breadth_first_search(2);
    
    EXPECT_GE(graph.get_vertex(1).get_discovery_time(), 0);
    EXPECT_GE(graph.get_vertex(2).get_discovery_time(), 0);
}
