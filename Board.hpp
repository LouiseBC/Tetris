#ifndef Board_hpp
#define Board_hpp
#include <memory>
#include "Shapes.hpp"
#include "RGB.hpp"

class Engine;
class Graphics;
class Player;

class Board {
private:
    // Collision
    bool check_collision(const Tetromino::Rotation& r);
    void add_to_landed(const Tetromino::Rotation& r);
    bool game_is_over();
    
    // Progression
    int completed_row();
    void clear_row(const int& rownum);
    void update_score(const int& rowsCleared);
    void load_next_block();
    Tetromino* new_tetromino(const int& randomInt);
    std::vector<int> fill_bag();
    
    // Movement
    void hard_drop();
    bool allowed_left(const Tetromino::Rotation& rotation);
    bool allowed_right(const Tetromino::Rotation& rotation);
    bool rotation_allowed();
    void cast_shadow();
    
    // Rendering
    void draw_current();
    void draw_shadow();
    void draw_next();
    void draw_landed();
    void draw_border();
    
    // Access
    std::vector<Tetromino::Rotation> board_state() { return landed_rows; }
    Engine* game;
    Graphics* graphics;
    std::shared_ptr<Player> player;
    
    // Data
    int clearedLines { 0 };
    int score1 { 40 };
    int score2 { 100 };
    int score3 { 300 };
    int score4 { 1200 };
    
    bool drawShadow    { false };
    bool hardDrop      { false };
    float currDelay    { 0.0 };
    float dropDelay    { 0.4 };
    float defaultDrop  { 0.4 };
    float quickDrop    { 0.03 };
    
    float lockTimer { 0.4 };
    float defaultLockDelay { 0.4 };
    
    const int BORDER_WIDTH { 1 };
    const int GRIDSIZE   { 25 };
    const int MIDDLE_X { 125 };
    
    const int X_OFFSET { 50 };
    const int Y_OFFSET { 50 };
    const int ROWS { 20 };
    const int COLS { 10 };
    
    std::unique_ptr<Tetromino> current;
    std::unique_ptr<Tetromino> shadow;
    std::unique_ptr<Tetromino> next;
    std::vector<Tetromino::Rotation> landed_rows;
    std::vector<RGB> colours;
    
    std::vector<int> tetrominoBag;
    
public:
    friend class AI;
    
    Board();
    bool init(Engine* game, Graphics* graphics, std::shared_ptr<Player> state);
    
    int  cleared_rows() { return clearedLines; }
    void handle_input(SDL_Event& event);
    void update(const float& deltatime);
    void draw();
};

#endif /* Board_hpp */
