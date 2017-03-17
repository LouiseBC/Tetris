#include <iostream>
#include "MainState.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"

MainState::MainState(const bool human)
: isHuman{human} {}

bool MainState::init(Engine* gam, Graphics* graphs)
{
    game = gam;
    graphics = graphs;
    
    if (isHuman)
        player.reset(new Player);
    else
        player.reset(new AI);
    player->init(&board);
    
    hud.init(graphs);
    return board.init(gam, graphs, player);
}

void MainState::handle_input(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
        game->setQuit();
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game->setQuit();
        if (event.key.keysym.sym == SDLK_RETURN)
            pause = !pause;
        else
            board.handle_input(event);
    }
    if (event.type == SDL_KEYUP)
        board.handle_input(event);
}

void MainState::update(const float& dt)
{
    if (!pause && !player->game_over()) {
        board.update(dt);
        hud.render_score(player->score());
        hud.render_level(player->level());
        hud.render_rows(board.cleared_rows());
        player->update();
    }
}

void MainState::render()
{
    graphics->clear();
    
    board.draw();
    hud.draw();
    if (player->game_over())
        hud.draw_gameover();
    
    graphics->present();
}
