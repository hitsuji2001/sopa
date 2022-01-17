#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../header/util.h"
#include "../header/clock.h"

#define WINDOW_WIDTH (8 * DIGIT_WIDTH)
#define WINDOW_HEIGTH DIGIT_HEIGHT

#define FPS 60
#define DELTA_TIME (1.0f/FPS)

int main(int argc, char **argv) {
    Clock clock;
    clock.reverse = false;
    clock.pause = false;

    parse_clock_from_cmd(&clock, argc, argv);

    debug_clock(&clock);

    scc(SDL_Init(SDL_INIT_VIDEO));

    char *digit_file_path = "digits.png";

    SDL_Window *window = scp(SDL_CreateWindow("sopa", 0, 0, WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_RESIZABLE));
    SDL_Renderer *renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC));
    SDL_Texture *texture = scp(IMG_LoadTexture(renderer, digit_file_path));

    bool quit = false;
    while(!quit) {
        SDL_Event event;

        // Number of frame we are currently in
        Uint32 frame = (Uint32)(SDL_GetTicks() * DELTA_TIME * WIGGLE_SPEED) % NUMBER_OF_FRAMES;

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        scc(SDL_SetRenderDrawColor(renderer, 69, 69, 69, 255));
        scc(SDL_RenderClear(renderer));

        render_clock(renderer, &clock, frame, texture);
        advance_clock(&clock);

        SDL_RenderPresent(renderer);

        SDL_Delay((int) floorf(DELTA_TIME * 1000.0f));
    }

    SDL_Quit();
    return 0;
}
