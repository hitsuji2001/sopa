#ifndef __UTIL_H__
#define __UTIL_H__

#include <math.h>
#include <stdbool.h>
#include <string.h>

bool is_alpha(const char character);
long from_string_to_long(const char *string);
bool string_contains(const char *string, const char keyword);
int string_contains_more_than_one(const char *string, const char keyword);

#endif
