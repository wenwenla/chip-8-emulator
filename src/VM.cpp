#include "VM.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cassert>

static inline Pointer getAddr(Pointer op)
{
    return op & 0xFFF;
}
static inline Byte getX(Pointer op)
{
    return op >> 8 & 0xF;
}
static inline Byte getY(Pointer op)
{
    return op >> 4 & 0xF;
}
static inline Byte getConstant(Pointer op)
{
    return op & 0xFF;
}
static inline int inc(Pointer& pc)
{
    pc += 2;
    return pc;
}

VM::VM()
{
    srand(time(0));
    memset(_mem, 0x00, sizeof(_mem));
    memset(_register, 0x00, sizeof(_register));
    memset(_gfx, 0x00, sizeof(_gfx));
    _timer_logic = 0;
    _timer_sound = 0;
    _logic_count = 0;
    _sound_count = 0;
    _last_time = clock();

    _I = 0;
    _pc = 512;
    memset(_stack, 0x00, sizeof(_stack));
    _sp = 0;
    memset(_key, 0x00, sizeof(_key));

    _init_func();
    _load_font();
}

void VM::func0(Pointer op)
{
    if(op == 0xE0)
    {
        //clear display
        memset(_gfx, 0x00, sizeof(_gfx));
        inc(_pc);
    }
    else if(op == 0xEE)
    {
        //return
        --_sp;
        assert(_sp >= 0);
        _pc = _stack[_sp];
    }
    else
    {
        //unsupported operations
        fprintf(stderr, "Invalid op: %d\n", op);
    }
}

void VM::func1(Pointer op)
{
    int addr = getAddr(op);
    //goto addr
    _pc = addr;
}

void VM::func2(Pointer op)
{
    int addr = getAddr(op);
    //call addr
    assert(_sp < STACK_SIZE);
    _stack[_sp++] = inc(_pc);
    _pc = addr;
}

void VM::func3(Pointer op)
{
    Byte vx = _register[getX(op)];
    if(vx == getConstant(op))
    {
        inc(_pc);
        inc(_pc);
    }
    else
    {
        inc(_pc);
    }
}

void VM::func4(Pointer op)
{
    Byte vx = _register[getX(op)];
    if(vx == getConstant(op))
    {
        inc(_pc);
    }
    else
    {
        inc(_pc);
        inc(_pc);
    }
}

void VM::func5(Pointer op)
{
    Byte vx = _register[getX(op)];
    Byte vy = _register[getY(op)];
    if(vx == vy)
    {
        inc(_pc);
        inc(_pc);
    }
    else
    {
        inc(_pc);
    }
}

void VM::func6(Pointer op)
{
    _register[getX(op)] = getConstant(op);
    inc(_pc);
}

void VM::func7(Pointer op)
{
    _register[getX(op)] += getConstant(op);
    inc(_pc);
}

void VM::func8(Pointer op)
{
    int tmp;
    switch(op & 0xF)
    {
    case 0x0:
        _register[getX(op)] = _register[getY(op)];
        break;
    case 0x1:
        _register[getX(op)] |= _register[getY(op)];
        break;
    case 0x2:
        _register[getX(op)] &= _register[getY(op)];
        break;
    case 0x3:
        _register[getX(op)] ^= _register[getY(op)];
        break;
    case 0x4:
        tmp = (int)_register[getX(op)] + (int)_register[getY(op)];
        _register[15] = (tmp > 0xFF);
        _register[getX(op)] = (Byte)tmp;
        break;
    case 0x5:
        tmp = (int)_register[getX(op)] - (int)_register[getY(op)];
        _register[15] = (tmp >= 0);
        _register[getX(op)] = (Byte)tmp;
        break;
    case 0x6:
        _register[15] = (_register[getX(op)] & 0x1);
        _register[getX(op)] >>= 1;
        break;
    case 0x7:
        tmp = (int)_register[getY(op)] - (int)_register[getX(op)];
        _register[15] = (tmp >= 0);
        _register[getX(op)] = (Byte)tmp;
        break;
    case 0xE:
        _register[15] = (_register[getX(op)] >> 7 & 1);
        _register[getX(op)] <<= 1;
        break;
    default:
        fprintf(stderr, "Invalid op: %d\n", op);
    }
    inc(_pc);
}

void VM::func9(Pointer op)
{
    Byte vx = _register[getX(op)];
    Byte vy = _register[getY(op)];
    if(vx != vy)
    {
        inc(_pc);
        inc(_pc);
    }
    else
    {
        inc(_pc);
    }
}

void VM::funcA(Pointer op)
{
    _I = getAddr(op);
    inc(_pc);
}

