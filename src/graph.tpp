#include "../include/graph.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include "cerrno"
#include <nlohmann/json.hpp>
#include <filesystem>
namespace fs = std::filesystem;


using json = nlohmann::json;


template <typename VertexType, typename WeightType>
size_t Graph<VertexType, WeightType>::traversal_timer_ = 0;


template <typename VertexType, typename WeightType>
Graph<VertexType, WeightType>::Graph() : vertex_count_(0) {}


template <typename VertexType, typename WeightType>
Graph<VertexType, WeightType>::Graph(size_t vertex_count) : vertex_count_(vertex_count) {
    adjacency_list_.resize(vertex_count);
    for (size_t i = 0; i < vertex_count; ++i) {
        adjacency_list_[i].reserve(vertex_count);
    }

    discovery_time_ = DynamicArray<int>(vertex_count_, 0);
    finish_time_ = DynamicArray<int>(vertex_count_, 0);
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::resize(size_t new_size) {
    if (new_size >= adjacency_list_.size()) {
        adjacency_list_.resize(new_size);
        discovery_time_.resize(new_size);
        finish_time_.resize(new_size);
    }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::add_edge(VertexType from, VertexType to, WeightType weight) {
    if (from >= vertex_count_ || to >= vertex_count_) {
        resize(std::max(from, to) + 1);
    }

    adjacency_list_[from].emplace_back(std::move(Pair(to, weight)));

    adjacency_list_[to].emplace_back(std::move(Pair(from, weight)));

}



template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::add_vertex() {
    if (vertex_count_ >= adjacency_list_.size()) {
        size_t new_size = (vertex_count_ == 0) ? 1 : 2 * vertex_count_;
        adjacency_list_.resize(new_size);
    }
    adjacency_list_[vertex_count_] = DynamicArray<Pair<VertexType, WeightType>>();
    ++vertex_count_;
    discovery_time_.resize(vertex_count_);
    finish_time_.resize(vertex_count_);
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::remove_edge(VertexType from, VertexType to) {
    if (from >= vertex_count_ || to >= vertex_count_) throw std::out_of_range("out of range");

    auto& edges = adjacency_list_[from];
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        if ((*it).first_ == to) {
            edges.erase(it);
            break;
        }
    }

        auto& reversed_edges = adjacency_list_[to];
        for (auto it = reversed_edges.begin(); it != reversed_edges.end(); ++it) {
            if ((*it).first_ == from) {
                reversed_edges.erase(it);
                break;
            }
        }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::remove_vertex(VertexType vertex) {
    if (vertex >= vertex_count_) throw std::out_of_range("Attempt to remove non-existent vertex");

    for (size_t i = 0; i < vertex_count_; ++i) {
        if (i != vertex) {
            auto& edges = adjacency_list_[i];
            for (auto it = edges.begin(); it != edges.end(); ) {
                if ((*it).first_ == vertex) {
                    it = edges.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    adjacency_list_[vertex].clear();

    for (size_t i = 0; i < vertex_count_; ++i) {
        auto& edges = adjacency_list_[i];
        for (auto& edge : edges) {
            if (edge.first_ > vertex) {
                --edge.first_;
            }
        }
    }

    for (size_t i = vertex; i < vertex_count_ - 1; ++i) {
        adjacency_list_[i] = std::move(adjacency_list_[i + 1]);
    }
    adjacency_list_.resize(vertex_count_ - 1);

    --vertex_count_;
}


template <typename VertexType, typename WeightType>
bool Graph<VertexType, WeightType>::has_edge(VertexType from, VertexType to) const {
    if (from >= vertex_count_ || to >= vertex_count_) throw std::out_of_range("out of range");

    const auto& edges = adjacency_list_[from];
    for (auto it = edges.cbegin(); it != edges.cend(); ++it) {
        if ((*it).first_ == to) return true;
    }
    return false;
}


template <typename VertexType, typename WeightType>
json Graph<VertexType, WeightType>::to_json() {
    json j;
    j["vertex_count"] = vertex_count_;
    j["edges"] = json::array();

    for (size_t i = 0; i < vertex_count_; ++i) {
        for (auto& edge : adjacency_list_[i]) {
            j["edges"].push_back({
                {"from", i},
                {"to", edge.first_},
                {"weight", edge.second_}
            });
        }
    }

    return j;
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::save_to_json(const std::string& filename) {
    json j = to_json();
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for saving graph parameters");
    }

    file << j.dump(4);
    file.close();
}



template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::load_from_json(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for loading graph parameters");
    }

    json j;
    file >> j;
    file.close();

    vertex_count_ = j["vertex_count"];
    adjacency_list_.resize(vertex_count_);
    discovery_time_ = DynamicArray<int>(vertex_count_, 0);
    finish_time_ = DynamicArray<int>(vertex_count_, 0);

    for (const auto& edge : j["edges"]) {
        VertexType from = edge["from"];
        VertexType to = edge["to"];
        WeightType weight = edge["weight"];
        add_edge(from, to, weight);
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
void Graph<VertexType, WeightType>::reset_timer() const {
    traversal_timer_ = 0; 
}




template <typename VertexType, typename WeightType>
const DynamicArray<Pair<VertexType, WeightType>>& Graph<VertexType, WeightType>::get_adjacency_list(VertexType vertex) const {
    return adjacency_list_[vertex];
}



template <typename VertexType, typename WeightType>
constexpr size_t Graph<VertexType, WeightType>::vertex_count() const noexcept { return vertex_count_; }


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::clear() {
    for (size_t i = 0; i < vertex_count_; ++i) {
        adjacency_list_[i].clear();
    }
    vertex_count_ = 0;
}


template <typename VertexType, typename WeightType>
const DynamicArray<int>& Graph<VertexType, WeightType>::get_discovery_time() const {
    return discovery_time_;
}


template <typename VertexType, typename WeightType>
const DynamicArray<int>& Graph<VertexType, WeightType>::get_finish_time() const {
    return finish_time_;
}



template <typename VertexType, typename WeightType>
typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::iterator Graph<VertexType, WeightType>::begin() noexcept {
    return adjacency_list_.begin();
}


template <typename VertexType, typename WeightType>
typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::const_iterator Graph<VertexType, WeightType>::cbegin() const noexcept {
    return adjacency_list_.cbegin();
}


template <typename VertexType, typename WeightType>
typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::iterator Graph<VertexType, WeightType>::end() noexcept {
    return adjacency_list_.end();
}


template <typename VertexType, typename WeightType>
typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::const_iterator Graph<VertexType, WeightType>::cend() const noexcept {
    return adjacency_list_.cend();
}
