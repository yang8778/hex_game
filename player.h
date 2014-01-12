#ifndef PLAYER
#define PLAYER

#include "board.h"
#include "graph.h"

namespace HEX {

class Player : public Graph {
public:
    Player(NodeColor color, Board& board, uint trial_number = 10);

    ~Player() {}

    // human player's move operation via assigned position
    int move(uint x);

    // computer player's move operation via reflect 
    int move();

private:
    // reflect base on the current board, and select the best position using Monte Carlo simulation
    uint reflect();

    // no copy allowed
    Player(const Player&);
    Player& operator=(const Player&);

private:
    NodeColor m_color;
    NodeColor m_opponent_color;
    Board& m_board;
    uint m_board_size;
    uint m_trial_number;
};

} // namespace HEX 

#endif // PLAYER  
