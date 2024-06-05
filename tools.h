#ifndef _TOOLS_H
#define _TOOLS_H

#include<SDL2/SDL.h>

void *throwSDLError();

typedef struct{
    SDL_Texture *texture;
    short available;
    int texture_width;
    int texture_height;
}Texture;


#endif //
