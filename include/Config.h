#ifndef CONFIG_H
#define CONFIG_H
#include <SDL2/SDL.h>

constexpr int MEMORY_SIZE = 4096;
constexpr int STACK_SIZE = 48;
constexpr int REGISTER_COUNT = 16;
constexpr int WINDOW_WIDTH = 64;
constexpr int WINDOW_HEIGHT = 32;
constexpr int KEY_COUNT = 16;
constexpr int C = 8;
constexpr int KEY_HASH[16] = {
    SDL_SCANCODE_X,//0
    SDL_SCANCODE_1,//1
    SDL_SCANCODE_2,//2
    SDL_SCANCODE_3,//3
    SDL_SCANCODE_Q,//4
    SDL_SCANCODE_W,//5
    SDL_SCANCODE_E,//6
    SDL_SCANCODE_A,//7
    SDL_SCANCODE_S,//8
    SDL_SCANCODE_D,//9
    SDL_SCANCODE_Z,//A
    SDL_SCANCODE_C,//B
    SDL_SCANCODE_4,//C
    SDL_SCANCODE_R,//D
    SDL_SCANCODE_F,//E
    SDL_SCANCODE_V,//F
};

using Byte = unsigned char;
using Pointer = unsigned short;

#endif // CONFIG_H
