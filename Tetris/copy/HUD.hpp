#ifndef HUD_hpp
#define HUD_hpp
#include <SDL2/SDL.h>
#include <string>

class Graphics;

class HUD {
public:
    void init(Graphics* graphics);
    void render_score(const int& score);
    void render_level(const int& level);
    void draw();
    void draw_gameover();
    
private:
    const int BOARD_PADDING = 50;
    const int BOARD_WIDTH = 250;
    std::string fontPath {"assets/GreenFlame.ttf"};
    SDL_Color fontColour {1, 1, 1, 1};
    
    SDL_Texture* scoreText;
    SDL_Texture* score;
    SDL_Rect scoreTextPos;
    SDL_Rect scorePos;
    
    SDL_Texture* levelText;
    SDL_Texture* level;
    SDL_Rect levelTextPos;
    SDL_Rect levelPos;
    
    SDL_Texture* gameOver;
    SDL_Rect gameOverPos;
    
    Graphics* graphics;
};

#endif /* HUD_hpp */
