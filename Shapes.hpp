#ifndef Shapes_h
#define Shapes_h
#include "Tetromino.hpp"

class T_block : public Tetromino {
public:
    T_block(const int& xpos, const int& ypos) : Tetromino(52, 168, 83)
    {
        X = xpos;
        Y = ypos;
        tetrominoType = type::T;
        
        Rotation temp;
        // Rotation 1: T
        // Leave first row empty
        temp.push_back(SDL_Rect{ PADDING, + SIZE + PADDING*2, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE + PADDING*2, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE*2 + PADDING*3, SIZE, SIZE });
        rotations.push_back(temp); temp.clear();
        
        // Rotation 2: -|
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, PADDING, SIZE, SIZE });
        temp.push_back(SDL_Rect{ PADDING, SIZE + PADDING*2, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE*2 + PADDING*3, SIZE, SIZE });
        rotations.push_back(temp); temp.clear();
        
        // Rotation 3: ⊥
        // Leave first row empty
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE });
        temp.push_back(SDL_Rect{ PADDING, SIZE*2 + PADDING*3, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE*2 + PADDING*3, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE*2 + PADDING*3, SIZE, SIZE });
        rotations.push_back(temp); temp.clear();
        
        // Rotation 4: |-
        // Leave first column empty
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, PADDING, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE + PADDING*2, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE*2 + PADDING*3, SIZE, SIZE });
        rotations.push_back(temp);
    }
};

class S_block : public Tetromino {
public:
    S_block(const int& xpos, const int& ypos) : Tetromino(251, 188, 5)
    {
        X = xpos;
        Y = ypos;
        tetrominoType = type::S;
        
        Rotation S;
        // Leave first row empty
        S.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE});
        S.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE + PADDING*2, SIZE, SIZE});
        S.push_back(SDL_Rect{ PADDING, SIZE*2 + PADDING*3, SIZE, SIZE});
        S.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(S);
        
        Rotation I;
        I.push_back(SDL_Rect{ PADDING, PADDING, SIZE, SIZE});
        I.push_back(SDL_Rect{ PADDING, SIZE + PADDING*2, SIZE, SIZE});
        I.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE});
        I.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(I);
        
        rotations.push_back(S);
        rotations.push_back(I);
    }
};

class Z_block : public Tetromino {
public:
    Z_block(const int& xpos, const int& ypos): Tetromino(234, 67, 53)
    {
        X = xpos;
        Y = ypos;
        tetrominoType = type::Z;
        
        Rotation Z;
        // Leave first row empty
        Z.push_back(SDL_Rect{ PADDING, SIZE + PADDING*2, SIZE, SIZE});
        Z.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE});
        Z.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE*2 + PADDING*3, SIZE, SIZE});
        Z.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(Z);
        
        Rotation I;
        // Leave first column empty
        I.push_back(SDL_Rect{ SIZE*2 + PADDING*3, PADDING, SIZE, SIZE});
        I.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE});
        I.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE + PADDING*2, SIZE, SIZE});
        I.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(I);
        
        rotations.push_back(Z);
        rotations.push_back(I);
    }
};

class I_block : public Tetromino {
public:
    I_block(const int& xpos, const int& ypos) : Tetromino(44, 133, 244)
    {
        X = xpos;
        Y = ypos;
        tetrominoType = type::I;
        // Leave first row empty
        Rotation flat;
        flat.push_back(SDL_Rect{ PADDING, SIZE + PADDING*2, SIZE, SIZE});
        flat.push_back(SDL_Rect{ SIZE + PADDING*2, SIZE + PADDING*2, SIZE, SIZE});
        flat.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE + PADDING*2, SIZE, SIZE});
        flat.push_back(SDL_Rect{ SIZE*3 + PADDING*4, SIZE + PADDING*2, SIZE, SIZE});
        rotations.push_back(flat);
        
        // First 2 cols empty
        Rotation upright;
        upright.push_back(SDL_Rect{ SIZE*2 + PADDING*3, PADDING, SIZE, SIZE});
        upright.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE + PADDING*2, SIZE, SIZE});
        upright.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE*2 + PADDING*3, SIZE, SIZE});
        upright.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE*3 + PADDING*4, SIZE, SIZE});
        rotations.push_back(upright);
        
        rotations.push_back(flat);
        rotations.push_back(upright);
    }
};

class O_block : public Tetromino {
public:
    O_block(const int& xpos, const int& ypos) : Tetromino(101, 101, 101)
    {
        X = xpos;
        Y = ypos;
        tetrominoType = type::O;
        // Leave first row & column empty
        Rotation O;
        O.push_back(SDL_Rect{ SIZE + PADDING*2,    SIZE + PADDING*2, SIZE, SIZE});
        O.push_back(SDL_Rect{ SIZE*2 + PADDING*3,  SIZE + PADDING*2, SIZE, SIZE});
        O.push_back(SDL_Rect{ SIZE + PADDING*2,    SIZE*2 + PADDING*3, SIZE, SIZE});
        O.push_back(SDL_Rect{ SIZE*2 + PADDING*3,  SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.insert(rotations.begin(), N, O);
    }
};

class L_block : public Tetromino {
public:
    L_block(const int& xpos, const int& ypos) : Tetromino(255, 130, 163)
    {
        X = xpos;
        Y = ypos;
        tetrominoType = type::L;

        Rotation temp;
        // Position 1: _'
        // Leave first row empty
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3,  SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ PADDING,             SIZE*2 + PADDING*3, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,    SIZE*2 + PADDING*3, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3,  SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(temp);
        temp.clear();
        
        // Position 2: L
        // Leave first column empty
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   PADDING, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   SIZE*2 + PADDING*3, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3, SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(temp);
        temp.clear();
        
        // Position 3: ,–
        // Leave first row empty
        temp.push_back(SDL_Rect{ PADDING,             SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,    SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3,  SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ PADDING,             SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(temp);
        temp.clear();
        
        // Position 4: -|
        temp.push_back(SDL_Rect{ PADDING,            PADDING, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   PADDING, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(temp);
    }
};

class J_block : public Tetromino {
public:
    J_block(const int& xpos, const int& ypos) : Tetromino(243, 156, 18)
    {
        X = xpos;
        Y = ypos;
        tetrominoType = type::J;
        
        Rotation temp;
        // Position 1: '_
        // Leave first row empty
        temp.push_back(SDL_Rect{ PADDING,             SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ PADDING,             SIZE*2 + PADDING*3, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,    SIZE*2 + PADDING*3, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3,  SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(temp);
        temp.clear();
        
        // Position 2: |-
        // Leave first column empty
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   PADDING, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3, PADDING, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(temp);
        temp.clear();
        
        // Position 3: –,
        // Leave first row empty
        temp.push_back(SDL_Rect{ PADDING,             SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,    SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3,  SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE*2 + PADDING*3,  SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(temp);
        temp.clear();
        
        // Position 4: J
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   PADDING, SIZE, SIZE });
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   SIZE + PADDING*2, SIZE, SIZE});
        temp.push_back(SDL_Rect{ SIZE + PADDING*2,   SIZE*2 + PADDING*3, SIZE, SIZE});
        temp.push_back(SDL_Rect{ PADDING,            SIZE*2 + PADDING*3, SIZE, SIZE});
        rotations.push_back(temp);
    }
};

#endif /* Shapes_h */
