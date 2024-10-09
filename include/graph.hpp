#pragma once
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include "../../Data_Structures/Pair.hpp"
#include "../../Data_Structures/Dynamic_Array.hpp"

template <typename VertexType, typename WeightType>
class Graph {
private:
    size_t vertex_count_; // Count of vertices
    bool directed_;
    DynamicArray<DynamicArray<Pair<VertexType, WeightType>>> adjacency_list_;

    void resize(size_t new_size) {
        if (new_size >= adjacency_list_.size()) {
            adjacency_list_.resize(new_size);
        }
    }

public:
    Graph() : vertex_count_(0), directed_(false) {}

    Graph(size_t vertex, bool dir = false) : vertex_count_(vertex), directed_(dir) {
        adjacency_list_.resize(vertex);
    }

    void add_edge(VertexType from, VertexType to, WeightType weight) {
        if (from >= vertex_count_ || to >= vertex_count_) {
            resize(std::max(from, to) + 1);
        }

        adjacency_list_[from].push_back(std::move(Pair(to, weight)));

        if (!directed_) {
            adjacency_list_[to].push_back(std::move(Pair(from, weight))); 
        }
    }

    void add_vertex() {
        if (vertex_count_ >= adjacency_list_.size()) {
            adjacency_list_.resize(2 * vertex_count_);
        }
        ++vertex_count_;
    }


    void remove_edge(VertexType from, VertexType to) {
        if (from >= vertex_count_ || to >= vertex_count_) throw std::out_of_range("out of range");

        auto& edges = adjacency_list_[from];
        for (auto it = edges.begin(); it != edges.end(); ++it) {
            if ((*it).first_ == to) {
                edges.erase(it);
                break;
            }
        }

        if (!directed_) {
            auto& reversed_edges = adjacency_list_[to];
            for (auto it = reversed_edges.begin(); it != reversed_edges.end(); ++it) {
                if ((*it).first_ == from) {
                    reversed_edges.erase(it);
                    break;
                }
            }
        }
    }

   
    void remove_vertex(VertexType vertex) {
        if (vertex >= vertex_count_) throw std::out_of_range("out of range");

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


    bool has_edge(VertexType from, VertexType to) const {
        if (from >= vertex_count_ || to >= vertex_count_) throw std::out_of_range("out of range");

        const auto& edges = adjacency_list_[from];
        for (auto it = edges.cbegin(); it != edges.cend(); ++it) {
            if ((*it).first_ == to) return true;
        }
        return false;

    }


    const DynamicArray<Pair<VertexType, WeightType>>& get_adjacency_list(VertexType vertex) const {
        return adjacency_list_[vertex];
    }


    constexpr size_t vertex_count() const noexcept { return vertex_count_; }

    constexpr bool is_directed() const noexcept { return directed_; }

    void clear() {
        for (size_t i = 0; i < vertex_count_; ++i) {
            adjacency_list_[i].clear();
        } 
        vertex_count_ = 0;
    }

    typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::iterator begin() noexcept {
        return adjacency_list_.begin();
    }

    typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::const_iterator cbegin() const noexcept {
        return adjacency_list_.cbegin();
    }

    typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::iterator end() noexcept {
        return adjacency_list_.end();
    }

    typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::const_iterator cend() const noexcept {
        return adjacency_list_.cend();
    }
};
