#include "Board.hpp"
#include "MainState.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Board::Board()
: landed_rows(ROWS, std::vector<SDL_Rect>(COLS)) {}

bool Board::init(Engine *gam, Graphics *graphs, MainState* mainstate)
{
    game = gam;
    graphics = graphs;
    state = mainstate;
    
    srand(static_cast<int>(time(NULL)));
    
    current.reset(random_tetromino());
    current.reset(random_tetromino());
    current->move(MIDDLE_X, Y_OFFSET - GRIDSIZE*3);
    next.reset(random_tetromino());
    
    colours.push_back(RGB{101, 101, 101}); // Grey
    colours.push_back(RGB{255, 130, 163}); // Yellow
    colours.push_back(RGB{251, 188, 5}); // Pink
    //colours.push_back(RGB{243, 156, 18}); // Orange
    //colours.push_back(RGB{52, 168, 83}); // Green
    //colours.push_back(RGB{234, 67, 53}); // Red
    //colours.push_back(RGB{44, 133, 244}); // Blue
    return soundboard.init();
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
            if (rotation_allowed()) {
                current->rotate();
                lockDelay = defaultLockDelay;
            }
        }
        else if (event.key.keysym.sym == SDLK_DOWN) {
            dropDelay = quickDrop;
        }
        else if (event.key.keysym.sym == SDLK_SPACE) {
            hard_drop();
        }
    }
    else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN)
        dropDelay = defaultDrop;
}

bool Board::allowed_left()
{
    // No collision with wall
    for (auto block : current->current_pos()) {
        if (block.x == X_OFFSET + GRID_WIDTH) {
            return false;
        }
    }
    // No collision with other terominos
    for (auto block : current->current_pos()) {
        for (int i = landed_rows.size()-1; i >= 0; --i) {
            for (int j = 0; j < landed_rows[i].size(); ++j) {
                if (landed_rows[i][j].y == block.y + block.h + GRID_WIDTH && landed_rows[i][j].x == block.x-GRIDSIZE)
                    return false;
            }
        }
    }
    return true;
}

bool Board::allowed_right()
{
    // No collision with wall
    for (auto block : current->current_pos()) {
        if (block.x == X_OFFSET + GRIDSIZE*COLS - block.w) {
            return false;
        }
    }
    // No collision with other terominos
    for (auto block : current->current_pos()) {
        for (int i = landed_rows.size()-1; i >= 0; --i) {
            for (int j = 0; j < landed_rows[i].size(); ++j) {
                if (landed_rows[i][j].y == block.y + block.h + GRID_WIDTH && landed_rows[i][j].x == block.x+GRIDSIZE)
                    return false;
            }
        }
    }
    return true;
}

bool Board::rotation_allowed()
{
    // No collision with wall
    for (auto block : current->next_pos()) {
        if (block.x == X_OFFSET + GRIDSIZE*COLS - block.w || block.x == X_OFFSET + GRID_WIDTH) {
            return false;
        }
    }
    // No collision with other terominos
    for (auto block : current->next_pos()) {
        for (int i = landed_rows.size()-1; i >= 0; --i) {
            for (int j = 0; j < landed_rows[i].size(); ++j) {
                if (landed_rows[i][j].y == block.y + block.h + GRID_WIDTH && landed_rows[i][j].x == block.x)
                    return false;
            }
        }
    }
    return true;
}

void Board::hard_drop()
{
    while (check_collision() == false)
        current->drop();
    
    add_to_landed(current->current_pos());
    load_next_block();
    
    hardDrop = true;
}

void Board::update(const float& deltatime)
{
    // Drop blocks
    if (!hardDrop) {
        if (lockDelay == defaultLockDelay) { // Block isn't mid-lock
            currDelay += deltatime;
            if (currDelay >= dropDelay) {
                currDelay = 0.0f;
                current->drop();
            }
        }
        
        // Fix tetrominos to other blocks/ground
        if (check_collision() == true) {
            lockDelay -= deltatime;
            if (lockDelay <= 0) {
                add_to_landed(current->current_pos());
                load_next_block();
                lockDelay = defaultLockDelay;
            }
        }
    }
    else hardDrop = false;
    
    if (!state->game_over()) {
        // Clear all completed rows
        int completed = completed_row();
        if (completed >= 0) {
            int rowsCleared = 0;
            while (completed >= 0) {
                clear_row(completed);
                completed = completed_row();
                ++rowsCleared;
            }
            update_score(rowsCleared);
        }
    }
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
                if (landed_rows[i][j].y == block.y + block.h + GRID_WIDTH && landed_rows[i][j].x == block.x) {
                    return !game_is_over();
                    //return true;
                }
            }
        }
    }
    return false;
}

