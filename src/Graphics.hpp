#ifndef Graphics_hpp
#define Graphics_hpp
#include <SDL2/SDL.h>
#include <string>

class Graphics {
public:
    bool init();
    ~Graphics();
    
    void clear()    { SDL_SetRenderDrawColor(ren, 255, 255, 255, 1); SDL_RenderClear(ren); }
    void present()  { SDL_RenderPresent(ren); }
    
    SDL_Texture* renderText(const std::string &message, const std::string &filepath, SDL_Color colour, int fontsize);
    
    SDL_Renderer* renderer(){ return ren; }
    SDL_Window* window()    { return win; }
    
private:
    SDL_Renderer* ren;
    SDL_Window* win;
};

namespace graphics {
    const int WINDOW_WIDTH = 425;
    const int WINDOW_HEIGHT = 600;
}

#endif /* Graphics_hpp */
