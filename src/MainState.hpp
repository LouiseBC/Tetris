#ifndef MainState_hpp
#define MainState_hpp
#include "GameState.hpp"
#include "Board.hpp"
#include "HUD.hpp"

class MainState : public GameState {
public:
    bool init(Engine* game, Graphics* graphics);
    ~MainState();
    
    void handle_input(SDL_Event& event);
    void update(const float& deltatime);
    void render();
    
    int score() const { return playerScore; }
    void add_score(const int& score) { playerScore += score; }
    
    int level() const { return currLevel; }
    void incr_level() { ++currLevel; }
    
    bool game_over() { return gameOver; }
    void set_game_over() { gameOver = true; }
    
private:
    Engine* game;
    Graphics* graphics;
    
    Board board;
    HUD hud;
    
    bool start = false;
    bool pause = false;
    bool gameOver = false;
    int playerScore = 0;
    int currLevel = 1;
};

#endif /* MainState_hpp */
