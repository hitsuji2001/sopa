#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../header/util.h"
#include "../header/clock.h"

#define PICTURE_WIDTH 1920
#define PICTURE_HEIGHT 1080

#define DIGIT_WIDTH (PICTURE_WIDTH / 11)
#define DIGIT_HEIGHT (PICTURE_HEIGHT / 4)
#define NUMBER_OF_FRAMES 4
#define WIGGLE_SPEED 0.5f

#define WINDOW_WIDTH (8 * DIGIT_WIDTH)
#define WINDOW_HEIGTH DIGIT_HEIGHT

#define FPS 60
#define DELTA_TIME (1.0f/FPS)

int parse_flags(char *string, Clock *clock) {
    if(!string_contains(string, '-')) return -1;

    if(string_contains_more_than_one(string, '-') == 2) {
        if(strcmp(string, "--help") == 0) {
            printf("help message\n");
            exit (0);
        }
    }

    clock->reverse = false;
    clock->pause = false;

    for(size_t i = 1; i < strlen(string); ++i) {
        switch (string[i]) {
            case 'h':
                printf("help\n");
                exit(0);
            case 'r':
                clock->reverse = true;
                printf("reverse\n");
                break;
            case 'p':
                clock->pause = true;
                printf("pause\n");
                break;
            default:
                printf("Unrecognized flags `%s`\n", string);
                exit(1);
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

void render_digit_at(SDL_Renderer *renderer, const int digit, const int order, const int frame, SDL_Texture *texture) {
    SDL_Rect src_rect = { digit * DIGIT_WIDTH, DIGIT_HEIGHT * frame, DIGIT_WIDTH, DIGIT_HEIGHT };
    SDL_Rect dst_rect = { DIGIT_WIDTH * order, 0                   , DIGIT_WIDTH, DIGIT_HEIGHT };

    scc(SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect));
}

void render_clock(SDL_Renderer *renderer, Clock *clock, const int frame, SDL_Texture *texture) {
    //TODO: if clock->hour is more than 3 digits, it will overflow

    render_digit_at(renderer, clock->hour / 10, 0, frame, texture);
    render_digit_at(renderer, clock->hour % 10, 1, frame, texture);

    render_digit_at(renderer, 10, 2, frame, texture);

    render_digit_at(renderer, clock->minute / 10, 3, frame, texture);
    render_digit_at(renderer, clock->minute % 10, 4, frame, texture);

    render_digit_at(renderer, 10, 5, frame, texture);

    render_digit_at(renderer, clock->second / 10, 6, frame, texture);
    render_digit_at(renderer, clock->second % 10, 7, frame, texture);
}

int main(int argc, char **argv) {
#if 0
    (void)argc; (void) argv;
#else
    Clock clock;
    if(argc > 1) {
        if(parse_flags(argv[1], &clock) < 0) {
            if(from_string_to_clock(&clock, argv[1]) < 0) {
                parse_clock_from_long(&clock, from_string_to_long(argv[1]));
                print_clock(&clock);
            } else print_clock(&clock);
        } else {
            printf("Clock start with flags\n");
            if(from_string_to_clock(&clock, argv[2]) < 0) {
                parse_clock_from_long(&clock, from_string_to_long(argv[2]));
                print_clock(&clock);
            } else print_clock(&clock);
        }
    } else {
        parse_clock_from_long(&clock, 0);
        printf("Clock start\n");
    }
    debug_clock(&clock);
#endif

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
    }
    SDL_Quit();

    return 0;
}
