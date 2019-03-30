# cnake

cnake is snake clone written in C++ and ncurses. Actually in beta and have some bugs.

## Compiling

# On Linux

`make`

It needs ncurses, g++ and make.

Following command will install dependicies on Debian-based systems:

`sudo apt-get install libncurses-dev g++ make`

# On Windows

It's more complicated I think.

1. I assume that you have MinGWw64 (https://mingw-w64.org/) working and added to PATH
2. Download PDCurses (https://pdcurses.sourceforge.io/)
3. Extract it to `C:\pdcurses`
4. Open terminal at `C:\pdcurses\wincom`
5. `mingw32-make DLL=Y`
6. Copy `pdcurses.dll` here
7. Change directory to this one
8. Change the cnake.cpp file. 1st line, `#include <ncurses.h>`, need to be changed into `#include <curses.h>` (difference is `n` letter)
9. `g++ -IC:\pdcurses -L. cnake.cpp -lpdcurses -static-libstdc++ -static-libgcc -o cnake.exe`
