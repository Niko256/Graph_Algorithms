#include <gtest/gtest.h>
#include "../include/graph.hpp"
#include <string>

struct User {
    std::string name;
    int age;
    std::string email;

    User() : name(""), age(0), email("") {}
    User(const std::string& n, int a, const std::string& e) 
        : name(n), age(a), email(e) {}

    bool operator==(const User& other) const {
        return name == other.name && 
               age == other.age && 
               email == other.email;
    }
};

class GraphTest : public ::testing::Test {
protected:
    Graph<int, User, int> graph;
    
    void SetUp() override {
        User user1("Alice", 25, "alice@mail.com");
        User user2("Bob", 30, "bob@mail.com");
        
        graph.add_vertex(1, user1);
        graph.add_vertex(2, user2);
    }

    void TearDown() override {
        graph.clear();
    }
};

TEST_F(GraphTest, DefaultConstructor) {
    Graph<int, User, int> g;
    EXPECT_TRUE(g.is_empty());
    EXPECT_EQ(g.vertex_count(), 0);
    EXPECT_EQ(g.edge_count(), 0);
}

TEST_F(GraphTest, SizeConstructor) {
    Graph<int, User, int> g(5);
    EXPECT_FALSE(g.is_empty());
    EXPECT_EQ(g.vertex_count(), 5);
    EXPECT_EQ(g.edge_count(), 0);
    
    for(int i = 0; i < 5; ++i) {
        EXPECT_TRUE(g.has_vertex(i));
    }
}

TEST_F(GraphTest, CopyConstructor) {
    graph.add_edge(1, 2, 10);
    Graph<int, User, int> g2(graph);
    
    EXPECT_EQ(g2.vertex_count(), graph.vertex_count());
    EXPECT_EQ(g2.edge_count(), graph.edge_count());
    EXPECT_TRUE(g2.has_edge(1, 2));
    EXPECT_EQ(g2.get_edge(1, 2).get_weight(), 10);
}

TEST_F(GraphTest, MoveConstructor) {
    graph.add_edge(1, 2, 10);
    Graph<int, User, int> g2(std::move(graph));
    
    EXPECT_TRUE(graph.is_empty());
    EXPECT_EQ(g2.vertex_count(), 2);
    EXPECT_TRUE(g2.has_edge(1, 2));
}

TEST_F(GraphTest, AddVertex) {
    User user3("Charlie", 35, "charlie@mail.com");
    EXPECT_NO_THROW(graph.add_vertex(3, user3));
    EXPECT_TRUE(graph.has_vertex(3));
    EXPECT_EQ(graph.vertex_count(), 3);
    
    EXPECT_THROW(graph.add_vertex(3, user3), std::invalid_argument);
}

TEST_F(GraphTest, RemoveVertex) {
    graph.add_edge(1, 2, 10);
    EXPECT_NO_THROW(graph.remove_vertex(1));
    EXPECT_FALSE(graph.has_vertex(1));
    EXPECT_FALSE(graph.has_edge(1, 2));
    EXPECT_EQ(graph.vertex_count(), 1);
    
    EXPECT_THROW(graph.remove_vertex(3), std::invalid_argument);
}

TEST_F(GraphTest, AddEdge) {
    EXPECT_NO_THROW(graph.add_edge(1, 2, 10));
    EXPECT_TRUE(graph.has_edge(1, 2));
    EXPECT_TRUE(graph.has_edge(2, 1));
    EXPECT_EQ(graph.get_edge(1, 2).get_weight(), 10);
    
    EXPECT_THROW(graph.add_edge(1, 2, 20), std::invalid_argument);
    
    EXPECT_THROW(graph.add_edge(1, 1, 10), std::invalid_argument);
    
    EXPECT_THROW(graph.add_edge(1, 3, 10), std::invalid_argument);
}

TEST_F(GraphTest, RemoveEdge) {
    graph.add_edge(1, 2, 10);
    EXPECT_NO_THROW(graph.remove_edge(1, 2));
    EXPECT_FALSE(graph.has_edge(1, 2));
    EXPECT_FALSE(graph.has_edge(2, 1));
    
    EXPECT_THROW(graph.remove_edge(1, 2), std::invalid_argument);
}

TEST_F(GraphTest, EdgeWeight) {
    graph.add_edge(1, 2, 10);
    EXPECT_EQ(graph.get_edge(1, 2).get_weight(), 10);
    
    EXPECT_NO_THROW(graph.set_edge_weight(1, 2, 20));
    EXPECT_EQ(graph.get_edge(1, 2).get_weight(), 20);
    EXPECT_EQ(graph.get_edge(2, 1).get_weight(), 20);
}

TEST_F(GraphTest, VertexDegree) {
    EXPECT_EQ(graph.get_degree(1), 0);
    
    graph.add_edge(1, 2, 10);
    EXPECT_EQ(graph.get_degree(1), 1);
    EXPECT_EQ(graph.get_degree(2), 1);
    
    EXPECT_THROW(graph.get_degree(3), std::invalid_argument);
}


TEST_F(GraphTest, SetEdgeWeight) {
    graph.add_edge(1, 2, 10);
    EXPECT_NO_THROW(graph.set_edge_weight(1, 2, 20));
    EXPECT_EQ(graph.get_edge(1, 2).get_weight(), 20);
    EXPECT_EQ(graph.get_edge(2, 1).get_weight(), 20);
}


TEST_F(GraphTest, HasVertexAndEdge) {
    EXPECT_TRUE(graph.has_vertex(1));
    EXPECT_TRUE(graph.has_vertex(2));
    EXPECT_FALSE(graph.has_vertex(3));

    graph.add_edge(1, 2, 10);
    EXPECT_TRUE(graph.has_edge(1, 2));
    EXPECT_TRUE(graph.has_edge(2, 1));
    EXPECT_FALSE(graph.has_edge(1, 3));
}


TEST_F(GraphTest, IsConnected) {
    EXPECT_FALSE(graph.is_connected(1, 2));

    graph.add_edge(1, 2, 10);
    EXPECT_TRUE(graph.is_connected(1, 2));
    EXPECT_TRUE(graph.is_connected(2, 1));

    EXPECT_FALSE(graph.is_connected(1, 3));
}


TEST_F(GraphTest, GetVertexAndEdge) {
    const Vertex<int, User>& vertex1 = graph.get_vertex(1);
    EXPECT_EQ(vertex1.get_id(), 1);

    graph.add_edge(1, 2, 10);
    const Edge<int, int>& edge = graph.get_edge(1, 2);
    EXPECT_EQ(edge.get_from(), 1);
    EXPECT_EQ(edge.get_to(), 2);
    EXPECT_EQ(edge.get_weight(), 10);
}


TEST_F(GraphTest, ClearGraph) {
    graph.add_edge(1, 2, 10);
    EXPECT_FALSE(graph.is_empty());
    
    graph.clear();
    EXPECT_TRUE(graph.is_empty());
    EXPECT_EQ(graph.vertex_count(), 0);
    EXPECT_EQ(graph.edge_count(), 0);
}
