#include <iostream>
#include <vector>
#include <stdlib.h>

#include "ttt_functions.hpp"

// In order to compile this program, type g++ -std=c++11 ttt.cpp ttt_functions.cpp
// It needs to be compiled in a special c++ version...

int main() {

    std::string grid[] = {"-", "|", "-", "|", "-", "-----", "-", "|", "-", "|", "-", "-----", "-", "|", "-", "|", "-"};

    int moves_left = 9;
    std::string player1;
    std::string symbol1;
    std::string player2;
    std::string symbol2;


    std::cout << "Player 1, enter your name :\n";
    std::cin >> player1;
    std::cout << "\n" << player1 << ", enter your symbol :\n";
    std::cin >> symbol1;

    std::cout << "\nPlayer 2, enter your name :\n";
    std::cin >> player2;
    std::cout << "\n" << player2 << ", enter your symbol :\n";
    std::cin >> symbol2;

    std::vector<std::string> players = {player1, player2};
    std::vector<std::string> symbols = {symbol1, symbol2};

    srand (time(NULL));
    int start_player = rand() % 2;

    std::cout << player2[start_player] << " starts the game!";
    int row;
    int column;
    instructions();
    bool valid_move;
    bool winner = false;
    while (moves_left != 0 && !winner) {

        while(!valid_move) {
            display_status(grid);
            make_move(players[start_player]);

            std::cout << "\nWhich row?\n";
            std::cin >> row;

            std::cout << "\nWhich column?\n";
            std::cin >> column;

            valid_move = move(grid, row, column, symbols[start_player]);

            if (!valid_move) {
                std::cout << "Remember to choose a place that is not already chosen!\n";
                std::cout << "The square also needs to be within (1, 1) and (3, 3)!\n";
            }
        }


        winner = is_winner(grid);

        moves_left--;
        start_player = (start_player + 1) % 2;
        valid_move = false;
    }

    display_status(grid);
    if (winner) {
        std::cout << players[(start_player - 1) % 2] << " won!!!";
    } else {
        std::cout << "Tie!!! Everyone is a winner!";
    }



}