#ifndef Tetromino_hpp
#define Tetromino_hpp
#include "Graphics.hpp"
#include <vector>

class Tetromino {
public:
    using Rotation = std::vector<SDL_Rect>;
    using Block = SDL_Rect;
    enum type { I = 0, O, S, Z, T, J, L };
    
    void drop();
    void drop_shadow();
    void rotate();
    void left();
    void right();
    void move(const int& x, const int& y);
    
    Rotation current_pos() const;
    Rotation shadow_pos() const;
    Rotation next_pos() const;
    
    int type()     const { return tetrominoType; }
    int rotation() const { return currentRotation; }
    void set_rotation(const int& rotation) { currentRotation = rotation; }
    
    int x() { return X; }
    int y() { return Y; }
    
    int r() { return R; }
    int g() { return G; }
    int b() { return B; }
    
protected:
    Tetromino(const int& r, const int& g, const int& b) { R = r; G = g; B = b; }

    int tetrominoType;
    
    int X;
    int Y;
    int shadowY;
    
    int currentRotation = 0;
    std::vector<Rotation> rotations;
    
    const int SIZE = 24;
    const int N = 4;
    const int PADDING = 1;
    
    int R;
    int G;
    int B;
};

#endif