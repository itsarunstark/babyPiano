#ifndef _KEY_H
#define _KEY_H
#include<SDL2/SDL.h>
#include "window.h"
#include "tools.h"
#include "audio.h"

enum KeyType{
    MINOR = 0,
    MAJOR = 1
};

typedef struct Key{
    char label[4];
    int id;
    SDL_Rect rect,font_rect,active_rect,hover_rect;
    SDL_Rect *current_rect;
    SDL_Color color;
    int VERSION;
    void *soundmap;
    enum KeyType t;
    short clicked,hovered,state_changed;
    void (*onclick)(int);
    AudioRaw *stream;
    float freq;
    int minor_pointer_attached;
    struct Key *minor_key;
    SDL_Texture *background_texture;
    float threshold;
    Texture font_texture;
}Key;

extern SDL_Rect minorDim,majorDim;

extern int keyCounter;

extern SDL_Texture *background_texture_minor, 
            *background_texture_major;

Key *createKey(const char *_label,int id,short minor);
int initKeyTexture(Window *window);
void deleteKeys();


#endif // _KEY_H
