#include "tools.h"
#include<stdio.h>

void *throwSDLError(){
    fprintf(stderr,"Error : %s\n",SDL_GetError());
    return NULL;
}