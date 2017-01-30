#include "Board.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Board::Board()
: landed_rows(ROWS, std::vector<SDL_Rect>(COLS)) {}

bool Board::init(Engine *gam, Graphics *graphs)
{
    game = gam;
    graphics = graphs;
    
    srand(static_cast<int>(time(0)));
    
    current.reset(random_tetromino());//new I_block(STARTING_X, Y_OFFSET - GRIDSIZE));
    next.reset(random_tetromino());//new S_block(STARTING_X, Y_OFFSET - GRIDSIZE));
    
    return true;
}

void Board::handle_input(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) {
            if (allowed_left())
                current->left();
        }
        else if (event.key.keysym.sym == SDLK_RIGHT) {
            if (allowed_right())
                current->right();
        }
        else if (event.key.keysym.sym == SDLK_UP) {
            current->rotate();
        }
        else if (event.key.keysym.sym == SDLK_DOWN) {
            dropDelay = 0.05;
        }
        else if (event.key.keysym.sym == SDLK_SPACE) {
            hard_drop();
        }
    }
    else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN)
        dropDelay = 0.4;
}

bool Board::allowed_left()
{
    for (auto block : current->current_pos()) {
        if (block.x == X_OFFSET + GRID_WIDTH) {
            return false;
        }
    }
    return true;
}

bool Board::allowed_right()
{
    for (auto block : current->current_pos()) {
        if (block.x == X_OFFSET + GRIDSIZE*COLS - block.w) {
            return false;
        }
    }
    return true;
}

void Board::hard_drop()
{
    while (check_collision() == false)
        current->drop();
}

void Board::update(const float& deltatime)
{
    // Drop blocks
    currDelay += deltatime;
    if (currDelay >= dropDelay) {
        currDelay = 0.0f;
        current->drop();
    }
    
    // Fix tetrominos to other blocks/ground
    if (check_collision() == true) {
        add_to_landed(current->current_pos());
        load_next_block();
    }
    
    // Clear all completed rows
    int completed = completed_row();
    if (completed >= 0) do {
        clear_row(completed);
        completed = completed_row();
    } while (completed >= 0);
}

bool Board::check_collision()
{
    for (auto block : current->current_pos()) {
        // Reached bottom of screen
        if (block.y == Y_OFFSET + GRIDSIZE*ROWS - block.h)
            return true;
        
        // Collision with other block (starting with last added)
        for (int i = landed_rows.size()-1; i >= 0; --i) {
            for (int j = 0; j < landed_rows[i].size(); ++j) {
                if (landed_rows[i][j].y == block.y + block.h + GRID_WIDTH && landed_rows[i][j].x == block.x)
                    return true;
            }
        }
    }
    return false;
}

void Board::add_to_landed(const Tetromino::Rotation& rotation)
{
    for (auto block : rotation) {
        int rownum = (block.y - Y_OFFSET - GRID_WIDTH) / GRIDSIZE;
        int colnum = (block.x - X_OFFSET - GRID_WIDTH) / GRIDSIZE;
        landed_rows[rownum][colnum] = ( SDL_Rect{block.x, block.y, block.w, block.h} );
    }
}

int Board::completed_row()
{
    for (int i = landed_rows.size()-1; i >= 0; --i) {
        bool completed = true;
        
        for (auto col : landed_rows[i]) {
            if (col.w == 0) {
                completed = false;
                break;
            }
        }
        if (completed)
            return i;
    }
    return -1;
}

void Board::clear_row(const int& rownum)
{
    for (int i = rownum; i >= 1; --i) {
        landed_rows[i] = landed_rows[i-1];
        
        for (int j = 0; j < landed_rows[i].size(); ++j)
            landed_rows[i][j].y += GRIDSIZE;
    }
    landed_rows[0] = std::vector<SDL_Rect>(COLS);
}

void Board::load_next_block()
{
    current.reset();
    current = std::move(next);
    next.reset(random_tetromino());
}

Tetromino* Board::random_tetromino()
{
    int nextblock = (rand() % 7);
    
    switch(nextblock) {
        case 0: return new I_block(STARTING_X, Y_OFFSET - GRIDSIZE);
            break;
        case 1: return new O_block(STARTING_X, Y_OFFSET - GRIDSIZE);
            break;
        case 2: return new S_block(STARTING_X, Y_OFFSET - GRIDSIZE);
            break;
        case 3: return new Z_block(STARTING_X, Y_OFFSET - GRIDSIZE);
            break;
        case 4: return new T_block(STARTING_X, Y_OFFSET - GRIDSIZE);
            break;
        case 5: return new J_block(STARTING_X, Y_OFFSET - GRIDSIZE);
            break;
        case 6: return new L_block(STARTING_X, Y_OFFSET - GRIDSIZE);
            break;
        default: return new O_block(STARTING_X, Y_OFFSET - GRIDSIZE);
            break;
    }
}

void Board::draw()
{
    draw_grid();
    draw_current();
    draw_landed();
}

void Board::draw_current()
{
    SDL_SetRenderDrawColor(graphics->renderer(), current->r(), current->g(), current->b(), 1);
    for (auto block : current->current_pos()) {
        SDL_RenderFillRect(graphics->renderer(), &block);
    }
}

void Board::draw_landed()
{
    SDL_SetRenderDrawColor(graphics->renderer(), current->r(), current->g(), current->b(), 1);
    for (auto row : landed_rows){
        for (auto col : row) {
            SDL_RenderFillRect(graphics->renderer(), &col);
        }
    }
}

void Board::draw_grid()
{
    SDL_SetRenderDrawColor(graphics->renderer(), 1, 1, 1, 1);
    SDL_RenderDrawLine(graphics->renderer(), X_OFFSET, Y_OFFSET,
                                             X_OFFSET, Y_OFFSET + ROWS*GRIDSIZE);
    
    SDL_RenderDrawLine(graphics->renderer(), X_OFFSET + COLS*GRIDSIZE, Y_OFFSET,
                                             X_OFFSET + COLS*GRIDSIZE, Y_OFFSET + ROWS * GRIDSIZE);
    
    SDL_RenderDrawLine(graphics->renderer(), X_OFFSET, Y_OFFSET,
                                             X_OFFSET + COLS*GRIDSIZE, Y_OFFSET);
    
    SDL_RenderDrawLine(graphics->renderer(), X_OFFSET, Y_OFFSET + ROWS*GRIDSIZE,
                                             X_OFFSET + COLS*GRIDSIZE, Y_OFFSET + ROWS * GRIDSIZE);
}
