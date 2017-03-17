#include "AI.hpp"
#include <iostream>
#include <numeric>

void AI::init(Board* tetrisboard)
{
    board = tetrisboard;
    isComputer = true;
}

void AI::update()
{
    if (moveQueue.size() > 0)
        push_move();
    else
        choose_move();
}

void AI::choose_move()
{
    possibleStates.clear();
    possibleMoves.clear();
    currentCopy.reset(board->new_tetromino(board->current->type()));
    nextCopy.reset(board->new_tetromino(board->next->type()));
    
    for (int i = 0; i < 4; ++i) {
        currentCopy->move(board->current->x(), board->current->y());
        generate_moves(currentCopy);
        currentCopy->rotate();
    }
    if (possibleMoves.size() != possibleStates.size())
        std::cerr << "Error: Unequal states/moves\n";
    
    std::vector<double> stateScores = assign_scores();
    auto best = std::min_element(stateScores.begin(), stateScores.end());
    int bestindex = best-stateScores.begin();
    moveQueue = possibleMoves[bestindex];
}

std::vector<double> AI::assign_scores()
{
    std::vector<double> scores;
    
    for (auto state : possibleStates) {
        double currScore = 0;
        
        currScore += 0.80 * uncompleted_rows(state);
        currScore += 1.20 * holes(state);
        currScore += 0.20 * aggregate_height(state);
        currScore += 0.50 * row_transitions(state);
        currScore += 0.01 * column_transitions(state);
        
        scores.push_back(currScore/6);
    }
    
    return scores;
}

int AI::holes(const std::vector<Tetromino::Rotation>& state)
// Any space beneath the 'skyline' is a hole
{
    std::vector<int> skyline = make_skyline(state);
    
    int holes = 0;
    for (int x = 0; x < skyline.size(); ++x) {
        for (int y = state.size()-1-skyline[x]+1; y < state.size(); ++y) { // Skyline counts from 1 upwards
            if (skyline[x] > 0) {
                if (state[y][x].w == 0)
                    ++holes;
            }
        }
    }
    return holes;
}

int AI::aggregate_height(const std::vector<Tetromino::Rotation>& state)
// Sum height of each column
{
    std::vector<int>skyline = make_skyline(state);
    return std::accumulate(skyline.begin(), skyline.end(), 0);
}

int AI::column_transitions(const std::vector<Tetromino::Rotation>& state)
// No. changes from full to empty and vice-versa
{
    std::vector<int> skyline = make_skyline(state);
    
    int transitions = 0.0;
    for (int x = 0; x < board->COLS; ++x) {
        for (int y = state.size() - skyline[x]; y < state.size()-1; ++y) {
            if (state[y][x].w != state[y+1][x].w) ++ transitions;
        }
    }
    return transitions;
}

int AI::row_transitions(const std::vector<Tetromino::Rotation>& state)
// No. changes from full to empty and vice-versa
{
    std::vector<int> skyline = make_skyline(state);
    auto highest = std::max_element(skyline.begin(), skyline.end());
    
    int transitions = 0.0;
    for (int y = state.size() - *highest; y < state.size(); ++y) {
        for (int x = 0; x < board->COLS-1; ++x) {
            if (state[y][x].w != state[y][x+1].w) ++ transitions;
        }
    }
    return transitions;
}

int AI::uncompleted_rows(const std::vector<Tetromino::Rotation>& state)
// No. of uncompleted rows that have >= 1 block
{
    const std::vector<int> oldskyline = make_skyline(board->board_state());
    auto highestRow = std::max_element(oldskyline.begin(), oldskyline.end());
    
    int nCompleted = 0;
    for (int i = state.size() - *highestRow; i < state.size(); ++i) { // Skyline counts 1 upwards
        bool completed = true;
        
        for (auto cols : state[i]) {
            if (cols.w == 0) {
                completed = false;
                break;
            }
        }
        if (completed) ++nCompleted;
    }
    nCompleted = *highestRow - nCompleted;
    return nCompleted;
}

std::vector<int> AI::make_skyline(const std::vector<Tetromino::Rotation>& state)
// Determines the highest row in each column
{
    int lowestY = board->Y_OFFSET + board->BORDER_WIDTH + (board->ROWS*board->GRIDSIZE);
    std::vector<int> skyline(board->COLS, 0);
    
    for (int i = state.size()-1; i >=0; --i) { // Rows
        for (int j = 0; j < state[i].size(); ++j) { // Cols
            if (state[i][j].y > 0 && (lowestY - state[i][j].y)/board->GRIDSIZE > skyline[j]) {
                skyline[j] = (lowestY - state[i][j].y)/board->GRIDSIZE;
            }
        }
    }
    return skyline;
}

