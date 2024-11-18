#include <gtest/gtest.h>
#include "../include/graph.hpp"

// Test resource class
class Warehouse {
private:
    std::string name_;
    std::vector<std::string> items_;
    double capacity_;
    std::map<std::string, size_t> inventory_;
    std::vector<std::pair<double, double>> coordinates_;
    bool is_active_;

public:
    Warehouse(const std::string& name = "default", double capacity = 1000.0) 
        : name_(name), capacity_(capacity), is_active_(true) {}
    
    // Add some memory-intensive operations
    void add_item(const std::string& item, size_t quantity) {
        items_.push_back(item);
        inventory_[item] = quantity;
    }

    void add_coordinate(double x, double y) {
        coordinates_.push_back({x, y});
    }

    bool operator==(const Warehouse& other) const {
        return name_ == other.name_ && capacity_ == other.capacity_;
    }
};

class BFSTest : public ::testing::Test {
protected:
    Graph<int, Warehouse, int> graph;

    void SetUp() override {
        // This will be called before each test
        Warehouse w1("Warehouse1", 1000.0);
        w1.add_item("Item1", 100);
        w1.add_coordinate(1.0, 1.0);

        Warehouse w2("Warehouse2", 2000.0);
        w2.add_item("Item2", 200);
        w2.add_coordinate(2.0, 2.0);

        graph.add_vertex(0, w1);
        graph.add_vertex(1, w2);
    }
};

// Test empty graph
TEST_F(BFSTest, EmptyGraphTest) {
    Graph<int, Warehouse, int> empty_graph;
    EXPECT_THROW(empty_graph.breadth_first_search(0), std::runtime_error);
}

// Test non-existent start vertex
TEST_F(BFSTest, NonExistentStartVertex) {
    EXPECT_THROW(graph.breadth_first_search(99), std::runtime_error);
}

// Test single vertex graph
TEST_F(BFSTest, SingleVertexGraph) {
    Graph<int, Warehouse, int> single_vertex_graph;
    Warehouse w("SingleWarehouse", 500.0);
    single_vertex_graph.add_vertex(0, w);
    
    EXPECT_NO_THROW(single_vertex_graph.breadth_first_search(0));
    EXPECT_EQ(single_vertex_graph.get_vertex(0).get_color(), 2); // Should be black after BFS
}

// Test simple path graph
TEST_F(BFSTest, SimplePathGraph) {
    graph.add_edge(0, 1, 1);
    
    EXPECT_NO_THROW(graph.breadth_first_search(0));
    EXPECT_EQ(graph.get_vertex(0).get_discovery_time(), 0);
    EXPECT_EQ(graph.get_vertex(1).get_discovery_time(), 1);
}

// Test cycle graph
TEST_F(BFSTest, CycleGraph) {
    Warehouse w3("Warehouse3", 3000.0);
    graph.add_vertex(2, w3);
    
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 0, 1);
    
    EXPECT_NO_THROW(graph.breadth_first_search(0));
    
    // Check if all vertices are discovered
    EXPECT_EQ(graph.get_vertex(0).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(1).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(2).get_color(), 2);
}

// Test disconnected graph
TEST_F(BFSTest, DisconnectedGraph) {
    Warehouse w3("Warehouse3", 3000.0);
    Warehouse w4("Warehouse4", 4000.0);
    graph.add_vertex(2, w3);
    graph.add_vertex(3, w4);
    
    graph.add_edge(0, 1, 1);
    graph.add_edge(2, 3, 1);
    
    graph.breadth_first_search(0);
    
    // Vertices 0 and 1 should be black, 2 and 3 should be white
    EXPECT_EQ(graph.get_vertex(0).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(1).get_color(), 2);
    EXPECT_EQ(graph.get_vertex(2).get_color(), 0);
    EXPECT_EQ(graph.get_vertex(3).get_color(), 0);
}

// Test complete graph
TEST_F(BFSTest, CompleteGraph) {
    Warehouse w3("Warehouse3", 3000.0);
    graph.add_vertex(2, w3);
    
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(2, 0, 1);
    
    graph.breadth_first_search(0);
    
    // Check discovery order
    EXPECT_LT(graph.get_vertex(0).get_discovery_time(),
              graph.get_vertex(1).get_discovery_time());
    EXPECT_LT(graph.get_vertex(0).get_discovery_time(),
              graph.get_vertex(2).get_discovery_time());
}

// Test for proper reset
TEST_F(BFSTest, ProperReset) {
    graph.add_edge(0, 1, 1);
    
    graph.breadth_first_search(0);
    graph.breadth_first_search(1);
    
    // After second BFS, vertex 1 should be discovered first
    EXPECT_EQ(graph.get_vertex(1).get_discovery_time(), 0);
}
