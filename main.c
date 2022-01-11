#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGTH 600

typedef struct {
    int hour;
    int minute;
    int second;
} Clock;

void print_clock(const Clock *clock) {
    printf("%02d:%02d:%02d\n", clock->hour, clock->minute, clock->second);
}

void increase_clock(Clock *clock) {
    if (clock->second < 60) clock->second++;
    else {
        clock->second = 0;
        if(clock->minute < 60) clock->minute++;
        else {
            clock->minute = 0;
            clock->hour++;
        }
    }
}

void decrease_clock(Clock *clock) {
    if(clock->second == 0 && clock->minute == 0 && clock->hour == 0) return;

    if (clock->second > 0) clock->second--;
    else {
        clock->second = 59;
        if(clock->minute > 0) clock->minute--;
        else {
            clock->minute = 59;
            clock->hour--;
        }
    }
}

Clock parse_clock_from_long(const long time) {
    Clock clock;

    clock.hour = time / (60 * 60);
    clock.minute = (time % (60 * 60)) / 60;
    clock.second = (time % (60 * 60)) % 60;

    return clock;
}

bool isAlpha(const char character) {
    return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'));
}

bool isNumeric(const char character) {
    return (character >= '0') && (character <= '9');
}

long from_string_to_long(const char *string) {
    long number = 0;
    for(size_t i = 0; i < strlen(string); ++i) {
        if(isAlpha(string[i])) {
            return -1; // Failed to parse long
        } else number += (string[i] - '0') * pow(10,(strlen(string) - i - 1));
    }

    return number;
}

int exceeded_time(int *time) {
    int res = *time / 60;
    *time %= 60;
    return res;
}

bool string_contains(const char *string, const char keyword) {
    for(size_t i = 0; i < strlen(string); ++i) {
        if(string[i] == keyword) return true;
    }

    return false;
}

int from_clock_format_to_clock(Clock *clock, const char *string) {
    if(!string_contains(string, ':') || string_contains(string, '-')) {
        clock = NULL;
        return -1;
    }
    
    size_t index = 0;
    char c_hours[10]  = {0};
    char c_minutes[3] = {0};
    char c_seconds[3] = {0};

    for(int i = 0; index < strlen(string); ++index) {
        if(string[index] == ':') {
            index++; break;
        } else c_hours[i++] = string[index];
    }
    clock->hour = from_string_to_long(c_hours);

    for(int i = 0; index < strlen(string); ++index) {
        if(string[index] == ':') {
            index++; break;
        } else c_minutes[i++] = string[index];
    }
    clock->minute = from_string_to_long(c_minutes);

    for(int i = 0; index < strlen(string); ++index) c_seconds[i++] = string[index];
    clock->second = from_string_to_long(c_seconds);

    clock->minute += exceeded_time(&clock->second);
    clock->hour += exceeded_time(&clock->minute);
    
    return 0;
}

int from_human_readable_format_to_clock(Clock *clock, const char *string) {
    if(!string_contains(string, 'h') || !string_contains(string, 'm') || !string_contains(string, 's')) {
        clock = NULL;
        return -1;
    }
}

int from_string_to_clock(Clock *clock, const char *string) {
    return from_clock_format_to_clock(clock, string) || from_human_readable_format_to_clock(clock, string);
}

//TODOO: expected format ./sopa [flags] [time]
//                              argv[1] argv[2 -> n]

//TODOOO: set up arguments
// %lld           : start the clock from %lld seconds - DONE
// %02d:%02d:%02d : start the clock from %02d:%02d:%02d - WORKING
// %dh            : start the clock from %d hours
// %dm            : start the clock from %d minutes
// %ds            : start the clock from %d seconds
// %dh%m%s        : start the clock from %d hours %d minutes %d seconds
// --NOTE: these 3 can be combined

//TODOO: set up flags
// -r    : reverse the clock from %lld seconds or the format above
// --help or -help: display help message
int main(int argc, char **argv) {
#if 0
    (void)argc; (void) argv;
#else
    if(argc > 1) {
        //char *number = argv[1];
        Clock clock;
        //clock = parse_clock_from_long(from_string_to_long(number));
        if(from_clock_format_to_clock(&clock, argv[1]) < 0) {
            printf("Failed to convert from string to Clock\n");
        } else print_clock(&clock);
    }
#endif

    return 0;
}
