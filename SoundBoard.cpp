#include <iostream>
#include "SoundBoard.hpp"

bool SoundBoard::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Error: OpenAudio Init" << Mix_GetError() << std::endl;
        return false;
    }
    
    
    return true;
}

void SoundBoard::music_start()
{
    
}

void SoundBoard::music_stop()
{
    
}

Mix_Chunk* loadSound(const std::string& filepath) {
    Mix_Chunk* temp = Mix_LoadWAV(filepath.c_str());
    if( temp == nullptr )
        std::cerr << "Failed to load sound', error:" << Mix_GetError() << std::endl;
    return temp;
}

Mix_Music* loadMusic(const std::string& filepath) {
    Mix_Music* temp = Mix_LoadMUS(filepath.c_str());
    if( temp == nullptr )
        std::cerr << "Failed to load music', error:" << Mix_GetError() << std::endl;
    return temp;
}