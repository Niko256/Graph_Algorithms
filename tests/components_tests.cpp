#include <gtest/gtest.h>
#include "../include/graph.hpp"

// Define Warehouse class as the heavy resource
class Warehouse {
public:
    Warehouse(int capacity = 0, double utilization = 0.0) 
        : storage_capacity_(capacity), space_utilization_(utilization) {}
    
    int get_capacity() const { return storage_capacity_; }
    double get_utilization() const { return space_utilization_; }
    
private:
    int storage_capacity_;
    double space_utilization_;
};

class ConnectedComponentsTest : public ::testing::Test {
protected:
    Graph<int, Warehouse, int> graph;

    void SetUp() override {
        // Common setup code if needed
    }

    void create_test_graph_with_warehouses() {
        graph.add_vertex(0, Warehouse(1000, 0.75));
        graph.add_vertex(1, Warehouse(2000, 0.50));
        graph.add_vertex(2, Warehouse(1500, 0.80));
        graph.add_vertex(3, Warehouse(3000, 0.60));
        graph.add_vertex(4, Warehouse(2500, 0.70));
    }
};

// Test empty graph
TEST_F(ConnectedComponentsTest, EmptyGraphThrowsException) {
    EXPECT_THROW(graph.find_connected_components(), std::runtime_error);
}

// Test single vertex graph
TEST_F(ConnectedComponentsTest, SingleVertexGraph) {
    graph.add_vertex(0, Warehouse(1000, 0.5));
    
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 1);
    EXPECT_EQ(components[0][0], 0);
}

// Test disconnected graph
TEST_F(ConnectedComponentsTest, DisconnectedGraph) {
    create_test_graph_with_warehouses();
    graph.add_edge(0, 1, 1);
    graph.add_edge(2, 3, 1);
    // Vertex 4 remains isolated

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 3);

    // Verify component sizes
    std::vector<size_t> expected_sizes = {2, 2, 1};
    std::vector<size_t> actual_sizes;
    for (const auto& component : components) {
        actual_sizes.push_back(component.size());
    }
    // Sort in descending order
    std::sort(actual_sizes.begin(), actual_sizes.end(), std::greater<size_t>());
    std::sort(expected_sizes.begin(), expected_sizes.end(), std::greater<size_t>());
    EXPECT_EQ(actual_sizes, expected_sizes);
}

// Test fully connected graph
TEST_F(ConnectedComponentsTest, FullyConnectedGraph) {
    create_test_graph_with_warehouses();
    
    // Create complete graph
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            graph.add_edge(i, j, 1);
        }
    }

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 5);
}

// Test chain graph
TEST_F(ConnectedComponentsTest, ChainGraph) {
    create_test_graph_with_warehouses();
    
    // Create a chain 0-1-2-3-4
    for (int i = 0; i < 4; ++i) {
        graph.add_edge(i, i + 1, 1);
    }

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 5);
}

// Test multiple distinct components
TEST_F(ConnectedComponentsTest, MultipleComponents) {
    create_test_graph_with_warehouses();
    
    // Create two components: (0,1,2) and (3,4)
    graph.add_edge(0, 1, 1);
    graph.add_edge(1, 2, 1);
    graph.add_edge(3, 4, 1);

    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 2);
    
    // Find and verify the sizes of components
    std::vector<size_t> component_sizes;
    for (const auto& component : components) {
        component_sizes.push_back(component.size());
    }
    std::sort(component_sizes.begin(), component_sizes.end());
    EXPECT_EQ(component_sizes[0], 2);
    EXPECT_EQ(component_sizes[1], 3);
}

// Test graph with self-loops
TEST_F(ConnectedComponentsTest, GraphWithSelfLoops) {
    graph.add_vertex(0, Warehouse(1000, 0.5));
    graph.add_vertex(1, Warehouse(2000, 0.6));
    
    // Adding regular edge
    graph.add_edge(0, 1, 1);
    
    // Self-loops should be ignored by the algorithm
    EXPECT_THROW(graph.add_edge(0, 0, 1), std::invalid_argument);
    
    auto components = graph.find_connected_components();
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].size(), 2);
}

// Test for JSON output
TEST_F(ConnectedComponentsTest, JsonOutput) {
    create_test_graph_with_warehouses();
    graph.add_edge(0, 1, 1);
    graph.add_edge(2, 3, 1);
    // Vertex 4 remains isolated

    graph.find_connected_components();
    
    // Verify JSON file existence
    EXPECT_TRUE(std::filesystem::exists("files/components.json"));
    
    // Read and verify JSON content
    std::ifstream file("files/components.json");
    ASSERT_TRUE(file.is_open());
    
    json components_data;
    file >> components_data;
    
    EXPECT_EQ(components_data["components_count"], 3);
    EXPECT_EQ(components_data["components"].size(), 3);
}
