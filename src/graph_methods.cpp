#include "../include/graph.hpp"

template <WeightType Weight>
void Graph<Weight>::add_vertex(Vertex vert_) noexcept {
    
}


template <WeightType Weight>
void Graph<Weight>::remove_vertex(Vertex vert_) noexcept {
    
}


template <WeightType Weight>
void Graph<Weight>::add_edge(Vertex from_, Vertex to_, Weight weight_) {
    
}


template <WeightType Weight>
void Graph<Weight>::has_edge(Vertex from_, Vertex to_) const noexcept {

}


template <WeightType Weight>
size_t Graph<Weight>::vertex_count() const noexcept {
    return vertex_count_;    
}


template <WeightType Weight>
const DbList<Pair<Vertex, Weight>> Graph<Weight>::get_adjancency_list(Vertex vert_) const {
    return adjacency_list_[vert_];
}


template <WeightType Weight>
Weight Graph<Weight>::get_edge_weight(Vertex from_, Vertex to_) const {
    
}


template <WeightType Weight>
void Graph<Weight>::set_edge_weight(Vertex from_, Vertex to_) const {
    
}


template <WeightType Weight>
void Graph<Weight>::add_undirected_edge(Vertex from_, Vertex to_, Weight weight) {
    
}


template <WeightType Weight>
void Graph<Weight>::remove_undirected_edge(Vertex from_, Vertex to_) {
    
}
