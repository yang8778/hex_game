#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <assert.h>
#include <unordered_map> // std::unordered_map
#include <deque>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <vector>

namespace HEX {

typedef unsigned int uint;
enum class NodeColor { UNOCCUPIED, RED, BLUE };

class Board {
public:
    typedef std::unordered_map<uint, NodeColor> NodeStatusMap;

public:
    const static uint N = 11; 

    Board();

    Board(const Board& rhs_board);

    ~Board() {}

    // return surround nodes' index for a specific node in HEX board
    std::vector<uint> get_surround_nodes(uint row, uint col); 

    // return current unoccupied nodes in the board
    std::vector<uint> get_available_nodes();

    // check whether there is a full path between two opposite sides
    bool has_complete_path(NodeColor node_color);

    void draw_board();

    // recover board to the original status
    inline void reset_board() {
        uint node_size = N * N;
        for (uint i = 0; i < node_size; ++i) 
            m_node_status_map[i] = NodeColor::UNOCCUPIED;
    }    

    // returns board node's status, eg NodeColor::UNOCCUPIED
    inline NodeColor get_node_status(uint x) {
        assert (m_node_status_map.count(x) == 1);
        return m_node_status_map[x];
    }

    // set board node's color
    inline void set_node_color(uint x, NodeColor color) {
        assert (m_node_status_map.count(x) == 1);
        assert (color == NodeColor::RED || color == NodeColor::BLUE);
        m_node_status_map[x] = color;
    }  

    inline std::pair<uint, uint> number2loc(uint x) {
        assert (x >= 0 && x < N * N);
        uint row, col;
        row = static_cast<uint>(x/N); 
        col = x%N;
        return std::make_pair(row, col);
    }

    inline uint loc2number(uint row, uint col) {
        assert(row >= 0 && row < N);
        assert(col >= 0 && col < N); 
        return row * N + col;
    }

private:
    std::list<std::list<uint> > make_union(NodeColor node_color, uint row, std::unordered_map<uint, int>& curr_union_map); 

    std::unordered_set<int> prev_unions(std::list<uint>& u, std::unordered_map<uint, int>& prev_union_map); 
    
    bool merge_union(std::list<uint>& u1, std::list<uint>& u2, std::unordered_map<uint, int>& prev_union_map); 

private:
    NodeStatusMap m_node_status_map;
};

} // namespace HEX 

#endif // BOARD_H 
