#include "../include/graph.hpp"
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;


template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, Resource, WeightType>::Graph(const Graph& other) : 
            adjacency_list_(other.adjacency_list_),
            vertex_pool_(other.vertex_pool_),
            vertex_count_(other.vertex_count_),
            log_json_(other.log_json_) {}


template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, Resource, WeightType>& Graph<VertexId, Resource, WeightType>::operator=(const Graph& other) {
    if (this != &other) {
        adjacency_list_ = other.adjacency_list_;
        vertex_pool_= other.vertex_pool_;
        vertex_count_ = other.vertex_count_;
        log_json_ = other.log_json_;
    }
    return *this;
}

template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, Resource, WeightType>::Graph(size_t vertex_count) : vertex_count_(vertex_count) {
    initialize_graph(vertex_count);
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::initialize_graph(size_t n) {
    clear();
    vertex_count_ = n;
    
    adjacency_list_.clear(); 
    
    vertex_pool_.clear();
    
    // Add vertices
    for(size_t i = 0; i < n; ++i) {
        vertex_pool_[i] = Vertex<VertexId, Resource>(i);
        adjacency_list_[i] = std::unordered_map<VertexId, SharedPtr<Edge<VertexId, WeightType>>>();
    }
}


template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, Resource, WeightType>::Graph(Graph&& other) noexcept : 
    adjacency_list_(std::move(other.adjacency_list_)), 
    vertex_pool_(std::move(other.vertex_pool_)), 
    vertex_count_(other.vertex_count_),
    log_json_(std::move(other.log_json_)) {
        
        other.adjacency_list_.clear();
        other.vertex_pool_.clear();
        other.log_json_.clear();
        other.vertex_count_ = 0;
    }


template <typename VertexId, typename Resource, typename WeightType>
Graph<VertexId, Resource, WeightType>& Graph<VertexId, Resource, WeightType>::operator=(Graph&& other) noexcept {
    if (this != &other) {
        adjacency_list_ = std::move(other.adjacency_list_);
        vertex_pool_ = std::move(other.vertex_pool_);
        vertex_count_ = other.vertex_count_;
        log_json_ = std::move(other.log_json_);
        
        
        other.adjacency_list_.clear();
        other.vertex_pool_.clear();
        other.log_json_.clear();
        other.vertex_count_ = 0;
    }
    return *this;
}



template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::add_edge(VertexId from, VertexId to, WeightType weight) {
    if (!has_vertex(from) || !has_vertex(to)) {
        throw std::invalid_argument("Vertices do not exist");
    }
    if (has_edge(from, to)) {
        throw std::invalid_argument("Edge already exists");
    }
    if (from == to) {
        throw std::invalid_argument("Self-loops are not allowed");
    }

    auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(new Edge<VertexId, WeightType>(from, to, weight));

    adjacency_list_[from][to] = edge_ptr;
    adjacency_list_[to][from] = SharedPtr(edge_ptr);
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::add_vertex(VertexId id) {
    if (has_vertex(id)) {
        throw std::invalid_argument("Vertex already exists");
    }

    vertex_pool_[id] = Vertex<VertexId, Resource>(id);

    ++vertex_count_;
}



template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::add_vertex(VertexId id, const Resource& data) {
    if (has_vertex(id)) {
        throw std::invalid_argument("Vertex already exists");
    }

    vertex_pool_[id] = Vertex<VertexId, Resource>(id, data);
    adjacency_list_[id] = std::unordered_map<VertexId, SharedPtr<Edge<VertexId, WeightType>>>();

    ++vertex_count_;
}



template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::remove_edge(VertexId from, VertexId to) {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }

    // remove from adjacency lists
    adjacency_list_[from].erase(to);
    adjacency_list_[to].erase(from);
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::remove_vertex(VertexId vertex) {
    if (!has_vertex(vertex)) {
        throw std::invalid_argument("Vertex does not exist");
    }
    
    for (const auto& [v, _] : adjacency_list_[vertex]) {
        adjacency_list_[v].erase(vertex);
    }

    adjacency_list_.erase(vertex);
    vertex_pool_.erase(vertex);
    --vertex_count_;
}


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::reset_parameters() {
    for (auto& [id, vertex] : vertex_pool_) {
        vertex.set_color(0);
        vertex.set_discovery_time(-1);
        vertex.set_finish_time(-1);
    }
}

template <typename VertexId, typename Resource, typename WeightType>
json Graph<VertexId, Resource, WeightType>::to_json() {
    json j;
    j["vertices"] = json::array();
    j["edges"] = json::array();
    
    j["vertex_count"] = vertex_count_;
    
    std::unordered_map<VertexId, size_t> vertex_mapping;
    size_t new_index = 0;

    for (const auto& [id, vertex] : vertex_pool_) {
        j["vertices"].push_back({
            {"id", new_index}
        });
        vertex_mapping[id] = new_index++;
    }
    
    for (const auto& [from, edges] : adjacency_list_) {
        for (const auto& [to, edge_ptr] : edges) {
            if (from < to) {
                j["edges"].push_back({
                    {"from", vertex_mapping[from]},
                    {"to", vertex_mapping[to]},
                    {"weight", edge_ptr->get_weight()}
                });
            }
        }
    }
    
    return j;
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::save_to_json(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing");
    }
    json j = to_json();
    file << j.dump(4);
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::load_from_json(const std::string& filename) {
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
void Graph<VertexId, Resource, WeightType>::save_json_to_file(const std::string& filename, const json& data) {
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
const json Graph<VertexId, Resource, WeightType>::get_json() const {
    return this->log_json_;
}

template <typename VertexId, typename Resource, typename WeightType>
const std::unordered_map<VertexId, std::unordered_map<VertexId, SharedPtr<Edge<VertexId, WeightType>>>>& 
Graph<VertexId, Resource, WeightType>::get_adjacency_list() const {
    return adjacency_list_;
}


template <typename VertexId, typename Resource, typename WeightType>
size_t Graph<VertexId, Resource, WeightType>::get_degree(const VertexId& vertex) const {
    if (!has_vertex(vertex)) {
        throw std::invalid_argument("Vertex does not exist");
    }
    return adjacency_list_.at(vertex).size();
}

template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, Resource, WeightType>::is_connected(const VertexId& from, const VertexId& to) const {
    return has_edge(from, to);
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::set_edge_weight(const VertexId& from, const VertexId& to, 
                                                 const WeightType& weight) {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }
    adjacency_list_[from][to]->set_weight(weight);
    adjacency_list_[to][from]->set_weight(weight);
}

template <typename VertexId, typename Resource, typename WeightType>
const Vertex<VertexId, Resource>& Graph<VertexId, Resource, WeightType>::get_vertex(const VertexId& id) const {
    if (!has_vertex(id)) {
        throw std::invalid_argument("Vertex does not exist");
    }
    return vertex_pool_.at(id);
}

template <typename VertexId, typename Resource, typename WeightType>
const Edge<VertexId, WeightType>& Graph<VertexId, Resource, WeightType>::get_edge(const VertexId& from, 
                                                                       const VertexId& to) const {
    if (!has_edge(from, to)) {
        throw std::invalid_argument("Edge does not exist");
    }
    return *(adjacency_list_.at(from).at(to));
}

template <typename VertexId, typename Resource, typename WeightType>
const std::unordered_map<VertexId, Vertex<VertexId, Resource>>& Graph<VertexId, Resource, WeightType>::get_vertices() const {
    return vertex_pool_;
}

template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, Resource, WeightType>::has_vertex(const VertexId& vertex) const {
    return vertex_pool_.find(vertex) != vertex_pool_.end();
}

template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, Resource, WeightType>::has_edge(const VertexId& from, const VertexId& to) const {
    auto it = adjacency_list_.find(from);
    if (it == adjacency_list_.end()) return false;
    return it->second.find(to) != it->second.end();
}

template <typename VertexId, typename Resource, typename WeightType>
size_t Graph<VertexId, Resource, WeightType>::vertex_count() const noexcept {
    return vertex_count_;
}

template <typename VertexId, typename Resource, typename WeightType>
size_t Graph<VertexId, Resource, WeightType>::edge_count() const noexcept {
    size_t count = 0;
    for (const auto& [_, edges] : adjacency_list_) {
        count += edges.size();
    }
    return count / 2;
}

template <typename VertexId, typename Resource, typename WeightType>
bool Graph<VertexId, Resource, WeightType>::is_empty() const noexcept {
    return vertex_count_ == 0;
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::clear() {
    adjacency_list_.clear();
    vertex_pool_.clear();
    vertex_count_ = 0;
}

#include "../include/edge.hpp"
#include "../include/vertex.hpp"
