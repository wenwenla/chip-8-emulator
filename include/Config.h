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
    SDL_SCANCODE_KP_0,
    SDL_SCANCODE_KP_1,
    SDL_SCANCODE_KP_2,
    SDL_SCANCODE_KP_3,
    SDL_SCANCODE_KP_4,
    SDL_SCANCODE_KP_5,
    SDL_SCANCODE_KP_6,
    SDL_SCANCODE_KP_7,
    SDL_SCANCODE_KP_8,
    SDL_SCANCODE_KP_9,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
};

using Byte = unsigned char;
using Pointer = unsigned short;

#endif // CONFIG_H