void AI::generate_moves(std::unique_ptr<Tetromino>& tetromino)
{
    // Move left as far as possible.
    int nLefts = 0;
    while(board->allowed_left(tetromino->current_pos())){
        tetromino->left();
        ++nLefts;
        
        std::vector<Tetromino::Rotation> outcome = simulate_drop_outcome(tetromino->x(), tetromino->y(),
                                                                         tetromino->rotation(), tetromino->type());
        if (!outcome.empty()) {
            possibleStates.push_back(outcome);
            possibleMoves.push_back(create_moves(nLefts, 0));
        }
    }

    // Reset position to include middle and move right as far as possible.
    tetromino->move(board->current->x(), board->current->y());
    if(board->allowed_left(tetromino->current_pos()))
       tetromino->left();
    int nRights = -1;
    
    while(board->allowed_right(tetromino->current_pos())) {
        tetromino->right();
        ++nRights;
        
        std::vector<Tetromino::Rotation> outcome = simulate_drop_outcome(tetromino->x(), tetromino->y(),
                                                                         tetromino->rotation(), tetromino->type());
        if (!outcome.empty()) {
            possibleStates.push_back(outcome);
            possibleMoves.push_back(create_moves(0, nRights));
        }
    }
}

std::vector<Tetromino::Rotation> AI::simulate_drop_outcome(const int& x, const int& y, const int& rotation, const int& type)
{
    std::unique_ptr<Tetromino> temp(board->new_tetromino(type));
    temp->move(x, y);
    temp->set_rotation(rotation);
    
    // Simulate hard drop
    while (board->check_collision(temp->current_pos()) == false)
        temp->drop();
    
    if (temp->y() >= board->Y_OFFSET + board->BORDER_WIDTH)
        return add_to_boardState(temp->current_pos());
    
    else return std::vector<Tetromino::Rotation>();
}

std::vector<Tetromino::Rotation> AI::add_to_boardState(const Tetromino::Rotation& rotation) {
    
    std::vector<Tetromino::Rotation> possibleState = board->board_state();
    for (auto block : rotation) {
        int rownum = (block.y - board->Y_OFFSET - board->BORDER_WIDTH) / board->GRIDSIZE;
        int colnum = (block.x - board->X_OFFSET - board->BORDER_WIDTH) / board->GRIDSIZE;
        possibleState[rownum][colnum] = ( SDL_Rect{block.x, block.y, block.w, block.h} );
        
    }
    return possibleState;
}

std::queue<SDL_Event> AI::create_moves(const int& lefts, const int& rights)
{
    std::queue<SDL_Event> queue;
    
    for (int i = 0; i < currentCopy->rotation(); ++i) {
        SDL_Event event;
        event.key.type = SDL_KEYDOWN;
        event.key.keysym.sym = SDLK_UP;
        queue.push(event);
        event.key.type = SDL_KEYUP;
        event.key.keysym.sym = SDLK_UP;
        queue.push(event);
    }
    
    for (int i = 0; i < lefts; ++i) {
        SDL_Event event;
        event.key.type = SDL_KEYDOWN;
        event.key.keysym.sym = SDLK_LEFT;
        queue.push(event);
        event.key.type = SDL_KEYUP;
        event.key.keysym.sym = SDLK_LEFT;
        queue.push(event);
    }
    
    for (int i = 0; i < rights; ++i) {
        SDL_Event event;
        event.key.type = SDL_KEYDOWN;
        event.key.keysym.sym = SDLK_RIGHT;
        queue.push(event);
        event.key.type = SDL_KEYUP;
        event.key.keysym.sym = SDLK_RIGHT;
        queue.push(event);
    }
    
    SDL_Event event;
    event.key.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_SPACE;
    queue.push(event);
    event.key.type = SDL_KEYUP;
    event.key.keysym.sym = SDLK_SPACE;
    queue.push(event);
    
    return queue;
}

void AI::push_move()
{
    // 1 move == key down && key up
    SDL_PushEvent(&moveQueue.front());
    moveQueue.pop();
    
    SDL_PushEvent(&moveQueue.front());
    moveQueue.pop();
}