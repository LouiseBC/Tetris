#include "Graphics.hpp"
#include <iostream>
#include <SDL2_TTF/SDL_TTF.h>
//#include <SDL2_mixer/SDL_mixer.h>


bool Graphics::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Error: init" << SDL_GetError() << std::endl;
        return false;
    }
    
    if (TTF_Init() < 0) {
        std::cerr << "Error: TTF init" << TTF_GetError << std::endl;
        return false;
    }
    
    win = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              graphics::WINDOW_WIDTH, graphics::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        std::cerr << "Error: CreateWindow" << SDL_GetError() << std::endl;
        return false;
    }
    
    
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        std::cerr << "Error: CreateRenderer" << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

Graphics::~Graphics() {
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}

SDL_Texture* Graphics::renderText(const std::string &message, const std::string &filepath, SDL_Color colour, int fontsize) {
    TTF_Font *font = TTF_OpenFont(filepath.c_str(), fontsize);
    if (font == nullptr){
        std::cerr << "Error: TTF_OpenFont" << TTF_GetError;
        return nullptr;
    }
    SDL_Surface *surf = TTF_RenderText_Solid(font, message.c_str(), colour);
    if (surf == nullptr){
        TTF_CloseFont(font);
        std::cout << "Error: TTF_RenderText" << TTF_GetError;
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    if (texture == nullptr)
        std::cout << "Error: CreateTexture" << SDL_GetError();
    
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}