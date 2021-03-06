#include "HUD.hpp"
#include "Graphics.hpp"

void HUD::init(Graphics *graphs)
{
    graphics = graphs;
    
    scoreText = graphics->renderText("Score: ", fontPath, fontColour, 15);
    int W; int H;
    SDL_QueryTexture(scoreText, NULL, NULL, &W, &H);
    scoreTextPos = SDL_Rect{ BOARD_PADDING, BOARD_PADDING-H-3, W, H };
    render_score(0);
    
    levelText = graphics->renderText("Level: ", fontPath, fontColour, 15);
    SDL_QueryTexture(levelText, NULL, NULL, &W, &H);
    levelTextPos = SDL_Rect{ BOARD_PADDING + BOARD_WIDTH - W-12, BOARD_PADDING-H-3, W, H };
    render_level(0);
    
    rowsText = graphics->renderText("Rows cleared: ", fontPath, fontColour, 15);
    SDL_QueryTexture(rowsText, NULL, NULL, &W, &H);
    rowsTextPos = SDL_Rect{ BOARD_PADDING, BOARD_PADDING + BOARD_HEIGHT + H/2, W, H};
    render_rows(0);

    gameOver = graphics->renderText("Game Over!", fontPath, fontColour, 25);
    SDL_QueryTexture(gameOver, NULL, NULL, &W, &H);
    gameOverPos = SDL_Rect{ BOARD_PADDING+(BOARD_WIDTH/2 - W/2), BOARD_PADDING*5, W, H };
}

void HUD::render_score(const int& currScore)
{
    std::string scorestring = std::to_string(currScore);
    score = graphics->renderText(scorestring, fontPath, fontColour, 15);
    int W; int H;
    SDL_QueryTexture(score, NULL, NULL, &W, &H);
    scorePos = SDL_Rect{ BOARD_PADDING+scoreTextPos.w, scoreTextPos.y, W, H };
}

void HUD::render_level(const int& currLevel)
{
    std::string levelstring = std::to_string(currLevel);
    level = graphics->renderText(levelstring, fontPath, fontColour, 15);
    int W; int H;
    SDL_QueryTexture(level, NULL, NULL, &W, &H);
    levelPos = SDL_Rect{ levelTextPos.x+levelTextPos.w + 5, BOARD_PADDING-H-3, W, H };
}

void HUD::render_rows(const int &nrows) {
    std::string rowscount = std::to_string(nrows);
    nRows = graphics->renderText(rowscount, fontPath, fontColour, 15);
    int W; int H;
    SDL_QueryTexture(nRows, NULL, NULL, &W, &H);
    nRowsPos = SDL_Rect{ rowsTextPos.x+rowsTextPos.w + 5, BOARD_PADDING + BOARD_HEIGHT + H/2, W, H};
}

void HUD::draw_gameover()
{
    SDL_RenderCopy(graphics->renderer(), gameOver, NULL, &gameOverPos);
}

void HUD::draw()
{
    SDL_RenderCopy(graphics->renderer(), scoreText, NULL, &scoreTextPos);
    SDL_RenderCopy(graphics->renderer(), score, NULL, &scorePos);
    
    SDL_RenderCopy(graphics->renderer(), levelText, NULL, &levelTextPos);
    SDL_RenderCopy(graphics->renderer(), level, NULL, &levelPos);
    
    SDL_RenderCopy(graphics->renderer(), rowsText, NULL, &rowsTextPos);
    SDL_RenderCopy(graphics->renderer(), nRows, NULL, &nRowsPos);
}