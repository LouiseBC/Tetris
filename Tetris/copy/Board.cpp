#include "Board.hpp"
#include "MainState.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Board::Board()
: landed_rows(ROWS, std::vector<SDL_Rect>(COLS)) {}

bool Board::init(Engine *gam, Graphics *graphs, std::shared_ptr<Player> plyr)
{
    game = gam;
    graphics = graphs;
    player = plyr;
    
    srand(static_cast<int>(time(NULL)));
    
    int random = (rand() % 7);
    current.reset(new_tetromino(random));
    current->move(MIDDLE_X, Y_OFFSET - GRIDSIZE); // start first tetromino visible
    
    // copy of current
    if (drawShadow)
        shadow.reset(new_tetromino(random));
    
    random = (rand() % 7);
    next.reset(new_tetromino(random));
    
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
            if (allowed_left(current->current_pos())) {
                current->left();
                lockDelay = defaultLockDelay;
            }
        }
        else if (event.key.keysym.sym == SDLK_RIGHT) {
            if (allowed_right(current->current_pos())) {
                current->right();
                lockDelay = defaultLockDelay;
            }
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

bool Board::allowed_left(const Tetromino::Rotation& rotation)
{
    // No collision with wall
    for (auto block : rotation) {
        if (block.x == X_OFFSET + BORDER_WIDTH) {
            return false;
        }
    }
    // No collision with other terominos
    for (auto block : rotation) {
        for (int i = landed_rows.size()-1; i >= 0; --i) {
            for (int j = 0; j < landed_rows[i].size(); ++j) {
                if (landed_rows[i][j].y == block.y + block.h + BORDER_WIDTH && landed_rows[i][j].x == block.x-GRIDSIZE)
                    return false;
            }
        }
    }
    return true;
}

bool Board::allowed_right(const Tetromino::Rotation& rotation)
{
    // No collision with wall
    for (auto block : rotation) {
        if (block.x == X_OFFSET + GRIDSIZE*COLS - block.w) {
            return false;
        }
    }
    // No collision with other terominos
    for (auto block : rotation) {
        for (int i = landed_rows.size()-1; i >= 0; --i) {
            for (int j = 0; j < landed_rows[i].size(); ++j) {
                if (landed_rows[i][j].y == block.y + block.h + BORDER_WIDTH && landed_rows[i][j].x == block.x+GRIDSIZE)
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
        if (block.x == X_OFFSET + BORDER_WIDTH || block.x == X_OFFSET + GRIDSIZE*COLS - block.w) {
            return false;
        }
    }
    // No collision with other terominos
    for (auto block : current->next_pos()) {
        for (int i = landed_rows.size()-1; i >= 0; --i) {
            for (int j = 0; j < landed_rows[i].size(); ++j) {
                if (landed_rows[i][j].y == block.y + block.h + BORDER_WIDTH && landed_rows[i][j].x == block.x)
                    return false;
            }
        }
    }
    return true;
}

void Board::hard_drop()
{
    while (check_collision(current->current_pos()) == false)
        current->drop();
    
    if (!game_is_over()) {
        add_to_landed(current->current_pos());
        load_next_block();
    }
    hardDrop = true;
}

void Board::update(const float& deltatime)
{
    if (drawShadow)
        cast_shadow();
    
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
        if (check_collision(current->current_pos()) == true) {
            if (!game_is_over()) {
                lockDelay -= deltatime;
                if (lockDelay <= 0) {
                    add_to_landed(current->current_pos());
                    load_next_block();
                    lockDelay = defaultLockDelay;
                }
            }
        }
    }
    else hardDrop = false;
    
    // Clear all completed rows
    if (!player->game_over()) {
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

void Board::cast_shadow()
{
    shadow->move(current->x(), current->y());
    shadow->set_rotation(current->rotation());
    
    while (check_collision(shadow->current_pos()) == false) {
        shadow->drop();
    }
}

bool Board::check_collision(const Tetromino::Rotation& rotation)
{
    for (auto block : rotation) {
        // Reached bottom of screen
        if (block.y == Y_OFFSET + GRIDSIZE*ROWS - block.h)
            return true;
        
        // Collision with other block
        for (int i = landed_rows.size()-1; i >= 0; --i) {
            for (int j = 0; j < landed_rows[i].size(); ++j) {
                if (landed_rows[i][j].y == block.y + block.h + BORDER_WIDTH && landed_rows[i][j].x == block.x) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::game_is_over()
// True when reached top of screen && check_collision()
{
    for (auto block : current->current_pos()) {
        if (block.y < Y_OFFSET + BORDER_WIDTH) {
            player->set_game_over();
            return true;
        }
    }
    return false;
}

void Board::add_to_landed(const Tetromino::Rotation& rotation)
{
    for (auto block : rotation) {
        int rownum = (block.y - Y_OFFSET - BORDER_WIDTH) / GRIDSIZE;
        int colnum = (block.x - X_OFFSET - BORDER_WIDTH) / GRIDSIZE;
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
        
        for (int j = 0; j < landed_rows[i].size(); ++j) {
            if (landed_rows[i][j].w > 0)
                landed_rows[i][j].y += GRIDSIZE;
        }
    }
    landed_rows[0] = std::vector<SDL_Rect>(COLS);
    ++clearedLines;
}

void Board::update_score(const int& rowsCleared)
{
    int score = 0;
    if (rowsCleared == 1) score = score1 * (player->level() + 1);
    if (rowsCleared == 2) score = score2 * (player->level() + 1);
    if (rowsCleared == 3) score = score3 * (player->level() + 1);
    if (rowsCleared == 4) score = score4 * (player->level() + 1);
    player->add_score(score);
    
    if (clearedLines % 10 == 0) {
        player->incr_level();
        dropDelay -= 0.05;
    }
}

void Board::load_next_block()
{
    current.reset();
    current = std::move(next);
    current->move(MIDDLE_X, Y_OFFSET - GRIDSIZE*3); // Move to top of screen
    
    shadow.reset(new_tetromino(current->type()));
    
    next.reset(new_tetromino((rand() % 7)));
}

Tetromino* Board::new_tetromino(const int& randomInt)
{
    switch(randomInt) {
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
    if (drawShadow)
        draw_shadow();
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

void Board::draw_shadow()
{
    SDL_SetRenderDrawBlendMode(graphics->renderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(graphics->renderer(), shadow->r(), shadow->g(), shadow->b(), 100);
    for (auto block : shadow->current_pos()) {
        SDL_RenderFillRect(graphics->renderer(), &block);
    }
    SDL_SetRenderDrawBlendMode(graphics->renderer(), SDL_BLENDMODE_NONE);
}

void Board::draw_next()
{
    SDL_SetRenderDrawColor(graphics->renderer(), next->r(), next->g(), next->b(), 1);
    for (auto block : next->current_pos()){
        SDL_RenderFillRect(graphics->renderer(), &block);
    }
}

void Board::draw_landed()
{
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
