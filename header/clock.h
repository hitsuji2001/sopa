#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "./util.h"

typedef struct {
    int hour;
    int minute;
    int second;
    int tick;

    bool reverse;
    bool pause;
} Clock;

void clock_increase(Clock *clock);
void clock_decrease(Clock *clock);
void clock_advance(Clock *clock);
void clock_set(const Clock *src_clock, Clock *dst_clock);

void display_help();

float calculate_scaler(SDL_Window *window, float *fit_scale, float user_scale);

void get_initial_draw_position(int *x, int *y, const int order, SDL_Window *window, float *fit_scale, float user_scale);
void render_digit_at(SDL_Renderer *renderer, const int digit, const int order, const int frame, float *fit_scale, float user_scale, SDL_Texture *texture, SDL_Window *window);
void render_clock(SDL_Renderer *renderer, Clock *clock, const int frame, float *fit_scale, float user_scale, SDL_Texture *texture, SDL_Window *window);

int exceeded_time(int *time);

int from_clock_format_to_clock(Clock *clock, const char *string);
int from_human_readable_format_to_clock(Clock *clock, const char *string);

int parse_clock_from_long(Clock *clock, const long time);
int parse_clock_from_string(Clock *clock, const char *string);
int parse_flags_for_clock(Clock *clock, const char *string);
int parse_clock_from_cmd(Clock *clock, int argc, char **argv);

#endif
