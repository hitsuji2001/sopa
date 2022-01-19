#include "../../header/util.h"

bool is_alpha(const char character) {
    return ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'));
}

bool string_contains(const char *string, const char keyword) {
    for(size_t i = 0; i < strlen(string); ++i) {
        if(string[i] == keyword) return true;
    }

    return false;
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

int string_contains_more_than_one(const char *string, const char keyword) {
    int count = 0;
    for(size_t i = 0; i < strlen(string); ++i) {
        if(string[i] == keyword) count++;
    }

    return count;
}

int scc(int code) {
    if (code < 0) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }

    return code;
}

void *scp(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }

    return ptr;
}
