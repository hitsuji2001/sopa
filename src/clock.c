#include "../header/clock.h"

void print_clock(const Clock *clock) {
    printf("%02d:%02d:%02d\n", clock->hour, clock->minute, clock->second);
}

void debug_clock(const Clock *clock) {
    print_clock(clock);
    printf("reverse: %d\n", clock->reverse);
    printf("pause: %d\n", clock->pause);
}

void increase_clock(Clock *clock) {
    if (clock->tick < 60) clock->tick++;
    else {
        clock->tick = 0;
        if (clock->second < 60) clock->second++;
        else {
            clock->second = 0;
            if (clock->minute < 60) clock->minute++;
            else {
                clock->minute = 0;
                clock->hour++;
            }
        }
    }
}

void decrease_clock(Clock *clock) {
    if (clock->second == 0 && clock->minute == 0 && clock->hour == 0) return;

    if (clock->tick > 0) clock->tick--;
    else {
        clock->tick = 59;
        if (clock->second > 0) clock->second--;
        else {
            clock->second = 59;
            if (clock->minute > 0) clock->minute--;
            else {
                clock->minute = 59;
                clock->hour--;
            }
        }
    }
}

void advance_clock(Clock *clock) {
    if (clock->pause) return;

    if (!clock->reverse) increase_clock(clock);
    else decrease_clock(clock);

    return;
}

void display_help() {
    fprintf(stdout, "help\n");
}

void render_digit_at(SDL_Renderer *renderer, const int digit, const int order, const int frame, SDL_Texture *texture) {
    SDL_Rect src_rect = { digit * DIGIT_WIDTH, DIGIT_HEIGHT * frame, DIGIT_WIDTH, DIGIT_HEIGHT };
    SDL_Rect dst_rect = { DIGIT_WIDTH * order, 0                   , DIGIT_WIDTH, DIGIT_HEIGHT };

    scc(SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect));
}

void render_clock(SDL_Renderer *renderer, Clock *clock, const int frame, SDL_Texture *texture) {
    //TODO: if clock->hour is more than 3 digits, it will overflow

    render_digit_at(renderer, clock->hour / 10, 0, (frame + 1) % NUMBER_OF_FRAMES, texture);
    render_digit_at(renderer, clock->hour % 10, 1, (frame + 2) % NUMBER_OF_FRAMES, texture);

    render_digit_at(renderer, 10, 2, frame, texture);

    render_digit_at(renderer, clock->minute / 10, 3, (frame + 3) % NUMBER_OF_FRAMES, texture);
    render_digit_at(renderer, clock->minute % 10, 4, (frame + 4) % NUMBER_OF_FRAMES, texture);

    render_digit_at(renderer, 10, 5, frame, texture);

    render_digit_at(renderer, clock->second / 10, 6, (frame + 2) % NUMBER_OF_FRAMES, texture);
    render_digit_at(renderer, clock->second % 10, 7, (frame + 1) % NUMBER_OF_FRAMES, texture);
}

int parse_clock_from_long(Clock *clock, const long time) {
    clock->hour = time / (60 * 60);
    clock->minute = (time % (60 * 60)) / 60;
    clock->second = (time % (60 * 60)) % 60;

    return 0;
}

int exceeded_time(int *time) {
    int res = *time / 60;
    *time %= 60;
    return res;
}

int from_clock_format_to_clock(Clock *clock, const char *string) {
    if (!string_contains(string, ':') || string_contains(string, '-')) {
        clock = NULL;
        return -1;
    }
    
    size_t index = 0;
    size_t i = 0;
    size_t j = 0;
    char buffer[10] = {0};
    long attri[3] = {0};

    if (string_contains_more_than_one(string, ':')) {
        while(index < strlen(string)) {
            if (is_alpha(string[index])) {
                clock = NULL;
                return -1;
            }
            if (string[index] == ':') {
                i = 0;
                attri[j] = from_string_to_long(buffer);
                j++;
                memset(buffer, 0, sizeof(buffer));
            } else buffer[i++] = string[index];
            index++;
        }
        attri[j++] = from_string_to_long(buffer);
    }

    clock->hour = attri[0];
    clock->minute = attri[1];
    clock->second = attri[2];

    clock->minute += exceeded_time(&clock->second);
    clock->hour += exceeded_time(&clock->minute);
    
    return 0;
}