bool Board::game_is_over()
{
    // Overflowing top of screen
    for (auto block : current->current_pos()) {
        if (block.y <= Y_OFFSET - block.h) {
            state->set_game_over();
            return true;
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
    ++clearedLines;
}

void Board::update_score(const int& rowsCleared)
{
    int score = 0;
    if (rowsCleared == 1) score = score1 * state->level();
    if (rowsCleared == 2) score = score2 * state->level();
    if (rowsCleared == 3) score = score3 * state->level();
    if (rowsCleared == 4) score = score4 * state->level();
    state->add_score(score);
    
    if (clearedLines % 10 == 0) {
        state->incr_level();
        dropDelay -= 0.05;
    }
}

void Board::load_next_block()
{
    current.reset();
    current = std::move(next);
    
    // Move to top of screen
    current->move(MIDDLE_X, Y_OFFSET - GRIDSIZE*3);

    next.reset(random_tetromino());
}

Tetromino* Board::random_tetromino()
{
    int nextblock = (rand() % 7);
    
    switch(nextblock) {
        case 0: return new I_block(COLS*GRIDSIZE + X_OFFSET + 10, Y_OFFSET);
            break;
        case 1: return new O_block(COLS*GRIDSIZE + X_OFFSET + 10, Y_OFFSET);
            break;
        case 2: return new S_block(COLS*GRIDSIZE + X_OFFSET + 10, Y_OFFSET);
            break;
        case 3: return new Z_block(COLS*GRIDSIZE + X_OFFSET + 10, Y_OFFSET);
            break;
        case 4: return new T_block(COLS*GRIDSIZE + X_OFFSET + 10, Y_OFFSET);
            break;
        case 5: return new J_block(COLS*GRIDSIZE + X_OFFSET + 10, Y_OFFSET);
            break;
        case 6: return new L_block(COLS*GRIDSIZE + X_OFFSET + 10, Y_OFFSET);
            break;
        default: return new O_block(COLS*GRIDSIZE + X_OFFSET + 10, Y_OFFSET);
            break;
    }
}

void Board::draw()
{
    draw_current();
    draw_grid();
    draw_landed();
    draw_next();
}

void Board::draw_current()
{
    SDL_SetRenderDrawColor(graphics->renderer(), current->r(), current->g(), current->b(), 1);
    for (auto block : current->current_pos()) {
        SDL_RenderFillRect(graphics->renderer(), &block);
    }
}

void Board::draw_next()
{
    SDL_SetRenderDrawColor(graphics->renderer(), next->r(), next->g(), next->b(), 1);
    for (auto block : next->current_pos()){
        SDL_RenderFillRect(graphics->renderer(), &block);
    }
}

void Board::draw_shadow()
{
    
}

void Board::draw_landed()
{
    //SDL_SetRenderDrawColor(graphics->renderer(), current->r(), current->g(), current->b(), 1);
    for (int i = landed_rows.size()-1; i >= 0; --i){
        SDL_SetRenderDrawColor(graphics->renderer(), colours[i%colours.size()].r, colours[i%colours.size()].g, colours[i%colours.size()].b, 1);
        for (int j = 0; j < landed_rows[i].size(); ++j) {
            SDL_RenderFillRect(graphics->renderer(), &landed_rows[i][j]);
        }
    }
}


void Board::draw_grid()
{
    
    SDL_SetRenderDrawColor(graphics->renderer(), 255, 255, 255, 1);
    SDL_Rect topbanner {0, 0, graphics::WINDOW_WIDTH, Y_OFFSET};
    SDL_RenderFillRect(graphics->renderer(), &topbanner);
    
    SDL_Rect bottombanner {0, Y_OFFSET + ROWS*GRIDSIZE, graphics::WINDOW_WIDTH, Y_OFFSET};
    SDL_RenderFillRect(graphics->renderer(), &bottombanner);
    
    SDL_Rect leftbanner {0, 0, X_OFFSET, Y_OFFSET + ROWS*GRIDSIZE};
    SDL_RenderFillRect(graphics->renderer(), &leftbanner);
    
    SDL_Rect rightbanner {X_OFFSET + COLS*GRIDSIZE, 0, graphics::WINDOW_WIDTH - X_OFFSET+COLS*GRIDSIZE, graphics::WINDOW_HEIGHT};
    SDL_RenderFillRect(graphics->renderer(), &rightbanner);
    
    
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
