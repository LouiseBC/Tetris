#ifndef Tetromino_hpp
#define Tetromino_hpp
#include "Graphics.hpp"
#include <vector>

class Tetromino {
public:
    using Rotation = std::vector<SDL_Rect>;
    using Block = SDL_Rect;
    
    void drop();
    void hard_drop();
    void rotate();
    void left();
    void right();
    
    const Rotation current_pos() const;
    const Rotation next_pos() const;
    
    int x() { return X; }
    int y() { return Y; }
    
    int r() { return R; }
    int g() { return G; }
    int b() { return B; }
    
protected:
    Tetromino(const int& r, const int& g, const int& b) { R = r; G = g; B = b; }
    
    int X;
    int Y;
    
    int currentRotation = 0;
    std::vector<Rotation> rotations;
    
    // Construction Details
    const int SIZE = 24;
    const int N = 4;
    const int PADDING = 1;
    
    int R;
    int G;
    int B;
};

#endif