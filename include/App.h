#ifndef APP_H
#define APP_H

#include "Config.h"
#include "VM.h"

class App
{
public:
    App(const char* bin);
    ~App();
    void run();
private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    bool _running = false;
    int _key = 0;
    VM vm;
};

#endif // APP_H
