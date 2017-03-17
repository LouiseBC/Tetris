#ifndef Player_hpp
#define Player_hpp
class Board;

class Player {
public:
    virtual void init(Board* board) {}
    virtual void update() {};
    
    int score() { return currScore; }
    int level() { return currLevel; }
    
    void add_score(const int& score) { currScore += score; }
    void incr_level() { ++currLevel; }
    
    bool game_over() { return gameOver; }
    void set_game_over() { gameOver = true; }
    
protected:
    bool gameOver { false };
    
    int currScore { 0 };
    int currLevel { 0 };
};
#endif /* Player_hpp */
