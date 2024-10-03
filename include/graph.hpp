#pragma once
#include <cstddef>
#include <type_traits>
#include <utility>
#include <concepts>
#include "../../containers/pair.hpp"
#include "../../containers/dynamic_array.hpp"
#include "../../containers/doubly_linked_list.hpp"


using Vertex = size_t;

template <typename W>
concept WeightType = std::is_floating_point_v<W> || std::is_integral_v<W>;

// adjacency list
template <WeightType Weight>
class Graph {
private:
    size_t vertex_count_;
    DynamicArray<DbList<Pair<Vertex, Weight>>> adjacency_list_;

public:
    Graph(size_t vertex_count) : vertex_count_(vertex_count), adjacency_list_(vertex_count) {}

    void add_edge(Vertex from_, Vertex to_, Weight weight_) {
        adjacency_list_[from_].push_back(make_my_pair(to_, weight_);
    }

    void remove_edge(Vertex from_, Vertex to_) {
        
    }

    void remove_vertex(Vertex index_) {
        
    }

    void remove_vertex() {
        
    }

    bool has_edge(Vertex from_, Vertex to_) {
        
    }

    Weight get_weight() {
        
    }

    const DbList<Pair<Vertex, Weight>>& get_adjacency_list() const {
        
    }

    size_t vertex_count() const noexcept {
        
    }

    size_t edge_count() const noexcept {
        
    }
};