void VM::funcB(Pointer op)
{
    _pc = _register[0] + getAddr(op);
}

void VM::funcC(Pointer op)
{
    int rnd = rand() & getConstant(op);
    _register[getX(op)] = rnd;
    inc(_pc);
}

void VM::funcD(Pointer op)
{
    int vx = _register[getX(op)];
    int vy = _register[getY(op)];
    int n = (op & 0xF);
    _register[15] = 0;
    for(int i = 0; i < n; ++i)
    {
        if(vy + i >= WINDOW_HEIGHT) {
            puts("???");
            printf("%d\n", vy + i);
            break;
        }
        int st = _I + i;
        for(int j = 0; j < 8; ++j)
        {
            int o = _gfx[vy + i][vx + j];
            _gfx[vy + i][vx + j] ^= (_mem[st] >> (7 - j) & 1);
            if(o == 1 && _gfx[vy + i][vx + j] == 0)
            {
                _register[15] = 1;
            }
        }
    }
    inc(_pc);
}

void VM::funcE(Pointer op)
{
    int last = (op & 0xFF);
    int vx = _register[getX(op)];
    if(last == 0x9E)
    {
        if(_key[vx])
        {
            inc(_pc);
            inc(_pc);
        }
        else
        {
            inc(_pc);
        }
    }
    else if(last == 0xA1)
    {
        if(_key[vx])
        {
            inc(_pc);
        }
        else
        {
            inc(_pc);
            inc(_pc);
        }
    }
    else
    {
        fprintf(stderr, "Invalid op: %d\n", op);
    }
}

void VM::funcF(Pointer op)
{
    int x = getX(op), key;
    switch(op & 0xFF)
    {
    case 0x07:
        _register[x] = _timer_logic;
//        printf("get timer_logic: %d\n", _timer_logic);
        inc(_pc);
        break;
    case 0x0A:
        key = getKey();
        if(key != -1)
        {
            _register[x] = key;
            inc(_pc);
        }
        break;
    case 0x15:
        _timer_logic = _register[x];
//        printf("set timer_logic: %d\n", _timer_logic);
        inc(_pc);
        break;
    case 0x18:
        _timer_sound = _register[x];
        inc(_pc);
        break;
    case 0x1E:
        _I += _register[x];
        inc(_pc);
        break;
    case 0x29:
        //load font
//        fprintf(stdout, "Load font: %x\n", _register[x]);
        _I = 5 * (Pointer)_register[x];
        //exit(EXIT_FAILURE);
        inc(_pc);
        break;
    case 0x33:
        //BCD
        _mem[_I] = _register[x] / 100 % 10;
        _mem[_I + 1] = _register[x] / 10 % 10;
        _mem[_I + 2] = _register[x] % 10;
        inc(_pc);
        break;
    case 0x55:
        //memcpy(_mem + _I, _register, sizeof(_register));
        for(int i = 0; i <= x; ++i) {
            _mem[_I + i] = _register[i];
        }
        inc(_pc);
        break;
    case 0x65:
        //memcpy(_register, _mem + _I, sizeof(_register));
        for(int i = 0; i <= x; ++i) {
            _register[i] = _mem[_I + i];
        }
        inc(_pc);
        break;
    default:
        fprintf(stderr, "Invalid op: %d\n", op);
    }
}

int VM::getKey()
{
    int res = -1;
    for(int i = 0; i < KEY_COUNT; ++i)
    {
        if(_key[i])
        {
            res = i;
            break;
        }
    }
    return res;
}

void VM::_init_func()
{
    _func[0] = func0;
    _func[1] = func1;
    _func[2] = func2;
    _func[3] = func3;
    _func[4] = func4;
    _func[5] = func5;
    _func[6] = func6;
    _func[7] = func7;
    _func[8] = func8;
    _func[9] = func9;
    _func[10] = funcA;
    _func[11] = funcB;
    _func[12] = funcC;
    _func[13] = funcD;
    _func[14] = funcE;
    _func[15] = funcF;
}

