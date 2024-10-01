#ifdef _DEBUG
#pragma optimize("", off)
#endif

#include "Game/Game.h"

int main(int argc, char* argv[])
{
    Game game;
    game.Play();

    return 0;
}