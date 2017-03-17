#include "AI.hpp"
#include <iostream>

void AI::init(Board* tetrisboard)
{
    board = tetrisboard;
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
    
    for (int i = 0; i < 4; ++i) {
        currentCopy->move(board->current->x(), board->current->y());
        generate_moves();
        currentCopy->rotate();
    }
    if (possibleMoves.size() != possibleStates.size())
        std::cerr << "Error: Unequal states/moves\n";
    
    std::vector<double> stateScores = assign_scores();
    auto best = std::min_element(stateScores.begin(), stateScores.end());
    int bestindex = best-stateScores.begin();
    std::cerr << "Best: " << bestindex << std::endl << std::endl;
    moveQueue = possibleMoves[bestindex];
}

std::vector<double> AI::assign_scores()
{
    std::vector<double> scores;
    
    for (auto state : possibleStates) {
        double currScore = 0;
        
        currScore += 0.8 * uncompleted_rows(state);
        currScore += 1.2 * holes(state);
        //currScore += 0.01 * wells(state);
        currScore += 0.01 * column_transitions(state);
        currScore += 0.5 * row_transitions(state);
        
        currScore += 0.2 * peak_difference();
        currScore += 0.05 * highest_row(state);
        
        scores.push_back(currScore/6);
        std::cerr << "Total: " << double(currScore/6) << std::endl;
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
    std::cerr << "Holes: " << holes << std::endl;
    return holes;
}

int AI::wells(const std::vector<Tetromino::Rotation>& state)
{
    int wells = 0;
    for (auto row : state) {
        for (int i = 1; i < row.size()-1; ++i) {
            if (row[i-1].w > 0 && row[i].w == 0 && row[i+1].w > 0) ++wells;
        }
    }
    std::cerr << wells << std::endl;
    return wells;
}

int AI::highest_row(const std::vector<Tetromino::Rotation>& state)
{
    std::vector<int> skyline = make_skyline(state);
    auto highest = std::max_element(skyline.begin(), skyline.end());
    std::cerr << "Highest: " << *highest << std::endl;
    return *highest;
}

int AI::column_transitions(const std::vector<Tetromino::Rotation>& state)
{
    std::vector<int> skyline = make_skyline(state);
    
    int transitions = 0.0;
    for (int x = 0; x < board->COLS; ++x) {
        for (int y = state.size() - skyline[x]; y < state.size()-1; ++y) {
            if (state[y][x].w != state[y+1][x].w) ++ transitions;
        }
    }
    std::cerr << "YTransitions: " << transitions << std::endl;
    return transitions;
}

int AI::row_transitions(const std::vector<Tetromino::Rotation>& state)
{
    std::vector<int> skyline = make_skyline(state);
    auto highest = std::max_element(skyline.begin(), skyline.end());
    
    int transitions = 0.0;
    for (int y = state.size() - *highest; y < state.size(); ++y) {
        for (int x = 0; x < board->COLS-1; ++x) {
            if (state[y][x].w != state[y][x+1].w) ++ transitions;
        }
    }
    std::cerr << "XTransitions: " << transitions << std::endl;
    return transitions;
}

int AI::empty_rows(const std::vector<Tetromino::Rotation>& state)
{
    // only care about emptiness in old boardstate: creating new half-empty lines is ok
    std::vector<int> oldskyline = make_skyline(board->board_state());
    auto highest = std::max_element(oldskyline.begin(), oldskyline.end());
    
    double emptiness = 0.0;
    for (int i = state.size() - *highest; i < state.size(); ++i) { // Skyline counts 1 upwards
        for (auto cols : state[i])
            if (cols.w == 0) ++emptiness;
    }
    emptiness = emptiness / (*highest * board->COLS);
    std::cerr << "Emptiness: " << emptiness << std::endl;
    return emptiness;
}

int AI::peak_difference()
// Highest peak
{
    std::vector<int> oldskyline = make_skyline(board->board_state());
    int max = *std::max_element(oldskyline.begin(), oldskyline.end());
    int min = *std::min_element(oldskyline.begin(), oldskyline.end());
    int difference = max-min;
    std::cerr << "Alt diff: " << difference << std::endl;
    return difference;
}

int AI::uncompleted_rows(const std::vector<Tetromino::Rotation>& state)
// 1 - (percentage completed rows)
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
    //nCompleted = 1 - (nCompleted / static_cast<double>(*highestRow));
    std::cerr << "%Completed: " << nCompleted << std::endl;
    return nCompleted;
}

std::vector<int> AI::make_skyline(const std::vector<Tetromino::Rotation>& state)
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

void AI::generate_moves()
{
    // Move left as far as possible.
    int nLefts = 0;
    while(board->allowed_left(currentCopy->current_pos())){
        currentCopy->left();
        ++nLefts;
        
        std::vector<Tetromino::Rotation> outcome = simulate_drop_outcome(currentCopy->x(), currentCopy->y(), currentCopy->rotation());
        if (!outcome.empty()) {
            possibleStates.push_back(outcome);
            possibleMoves.push_back(create_moves(nLefts, 0));
        }
    }

    // Reset position to include middle and move right as far as possible.
    currentCopy->move(board->current->x(), board->current->y());
    if(board->allowed_left(currentCopy->current_pos()))
       currentCopy->left();
    int nRights = -1;
    
    while(board->allowed_right(currentCopy->current_pos())) {
        currentCopy->right();
        ++nRights;
        
        std::vector<Tetromino::Rotation> outcome = simulate_drop_outcome(currentCopy->x(), currentCopy->y(), currentCopy->rotation());
        if (!outcome.empty()) {
            possibleStates.push_back(outcome);
            possibleMoves.push_back(create_moves(0, nRights));
        }
    }
}

std::vector<Tetromino::Rotation> AI::simulate_drop_outcome(const int& x, const int& y, const int& rotation)
{
    std::unique_ptr<Tetromino> temp(board->new_tetromino(board->current->type()));
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