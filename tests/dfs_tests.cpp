#include <gtest/gtest.h>
#include "../include/graph.hpp"

TEST(DFSTest, SimpleGraph) {
    Graph<int, int> graph(5);

    graph.add_edge(0, 1, 1);
    graph.add_edge(0, 2, 1);
    graph.add_edge(1, 3, 1);
    graph.add_edge(2, 4, 1);

    graph.depth_first_search(0);

    const auto& discovery_time = graph.get_discovery_time();
    const auto& finish_time = graph.get_finish_time();

    EXPECT_EQ(discovery_time[0], 0);
    EXPECT_EQ(finish_time[0], 9);

    EXPECT_EQ(discovery_time[1], 1);
    EXPECT_EQ(finish_time[1], 4);

    EXPECT_EQ(discovery_time[2], 5);
    EXPECT_EQ(finish_time[2], 8);

    EXPECT_EQ(discovery_time[3], 2);
    EXPECT_EQ(finish_time[3], 3);

    EXPECT_EQ(discovery_time[4], 6);
    EXPECT_EQ(finish_time[4], 7);
}


TEST(DFSTest, CyclicGraph) {
    Graph<int, int> graph(4);

    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 3, 1);
    graph.add_edge(3, 0, 1);

    graph.depth_first_search(0);

    const auto& discovery_time = graph.get_discovery_time();
    const auto& finish_time = graph.get_finish_time();

    EXPECT_EQ(discovery_time[0], 0);
    EXPECT_EQ(finish_time[0], 7);

    EXPECT_EQ(discovery_time[1], 1);
    EXPECT_EQ(finish_time[1], 4);

    EXPECT_EQ(discovery_time[2], 5);
    EXPECT_EQ(finish_time[2], 6);

    EXPECT_EQ(discovery_time[3], 2);
    EXPECT_EQ(finish_time[3], 3);
}


TEST(DFSTest, DisconnectedGraph) {

    Graph<int, int> graph(6);

    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(3, 4, 1);
    graph.add_edge(4, 5, 1);

    graph.depth_first_search(0);

    const auto& discovery_time = graph.get_discovery_time();
    const auto& finish_time = graph.get_finish_time();

    EXPECT_EQ(discovery_time[0], 0);
    EXPECT_EQ(finish_time[0], 5);

    EXPECT_EQ(discovery_time[1], 1);
    EXPECT_EQ(finish_time[1], 4);

    EXPECT_EQ(discovery_time[2], 2);
    EXPECT_EQ(finish_time[2], 3);

    EXPECT_EQ(discovery_time[3], 6);
    EXPECT_EQ(finish_time[3], 11);

    EXPECT_EQ(discovery_time[4], 7);
    EXPECT_EQ(finish_time[4], 10);

    EXPECT_EQ(discovery_time[5], 8);
    EXPECT_EQ(finish_time[5], 9);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
