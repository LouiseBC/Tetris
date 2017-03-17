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
        states.push_back(std::move(gamestate));
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
    quit();
}

void Engine::update()
{
    if(states.size() > 0){
        while(SDL_PollEvent(&event))
            states.back()->handle_input(event);
    }
    
    if (states.size() > 0){
        currentTime = SDL_GetTicks();
        float deltatime = static_cast<float>((currentTime - oldTime) / 1000.0f );
        states.back()->update(deltatime);
        oldTime = currentTime;
    }
}

void Engine::render()
{
    if(states.size() > 0)
        states.back()->render();
}

void Engine::quit() {
    while(states.size() > 0)
    {
        // delete unique_ptr?
        states.pop_back();
    }
}

void Engine::changeState(std::unique_ptr<GameState> state)
{
    if (states.size() > 0 )
        states.pop_back();
    
    if (state->init(this, &graphics)) {
        quitGame = true;
        std::cerr << "Error changing gamestate\n";
        return;
    }
    states.push_back(std::move(state));
}

void Engine::setQuit()
{
     quitGame = true;
}
    