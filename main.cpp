#include "gamewindow.hpp"
#include <cassert>

int main()
{
    assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

    GameWindow mainWindow;

    mainWindow.mainLoop();

    SDL_Quit();

    return 0;
}
