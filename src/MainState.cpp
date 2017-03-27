#include "MainState.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"
#include <iostream>

bool MainState::init(Engine* gam, Graphics* graphs)
{
    game = gam;
    graphics = graphs;
    hud.init(graphs);
    return board.init(gam, graphs, this);
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
        if (event.key.keysym.sym == SDLK_RETURN && start == false)
            start = true;
        if (event.key.keysym.sym == SDLK_p)
            pause = !pause;
        else
            board.handle_input(event);
    }
    if (event.type == SDL_KEYUP)
        board.handle_input(event);
}

void MainState::update(const float& dt)
{
    if (start && !pause && !gameOver) {
        board.update(dt);
        hud.render_score(playerScore);
        hud.render_level(currLevel);
    }
}

void MainState::render()
{
    graphics->clear();
    
    board.draw();
    hud.draw();
    if (gameOver)
        hud.draw_gameover();
    
    graphics->present();
}

