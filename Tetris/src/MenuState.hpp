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
    void change_gametype();
	void set_text(const std::string text);
    void set_playopt_text(const std::string text, const std::string text2);

    Engine* game;
    Graphics* graphics;

    // MenuText Details
    SDL_Texture* text;
    SDL_Rect textPos;
    SDL_Texture* playHuman;
    SDL_Rect humanPos;
    SDL_Texture* playAI;
    SDL_Rect aiPos;
    
    // Playtype Selection
    SDL_Rect selection;
    bool gameType { true };

    const std::string fontPath {"assets/GreenFlame.ttf"};
    SDL_Color fontColour {1, 1, 1, 1};
    
};

#endif
