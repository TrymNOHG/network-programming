void display_status(std::string grid[]);

inline
void make_move(std::string current_player) {
    std::cout << current_player << " make a move!";
}

inline
void instructions() {
    std::cout << "\nIn order to make a move, enter the coordinate that you want to play your move.\n";
}

bool move(std::string grid[], int xcoord, int ycoord, std::string symbol);

bool is_winner(std::string grid[]);
bool check_horizontal(std::string grid[]);
bool check_vert(std::string grid[]);
bool check_diagonal(std::string grid[]);