#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <stdio.h>

#include "./util.h"

typedef struct {
    int hour;
    int minute;
    int second;
    int fake_second;
    bool reverse;
    bool pause;
} Clock;

void print_clock(const Clock *clock);
void debug_clock(const Clock *clock);
void increase_clock(Clock *clock);
void decrease_clock(Clock *clock);
void advance_clock(Clock *clock);

int exceeded_time(int *time);

int parse_clock_from_long(Clock *clock, const long time);
int from_clock_format_to_clock(Clock *clock, const char *string);
int from_human_readable_format_to_clock(Clock *clock, const char *string);
int from_string_to_clock(Clock *clock, const char *string);

#endif
