#include "MenuState.hpp"
#include "MainState.hpp"
#include "Engine.hpp"

bool MenuState::init(Engine* gam, Graphics* graphs)
{
    game = gam;
    graphics = graphs;

    set_text("Press Enter to Start");

    return true;
}

void MenuState::handle_input(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
        game->setQuit();
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game->setQuit();
        if (event.key.keysym.sym == SDLK_RETURN)
            game->changeState(std::unique_ptr<GameState>(new MainState));
    }

}

void MenuState::update(const float& dt)
{

}

void MenuState::set_text(const std::string msg)
{
    text = graphics->renderText(msg, fontPath, fontColour, 25);
    int W; int H;
    SDL_QueryTexture(text, NULL, NULL, &W, &H);
    textPos = SDL_Rect{ static_cast<int>(graphics::WINDOW_WIDTH*0.5 - W*0.5),
                        static_cast<int>(graphics::WINDOW_HEIGHT*0.5 - H*0.5),
                        W,
                        H };
}

void MenuState::render()
{
    graphics->clear();
    
    SDL_RenderCopy(graphics->renderer(), text, NULL, &textPos);

    graphics->present();
}

