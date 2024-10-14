/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#ifdef _DEBUG
#pragma optimize("", off)
#endif

#include "Game/TestGame.h"

int main(int argc, char* argv[])
{
    // TODO replace this with Game again
    // Any game using this engine will have to place their subclass of Game here
    TestGame game;
    game.Play();

    return 0;
}