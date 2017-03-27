#include <iostream>
#include "Engine.hpp"
#include "GameState.hpp"

bool Engine::init(std::unique_ptr<GameState> gamestate) {
    if (graphics.init()) {
        if (!gamestate->init(this, &graphics)) {
            quitGame = true;
            std::cerr << "Error initialising gamestate\n";
            return false;
        }
        state = std::move(gamestate);
        return true;
    }
    std::cerr << "Error initialising graphics\n";
    return false;
}

void Engine::start()
{
    while(quitGame == false)
    {
        update();
        render();
    }
}

void Engine::update()
{
    if (state != nullptr) {
        while(SDL_PollEvent(&event))
            state->handle_input(event);
    }
    
    if (state != nullptr) {
        currentTime = SDL_GetTicks();
        float deltatime = static_cast<float>((currentTime - oldTime) / 1000.0f );
        state->update(deltatime);
        oldTime = currentTime;
    }
}

void Engine::render()
{
    if (state != nullptr)
        state->render();
}

void Engine::changeState(std::unique_ptr<GameState> newstate)
{
    state.reset();
    
    if (!newstate->init(this, &graphics)) {
        quitGame = true;
        std::cerr << "Error changing gamestate\n";
        return;
    }
    state = std::move(newstate);
}

void Engine::setQuit()
{
     quitGame = true;
}
    