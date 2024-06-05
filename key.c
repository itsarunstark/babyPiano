#include "key.h"
#include <SDL2/SDL_ttf.h>
#include "window.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
int keyCounter = 0;
SDL_Color majorColor = {255,255,255,255};
SDL_Color minorColor = {0,0,0,255};

SDL_Rect minorDim,majorDim;
Key *KeyMaps[88];

SDL_Texture *background_texture_minor,*background_texture_major;

static int __create_background_texture(SDL_Renderer *renderer){
    background_texture_major =  SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,majorDim.w,majorDim.h);
    if(!background_texture_major) return -1;
    background_texture_minor =  SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,minorDim.w,minorDim.h);
    if(!background_texture_minor) {SDL_DestroyTexture(background_texture_major);return -1;};
    return 0;
}

int initKeyTexture(Window *window){
    SDL_Color temp;
    if(__create_background_texture(window->renderer)){printf("SDL Error %s\n",SDL_GetError());return -1;}
    SDL_GetRenderDrawColor(window->renderer,&temp.r,&temp.g,&temp.b,&temp.a);
    SDL_SetRenderTarget(window->renderer,background_texture_major);
    SDL_SetRenderDrawColor(window->renderer,majorColor.r,majorColor.g,majorColor.b,majorColor.a);
    SDL_RenderClear(window->renderer);
    SDL_SetRenderTarget(window->renderer,background_texture_minor);
    SDL_SetRenderDrawColor(window->renderer,minorColor.r,minorColor.g,minorColor.b,minorColor.a);
    SDL_RenderClear(window->renderer);
    SDL_SetRenderDrawColor(window->renderer,temp.r,temp.g,temp.b,temp.a);
    SDL_SetRenderTarget(window->renderer,NULL);
    printf("Error: %s\n",SDL_GetError());
    return 0;
}


Key *createKey(const char *_label,int id,short minor){
    Key *key = (Key *)malloc(sizeof(Key));
    if (minor){
        key->background_texture = background_texture_minor;
        SDL_memcpy(&key->rect,&minorDim,sizeof(SDL_Rect));
    }
    else{
        key->background_texture = background_texture_major;
        SDL_memcpy(&key->rect,&majorDim,sizeof(SDL_Rect));
    }
    key->rect.x = 0;
    key->rect.y = 0;
    key->clicked = 0;
    key->hovered = 0;
    key->state_changed = 0;
    key->id = id;
    key->font_texture.available = 0;
    key->minor_pointer_attached = 0;
    if(!strcpy(key->label,_label)){
        fprintf(stderr,"Error while creating key %s\n",_label);
    };

    KeyMaps[keyCounter++] = key;
    return key;

}


void deleteKeys(){
    Key *key;
    while(keyCounter--){
        key = KeyMaps[keyCounter];
        if(key->font_texture.available) SDL_DestroyTexture(key->font_texture.texture);
    }
    SDL_DestroyTexture(background_texture_major);
    SDL_DestroyTexture(background_texture_minor);
    printf("Cleared Major Key and Minor key textures.\n");
}
