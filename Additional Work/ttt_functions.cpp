#include <iostream>
#include <vector>

void display_status(std::string grid[]){
    std::cout << "Current Grid Layout" << std::endl;
    for (int i = 0; i < 17; i++) {
        if (grid[i] == "-----") {
            std::cout << std::endl << grid[i] << std::endl;
        } else {
            std::cout << grid[i];
        }
    }
    std::cout << std::endl;

}

bool move(std::string grid[], int xcoord, int ycoord, std::string symbol) {
    int current_index = ((xcoord - 1) * 3 + (ycoord - 1)) * 2;
    if (grid[current_index] != "-") {
        return false;
    } else {
        grid[current_index] = symbol;
        return true;
    }
}

bool check_horizontal(const std::string grid[]) {
    int count = 0;
    std::string prev_symbol;
    for (int row = 1; row < 4; row++) {
        prev_symbol = grid[(row - 1) * 6];
        for (int column = 1; column < 4; column++) {
            if (grid[((row - 1) * 3 + (column - 1)) * 2] == "-") {
                break;
            }
            if (prev_symbol == grid[((row - 1) * 3 + (column - 1)) * 2]) {
                count++;
                prev_symbol = grid[((row - 1) * 3 + (column - 1)) * 2];
            }
            else{
                count = 0;
            }
        }
        if (count == 3) {
            return true;
        } else {
            count = 0;
        }
    }
    return false;
}

bool check_vert(const std::string grid[]) {
    int count = 0;
    std::string prev_symbol;
    for (int column = 1; column < 4; column++) {
        prev_symbol = grid[((column - 1)) * 2];
        for (int row = 1; row < 4; row++) {
            if (grid[((row - 1) * 3 + (column - 1)) * 2] == "-") {
                break;
            }
            if (prev_symbol == grid[((row - 1) * 3 + (column - 1)) * 2]) {
                count++;
                prev_symbol = grid[((row - 1) * 3 + (column - 1)) * 2];
            }
            else{
                count = 0;
            }
        }
        if (count == 3) {
            return true;
        } else {
            count = 0;
        }
    }
    return false;
}

bool check_diagonal(const std::string grid[]) {
    int count = 0;
    std::string prev_symbol = grid[0];
    for (int i = 1; i < 4; i++) {
        if (grid[(i - 1) << 3] == "-") {
                break;
        }
        if (prev_symbol == grid[(i - 1) << 3]) {
            count++;
            prev_symbol = grid[(i - 1) << 3];
        } else {
            count = 0;
            break;
        }
    }

    if (count == 3) {
        return true;
    } else {
        count = 0;
    }

    prev_symbol = grid[4];
    for (int i = 1; i < 4; i++) {
        if (grid[(i - 1) << 3] == "-") {
                break;
        }
        if (prev_symbol == grid[(i - 1) << 2]) {
            count++;
            prev_symbol = grid[(i - 1) << 2];
        } else {
            return false;
        }
    }
    return count == 3;
}

bool is_winner(std::string grid[]) {
    return check_horizontal(grid) || check_vert(grid) || check_diagonal(grid);
}