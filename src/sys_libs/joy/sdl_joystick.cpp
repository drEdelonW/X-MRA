#include <SDL.h>
#include <stdio.h>
#include <cinttypes>

#define to_float(x)     ((x) / (0x8000 * 1.0f))

struct JoystickState {
    int axis[SDL_CONTROLLER_AXIS_MAX];
    bool buttons[SDL_CONTROLLER_BUTTON_MAX];

    JoystickState() {
        for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)
            axis[i] = 0;
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
            buttons[i] = false;
    }
};

void updateJoystickState(SDL_Event &event, JoystickState &state) {
    if (event.type == SDL_JOYAXISMOTION) {
        state.axis[event.jaxis.axis] = event.jaxis.value;
    } else if (event.type == SDL_JOYBUTTONDOWN) {
        state.buttons[event.jbutton.button] = true;
    } else if (event.type == SDL_JOYBUTTONUP) {
        state.buttons[event.jbutton.button] = false;
    }
}


int main_joy(int argc, char *argv[]) {
    SDL_Init(/*SDL_INIT_VIDEO|*/SDL_INIT_JOYSTICK);

    // SDL_Window *window = nullptr;
    // SDL_Renderer *renderer = nullptr;
    // SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_SHOWN, &window, &renderer);

    if (SDL_NumJoysticks() > 0) {
        SDL_Joystick *joystick;
        joystick = SDL_JoystickOpen(0);
        printf("Name: %s\n", SDL_JoystickNameForIndex(0));

        SDL_Event event;
        JoystickState joystickState;
        while (1) {
            SDL_WaitEvent(&event);
            if (event.type == SDL_QUIT) {
                break;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    break;
                }
            } else 

            updateJoystickState(event, joystickState);
            
            if (joystickState.buttons[SDL_CONTROLLER_BUTTON_GUIDE]) {
                break;
            }
            // Output joystick state
            printf("Axis: ");       for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; ++i)   {   printf("%i:%3.3f  ", i, to_float(joystickState.axis[i])); } printf("              \r");
            printf("\t\t\t\t\t\t\t\tButtons: "); for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {   printf("%i:%s  ", i, joystickState.buttons[i] ? "V" : " "); }
            printf("              \r");
        }
        
        SDL_JoystickClose(joystick);
    } else {
        printf("no Joystick found");
    }
    printf("\n");
    
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    return 0;
}