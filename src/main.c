#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../header/util.h"
#include "../header/clock.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGTH 300

int parse_flags(char *string) {
    if(!string_contains(string, '-')) return -1;

    if(strcmp(string, "-help") == 0) {
        printf("help message\n");
    }
    else {
        for(size_t i = 1; i < strlen(string); ++i) {
            switch (string[i]) {
                case 'h':
                    printf("help\n");
                    break;
                case 'r':
                    printf("reverse\n");
                    break;
                case 'p':
                    printf("pause\n");
                    break;
                default:
                    return -1;
            }
        }
    }

    return 0;
}

int scc(int code) {
    if(code < 0) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }

    return code;
}

void *scp(void *ptr) {
    if(ptr == NULL) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }

    return ptr;
}

int main(int argc, char **argv) {
#if 1
    (void)argc; (void) argv;
#else
    Clock clock;
    if(argc > 1) {
        if(parse_flags(argv[1]) < 0) {
            if(from_string_to_clock(&clock, argv[1]) < 0) {
                clock = parse_clock_from_long(from_string_to_long(argv[1]));
                print_clock(&clock);
            } else print_clock(&clock);
        } else {
            printf("Clock start with flags\n");
            if(from_string_to_clock(&clock, argv[2]) < 0) {
                clock = parse_clock_from_long(from_string_to_long(argv[2]));
                print_clock(&clock);
            } else print_clock(&clock);
        }
    } else {
        printf("Clock start\n");
    }
#endif

    scc(SDL_Init(SDL_INIT_VIDEO));

    char *digit_file_path = "digits.png";
#define PICTURE_WIDTH 1920
#define PICTURE_HEIGHT 1080
#define DIGIT_WIDTH (PICTURE_WIDTH / 11)
#define DIGIT_HEIGHT (PICTURE_HEIGHT / 4)

    SDL_Window *window = scp(SDL_CreateWindow("sopa", 0, 0, WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_RESIZABLE));
    SDL_Renderer *renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC));
    SDL_Texture *texture = scp(IMG_LoadTexture(renderer, digit_file_path));

    SDL_Rect src_rect;
    SDL_Rect dst_rect = {0, 0, DIGIT_WIDTH, DIGIT_HEIGHT};

    bool quit = false;
    while(!quit) {
        SDL_Event event;

        Uint32 seconds = SDL_GetTicks() / 1000;
        Uint32 frame = seconds % 10;
        src_rect = (SDL_Rect){ frame * DIGIT_WIDTH, DIGIT_HEIGHT * 3, DIGIT_WIDTH, DIGIT_HEIGHT };

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        scc(SDL_SetRenderDrawColor(renderer, 69, 69, 69, 255));
        scc(SDL_RenderClear(renderer));

        scc(SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect));

        SDL_RenderPresent(renderer);
    }
    SDL_Quit();

    return 0;
}
