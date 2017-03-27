#ifndef Game_hpp
#define Game_hpp
#include <vector>
#include <memory>
#include "Graphics.hpp"

class GameState;

class Engine {
public:
    bool init(std::unique_ptr<GameState> gamestate);
    void start();
    
    void changeState(std::unique_ptr<GameState> state);
    
    void update();
    void render();
    
    void setQuit();
    
private:
    Graphics graphics;
    std::unique_ptr<GameState> state;
    std::vector<std::unique_ptr<GameState>> states;
    
    bool quitGame { false };
    SDL_Event event;
    
    Uint32 oldTime = 0;
    Uint32 currentTime = 0;
};

#endif
