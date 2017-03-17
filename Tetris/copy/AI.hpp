#ifndef AI_hpp
#define AI_hpp
#include <queue>
#include "Player.hpp"
#include "Board.hpp"

class AI : public Player {
public:
    void init(Board* board) override;
    void update() override;
    
    void push_move();
    void choose_move();
    
    void generate_moves();
    std::queue<SDL_Event> create_moves(const int& lefts, const int& rights);
    std::vector<Tetromino::Rotation> simulate_drop_outcome(const int& x, const int& y, const int& rotation);
    std::vector<Tetromino::Rotation> add_to_boardState(const Tetromino::Rotation& rotation);
    
    std::vector<double> assign_scores();
    int highest_row(const std::vector<Tetromino::Rotation>& state);
    int holes(const std::vector<Tetromino::Rotation>& state);
    int wells(const std::vector<Tetromino::Rotation>& state);
    int empty_rows(const std::vector<Tetromino::Rotation>& state);
    int peak_difference();
    int uncompleted_rows(const std::vector<Tetromino::Rotation>& state);
    int column_transitions(const std::vector<Tetromino::Rotation>& state);
    int row_transitions(const std::vector<Tetromino::Rotation>& state);
    
    std::vector<int> make_skyline(const std::vector<Tetromino::Rotation>& state);
private:
    Board* board;
    std::unique_ptr<Tetromino> currentCopy;
    
    std::vector<std::vector<Tetromino::Rotation>> possibleStates;
    std::vector<std::queue<SDL_Event>> possibleMoves;
    std::queue<SDL_Event> moveQueue;
};

#endif /* AI_hpp */
