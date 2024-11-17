#include "../include/edge.hpp"
#include <utility>

template <typename VertexId, typename W>
Edge<VertexId, W>::Edge(VertexId f, VertexId t, W w) : from_(f), to_(t), weight_(w) {}

template <typename VertexId, typename W>
Edge<VertexId, W>::Edge(const Edge& other_edge): 
        from_(other_edge.from_), to_(other_edge.to_), 
        weight_(other_edge.weight_), is_bridge_(other_edge.is_bridge_) {}


template <typename VertexId, typename W>
Edge<VertexId, W>& Edge<VertexId, W>::operator=(const Edge& other_edge) {
    if (this != &other_edge) {
        from_ = other_edge.from_;
        to_ = other_edge.to_;
        weight_ = other_edge.weight_;
        is_bridge_ = other_edge.is_bridge_;
    }
    return *this;
}

template <typename VertexId, typename W>
Edge<VertexId, W>::Edge(Edge&& other_edge) noexcept: 
        from_(std::move(other_edge.from_)), to_(std::move(other_edge.to_)), 
        weight_(std::move(other_edge.weight_)), is_bridge_(other_edge.is_bridge_) {

    other_edge.is_bridge_ = false;
}

template <typename VertexId, typename W>
Edge<VertexId, W>& Edge<VertexId, W>::operator=(Edge&& other_edge) noexcept {
    if (this != &other_edge) {
        from_ = std::move(other_edge.from_);
        to_ = std::move(other_edge.to_);
        weight_ = std::move(other_edge.weight_);
        is_bridge_ = other_edge.is_bridge_;
        other_edge.is_bridge_ = false;
    }
    return *this;
}

template <typename VertexId, typename W>
Edge<VertexId, W>::~Edge() = default;

template <typename VertexId, typename W>
bool Edge<VertexId, W>::operator==(const Edge& other_edge) const {
    return from_ == other_edge.from_ && 
           to_ == other_edge.to_ && 
           weight_ == other_edge.weight_;
}

template <typename VertexId, typename W>
bool Edge<VertexId, W>::operator!=(const Edge& other_edge) const {
    return !(*this == other_edge);
}

template <typename VertexId, typename W>
const VertexId& Edge<VertexId, W>::get_from() const {
    return from_;
}

template <typename VertexId, typename W>
const VertexId& Edge<VertexId, W>::get_to() const {
    return to_;
}

template <typename VertexId, typename W>
const W Edge<VertexId, W>::get_weight() const {
    return weight_;
}

template <typename VertexId, typename W>
void Edge<VertexId, W>::set_weight(W new_weight) {
    weight_ = new_weight;
}

template <typename VertexId, typename W>
bool Edge<VertexId, W>::is_bridge() const noexcept {
    return is_bridge_;
}

template <typename VertexId, typename W>
void Edge<VertexId, W>::set_bridge(bool value) {
    is_bridge_ = value;
}
