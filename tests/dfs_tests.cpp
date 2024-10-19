#include <gtest/gtest.h>
#include "../include/graph.hpp"

TEST(DFSTest, EmptyGraph) {
    Graph<int, int> graph(0);
    graph.depth_first_search(0);

    EXPECT_EQ(graph.vertex_count(), 0);
}


TEST(DFSTest, SingleVertexGraph) {
    Graph<int, int> graph(1);
    graph.depth_first_search(0);

    EXPECT_EQ(graph.vertex_count(), 1);
}


TEST(DFSTest, LinearGraph) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.depth_first_search(0);

    EXPECT_EQ(graph.vertex_count(), 3);
}


TEST(DFSTest, CompleteGraph) {
    Graph<int, int> graph(3);
    graph.add_edge(0, 1, 1);
    graph.add_edge(0, 2, 1);
    graph.add_edge(1, 2, 1);
    graph.depth_first_search(0);
   
    EXPECT_EQ(graph.vertex_count(), 3);
}


TEST(DFSTest, IsolatedVerticesGraph) {
    Graph<int, int> graph(3);
    graph.depth_first_search(0);

    EXPECT_EQ(graph.vertex_count(), 3);
}



