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
    if(clock->fake_second < 60) clock->fake_second++;
    else {
        clock->fake_second = 0;
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
}

void decrease_clock(Clock *clock) {
    if(clock->second == 0 && clock->minute == 0 && clock->hour == 0) return;

    if(clock->fake_second > 0) clock->fake_second--;
    else {
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
}

void advance_clock(Clock *clock) {
    if(clock->pause) return;

    if(!clock->reverse) increase_clock(clock);
    else decrease_clock(clock);

    return;
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
    if(!string_contains(string, ':') || string_contains(string, '-')) {
        clock = NULL;
        return -1;
    }
    
    size_t index = 0;
    size_t i = 0;
    size_t j = 0;
    char buffer[10] = {0};
    long attri[3] = {0};

    if(string_contains_more_than_one(string, ':')) {
        while(index < strlen(string)) {
            if(is_alpha(string[index])) {
                clock = NULL;
                return -1;
            }
            if(string[index] == ':') {
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
    if((!string_contains(string, 'h') &&
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
        if(is_alpha(string[index])) {
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

int from_string_to_clock(Clock *clock, const char *string) {
    if(string_contains(string, ':') && (string_contains(string, 'h') || string_contains(string, 'm') || string_contains(string, 's') || string_contains(string, 'd'))) return -1;
    else if(string_contains(string, ':')) return from_clock_format_to_clock(clock, string);
    else return from_human_readable_format_to_clock(clock, string);
}
