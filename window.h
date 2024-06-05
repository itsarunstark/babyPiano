#ifndef _WINDOW_H
#define _WINDOW_H
#define SDL_MAIN_HANDLED
#include<SDL2/SDL.h>


typedef struct{
    char *title;
    short exit_status;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int fps,width,height;
    SDL_Event event;
    SDL_Surface *window_surface;
    SDL_Color background;
    void (*eventCallback)(SDL_Event *);
}Window;


Window *createWindow(const char *title,int w,int h,Uint32 flags);
void loopEvent(Window *);
void cleanUp();



#endif // _WINDOW_H
