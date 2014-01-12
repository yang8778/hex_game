#include "board.h"

namespace HEX {

Board::Board() {    
    reset_board();
}

Board::Board(const Board& rhs_board)
: m_node_status_map(rhs_board.m_node_status_map)
{
}

std::vector<uint> Board::get_surround_nodes(uint row, uint col) {
    std::vector<uint> surround_nodes;
    // four corners
    if (row == 0 && col == 0) {
        surround_nodes.push_back(loc2number(row, col + 1));
        surround_nodes.push_back(loc2number(row + 1, col));
    }
    else if (row == 0 && col == N - 1) {
        surround_nodes.push_back(loc2number(row, col - 1));
        surround_nodes.push_back(loc2number(row + 1, col));
        surround_nodes.push_back(loc2number(row + 1, col - 1));
    } 
    else if (row == N - 1 && col == 0) {
        surround_nodes.push_back(loc2number(row - 1, col));
        surround_nodes.push_back(loc2number(row, col + 1));
        surround_nodes.push_back(loc2number(row - 1, col + 1));
    } 
    else if (row == N - 1 && col == N - 1) {
        surround_nodes.push_back(loc2number(row, col - 1));
        surround_nodes.push_back(loc2number(row - 1, col));
    }
    // other edge nodes
    else if (row == 0 && col > 0 && col < N - 1) {
        surround_nodes.push_back(loc2number(row, col + 1));
        surround_nodes.push_back(loc2number(row, col - 1));
        surround_nodes.push_back(loc2number(row + 1, col - 1));
        surround_nodes.push_back(loc2number(row + 1, col));
    }
    else if (row == N - 1 && col > 0 && col < N - 1) {
        surround_nodes.push_back(loc2number(row, col + 1));
        surround_nodes.push_back(loc2number(row, col - 1));
        surround_nodes.push_back(loc2number(row - 1, col));
        surround_nodes.push_back(loc2number(row - 1, col + 1));
    }
    else if (row > 0 && row < N - 1 && col == 0) {
        surround_nodes.push_back(loc2number(row - 1, col));
        surround_nodes.push_back(loc2number(row + 1, col));
        surround_nodes.push_back(loc2number(row - 1, col + 1));
        surround_nodes.push_back(loc2number(row, col + 1));
    }
    else if (row > 0 && row < N - 1 && col == N - 1) {
        surround_nodes.push_back(loc2number(row - 1, col));
        surround_nodes.push_back(loc2number(row + 1, col));
        surround_nodes.push_back(loc2number(row + 1, col - 1));
        surround_nodes.push_back(loc2number(row, col - 1));
    }
    // other inner nodes
    else {
        surround_nodes.push_back(loc2number(row, col + 1));
        surround_nodes.push_back(loc2number(row, col - 1));
        surround_nodes.push_back(loc2number(row - 1, col));
        surround_nodes.push_back(loc2number(row - 1, col + 1));
        surround_nodes.push_back(loc2number(row + 1, col - 1));
        surround_nodes.push_back(loc2number(row + 1, col));
    }
    return surround_nodes;
}

std::vector<uint> Board::get_available_nodes() {    
    std::vector<uint> available;
    for (auto it = m_node_status_map.begin(); it != m_node_status_map.end(); ++it) {
        if (it->second == NodeColor::UNOCCUPIED) {
            available.push_back(it->first);
        }
    }
    return available;
}

// make unions in the current level
std::list<std::list<uint> > Board::make_union(NodeColor node_color, uint row, std::unordered_map<uint, int>& curr_union_map) {
    std::list<std::list<uint> > ret;
    int left = 0, right = 0;
    while (right < N) {
        if (get_node_status(loc2number(row, left)) == node_color) {
            std::list<uint> _union(1, left);
            curr_union_map[left] = left;
            right++;
            while (right < N && get_node_status(loc2number(row, right)) == node_color) {
                _union.push_back(right);
                curr_union_map[right] = left;
                right++; 
            }
            left = right;
            ret.push_back(_union);
        }
        else { 
            left++; 
            right++;
        }
    }
    return ret;
}

// for the given union, find its available unions in the previous level
std::unordered_set<int> Board::prev_unions(std::list<uint>& u, std::unordered_map<uint, int>& prev_union_map) {
    std::unordered_set<int> u_prev_unions;
    for (auto idx : u) {
        int t1 = prev_union_map[idx];
        if (t1 >= 0 && u_prev_unions.count(t1) == 0) 
            u_prev_unions.insert(t1);
        if (idx + 1 < N) {
            int t2 = prev_union_map[idx + 1];
            if (t2 >= 0 && u_prev_unions.count(t2) == 0) 
                u_prev_unions.insert(t2);
        } 
    }
    return u_prev_unions;
};

bool Board::merge_union(std::list<uint>& u1, std::list<uint>& u2, std::unordered_map<uint, int>& prev_union_map) {
    std::unordered_set<int> prev_unions_u1 = prev_unions(u1, prev_union_map);
    std::unordered_set<int> prev_unions_u2 = prev_unions(u2, prev_union_map);
    bool ret = false;
    for (auto i : prev_unions_u1) {
        if (prev_unions_u2.count(i) > 0) {
            ret = true;
            break;
        } 
    } 
    // merge two unions if they have a same union in the previous level
    if (ret)
        u1.insert(u1.end(), u2.begin(), u2.end());
    return ret;
};

// optimize the check strategy from calculating shortest path via dijkstra algorithm
// to Union-Find algorithm.We only have to examine a previous row and a current row,
// we match through each row, and if we get to the end, we find a complete path.
bool Board::has_complete_path(NodeColor node_color) {
    std::deque<bool> prev_open(N, false);
    std::deque<bool> curr_open(N, false);
    std::unordered_map<uint, int> prev_union_map; 
    std::unordered_map<uint, int> curr_union_map; 
    for (uint i = 0; i < N; ++i) {
        prev_union_map[i] = -1;
        curr_union_map[i] = -1;
    }
    // path connects North-South
    if (node_color == NodeColor::RED) {
        std::list<std::list<uint> > curr_union = Board::make_union(node_color, 0, curr_union_map);
        for (uint col = 0; col < N; ++col) 
            curr_open[col] = (get_node_status(loc2number(0, col)) == node_color);
        // judge if need continue
        if (std::none_of(curr_open.begin(), curr_open.end(), [](bool i){return i == true;}))
            return false;
        for (uint row = 1; row < N; ++row) {
            prev_open = curr_open;
            curr_open = std::deque<bool>(N, false);    
            prev_union_map = curr_union_map; 
            for (uint i = 0; i < N; ++i) {
                curr_union_map[i] = -1;
            }
            // make current unions
            std::list<std::list<uint> > curr_round = Board::make_union(node_color, row, curr_union_map);
            // merge current unions
            std::list<std::list<uint> > next_round;
            while (curr_round.size() != next_round.size()) {
                if (!next_round.empty()) {
                    curr_round = next_round;
                    next_round.clear();
                }
                std::list<std::list<uint> > curr_round_copy = curr_round;
                while (!curr_round_copy.empty()) {
                    std::list<uint> candidates = curr_round_copy.front();
                    curr_round_copy.pop_front();
                    auto it_union = curr_round_copy.begin(); 
                    while (it_union != curr_round_copy.end()) {
                        bool merged = merge_union(candidates, *it_union, prev_union_map);
                        if (merged) {
                            int merged_flag = curr_union_map[candidates.front()];
                            for (auto i : (*it_union)) 
                                curr_union_map[i] = merged_flag; 
                            it_union = curr_round_copy.erase(it_union);
                        }
                        else 
                            it_union++;
                    }
                    next_round.push_back(candidates);
                }
            }
            for (auto round_candidates : next_round) {
                // set curr_open
                bool round_candidates_passed = false;
                for (auto i : round_candidates) {
                    if (prev_open[i] || ((i + 1) < N && prev_open[i + 1])) {
                        round_candidates_passed = true;
                        break;
                    } 
                }
                if (round_candidates_passed) {
                    for (auto i : round_candidates) 
                        curr_open[i] = true;
                }
            }
            if (std::none_of(curr_open.begin(), curr_open.end(), [](bool i){return i == true;})) 
                return false;
        }
        return true;
    }
}

void Board::draw_board() {
    std::cout << "  ";
    for (uint r = 0; r < N; ++r) {
        std::cout << static_cast<char>(r + 65) << " ";
    }
    std::cout << std::endl;
    for (uint i = 0; i < N; ++i) {
        for (uint ii = 0; ii < i; ++ii)
            std::cout << " ";
        uint row = i + 1;
        if (row < 10)
            std::cout << row << "  ";
        else 
            std::cout << row << " ";
        for (uint j = 0; j < N; ++j) {
            uint index = loc2number(i, j);
            NodeColor node_status = get_node_status(index);
            if (node_status == NodeColor::UNOCCUPIED)
                std::cout << ". ";
            else if(node_status == NodeColor::RED)
                std::cout << "o ";
            else if(node_status == NodeColor::BLUE)
                std::cout << "x ";
        }
        std::cout << row << std::endl;; 
    }
    for (uint r = 0; r < N + 3; ++r)
        std::cout << " ";
    for (uint r = 0; r < N; ++r) {
        std::cout << static_cast<char>(r + 65) << " ";
    }
    std::cout << std::endl;
}

} // namespace HEX 



