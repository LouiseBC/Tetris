#include "Tetromino.hpp"

const Tetromino::Rotation Tetromino::current_pos() const
{
    Rotation curr;
    for (auto block : rotations[currentRotation])
        curr.push_back(SDL_Rect{block.x + X,
                                block.y + Y,
                                block.w,
                                block.h});
    return curr;
}

const Tetromino::Rotation Tetromino::next_pos() const
{
    Rotation next;
    for (auto block : rotations[ (currentRotation + 1) % 4 ])
        next.push_back(SDL_Rect{block.x + X,
            block.y + Y,
            block.w,
            block.h});
    return next;
}


void Tetromino::drop()
{
    Y += SIZE + PADDING;
}

void Tetromino::hard_drop()
{
    
}

void Tetromino::left()
{
    X -= SIZE + PADDING;
}

void Tetromino::right()
{
    X += SIZE + PADDING;
}

void Tetromino::rotate()
{
    currentRotation = (currentRotation + 1) % N;
}