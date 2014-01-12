#include <iostream>
#include <string>
#include <ctime> // std::time
#include <cstdlib> // std::rand, std::srand
#include "board.h"
#include "player.h"

using namespace HEX; 

static void Usage() {
    std::cout << "Usage: <command> easy|normal|hard\n";
    std::cout << "easy|normal|hard -- choose the degree of game difficulty\n";
}

int main(int argc, char *argv[]) {
    typedef unsigned int uint;
    uint trial_number;
    if (argc < 2) {
        Usage();
        return 1;
    }
    else {
        std::string strength = argv[1];
        if (strength == "easy") 
            trial_number = 10;
        else if (strength == "normal") 
            trial_number = 100;
        else if (strength == "hard")
            trial_number = 1000;
        else {
            Usage();
            return 1;
        }
    }
    std::srand(std::time(NULL));
    Board board;
    // RED stands for computer
    Player player_red(NodeColor::RED, board, trial_number);
    // BLUE stands for human
    Player player_blue(NodeColor::BLUE, board);
    
    char choice;
    do {
        std::cout << "Do you want to go first (Y or N)?";
        std::cin >> choice;
    } while (choice != 'Y' && choice != 'N');
     
    if (choice == 'Y') {
        std::cout << "****************************************************" << std::endl;
        std::cout << "*    Human *  X  * connects West-East, moves 1st *" << std::endl;
        std::cout << "* Computer *  O  * connects North-South              *" << std::endl;
        std::cout << "****************************************************" << std::endl;
    } 
    else {
        std::cout << "****************************************************" << std::endl;
        std::cout << "* Computer *  O  * connects North-South, moves 1st *" << std::endl;
        std::cout << "*    Human *  X  * connects West-East              *" << std::endl;
        std::cout << "****************************************************" << std::endl;
    }

    int move = 1;
    std::pair<uint, uint> location;
    if (choice == 'N') {
        // computer move
        int idx = player_red.move();
        std::cout << "Move " << move << ": Computer" << std::endl;
        location = board.number2loc(idx);
        std::cout << "Row: " << location.first << " Column: " << location.second << std::endl;
        move++;
    }

    while (true) {
        uint row, col;
        int idx;
        // human move
        std::cout << "Move " << move << ": Human" << std::endl;
        do {
            std::cout << "Row: ";
            std::cin >> row; 
            std::cout << "Column: ";
            std::cin >> col; 
            idx = player_blue.move(board.loc2number(row, col)); 
        } while (idx == -2);
        if (idx == -1)
            break;
        move++;

        // computer move
        idx = player_red.move();
        std::cout << "Move " << move << ": Computer" << std::endl;
        location = board.number2loc(idx);
        std::cout << "Row: " << location.first << " Column: " << location.second << std::endl;
        if (idx == -1)
            break;
        move++;
    }

    return 0;
}