bool VM::load(const char* fname)
{
    FILE* file = fopen(fname, "rb");
    if(!file)
    {
        fprintf(stderr, "Open %s failed!\n", fname);
        return false;
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    if(size > 4096 - 512)
    {
        fprintf(stderr, "File %s is too large\n", fname);
        return false;
    }
    rewind(file);
    fread(_mem + 512, 1, size, file);
    fclose(file);
    return true;
}

void VM::insCycle()
{
    _updateClock();
    Pointer op = ((Pointer)_mem[_pc] << 8) | _mem[_pc + 1];
    //printf("%x\n", op);
    (this->*_func[op >> 12])(op);
}

int VM::gfxState(int x, int y)
{
    return _gfx[y][x];
}

void VM::setKey(int key)
{
    for(int i = 0; i < KEY_COUNT; ++i)
    {
        _key[i] = (key >> i & 1);
    }
}

void VM::_updateClock() {
    long now = clock();
    constexpr int CNT = 1000 / 60;
    long time_pass = (now - _last_time);
    _last_time = now;
    _logic_count += time_pass;
    _sound_count += time_pass;

    if(_logic_count > CNT) {
        int sub = _logic_count / CNT;
        _logic_count %= CNT;
        if((int)(_timer_logic) > sub) {
            _timer_logic -= sub;
        } else {
            _timer_logic = 0;
        }
    }
    if(_sound_count > CNT) {
        int sub = _sound_count / CNT;
        _sound_count %= CNT;
        if((int)(_timer_sound) > sub) {
            _timer_sound -= sub;
        } else {
            _timer_sound = 0;
        }
    }
}

bool VM::isBeepOn() {
    return _timer_sound != 0;
}

void VM::_load_font() {
    //0x0
    _mem[0]  = 0b11110000;
    _mem[1]  = 0b10010000;
    _mem[2]  = 0b10010000;
    _mem[3]  = 0b10010000;
    _mem[4]  = 0b11110000;
    //0x100
    _mem[5]  = 0b00100000;
    _mem[6]  = 0b00100000;
    _mem[7]  = 0b00100000;
    _mem[8]  = 0b00100000;
    _mem[9]  = 0b00100000;
    //0x200
    _mem[10] = 0b11110000;
    _mem[11] = 0b00010000;
    _mem[12] = 0b11110000;
    _mem[13] = 0b10000000;
    _mem[14] = 0b11110000;
    //0x300
    _mem[15] = 0b11110000;
    _mem[16] = 0b00010000;
    _mem[17] = 0b11110000;
    _mem[18] = 0b00010000;
    _mem[19] = 0b11110000;
    //0x400
    _mem[20] = 0b10010000;
    _mem[21] = 0b10010000;
    _mem[22] = 0b11110000;
    _mem[23] = 0b00010000;
    _mem[24] = 0b00010000;
    //0x500
    _mem[25] = 0b11110000;
    _mem[26] = 0b10000000;
    _mem[27] = 0b11110000;
    _mem[28] = 0b00010000;
    _mem[29] = 0b11110000;
    //0x600
    _mem[30] = 0b11110000;
    _mem[31] = 0b10000000;
    _mem[32] = 0b11110000;
    _mem[33] = 0b10010000;
    _mem[34] = 0b11110000;
    //0x700
    _mem[35] = 0b11110000;
    _mem[36] = 0b00010000;
    _mem[37] = 0b00010000;
    _mem[38] = 0b00010000;
    _mem[39] = 0b00010000;
    //0x800
    _mem[40] = 0b11110000;
    _mem[41] = 0b10010000;
    _mem[42] = 0b11110000;
    _mem[43] = 0b10010000;
    _mem[44] = 0b11110000;
    //0x900
    _mem[45] = 0b11110000;
    _mem[46] = 0b10010000;
    _mem[47] = 0b11110000;
    _mem[48] = 0b00010000;
    _mem[49] = 0b11110000;
    //0xA00
    _mem[50] = 0b01100000;
    _mem[51] = 0b10010000;
    _mem[52] = 0b11110000;
    _mem[53] = 0b10010000;
    _mem[54] = 0b10010000;
    //0xB00
    _mem[55] = 0b11100000;
    _mem[56] = 0b10010000;
    _mem[57] = 0b11100000;
    _mem[58] = 0b10010000;
    _mem[59] = 0b11100000;
    //0xC00
    _mem[60] = 0b01110000;
    _mem[61] = 0b10000000;
    _mem[62] = 0b10000000;
    _mem[63] = 0b10000000;
    _mem[64] = 0b01110000;
    //0xD00
    _mem[65] = 0b11100000;
    _mem[66] = 0b10010000;
    _mem[67] = 0b10010000;
    _mem[68] = 0b10010000;
    _mem[69] = 0b11100000;
    //0xE
    _mem[70] = 0b11110000;
    _mem[71] = 0b10000000;
    _mem[72] = 0b11110000;
    _mem[73] = 0b10000000;
    _mem[74] = 0b11110000;
    //0xF
    _mem[75] = 0b11110000;
    _mem[76] = 0b10000000;
    _mem[77] = 0b11110000;
    _mem[78] = 0b10000000;
    _mem[79] = 0b10000000;
}
