#include "../header/clock.h"

void clock_increase(Clock *clock) {
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

void clock_decrease(Clock *clock) {
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

void clock_advance(Clock *clock) {
    if (clock->pause) return;

    if (!clock->reverse) clock_increase(clock);
    else clock_decrease(clock);

    return;
}

void display_help() {
    fprintf(stdout, "-------------------------------------------------------------------------------------\n");
    fprintf(stdout, "[help]\n");
    fprintf(stdout, "Syntax: sopa [flags] [time]\n");
    fprintf(stdout, "[Flags]\n");
    fprintf(stdout, "-h or --help: display this help messages\n");
    fprintf(stdout, "-p: start the clock in pause state\n");
    fprintf(stdout, "-r: start the clock in reverse state\n");
    fprintf(stdout, "[Time format]\n");
    fprintf(stdout, "number: in seconds\n");
    fprintf(stdout, "hh:mm:ss\n");
    fprintf(stdout, "...d...h...m...s: with ... is your time in days, hours, minutes, seconds respectively\n");
    fprintf(stdout, "-------------------------------------------------------------------------------------\n");
}

void clock_set(const Clock *src_clock, Clock *dst_clock) {
    dst_clock->hour = src_clock->hour;
    dst_clock->minute = src_clock->minute;
    dst_clock->second = src_clock->second;
    dst_clock->tick = src_clock->tick;
    dst_clock->reverse = src_clock->reverse;
    dst_clock->pause = src_clock->pause;
}

float calculate_scaler(SDL_Window *window, float *fit_scale, float user_scale) {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    const float window_ratio = w / h;
    const float text_ratio = (DIGIT_WIDTH * 8) / DIGIT_HEIGHT;
    
    if(text_ratio > window_ratio) *fit_scale = (float)w / (DIGIT_WIDTH * 8);
    else *fit_scale = (float)h / DIGIT_HEIGHT;

    return *fit_scale * user_scale;
}

void get_initial_draw_position(int *x, int *y, const int order, SDL_Window *window, float *fit_scale, float user_scale) {
    float scaler = calculate_scaler(window, fit_scale, user_scale);
    float time_table_width = DIGIT_WIDTH * 8 * scaler;
    float time_table_height = DIGIT_HEIGHT * scaler;
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    *x = (int)((DIGIT_WIDTH * scaler) * order + (w - time_table_width) / 2);
    *y = (int)((h - time_table_height) / 2);

    return;
}

void render_digit_at(SDL_Renderer *renderer, const int digit, const int order, const int frame,
                    float *fit_scale, float user_scale, SDL_Texture *texture, SDL_Window *window) {
    int pen_x, pen_y;
    get_initial_draw_position(&pen_x, &pen_y, order, window, fit_scale, user_scale);
    SDL_Rect src_rect = { 
                            digit * DIGIT_WIDTH,
                            DIGIT_HEIGHT * frame,
                            DIGIT_WIDTH, 
                            DIGIT_HEIGHT 
                        };
    SDL_Rect dst_rect = {
                            pen_x,
                            pen_y, 
                            DIGIT_WIDTH * user_scale * *fit_scale,
                            DIGIT_HEIGHT * user_scale * *fit_scale 
                        };

    scc(SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect));
}

void render_clock(SDL_Renderer *renderer, Clock *clock, const int frame, 
                float *fit_scale, float user_scale, SDL_Texture *texture, SDL_Window *window) {
    //TODO: if clock->hour is more than 3 digits, it will overflow

    render_digit_at(renderer, clock->hour / 10, 0, (frame + 1) % NUMBER_OF_FRAMES, fit_scale, user_scale, texture, window);
    render_digit_at(renderer, clock->hour % 10, 1, (frame + 2) % NUMBER_OF_FRAMES, fit_scale, user_scale, texture, window);

    render_digit_at(renderer, 10, 2, frame, fit_scale, user_scale, texture, window);

    render_digit_at(renderer, clock->minute / 10, 3, (frame + 3) % NUMBER_OF_FRAMES, fit_scale, user_scale, texture, window);
    render_digit_at(renderer, clock->minute % 10, 4, (frame + 4) % NUMBER_OF_FRAMES, fit_scale, user_scale, texture, window);

    render_digit_at(renderer, 10, 5, frame, fit_scale, user_scale, texture, window);

    render_digit_at(renderer, clock->second / 10, 6, (frame + 2) % NUMBER_OF_FRAMES, fit_scale, user_scale, texture, window);
    render_digit_at(renderer, clock->second % 10, 7, (frame + 1) % NUMBER_OF_FRAMES, fit_scale, user_scale, texture, window);
}

int parse_clock_from_long(Clock *clock, const long time) {
    if(time < 0) return -1;

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

    //TODO: Think of a better way to parse these things
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
    } else { // Not suported format
        fprintf(stderr, "Too many arguments\n");
        fprintf(stderr, "See '-h' or '--help' for more information\n");
        exit(1);
    }

    return 0;
}
