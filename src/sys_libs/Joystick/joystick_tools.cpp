
#include "joystick.hpp"

bool GCGetButton(GameControllerButton btn){
    return gp.btns & (1<<btn);
    // return SDL_GameControllerGetButton(gPad, (SDL_GameControllerButton)btn);
}