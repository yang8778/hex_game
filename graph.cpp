#include <iostream>
#include <assert.h>
#include "graph.h"

namespace HEX {

void Graph::dijkstra(uint s) {
    Graph::VertexList vertices = get_vertices();
    assert (s < vertices.size());
    // initialize single source
    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
        (*it)->m_distance = std::numeric_limits<float>::infinity();
        (*it)->m_predecessor = -1;
    }
    vertices[s]->m_distance = 0;

    // Q <-- V[G]
    for (auto sp : vertices) {
        m_priority_deque.push_back(sp);
    }
    std::make_heap(m_priority_deque.begin(), m_priority_deque.end(), VertexComparision()); 
    
    while (!m_priority_deque.empty()) {
        std::shared_ptr<VertexNode> sp_u = m_priority_deque.front();
        //std::cout << "u: " << sp_u->m_index << " " << sp_u->m_distance << " " << sp_u->m_predecessor << std::endl; 
        m_priority_deque.pop_front(); 
        for (auto it = sp_u->m_edge_list.begin(); it != sp_u->m_edge_list.end(); ++it) {
            std::shared_ptr<VertexNode> sp_v = vertices[(*it)->m_adjacent_vertex];
            float edge_value = get_edge_value(sp_u->m_index, sp_v->m_index);
            if (get_node_status(sp_v->m_index) != NodeColor::UNOCCUPIED && sp_v->m_distance > sp_u->m_distance + edge_value) {
                sp_v->m_distance = sp_u->m_distance + edge_value;
                sp_v->m_predecessor = sp_u->m_index; 
                //std::cout << "v after relax: " << sp_v->m_index << " " << sp_v->m_distance << " " << sp_v->m_predecessor << std::endl; 
            }
        }
        std::make_heap(m_priority_deque.begin(), m_priority_deque.end(), VertexComparision()); 
    } 
}

uint Graph::edges_number() {
    uint numbers = 0;
    for (auto it = m_vertices.begin(); it != m_vertices.end(); ++ it) {
        numbers += (*it)->m_edge_list.size(); 
    }
    // in undirected graph, the edges number equals to the sum of 
    // all adjacent lists' size divide 2; otherwise not divide
    return numbers/2;
}

float Graph::get_edge_value(uint x, uint y) {
    uint sz = vertices_number();
    assert (x < sz && y < sz); 
    float edge_value = std::numeric_limits<float>::infinity();
    for (auto it = m_vertices[x]->m_edge_list.begin(); it != m_vertices[x]->m_edge_list.end(); ++it) {
        if (y == (*it)->m_adjacent_vertex) {
            edge_value = (*it)->m_weight;
            break;
        }
    }
    return edge_value;
}

bool Graph::is_adjacent(uint x, uint y) {
    float edge_value = get_edge_value(x, y);
    if (edge_value == std::numeric_limits<float>::infinity()) 
        return false;
    else
        return true;
}

Graph::VertexList Graph::neighbors(uint x) {
    assert (x < vertices_number());
    Graph::VertexList neighbors;
    for (auto it = m_vertices[x]->m_edge_list.begin(); it != m_vertices[x]->m_edge_list.end(); ++it) {
        std::shared_ptr<VertexNode> sp = m_vertices[(*it)->m_adjacent_vertex];
        neighbors.push_back(sp);  
    } 
    return neighbors;
}

void Graph::add_edge(uint x, uint y, float v) {
    if (is_adjacent(x, y)) 
        return;
    else {
        std::shared_ptr<EdgeNode> sp = std::make_shared<EdgeNode>(x, y, v);  
        m_vertices[x]->m_edge_list.push_back(sp);
    } 
}

void Graph::set_edge_value(uint x, uint y, float v) {
    uint sz = vertices_number();
    assert (x < sz && y < sz && v > 0); 
    for (auto it = m_vertices[x]->m_edge_list.begin(); it != m_vertices[x]->m_edge_list.end(); ++it) {
        if (y == (*it)->m_adjacent_vertex) {
            (*it)->m_weight = v;
            break;
        }
    }
}

void Graph::remove_edge(uint x, uint y) {
    uint sz = vertices_number();
    assert (x < sz && y < sz); 
    for (auto it = m_vertices[x]->m_edge_list.begin(); it != m_vertices[x]->m_edge_list.end(); ) {
        if (y == (*it)->m_adjacent_vertex) {
            it = m_vertices[x]->m_edge_list.erase(it);
        }
        else {
            ++it;
        }
    }
}

} // namespace HEX 


