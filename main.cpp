#include <iostream>
#include "Engine.hpp"
#include "MainState.hpp"

int main() {
    Engine game;
    if (game.init(std::unique_ptr<GameState>(new MainState)))
        game.start();
}
