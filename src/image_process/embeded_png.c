#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../../header/stb_image.h"

#define PNG_BITS 8

int main(void) {
    int width, height, bpp;
    const char *file_name = "./digits.png";

    uint32_t* rgb_image = (uint32_t*)stbi_load(file_name, &width, &height, &bpp, 4);
    
    if (rgb_image == NULL) {
        fprintf(stderr, "Could not load file `%s`\n", file_name);
        exit(1);
    }

    printf("#ifndef __DIGITS_H__\n");
    printf("#define __DIGITS_H__\n");

    printf("\n");
    printf("int png_width = %d;\n", width);
    printf("int png_height = %d;\n", height);
    printf("int png_depth = %d;\n", PNG_BITS * bpp);
    printf("int png_pitch = %ld;\n", width * sizeof(int));
    printf("\n");

    printf("uint32_t png_image[] = {");
    for (size_t i = 0; i < (size_t)(width * height); ++i) {
        printf("0x%x, ", rgb_image[i]);
    }
    printf("};\n");

    printf("\n#endif");

    stbi_image_free(rgb_image);

    return 0;
}
