#include "../include/edge.hpp"
#include <utility>

template <typename T, typename W>
Edge<T, W>::Edge(T f, T t, W w) : from_(f), to_(t), weight_(w) {}

template <typename T, typename W>
Edge<T, W>::Edge(const Edge& other_edge): 
        from_(other_edge.from_), to_(other_edge.to_), 
        weight_(other_edge.weight_), is_bridge_(other_edge.is_bridge_) {}


template <typename T, typename W>
Edge<T, W>& Edge<T, W>::operator=(const Edge& other_edge) {
    if (this != &other_edge) {
        from_ = other_edge.from_;
        to_ = other_edge.to_;
        weight_ = other_edge.weight_;
        is_bridge_ = other_edge.is_bridge_;
    }
    return *this;
}

template <typename T, typename W>
Edge<T, W>::Edge(Edge&& other_edge) noexcept: 
        from_(std::move(other_edge.from_)), to_(std::move(other_edge.to_)), 
        weight_(std::move(other_edge.weight_)), is_bridge_(other_edge.is_bridge_) {

    other_edge.is_bridge_ = false;
}

template <typename T, typename W>
Edge<T, W>& Edge<T, W>::operator=(Edge&& other_edge) noexcept {
    if (this != &other_edge) {
        from_ = std::move(other_edge.from_);
        to_ = std::move(other_edge.to_);
        weight_ = std::move(other_edge.weight_);
        is_bridge_ = other_edge.is_bridge_;
        other_edge.is_bridge_ = false;
    }
    return *this;
}

template <typename T, typename W>
Edge<T, W>::~Edge() = default;

template <typename T, typename W>
bool Edge<T, W>::operator==(const Edge& other_edge) const {
    return from_ == other_edge.from_ && 
           to_ == other_edge.to_ && 
           weight_ == other_edge.weight_;
}

template <typename T, typename W>
bool Edge<T, W>::operator!=(const Edge& other_edge) const {
    return !(*this == other_edge);
}

template <typename T, typename W>
T Edge<T, W>::get_from() const {
    return from_;
}

template <typename T, typename W>
T Edge<T, W>::get_to() const {
    return to_;
}

template <typename T, typename W>
W Edge<T, W>::get_weight() const {
    return weight_;
}

template <typename T, typename W>
void Edge<T, W>::set_weight(W new_weight) {
    weight_ = new_weight;
}

template <typename T, typename W>
bool Edge<T, W>::is_bridge() const noexcept {
    return is_bridge_;
}

template <typename T, typename W>
void Edge<T, W>::set_bridge(bool value) {
    is_bridge_ = value;
}
