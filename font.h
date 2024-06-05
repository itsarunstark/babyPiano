#ifndef _FONT_H
#define _FONT_H
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include "window.h"
#include "tools.h"

typedef struct{
    TTF_Font *font12,*font16,*font20,*font32;

}Font;

void initFont();
void cleanFonts();
int createTexture(Window *window,Texture *font_ptr,TTF_Font *font,const char *text,SDL_Color color);
Font *loadFont(const char *name);


#endif //