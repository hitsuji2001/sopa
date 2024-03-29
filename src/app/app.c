#include "../../header/app.h"
#include "../../header/digits.h"

void app_run(int argc, char **argv) {
    Clock clock;
    Clock initial_clock;

    float user_scale = MAX_SCALE;
    float fit_scale = MAX_SCALE;

    if (parse_clock_from_cmd(&clock, argc, argv) < 0) {
        fprintf(stderr, "Can't not recognize your syntax\n");
        fprintf(stdout, "Include '-h' or '--help' for more information\n");
        exit(1);
    }

    clock_set(&clock, &initial_clock);

    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *window = scp(SDL_CreateWindow("sopa", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_RESIZABLE));
    SDL_Renderer *renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC));
    SDL_Surface *surface = scp(SDL_CreateRGBSurfaceFrom(png_image,
                                                        (int)png_width, 
                                                        (int)png_height, 
                                                        (int)png_depth, 
                                                        (int)png_pitch,
                                                        0x000000FF,
                                                        0x0000FF00,
                                                        0x00FF0000,
                                                        0xFF000000));

    SDL_Texture *texture = scp(SDL_CreateTextureFromSurface(renderer, surface));

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
                case SDL_KEYDOWN: {
                  switch (event.key.keysym.sym) {
                      case SDLK_SPACE:
                          clock.pause = !clock.pause;
                          break;
                      case SDLK_p:
                          clock.pause = !clock.pause;
                          break;
                      case SDLK_r:
                          clock_set(&initial_clock, &clock);
                          break;
                      case SDLK_q:
                          quit = true;
                          break;
                      case SDLK_MINUS:
                          user_scale -= 0.1f;
                          break;
                      case SDLK_PLUS:
                          user_scale += 0.1f;
                          break;
                      case SDLK_EQUALS:
                          user_scale += 0.1f;
                          break;
                      case SDLK_0:
                          user_scale = MAX_SCALE;
                          fit_scale = MAX_SCALE;
                          break;
                      case SDLK_F11: {
                          Uint32 window_flags;
                          scc(window_flags = SDL_GetWindowFlags(window));
                          if(window_flags & SDL_WINDOW_FULLSCREEN_DESKTOP) scc(SDL_SetWindowFullscreen(window, 0));
                          else scc(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP));
                          }
                          break;
                  }
               }
            }
        }
        
        //Render start
        scc(SDL_SetRenderDrawColor(renderer, 69, 69, 69, 255));
        scc(SDL_RenderClear(renderer));

        render_clock(renderer, &clock, frame, &fit_scale, user_scale, texture, window);

        SDL_RenderPresent(renderer);

        SDL_Delay((int) floorf(DELTA_TIME * 1000.0f));
        //Render end
        
        // Update start
        if (clock.pause) scc(SDL_SetTextureColorMod(texture, 220, 120, 120));
        else scc(SDL_SetTextureColorMod(texture, 255, 255, 255));
        clock_advance(&clock);
        // Update end
    }

    SDL_Quit();
    return;
}
