#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <list>
#include <memory> // std::shared_ptr
#include <tuple> // std::tuple std::get
#include <limits> // std::numeric_limits
#include <assert.h> // assert
#include <algorithm> // std::make_heap
#include <deque> // std::deque
#include "board.h"

namespace HEX {

typedef unsigned int uint;

struct EdgeNode {
    EdgeNode(uint start_vertex, uint adjacent_vertex, float weight)
    : m_start_vertex(start_vertex)
    , m_adjacent_vertex(adjacent_vertex)
    , m_weight(weight)
    {}
    
    friend std::ostream& operator<<(std::ostream& out, const EdgeNode& edge) {
        out << edge.m_start_vertex << "->" << edge.m_adjacent_vertex << " (" << edge.m_weight << ")" << std::endl;
        return out;
    }

    uint m_start_vertex; // the index of start VertexNode
    uint m_adjacent_vertex; // the index of adjacent VertexNode
    float m_weight;
};

struct VertexNode {
    VertexNode()
    : m_index(std::numeric_limits<uint>::max())
    , m_distance(std::numeric_limits<float>::infinity())
    , m_predecessor(std::numeric_limits<uint>::max())
    , m_status(NodeColor::UNOCCUPIED)
    {}
    uint m_index;
    NodeColor m_status;    
    float m_distance; // the distance estimation
    uint m_predecessor; // the index of vertex' predecessor node in the shortest path 
    std::list<std::shared_ptr<EdgeNode> > m_edge_list; 
};

class VertexComparision {
public:
    bool operator()(const std::shared_ptr<VertexNode>& lhs, const std::shared_ptr<VertexNode>& rhs) {
        return (lhs->m_distance > rhs->m_distance); 
    }
};

class Graph {
public:
    typedef std::vector<std::shared_ptr<VertexNode> > VertexList; 
    typedef std::deque<std::shared_ptr<VertexNode> > PriorityDeque;

public:
    // default constructor
    Graph() {}

    virtual ~Graph() {}

    //implement of dijkstra algorithm
    void dijkstra(uint s);

    // returns the number of vertices in the graph
    inline uint vertices_number() const {
        return m_vertices.size();   
    }

    inline VertexList get_vertices() {
        return m_vertices;
    }

    // returns the number of edges in the graph
    uint edges_number();

    // returns the value associated to the edge (x, y)
    float get_edge_value(uint x, uint y);

    // tests whether there is an edge from node x to node y
    bool is_adjacent(uint x, uint y);

    // lists all nodes y such that there is an edge from x to y
    VertexList neighbors(uint x);

    // adds to G the edge from x to y with weight v default 1,if it is not there
    void add_edge(uint x, uint y, float v = 1);

    // sets the value associated to the edge(x, y) to v if there is an edge between x and y
    void set_edge_value(uint x, uint y, float v);  
   
    // removes the edge from x to y,if it is there
    void remove_edge(uint x, uint y);

    // returns the value associated with node x
    inline float get_node_value(uint x) const { 
        assert (x < vertices_number());
        return m_vertices[x]->m_distance;
    } 
    
    // sets the value associated with the node x to a if the node exists
    inline void set_node_value(uint x, float a) {
        if (x < m_vertices.size()) {
            m_vertices[x]->m_distance = a;
        }      
    }
    
    // gets the node's predecessor's index if it exists in shortest path
    inline uint get_node_predecessor(uint x) {
        return m_vertices[x]->m_predecessor; 
    }

    // sets the node's predecessor's index if the node exists
    inline void set_node_predecessor(uint x, uint predecessor) {
        if (x < m_vertices.size()) {
            m_vertices[x]->m_predecessor = predecessor;
        }      
    }

    inline NodeColor get_node_status(uint x) {
        assert (x >= 0 && x < m_vertices.size());
        return m_vertices[x]-> m_status;
    }

    inline void set_node_color(uint x, NodeColor color) {
        assert (x >= 0 && x < m_vertices.size());
        assert (color == NodeColor::RED || color == NodeColor::BLUE);
        m_vertices[x]-> m_status = color;
    } 

protected:
    VertexList m_vertices;
    PriorityDeque m_priority_deque;

private:
    // no copy allowed
    Graph(const Graph&);
    Graph& operator=(const Graph&);
};

} // namespace HEX 

#endif // GRAPH_H


