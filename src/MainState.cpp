#include "MainState.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"

bool MainState::init(Engine* gam, Graphics* graphs)
{
    game = gam;
    graphics = graphs;
    return board.init(gam, graphs);
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
        if (event.key.keysym.sym == SDLK_SPACE && start == false)
            start = true;
        else
            board.handle_input(event);
    }
    if (event.type == SDL_KEYUP)
        board.handle_input(event);
}

void MainState::update(const float& dt)
{
    if (start)
        board.update(dt);
}

void MainState::render()
{
    graphics->clear();
    
    board.draw();
    
    graphics->present();
}

