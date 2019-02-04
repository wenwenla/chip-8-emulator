#include "App.h"
#include <iostream>

App::App(const char* bin)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL_Init failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    _window = SDL_CreateWindow("Chip-8 emulator",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH * C, WINDOW_HEIGHT * C, SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

    if(!vm.load(bin))
    {
        exit(EXIT_FAILURE);
    }
}

App::~App()
{
    if(_window)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    if(_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    SDL_Quit();
}

void App::run()
{
    _running = true;

    while(_running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                _running = false;
            }
        }

//        vm.debug();

        vm.insCycle();
        //clear
        SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(_renderer);

        //draw
        SDL_SetRenderDrawColor(_renderer, 0xCC, 0xEE, 0xCC, 0xFF);
        for(int x = 0; x < WINDOW_WIDTH; ++x)
        {
            for(int y = 0; y < WINDOW_HEIGHT; ++y)
            {
                if(vm.gfxState(x, y))
                {
                    SDL_Rect rect{C * x, C * y, C, C};
                    SDL_RenderFillRect(_renderer, &rect);
                }
            }
        }

        //update keyboard state
        int key = 0;
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        for(int k = 0; k < KEY_COUNT; ++k)
        {
            if(keyState[KEY_HASH[k]])
            {
                key |= (1 << k);
            }
        }
        vm.setKey(key);

        //play sound
        if(vm.isBeepOn()) {

        } else {

        }

        SDL_RenderPresent(_renderer);
        SDL_Delay(1);
    }
}
