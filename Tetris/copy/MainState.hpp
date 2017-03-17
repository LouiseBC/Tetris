#ifndef MainState_hpp
#define MainState_hpp
#include "GameState.hpp"
#include "Board.hpp"
#include "HUD.hpp"
#include "Player.hpp"
#include "AI.hpp"

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
    std::shared_ptr<Player> player;
    //AI ai;
    HUD hud;
    
    bool start = true;
    bool pause = true;
};

#endif /* MainState_hpp */
