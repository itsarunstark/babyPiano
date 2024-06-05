#include<stdlib.h>
#include "window.h"
#include "tools.h"
#include "key.h"
#include<stdio.h>

int counter = 0;
Window *windows[10];
#define FPS 40

void dummyCallback(SDL_Event *eve){};

Window *createWindow(const char *title,int w,int h,Uint32 flags){
    Window *window = (Window *)malloc(sizeof(Window));
    if (!window) return NULL;
    windows[counter++] = window;

    window->window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,flags);
    if (!window->window) {cleanUp();return throwSDLError();}

    window->renderer = SDL_CreateRenderer(window->window,-1,SDL_RENDERER_ACCELERATED);  
    if (!window->renderer) {cleanUp();return throwSDLError();}

    window->window_surface = SDL_GetWindowSurface(window->window);
    if (!window->window_surface) {cleanUp();throwSDLError();}

    window->fps = FPS;
    window->exit_status = 0;
    window->eventCallback = dummyCallback;

    return window;

}


void cleanUp(){
    Window *window;
    while(counter--){
        window = windows[counter];
        if (window->window_surface) SDL_FreeSurface(window->window_surface);
        if (window->renderer) SDL_DestroyRenderer(window->renderer);
        if (window->window) SDL_DestroyWindow(window->window);
        free(window);
    }
}

void loopEvent(Window *window){
    SDL_Rect rect = {0,0,100,100};
    while(SDL_PollEvent(&window->event)){
        switch(window->event.type){
            case SDL_QUIT:
                window->exit_status = 1;
        }

        window->eventCallback(&window->event);
    }
    SDL_SetRenderDrawColor(window->renderer,window->background.r,window->background.g,
                            window->background.b,
                            window->background.a);
    SDL_RenderClear(window->renderer);
    SDL_SetRenderDrawColor(window->renderer,0,0,0,255);
    SDL_RenderFillRect(window->renderer,&rect);
    SDL_SetRenderDrawColor(window->renderer,50,100,50,255);
    rect.x = 20;
    rect.y = 20;
    SDL_RenderFillRect(window->renderer,&rect);
    SDL_Delay(1000./window->fps);
}
