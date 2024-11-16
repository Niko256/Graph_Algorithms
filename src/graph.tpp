#include "../include/graph.hpp"
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;


template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, WeightType>::Graph(const Graph& other) : 
            adjacency_list_(other.adjacency_list_),
            vertex_pool_(other.vertex_pool_),
            vertex_count_(other.vertex_count_) {}


template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, WeightType>& Graph<VertexId, WeightType>::operator=(const Graph& other) {
    if (this != &other) {
        adjacency_list_ = other.adjacency_list_;
        vertex_pool_= other.vertex_pool_;
        vertex_count_ = other.vertex_count_;
    }
    return *this;
}

template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, WeightType>::Graph(size_t vertex_count) : vertex_count_(vertex_count) {
    initialize_graph(vertex_count);
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::initialize_graph(size_t n) {
    clear();
    vertex_count_ = n;
    
    // Initialize adjacency list with size n
    adjacency_list_ = std::unordered_map<VertexId, std::unordered_map<VertexId, WeightType>>();
    
    // Initialize vertices map
    vertex_pool_ = std::unordered_map<VertexId, Vertex<VertexId, Resource>>();
    
    // Add vertices
    for(size_t i = 0; i < n; ++i) {
        vertex_pool_[i] = Vertex<VertexId, Resource>(i);
        adjacency_list_[i] = std::unordered_map<VertexId, WeightType>();
    }
}


template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, WeightType>::Graph(Graph&& other) noexcept : 
        adjacency_list_(std::move(other.adjacency_list_)), 
        vertex_pool_(std::move(other.vertex_pool_)), vertex_count_(other.vertex_count_) {

    other.vertex_count_ = 0;
}


template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, WeightType>& Graph<VertexId, WeightType>::operator=(Graph&& other) noexcept {
    if (this != &other) {
        adjacency_list_ = std::move(other.adjacency_list_);
        vertices_ = std::move(other.vertices_);
        vertex_count_ = other.vertex_count_;
        other.vertex_count_ = 0;
    }
    return *this;
}



template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::add_edge(VertexId from, VertexId to, WeightType weight) {
    if (!has_vertex(from) || !has_vertex(to)) {
        throw std::invalid_argument("Vertices do not exist");
    }
    if (has_edge(from, to)) {
        throw std::invalid_argument("Edge already exists");
    }
    if (from == to) {
        throw std::invalid_argument("Self-loops are not allowed");
    }
    
    adjacency_list_[from].emplace(to, Edge<VertexId, WeightType>(from, to, weight));
    adjacency_list_[to].emplace(from, Edge<VertexId, WeightType>(to, from, weight));
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::add_vertex(VertexId id) {
    if (has_vertex(id)) {
        throw std::invalid_argument("Vertex already exists");
    }
    vertices_[id] = Vertex<VertexId, WeightType>(id);
    adjacency_list_[id] = std::unordered_map<VertexId, Edge<VertexId, WeightType>>();
    ++vertex_count_;
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::remove_edge(VertexId from, VertexId to) {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }
    adjacency_list_[from].erase(to);
    adjacency_list_[to].erase(from);
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::remove_vertex(VertexId vertex) {
    if (!has_vertex(vertex)) {
        throw std::invalid_argument("Vertex does not exist");
    }
    
    // Remove all edges connected to this vertex
    for (auto& [v, edges] : adjacency_list_) {
        edges.erase(vertex);
    }
    
    // Remove vertex's adjacency list and the vertex itself
    adjacency_list_.erase(vertex);
    vertices_.erase(vertex);
    --vertex_count_;
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::reset_parameters() {
    // Reset all vertices
    for (auto& [id, vertex] : vertices_) {
        vertex.set_color(0);
        vertex.set_discovery_time(-1);
        vertex.set_finish_time(-1);
    }
}


template <typename VertexId, typename Resource, typename WeightType>
json Graph<VertexId, WeightType>::to_json() const {
    json j;
    j["vertices"] = json::array();
    j["edges"] = json::array();
    
    j["vertex_count"] = vertex_count_;
    
    for (const auto& [id, vertex] : vertices_) {
        j["vertices"].push_back({
            {"id", id}
        });
    }
    
    for (const auto& [from, edges] : adjacency_list_) {
        for (const auto& [to, edge] : edges) {
            if (from < to) { // Avoid duplicating edges
                j["edges"].push_back({
                    {"from", from},
                    {"to", to},
                    {"weight", edge.get_weight()}
                });
            }
        }
    }
    
    return j;
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::save_to_json(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing");
    }
    json j = to_json();
    file << j.dump(4);
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::load_from_json(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for reading");
    }
    
    json j;
    file >> j;
    
    clear();
    
    for (const auto& vertex : j["vertices"]) {
        add_vertex(vertex["id"]);
    }
    
    for (const auto& edge : j["edges"]) {
        add_edge(edge["from"], edge["to"], edge["weight"]);
    }
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::save_json_to_file(const std::string& filename, const json& data) {
    std::string directory = "files";
    if (!fs::exists(directory)) {
        fs::create_directory(directory);
    }
    
    std::string absolute_path = fs::current_path().string() + "/" + directory + "/" + filename;
    std::ofstream file(absolute_path);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + absolute_path);
    }
    file << data.dump(4);
    file.close();
}


template <typename VertexId, typename Resource, typename WeightType>
const json Graph<VertexId, WeightType>::get_json() const {
    return this->log_json_;
}


template <typename VertexId, typename Resource, typename WeightType>
const std::unordered_map<VertexId, std::unordered_map<VertexId, Edge<VertexId, WeightType>>>& 
Graph<VertexId, WeightType>::get_adjacency_list() const {
    return adjacency_list_;
}


template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, WeightType>::operator==(const Graph& other) const {
    return vertices_ == other.vertices_ && adjacency_list_ == other.adjacency_list_;
}


template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, WeightType>::operator!=(const Graph& other) const {
    return !(*this == other);
}


template <typename VertexId, typename Resource, typename WeightType>
size_t Graph<VertexId, WeightType>::get_degree(const VertexId& vertex) const {
    if (!has_vertex(vertex)) {
        throw std::invalid_argument("Vertex does not exist");
    }
    return adjacency_list_.at(vertex).size();
}



template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, WeightType>::is_connected(const VertexId& from, const VertexId& to) const {
    return has_edge(from, to);
}





template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::set_edge_weight(const VertexId& from, const VertexId& to, 
                                                   const WeightType& weight) {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }
    adjacency_list_[from][to].set_weight(weight);
    adjacency_list_[to][from].set_weight(weight);
}



template <typename VertexId, typename Resource, typename WeightType>
const Vertex<VertexId, WeightType>& Graph<VertexId, WeightType>::get_vertex(const VertexId& id) const {
    if (!has_vertex(id)) {
        throw std::invalid_argument("Vertex does not exist");
    }
    return vertices_.at(id);
}


template <typename VertexId, typename Resource, typename WeightType>
const Edge<VertexId, WeightType>& Graph<VertexId, WeightType>::get_edge(const VertexId& from, 
                                                                           const VertexId& to) const {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }
    return adjacency_list_.at(from).at(to);
}



template <typename VertexId, typename Resource, typename WeightType>
const std::unordered_map<VertexId, Vertex<VertexId, WeightType>>& 
Graph<VertexId, WeightType>::get_vertices() const {
    return vertices_;
}



template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, WeightType>::has_vertex(const VertexId& vertex) const {
    return vertices_.find(vertex) != vertices_.end();
}



template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, WeightType>::has_edge(const VertexId& from, const VertexId& to) const {
    auto it = adjacency_list_.find(from);
    if (it == adjacency_list_.end()) return false;
    return it->second.find(to) != it->second.end();
}


template <typename VertexId, typename Resource, typename WeightType>
size_t Graph<VertexId, WeightType>::vertex_count() const noexcept {
    return vertex_count_;
}


template <typename VertexId, typename Resource, typename WeightType>
size_t Graph<VertexId, WeightType>::edge_count() const noexcept {
    size_t count = 0;
    for (const auto& [_, edges] : adjacency_list_) {
        count += edges.size();
    }
    return count / 2; 
}


template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, WeightType>::is_empty() const noexcept {
    return vertex_count_ == 0;
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, WeightType>::clear() {
    adjacency_list_.clear();
    vertices_.clear();
    vertex_count_ = 0;
}

#include "../include/edge.hpp"
#include "../include/vertex.hpp"
