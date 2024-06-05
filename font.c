#include "font.h"
#include "window.h"
#include<stdio.h>
#include "tools.h"


Font *fontMaps[20];
int fontCounters = 0;

void initFont(){
    if(TTF_Init()) printf("SDL Font initialization failed. %s\n",TTF_GetError());
    TTF_Font *font;
}


Font *loadFont(const char *name){
    Font *font = malloc(sizeof(Font));
    if(!font) {printf("Font initalization failed.\n");return NULL;};
    font->font12 = TTF_OpenFont(name,12);
    font->font16 = TTF_OpenFont(name,16);
    font->font20 = TTF_OpenFont(name,20);
    font->font32 = TTF_OpenFont(name,32);
    fontMaps[fontCounters++] = font;
    return font;
}

int createTexture(Window *window,Texture *font_ptr,TTF_Font *font,const char *text,SDL_Color color){
    // printf("Reached here %s\n",text);
    if(font_ptr->available) SDL_DestroyTexture(font_ptr->texture);
    SDL_Surface *tmp = TTF_RenderUTF8_Blended(font,text,color);
    if(!tmp) {printf("Error while Generating text %s with error %s\n",text,TTF_GetError());return -1;};
    // printf("Reach approved.\n");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(window->renderer,tmp);
    // printf("Reached here \n");
    font_ptr->texture = texture;
    font_ptr->available = 1;
    font_ptr->texture_width = tmp->w;
    font_ptr->texture_height = tmp->h;
    SDL_FreeSurface(tmp);
    return 1;
}

void cleanFonts(){
    Font *font;
    while(fontCounters--){
        font = fontMaps[fontCounters];
        TTF_CloseFont(font->font12);
        TTF_CloseFont(font->font16);
        TTF_CloseFont(font->font20);
        TTF_CloseFont(font->font32);
    }
}