int from_human_readable_format_to_clock(Clock *clock, const char *string) {
    if ((!string_contains(string, 'h') &&
        !string_contains(string, 'm') && 
        !string_contains(string, 's') &&
        !string_contains(string, 'd')) || string_contains(string, '-')) {
        clock = NULL;
        return -1;
    }

    clock->hour = 0;
    clock->minute = 0;
    clock->second = 0;

    size_t index = 0;
    size_t i = 0;
    char buffer[10] = {0};

    while(index < strlen(string)) {
        if (is_alpha(string[index])) {
            switch (string[index]) {
                case 'h':
                    clock->hour = from_string_to_long(buffer);
                    break;
                case 'd':
                    clock->hour = from_string_to_long(buffer) * 24;
                    break;
                case 'm':
                    clock->minute = from_string_to_long(buffer);
                    break;
                case 's':
                    clock->second = from_string_to_long(buffer);
                    break;
                default:
                    return -1;
            }
            i = 0;
            memset(buffer, 0, sizeof(buffer));
        } else {
            buffer[i++] = string[index];
        }
        index++;
    }

    clock->minute += exceeded_time(&clock->second);
    clock->hour += exceeded_time(&clock->minute);

    return 0;
}

int parse_clock_from_string(Clock *clock, const char *string) {
    if (string_contains(string, ':') && (string_contains(string, 'h') || string_contains(string, 'm') || string_contains(string, 's') || string_contains(string, 'd'))) return -1;
    else if (string_contains(string, ':')) return from_clock_format_to_clock(clock, string);
    else return from_human_readable_format_to_clock(clock, string);
}

int parse_flags_for_clock(Clock *clock, const char *string) {
    if (!string_contains(string, '-')) return -1;

    if (string_contains_more_than_one(string, '-') == 2) {
        if(strcmp(string, "--help") == 0) {
            display_help();
            exit (0);
        }
    } else if (string_contains_more_than_one(string, '-') == 1) {
        if(strlen(string) == 2 && strcmp(string, "-h") == 0) {
            display_help();
            exit(0);
        }

        for(size_t i = 1; i < strlen(string); ++i) {
            switch (string[i]) {
                case 'r':
                    clock->reverse = true;
                    break;
                case 'p':
                    clock->pause = true;
                    break;
                default:
                    fprintf(stderr, "Unrecognized flags \'%s\'\n", string);
                    exit(1);
            }
        }
    } else {
        fprintf(stderr, "Unreachable!\n");
        exit(1);
    }

    return 0;
}

int parse_clock_from_cmd(Clock *clock, int argc, char **argv) {
    clock->reverse = false;
    clock->pause = false;

    if (argc == 1) return parse_clock_from_long(clock, 0); // Provided no flag and no time
    else if (argc == 2) {
        if (string_contains(argv[1], '-')) { // Provided flags but no time
            parse_flags_for_clock(clock, argv[1]);
            if (clock->reverse) {
                fprintf(stderr, "You shouldn't count backward from \'00:00:00\'\n");
                exit(1);
            }
            return parse_clock_from_long(clock, 0);
        } else { // Provided time but no flag
            if (parse_clock_from_string(clock, argv[1]) < 0) return parse_clock_from_long(clock, from_string_to_long(argv[1]));
        }
    } else if (argc == 3) { // Provided flags and time
        parse_flags_for_clock(clock, argv[1]);
        if (parse_clock_from_string(clock, argv[2]) < 0) return parse_clock_from_long(clock, from_string_to_long(argv[2]));
    } else { // Unreachable
        fprintf(stderr, "Too much arguments provided\n");
        display_help();
        exit(1);
    }

    return 0;
}
