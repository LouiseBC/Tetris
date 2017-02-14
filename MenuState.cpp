#include "MenuState.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"

bool MainState::init(Engine* gam, Graphics* graphs)
{
    game = gam;
    graphics = graphs;

    return true;
}

MainState::~MainState()
{
    
}

void MainState::handle_input(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
        game->setQuit();
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game->setQuit();
    }

}

void MainState::update(const float& dt)
{

}

void MainState::render()
{
    graphics->clear();
    
    graphics->present();
}

