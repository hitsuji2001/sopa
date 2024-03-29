#ifndef __UTIL_H__
#define __UTIL_H__

#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "./magic_constants.h"

bool is_alpha(const char character);
bool string_contains(const char *string, const char keyword);

long from_string_to_long(const char *string);

int get_number_of_digit(long number);
int get_digit(long number, int n);
int string_contains_more_than_one(const char *string, const char keyword);
int scc(int code);

void *scp(void *ptr);

#endif
