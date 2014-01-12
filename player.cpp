#include <iostream>
#include <assert.h>
#include <algorithm>
#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock
#include <limits> // std::numeric_limits
#include "player.h"

namespace HEX {

Player::Player(NodeColor color, Board& board, uint trial_number) 
: m_color(color)
, m_board(board)
, m_board_size(Board::N * Board::N)
, m_trial_number(trial_number)
{
    // define the player and its opponent's color
    assert (m_color == NodeColor::RED || m_color == NodeColor::BLUE);
    if (m_color == NodeColor::RED)
        m_opponent_color = NodeColor::BLUE;
    else if (m_color == NodeColor::BLUE)
        m_opponent_color = NodeColor::RED;
    // add two safe guard vertices for calculating shortest path e.g between West-East
    uint safe_guard_1 = m_board_size;
    uint safe_guard_2 = m_board_size + 1;
    uint node_size = m_board_size + 2; 
    for (uint i = 0; i < node_size; ++i) {
        std::shared_ptr<VertexNode> sp = std::make_shared<VertexNode>();
        sp->m_index = i;
        if (i == safe_guard_1 || i == safe_guard_2)
            sp->m_status = m_color;
        m_vertices.push_back(sp);
    }
}

// actual move, upgrade the graph and judge whether the player is win
int Player::move(uint x) {
    // check board node's status
    NodeColor node_status = m_board.get_node_status(x);
    if (node_status == NodeColor::UNOCCUPIED) {
        // the graph is upgraded by changing the node's status and edges with its surrounding nodes
        set_node_color(x, m_color);
        m_board.set_node_color(x, m_color);
        uint safe_guard_1 = m_board_size;
        uint safe_guard_2 = m_board_size + 1;
        std::pair<uint, uint> location = m_board.number2loc(x);
        uint row = location.first;
        uint col = location.second;
        std::vector<uint> surround_nodes = m_board.get_surround_nodes(row, col);
        if ((m_color == NodeColor::BLUE && col == 0) || (m_color == NodeColor::RED && row == 0)) {
            add_edge(x, safe_guard_1); 
            add_edge(safe_guard_1, x); 
        }
        if ((m_color == NodeColor::BLUE && col == Board::N - 1) || (m_color == NodeColor::RED && row == Board::N - 1)) {
            add_edge(x, safe_guard_2); 
            add_edge(safe_guard_2, x); 
        }
        for (uint surround_node_index : surround_nodes) {
            NodeColor surround_node_status = m_board.get_node_status(surround_node_index);
            if (surround_node_status == NodeColor::UNOCCUPIED) {
                add_edge(x, surround_node_index); 
                add_edge(surround_node_index, x); 
            }
        }
        // redraw the board
        m_board.draw_board();
        // re-calculate path size between two safeguards via dijkstra algorithm
        dijkstra(safe_guard_1);
        // determine when the game is over and announce the winner
        bool win = (get_node_value(safe_guard_2) < std::numeric_limits<float>::infinity());
        if (win) {
            if (m_color == NodeColor::RED)
                std::cout << "Red won!" << std::endl;
            else if (m_color == NodeColor::BLUE)
                std::cout << "Blue won!" << std::endl;
            return -1; 
        }
        else 
            return x; 
    }
    else {
        std::cout << "Ilegal!" << std::endl;
        return -2; 
    }
}

int Player::move() {   
    return move(reflect()); 
}

// the computer player's thinking process and choose the best position using Monte Carlo method 
uint Player::reflect() {    
    #ifndef NDEBUG
    std::chrono::steady_clock::time_point clock_begin = std::chrono::steady_clock::now();    
    #endif
    std::vector<uint> available = m_board.get_available_nodes();
    std::pair<uint, uint> max_score = std::make_pair(std::numeric_limits<uint>::max(), 0); // (index, score) 
    Board virtual_board;        
    for (uint available_index : available) {
        // reset virtual board         
        virtual_board = m_board; 
        virtual_board.set_node_color(available_index, m_color);        
        std::vector<uint> rest(available.size());
        auto rest_it = std::copy_if(available.begin(), available.end(), rest.begin(), [available_index](uint x){ return (x != available_index); });
        rest.resize(std::distance(rest.begin(), rest_it)); // shrink container to new size
        uint win_count = 0;
        for (uint trial_index = 0; trial_index < m_trial_number; ++trial_index) {
            uint rest_size = rest.size();
            if (rest_size > 0) {
                std::random_shuffle(rest.begin(), rest.end());        
                for (uint i = 0; i < rest_size; ++i) {
                    if (i % 2 == 0) 
                        virtual_board.set_node_color(rest[i], m_opponent_color);
                    else 
                        virtual_board.set_node_color(rest[i], m_color);
                }
            }
            // judge whether the player is win only once at the end of randomly generating the virtual board
            // by checking whether there is a complete path between two opposite sides
            if (virtual_board.has_complete_path(m_color)) 
                win_count++;
        }
        // choose the best position
        if (win_count > max_score.second) {
            max_score.first = available_index;
            max_score.second = win_count;
        }
    }
    #ifndef NDEBUG
    std::chrono::steady_clock::time_point clock_end = std::chrono::steady_clock::now();    
    std::chrono::steady_clock::duration time_span = clock_end - clock_begin;
    double nseconds = double(time_span.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den; 
    std::cout << "Reflect took me " << nseconds << " seconds for " << m_trial_number << " trials." << std::endl;
    #endif
    return max_score.first;
}

} // namespace HEX 
