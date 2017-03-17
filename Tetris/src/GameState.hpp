#ifndef GameState_hpp
#define GameState_hpp
#include <SDL2/SDL.h>
#include <memory>

class Engine;
class Graphics;

class GameState {
public:
    virtual bool init(Engine* game, Graphics* graphics) = 0;
    virtual ~GameState() {}
    
    virtual void handle_input(SDL_Event& event) = 0;
    virtual void update(const float& deltatime) = 0;
    virtual void render() = 0;
};

#endif /* GameState_hpp */
