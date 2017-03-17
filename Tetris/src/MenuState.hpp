#ifndef MenuState_hpp
#define MenuState_hpp
#include "GameState.hpp"
#include "Graphics.hpp"

class MenuState : public GameState {
public:
    bool init(Engine* game, Graphics* graphics);
    
    void handle_input(SDL_Event& event);
    void update(const float& deltatime);
    void render();
private:
	void set_text(const std::string text);

    Engine* game;
    Graphics* graphics;

    SDL_Texture* text;
    SDL_Rect textPos;

    const std::string fontPath {"assets/GreenFlame.ttf"};
    SDL_Color fontColour {1, 1, 1, 1};
    
};

#endif
