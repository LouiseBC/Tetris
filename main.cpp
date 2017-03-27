#include <iostream>
#include "Engine.hpp"
#include "MenuState.hpp"

int main() {
    Engine game;
    if (game.init(std::unique_ptr<GameState>(new MenuState)))
        game.start();
}
