#include <iostream>
#include "Source/Game.h"
int main()
{
    HauntedHouseGame game;
    if (!game.init())
    {
        return -1;
    }

    game.run();
    std::cout << "Exiting Game!" << std::endl;
    return 0;
}