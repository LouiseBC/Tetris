#ifndef Board_hpp
#define Board_hpp
#include <memory>
#include <map>
#include "Shapes.hpp"
#include "RGB.hpp"
#include "SoundBoard.hpp"

class Engine;
class Graphics;
class MainState;

class Board {
private:
    // Collision
    bool check_collision();
    void add_to_landed(const Tetromino::Rotation& r);
    bool game_is_over();
    
    // Progression
    int completed_row();
    void clear_row(const int& rownum);
    void update_score(const int& rowsCleared);
    void load_next_block();
    Tetromino* random_tetromino();
    
    // Movement
    void hard_drop();
    bool allowed_left();
    bool allowed_right();
    bool rotation_allowed();
    
    // Rendering
    void draw_current();
    void draw_shadow();
    void draw_next();
    void draw_landed();
    void draw_grid();
    
    // Access
    Engine* game;
    Graphics* graphics;
    MainState* state;
    SoundBoard soundboard;
    
    // Data
    int score1 { 40 };
    int score2 { 100 };
    int score3 { 300 };
    int score4 { 1200 };
    int clearedLines { 0 };
    
    bool hardDrop      { false };
    float currDelay    { 0.0 };
    float dropDelay    { 0.4 };
    float defaultDrop  { 0.4 };
    float quickDrop    { 0.05 };
    
    float lockDelay { 0.4 };
    float defaultLockDelay { 0.4 };
    
    const int GRID_WIDTH { 1 };
    const int GRIDSIZE   { 25 };
    const int MIDDLE_X { 125 };
    
    const int X_OFFSET { 50 };
    const int Y_OFFSET { 50 };
    const int ROWS { 20 };
    const int COLS { 10 };
    
    std::unique_ptr<Tetromino> current;
    std::unique_ptr<Tetromino> shadow;
    std::unique_ptr<Tetromino> next;
    std::vector<std::vector<SDL_Rect>> landed_rows;
    std::vector<RGB> colours;
    
public:
    Board();
    bool init(Engine* game, Graphics* graphics, MainState* state);
    
    void handle_input(SDL_Event& event);
    void update(const float& deltatime);
    void draw();
};

#endif /* Board_hpp */
