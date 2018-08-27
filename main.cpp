#ifdef _WIN32
#include <windows.h> //Sleep()
#else
#include <unistd.h>
#endif

#include "synth.h"

int main(int argc, char** argv) {

    Synth s;

    while(1) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }
}
