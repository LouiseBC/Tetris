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
    
    /*if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Error: OpenAudio Init" << Mix_GetError() << std::endl;
        return false;
    }*/
    
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