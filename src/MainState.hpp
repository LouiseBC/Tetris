#ifndef MainState_hpp
#define MainState_hpp
#include "GameState.hpp"
#include "Board.hpp"

class MainState : public GameState {
public:
    bool init(Engine* game, Graphics* graphics);
    ~MainState();
    
    void handle_input(SDL_Event& event);
    void update(const float& deltatime);
    void render();
private:
    Engine* game;
    Graphics* graphics;
    
    Board board;
    
    bool start = false;
};

#endif /* MainState_hpp */
