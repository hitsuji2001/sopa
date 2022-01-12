#include <stdlib.h>

#include <SDL2/SDL.h>

#include "../header/util.h"
#include "../header/clock.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGTH 600

int parse_flags(char *string) {
    if(!string_contains(string, '-')) return -1;

    if(strlen(string) == 2) {
        if(string[1] == 'h') {
            printf("help\n");
        }
    } else {
        if(strcmp(string, "-help") == 0) {
            printf("help message\n");
        }
        else {
            for(size_t i = 1; i < strlen(string); ++i) {
                switch (string[i]) {
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
    }

    return 0;
}

//TODOO: expected format ./sopa [flags] [time]
//                              argv[1] argv[2 -> n]

//TODOOOOOOOOOOO: setting up SDL2
//Draw a window
//Draw a picture

//TODOOO: refactor code

//TOD: set up arguments
// %lld           : start the clock from %lld seconds - DONE
// %02d:%02d:%02d : start the clock from %02d:%02d:%02d - DONE
// %dh            : start the clock from %d hours - DONE
// %dm            : start the clock from %d minutes - DONE
// %ds            : start the clock from %d seconds - DONE
// %dh%m%s        : start the clock from %d hours %d minutes %d seconds - DONE
// --NOTE: these 3 can be combined

//TOD: set up flags
// -r    : reverse the clock from %lld seconds or the format above - DONE
// -p    : start at pause state - DONE
// -help : display help messages

int main(int argc, char **argv) {
#if 0
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

    return 0;
}
