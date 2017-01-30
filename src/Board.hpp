#ifndef Board_hpp
#define Board_hpp
#include <memory>
#include <map>
#include "Shapes.hpp"

class Engine;
class Graphics;

class Board {
private:
    void draw_grid();
    void draw_current();
    void draw_landed();
    
    void hard_drop();
    bool check_collision();
    void add_to_landed(const Tetromino::Rotation& r);
    void load_next_block();
    bool allowed_left();
    bool allowed_right();
    
    int completed_row();
    void clear_row(const int& rownum);
    Tetromino* random_tetromino();
    
    Engine* game;
    Graphics* graphics;
    
    float currDelay { 0.0 };
    float dropDelay { 0.4 };
    
    const int GRID_WIDTH { 1 };
    const int GRIDSIZE   { 25 };
    const int STARTING_X { 150 };
    
    const int X_OFFSET { 50 };
    const int Y_OFFSET { 50 };
    const int ROWS { 20 };
    const int COLS { 10 };
    
    std::unique_ptr<Tetromino> current;
    std::unique_ptr<Tetromino> shadow;
    std::unique_ptr<Tetromino> next;
    std::vector<std::vector<SDL_Rect>> landed_rows;
    
public:
    Board();
    bool init(Engine* game, Graphics* graphics);
    
    void handle_input(SDL_Event& event);
    void update(const float& deltatime);
    void draw();
};

#endif /* Board_hpp */
