#include "../include/vertex.hpp"

template <typename VertexId, typename Resource>
Vertex<VertexId, Resource>::Vertex(VertexId id, Resource data) : id_(id), data_(new Resource(data)) {}

template <typename VertexId, typename Resource>
Vertex<VertexId, Resource>::Vertex(VertexId id) : id_(id), data_(nullptr) {}

template <typename VertexId, typename Resource>
Vertex<VertexId, Resource>::Vertex(VertexId id, const Resource& data) : id_(id), data_(new Resource(data)) {}

template <typename VertexId, typename Resource>
Vertex<VertexId, Resource>::Vertex(VertexId id, Resource&& data) : id_(id), data_(new Resource(std::move(data))) {}

template <typename VertexId, typename Resource>
Vertex<VertexId, Resource>::Vertex(const Vertex& other) : 
    id_(other.id_),
    data_(*(other.get_data()) ? new Resource(*other.data_) : nullptr),
    visited_(other.visited_),
    color_(other.color_),
    discovery_time_(other.discovery_time_),
    finish_time_(other.finish_time_) {}


template <typename VertexId, typename Resource>
Vertex<VertexId, Resource>& Vertex<VertexId, Resource>::operator=(Vertex&& other) {
    if (this != &other) {
        id_ = std::move(other.id_);
        data_ = std::move(other.data_);
        visited_ = other.visited_;
        color_ = other.color_;
        discovery_time_ = other.discovery_time_;
        finish_time_ = other.finish_time_;
        
        other.visited_ = false;
        other.color_ = 0;
        other.discovery_time_ = -1;
        other.finish_time_ = -1;
    }
    return *this;
}

template <typename VertexId, typename Resource>
Vertex<VertexId, Resource>::Vertex(Vertex&& other) noexcept : id_(std::move(other.id_)), data_(std::move(other.data_)),
        visited_(other.visited_), color_(other.color_),
        discovery_time_(other.discovery_time_), finish_time_(other.finish_time_) {

    other.visited_ = false;
    other.color_ = 0;
    other.discovery_time_ = -1;
    other.finish_time_ = -1;
}

template <typename VertexId, typename Resource>
Vertex<VertexId, Resource>::~Vertex() = default;

template <typename VertexId, typename Resource>
bool Vertex<VertexId, Resource>::operator==(const Vertex& other) const {
    return id_ == other.id_ && 
           ((data_ == nullptr && other.data_ == nullptr) || 
            (data_ != nullptr && other.data_ != nullptr && *data_ == *other.data_)) && 
           visited_ == other.visited_ && 
           color_ == other.color_ &&
           discovery_time_ == other.discovery_time_ &&
           finish_time_ == other.finish_time_;
}

template <typename VertexId, typename Resource>
bool Vertex<VertexId, Resource>::operator!=(const Vertex& other) const {
    return id_ != other.id_ && 
           *data_ != *other.data_ && 
           visited_ != other.visited_ && 
           color_ != other.color_ &&
           discovery_time_ != other.discovery_time_ &&
           finish_time_ != other.finish_time_;
}

template <typename VertexId, typename Resource>
const VertexId& Vertex<VertexId, Resource>::get_id() const {
    return id_;
}

template <typename VertexId, typename Resource>
const UniquePtr<Resource>& Vertex<VertexId, Resource>::get_data() const {
    return data_;
}

template <typename VertexId, typename Resource>
size_t Vertex<VertexId, Resource>::get_color() const {
    return color_;
}

template <typename VertexId, typename Resource>
int Vertex<VertexId, Resource>::get_discovery_time() const {
    return discovery_time_;
}

template <typename VertexId, typename Resource>
int Vertex<VertexId, Resource>::get_finish_time() const {
    return finish_time_;
}

template <typename VertexId, typename Resource>
void Vertex<VertexId, Resource>::set_color(size_t other_color) {
    color_ = other_color;
}

template <typename VertexId, typename Resource>
void Vertex<VertexId, Resource>::set_discovery_time(int d_time) {
    discovery_time_ = d_time;
}

template <typename VertexId, typename Resource>
void Vertex<VertexId, Resource>::set_finish_time(int s_time) {
    finish_time_ = s_time;
}
