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

bool is_alpha(const char character) {
    return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'));
}

long from_string_to_long(const char *string) {
    long number = 0;
    for(size_t i = 0; i < strlen(string); ++i) {
        if(is_alpha(string[i])) {
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
    size_t i = 0;
    size_t j = 0;
    char buffer[10] = {0};
    long attri[3] = {0};

    while(index < strlen(string)) {
        if(string[index] == ':') {
            i = 0;
            attri[j] = from_string_to_long(buffer);
            j++;
        } else buffer[i++] = string[index];
        index++;
    }
    attri[2] = from_string_to_long(buffer);

    clock->hour = attri[0];
    clock->minute = attri[1];
    clock->second = attri[2];

    clock->minute += exceeded_time(&clock->second);
    clock->hour += exceeded_time(&clock->minute);
    
    return 0;
}

void rename_this_later(Clock *clock, const char character, const char *buffer) {
    switch (character) {
        case 'h':
            clock->hour = from_string_to_long(buffer);
            break;
        case 'm':
            clock->minute = from_string_to_long(buffer);
            break;
        case 's':
            clock->second = from_string_to_long(buffer);
            break;
        default:
            break;
    }
}

int from_human_readable_format_to_clock(Clock *clock, const char *string) {
    if((!string_contains(string, 'h') && !string_contains(string, 'm') && !string_contains(string, 's')) || string_contains(string, '-')) {
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
        if(is_alpha(string[index])) {
            rename_this_later(clock, string[index], buffer);
            i = 0;
        } else buffer[i++] = string[index];
        index++;
    }

    clock->minute += exceeded_time(&clock->second);
    clock->hour += exceeded_time(&clock->minute);

    return 0;
}

int from_string_to_clock(Clock *clock, const char *string) {
    return from_clock_format_to_clock(clock, string) || from_human_readable_format_to_clock(clock, string);
}

//TODOO: expected format ./sopa [flags] [time]
//                              argv[1] argv[2 -> n]

//TODOOO: set up arguments
// %lld           : start the clock from %lld seconds - DONE
// %02d:%02d:%02d : start the clock from %02d:%02d:%02d - DONE
// %dh            : start the clock from %d hours - DONE
// %dm            : start the clock from %d minutes - DONE
// %ds            : start the clock from %d seconds - DONE
// %dh%m%s        : start the clock from %d hours %d minutes %d seconds - DONE
// --NOTE: these 3 can be combined - NOT TESTED YET

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
        if(from_human_readable_format_to_clock(&clock, argv[1]) < 0) {
            printf("Failed to convert from string to Clock\n");
        } else print_clock(&clock);
    }
#endif

    return 0;
}
