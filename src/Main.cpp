#ifdef _DEBUG
#pragma optimize("", off)
#endif

#include "Game/TestGame.h"

int main(int argc, char* argv[])
{
    TestGame game;
    game.Play();

    return 0;
}