#include "../include/graph.hpp"
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;


template <typename VertexType, typename WeightType>
Graph<VertexType, WeightType>::Graph(const Graph& other) : 
            adjacency_list_(other.adjacency_list_),
            vertices_(other.vertices_),
            vertex_count_(other.vertex_count_) {}


template <typename VertexType, typename WeightType>
Graph<VertexType, WeightType>& Graph<VertexType, WeightType>::operator=(const Graph& other) {
    if (this != &other) {
        adjacency_list_ = other.adjacency_list_;
        vertices_ = other.vertices_;
        vertex_count_ = other.vertex_count_;
    }
    return *this;
}

template <typename VertexType, typename WeightType>
Graph<VertexType, WeightType>::Graph(size_t vertex_count) : vertex_count_(vertex_count) {
    initialize_graph(vertex_count);
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::initialize_graph(size_t n) {
    clear();
    vertex_count_ = n;
    
    // Initialize adjacency list with size n
    adjacency_list_ = std::unordered_map<VertexType, std::unordered_map<VertexType, Edge<VertexType, WeightType>>>();
    
    // Initialize vertices map
    vertices_ = std::unordered_map<VertexType, Vertex<VertexType, WeightType>>();
    
    // Add vertices
    for(size_t i = 0; i < n; ++i) {
        vertices_[i] = Vertex<VertexType, WeightType>(i);
        adjacency_list_[i] = std::unordered_map<VertexType, Edge<VertexType, WeightType>>();
    }
}


template <typename VertexType, typename WeightType>
Graph<VertexType, WeightType>::Graph(Graph&& other) noexcept : 
        adjacency_list_(std::move(other.adjacency_list_)), 
        vertices_(std::move(other.vertices_)), vertex_count_(other.vertex_count_) {

    other.vertex_count_ = 0;
}


template <typename VertexType, typename WeightType>
Graph<VertexType, WeightType>& Graph<VertexType, WeightType>::operator=(Graph&& other) noexcept {
    if (this != &other) {
        adjacency_list_ = std::move(other.adjacency_list_);
        vertices_ = std::move(other.vertices_);
        vertex_count_ = other.vertex_count_;
        other.vertex_count_ = 0;
    }
    return *this;
}



template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::add_edge(VertexType from, VertexType to, WeightType weight) {
    if (!has_vertex(from) || !has_vertex(to)) {
        throw std::invalid_argument("Vertices do not exist");
    }
    if (has_edge(from, to)) {
        throw std::invalid_argument("Edge already exists");
    }
    if (from == to) {
        throw std::invalid_argument("Self-loops are not allowed");
    }
    
    adjacency_list_[from].emplace(to, Edge<VertexType, WeightType>(from, to, weight));
    adjacency_list_[to].emplace(from, Edge<VertexType, WeightType>(to, from, weight));
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::add_vertex(VertexType id) {
    if (has_vertex(id)) {
        throw std::invalid_argument("Vertex already exists");
    }
    vertices_[id] = Vertex<VertexType, WeightType>(id);
    adjacency_list_[id] = std::unordered_map<VertexType, Edge<VertexType, WeightType>>();
    ++vertex_count_;
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::remove_edge(VertexType from, VertexType to) {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }
    adjacency_list_[from].erase(to);
    adjacency_list_[to].erase(from);
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::remove_vertex(VertexType vertex) {
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


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::reset_parameters() {
    // Reset all vertices
    for (auto& [id, vertex] : vertices_) {
        vertex.set_color(0);
        vertex.set_discovery_time(-1);
        vertex.set_finish_time(-1);
    }
}


template <typename VertexType, typename WeightType>
json Graph<VertexType, WeightType>::to_json() const {
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


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::save_to_json(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing");
    }
    json j = to_json();
    file << j.dump(4);
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::load_from_json(const std::string& filename) {
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


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::save_json_to_file(const std::string& filename, const json& data) {
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


template <typename VertexType, typename WeightType>
const json Graph<VertexType, WeightType>::get_json() const {
    return this->log_json_;
}


template <typename VertexType, typename WeightType>
const std::unordered_map<VertexType, std::unordered_map<VertexType, Edge<VertexType, WeightType>>>& 
Graph<VertexType, WeightType>::get_adjacency_list() const {
    return adjacency_list_;
}


template <typename VertexType, typename WeightType>
bool Graph<VertexType, WeightType>::operator==(const Graph& other) const {
    return vertices_ == other.vertices_ && adjacency_list_ == other.adjacency_list_;
}


template <typename VertexType, typename WeightType>
bool Graph<VertexType, WeightType>::operator!=(const Graph& other) const {
    return !(*this == other);
}


template <typename VertexType, typename WeightType>
size_t Graph<VertexType, WeightType>::get_degree(const VertexType& vertex) const {
    if (!has_vertex(vertex)) {
        throw std::invalid_argument("Vertex does not exist");
    }
    return adjacency_list_.at(vertex).size();
}



template <typename VertexType, typename WeightType>
bool Graph<VertexType, WeightType>::is_connected(const VertexType& from, const VertexType& to) const {
    return has_edge(from, to);
}





template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::set_edge_weight(const VertexType& from, const VertexType& to, 
                                                   const WeightType& weight) {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }
    adjacency_list_[from][to].set_weight(weight);
    adjacency_list_[to][from].set_weight(weight);
}



template <typename VertexType, typename WeightType>
const Vertex<VertexType, WeightType>& Graph<VertexType, WeightType>::get_vertex(const VertexType& id) const {
    if (!has_vertex(id)) {
        throw std::invalid_argument("Vertex does not exist");
    }
    return vertices_.at(id);
}


template <typename VertexType, typename WeightType>
const Edge<VertexType, WeightType>& Graph<VertexType, WeightType>::get_edge(const VertexType& from, 
                                                                           const VertexType& to) const {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }
    return adjacency_list_.at(from).at(to);
}



template <typename VertexType, typename WeightType>
const std::unordered_map<VertexType, Vertex<VertexType, WeightType>>& 
Graph<VertexType, WeightType>::get_vertices() const {
    return vertices_;
}



template <typename VertexType, typename WeightType>
bool Graph<VertexType, WeightType>::has_vertex(const VertexType& vertex) const {
    return vertices_.find(vertex) != vertices_.end();
}



template <typename VertexType, typename WeightType>
bool Graph<VertexType, WeightType>::has_edge(const VertexType& from, const VertexType& to) const {
    auto it = adjacency_list_.find(from);
    if (it == adjacency_list_.end()) return false;
    return it->second.find(to) != it->second.end();
}


template <typename VertexType, typename WeightType>
size_t Graph<VertexType, WeightType>::vertex_count() const noexcept {
    return vertex_count_;
}


template <typename VertexType, typename WeightType>
size_t Graph<VertexType, WeightType>::edge_count() const noexcept {
    size_t count = 0;
    for (const auto& [_, edges] : adjacency_list_) {
        count += edges.size();
    }
    return count / 2; 
}


template <typename VertexType, typename WeightType>
bool Graph<VertexType, WeightType>::is_empty() const noexcept {
    return vertex_count_ == 0;
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::clear() {
    adjacency_list_.clear();
    vertices_.clear();
    vertex_count_ = 0;
}

#include "../include/edge.hpp"
#include "../include/vertex.hpp"
