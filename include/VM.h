#ifndef VM_H
#define VM_H
#include "Config.h"
#include <functional>
#include <SDL2/SDL_mixer.h>
class VM
{
public:
    VM();
    ~VM() = default;

    bool load(const char* file);
    void insCycle();
    int gfxState(int x, int y);
    void setKey(int key);
    bool isBeepOn();
public:
    void debug()
    {
        puts("\n============");
        puts("Register:");
        for(int i = 0; i < REGISTER_COUNT; ++i) {
            printf("%02x ", i);
        }
        puts("");
        for(int i = 0; i < REGISTER_COUNT; ++i)
        {
            printf("%02x ", _register[i]);
        }
        puts("");
        printf("PC: %d(0x%02x)\n", _pc, _pc);
        printf("I: %d(0x%02x)\n", _I, _I);
        printf("Op: 0x%02x 0x%02x\n", _mem[_pc], _mem[_pc + 1]);
        puts("Stack: |->");
        for(int i = 0; i < _sp; ++i) {
            printf("%d(0x%x) ", _stack[i], _stack[i]);
        }
        puts("");
        getchar();
    }

    void show()
    {
        puts("\n==================");
        for(int i = 0; i < WINDOW_WIDTH; ++i)
        {
            for(int j = 0; j < WINDOW_HEIGHT; ++j)
            {
                putchar("01"[_gfx[j][i]]);
            }
            puts("");
        }
        puts("\n==================");
    }
private:
    Byte _mem[MEMORY_SIZE];
    Byte _register[REGISTER_COUNT];
    Byte _gfx[WINDOW_HEIGHT][WINDOW_WIDTH];
    Byte _timer_logic, _timer_sound;
    int _logic_count, _sound_count;
    long _last_time;

    Pointer _I, _pc;
    Pointer _stack[STACK_SIZE];
    Pointer _sp;
    Byte _key[KEY_COUNT];

private:
    void (VM::*_func[16])(Pointer);
    void _init_func();
    void _load_font();
    void func0(Pointer);
    void func1(Pointer);
    void func2(Pointer);
    void func3(Pointer);
    void func4(Pointer);
    void func5(Pointer);
    void func6(Pointer);
    void func7(Pointer);
    void func8(Pointer);
    void func9(Pointer);
    void funcA(Pointer);
    void funcB(Pointer);
    void funcC(Pointer);
    void funcD(Pointer);
    void funcE(Pointer);
    void funcF(Pointer);
    int getKey();
    void _updateClock();
};

#endif // VM_H
