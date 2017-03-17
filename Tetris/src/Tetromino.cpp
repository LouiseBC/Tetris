#include "Tetromino.hpp"

Tetromino::Rotation Tetromino::current_pos() const
{
    Rotation curr;
    for (auto block : rotations[currentRotation])
        curr.push_back(SDL_Rect{block.x + X,
                                block.y + Y,
                                block.w,
                                block.h});
    return curr;
}

Tetromino::Rotation Tetromino::shadow_pos() const
{
    Rotation shadow;
    for (auto block : rotations[currentRotation])
        shadow.push_back(SDL_Rect{block.x + X,
                                  block.y + shadowY,
                                  block.w,
                                  block.h});
    return shadow;
}

Tetromino::Rotation Tetromino::next_pos() const
{
    Rotation next;
    for (auto block : rotations[ (currentRotation + 1) % 4 ])
        next.push_back(SDL_Rect{block.x + X,
            block.y + Y,
            block.w,
            block.h});
    return next;
}

void Tetromino::move(const int& x, const int& y)
{
    X = x; Y = y;
}

void Tetromino::drop()
{
    Y += SIZE + PADDING;
}

void Tetromino::drop_shadow()
{
    shadowY += SIZE + PADDING;
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