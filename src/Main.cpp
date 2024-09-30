#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sol/sol.hpp>
#include <imgui/imgui.h>
#include <glm/glm.hpp>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    std::cout << "2DEngine initialization successful" << std::endl;
    return 0;
}