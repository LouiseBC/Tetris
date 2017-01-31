#ifndef SoundBoard_hpp
#define SoundBoard_hpp
#include <SDL2_mixer/SDL_mixer.h>
#include <string>

class SoundBoard {
public:
    bool init();
    void music_start();
    void music_stop();
    
private:
    Mix_Chunk* load_sound(const std::string& filepath);
    Mix_Music* load_music(const std::string& filepath);
    
    Mix_Music* music;
    Mix_Chunk* rotate;
    
};

#endif