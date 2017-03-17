#include "MenuState.hpp"
#include "MainState.hpp"
#include "Engine.hpp"

bool MenuState::init(Engine* gam, Graphics* graphs)
{
    game = gam;
    graphics = graphs;

    set_text("Press Enter to Start");
    set_playopt_text("1P", "AI");
    selection = SDL_Rect{ humanPos.x - 3, humanPos.y - 3, humanPos.w + 6, humanPos.h + 3 };

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
            game->changeState(std::unique_ptr<GameState>(new MainState(gameType)));
        if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT)
            change_gametype();
    }

}

void MenuState::change_gametype()
{
    int xDiff = aiPos.x - humanPos.x;
    int wDiff = aiPos.w - humanPos.w;
    // Current setting: Human
    if (gameType == true) {
        selection.x += xDiff;
        selection.w += wDiff;
        gameType = false;
    }
    // Current setting: AI
    else {
        selection.x -= xDiff;
        selection.w -= wDiff;
        gameType = true;
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
                        static_cast<int>(graphics::WINDOW_HEIGHT*0.35 - H*0.5),
                        W,
                        H };
}

void MenuState::set_playopt_text(const std::string msg, const std::string msg2)
{
    playHuman = graphics->renderText(msg, fontPath, fontColour, 25);
    playAI = graphics->renderText(msg2, fontPath, fontColour, 25);
    
    int W; int H;
    SDL_QueryTexture(playHuman, NULL, NULL, &W, &H);
    humanPos = SDL_Rect{ 160, 250, W, H };
    
    SDL_QueryTexture(playAI, NULL, NULL, &W, &H);
    aiPos = SDL_Rect{ humanPos.x+humanPos.w + 30, humanPos.y, W, H };
}

void MenuState::render()
{
    graphics->clear();
    
    SDL_SetRenderDrawColor(graphics->renderer(), 251, 188, 5, 255);
    SDL_RenderFillRect(graphics->renderer(), &selection);
    
    SDL_RenderCopy(graphics->renderer(), text, NULL, &textPos);
    SDL_RenderCopy(graphics->renderer(), playHuman, NULL, &humanPos);
    SDL_RenderCopy(graphics->renderer(), playAI, NULL, &aiPos);
    
    graphics->present();
}

