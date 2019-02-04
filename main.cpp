#include "App.h"
using namespace std;

int main(int argc, char* argv[])
{
    if(argc == 2) {
        App app(argv[1]);
        app.run();
    } else {
        App app("IBM Logo.ch8");
        app.run();
        puts("Usage: ./Chip.exe /path/to/your/rom");
    }
    return 0;
}
