#include <gtest/gtest.h>
#include "../include/graph.hpp"

struct User {
    int id;
    std::string name;
    int age;

    User() : id(0), name(""), age(0) {}
    User(int i, std::string n, int a) : id(i), name(n), age(a) {}

    bool operator==(const User& other) const {
        return id == other.id && name == other.name && age == other.age;
    }
};



class DFSTest : public ::testing::Test {
protected:
    Graph<int, User, int> graph;

    void SetUp() override {}

    void TearDown() override {
        graph.clear();
    }

    User create_test_user(int id) {
        return User(id, "User" + std::to_string(id), 20 + id);
    }
};

TEST_F(DFSTest, EmptyGraph) {
    EXPECT_THROW(graph.depth_first_search(1), std::runtime_error);
}

TEST_F(DFSTest, SingleVertex) {
    graph.add_vertex(1);
    EXPECT_NO_THROW(graph.depth_first_search(1));
    EXPECT_EQ(graph.get_vertex(1).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(1).get_discovery_time(), 0);
    EXPECT_EQ(graph.get_vertex(1).get_finish_time(), 1);
}

TEST_F(DFSTest, NonExistentStartVertex) {
    graph.add_vertex(1);
    EXPECT_THROW(graph.depth_first_search(2), std::runtime_error);
}

TEST_F(DFSTest, SimplePath) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 3, 1);

    graph.depth_first_search(1);

    EXPECT_LT(graph.get_vertex(1).get_discovery_time(), 
              graph.get_vertex(2).get_discovery_time());
    EXPECT_LT(graph.get_vertex(2).get_discovery_time(), 
              graph.get_vertex(3).get_discovery_time());
    
    EXPECT_EQ(graph.get_vertex(1).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(2).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(3).get_color(), 2);
}

TEST_F(DFSTest, Cycle) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 3, 1);
    graph.add_edge(3, 1, 1);

    graph.depth_first_search(1);

    for(int i = 1; i <= 3; ++i) {
        EXPECT_EQ(graph.get_vertex(i).get_color(), 2);
        EXPECT_GE(graph.get_vertex(i).get_discovery_time(), 0);
        EXPECT_GE(graph.get_vertex(i).get_finish_time(), 0);
    }
}

TEST_F(DFSTest, CompleteGraph) {
    for(int i = 1; i <= 4; ++i) {
        graph.add_vertex(i);
        for(int j = 1; j < i; ++j) {
            graph.add_edge(i, j, 1);
        }
    }

    graph.depth_first_search(1);

    for(int i = 1; i <= 4; ++i) {
        EXPECT_EQ(graph.get_vertex(i).get_color(), 2);
    }
}

TEST_F(DFSTest, DisconnectedGraph) {
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_edge(1, 2, 1);
    graph.add_edge(3, 4, 1);

    graph.depth_first_search(1);

    EXPECT_EQ(graph.get_vertex(1).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(2).get_color(), 2);
    
    EXPECT_EQ(graph.get_vertex(3).get_color(), 0);
    EXPECT_EQ(graph.get_vertex(4).get_color(), 0);
}

TEST_F(DFSTest, TreeStructure) {
    //       1
    //      / \
    //     2   3
    //    /     \
    //   4       5
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_vertex(5);
    graph.add_edge(1, 2, 1);
    graph.add_edge(1, 3, 1);
    graph.add_edge(2, 4, 1);
    graph.add_edge(3, 5, 1);

    graph.depth_first_search(1);

    for(int i = 1; i <= 5; ++i) {
        const auto& vertex = graph.get_vertex(i);
        EXPECT_LT(vertex.get_discovery_time(), vertex.get_finish_time());
        EXPECT_EQ(vertex.get_color(), 2);
    }
}
