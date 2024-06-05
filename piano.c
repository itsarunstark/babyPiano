#include "window.h"
#include "key.h"
#include "font.h"
#include "audio.h"
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <string.h>

#define WIDTH 1366
#define HEIGHT 500
#define RATE 16000
#define DURATION 10

Font *font;
Window *window;
Key *current_key;

float frequecy_major[] = {
    65.40639,
    73.41619,
    77.78175,

};

int current_key_available;

Uint8 keys_major_state[] = {
    SDL_SCANCODE_ESCAPE, //C2
    SDL_SCANCODE_F1, //D2
    SDL_SCANCODE_F2, //E2
    SDL_SCANCODE_F3, //F2
    SDL_SCANCODE_F4, //G2
    SDL_SCANCODE_F5, //A3
    SDL_SCANCODE_F6, //B3
    SDL_SCANCODE_F7, //C3
    SDL_SCANCODE_F8, //D3
    SDL_SCANCODE_F9, //E3
    SDL_SCANCODE_F10, //F3
    SDL_SCANCODE_F11, //G3
    SDL_SCANCODE_GRAVE, //A4
    SDL_SCANCODE_1, //B4
    SDL_SCANCODE_2, //C4
    SDL_SCANCODE_3, //D4
    SDL_SCANCODE_4, //E4
    SDL_SCANCODE_5, //F4
    SDL_SCANCODE_6, //G4
    SDL_SCANCODE_7, //A5
    SDL_SCANCODE_8, //B5
    SDL_SCANCODE_9, //C5
    SDL_SCANCODE_0, //D5
    SDL_SCANCODE_MINUS, //E5
    SDL_SCANCODE_EQUALS, //F5
    SDL_SCANCODE_BACKSPACE, //G5
    SDL_SCANCODE_TAB, //A6
    SDL_SCANCODE_Q, //B6
    SDL_SCANCODE_W, //C6
    SDL_SCANCODE_E, //D6
    SDL_SCANCODE_R, //E6
    SDL_SCANCODE_T, //F6
    SDL_SCANCODE_Y, //G6
    SDL_SCANCODE_U, //A7
    SDL_SCANCODE_I, //B7
    SDL_SCANCODE_O //C7
};

Uint8 keys_minor_state[] = {
    SDL_SCANCODE_P, //C2#
    SDL_SCANCODE_LEFTBRACKET, //D2#
    SDL_SCANCODE_RIGHTBRACKET, //F2#
    SDL_SCANCODE_A, //G2#
    SDL_SCANCODE_S, //A3#
    SDL_SCANCODE_D, //C3#
    SDL_SCANCODE_F, //D3#
    SDL_SCANCODE_G, //F3#
    SDL_SCANCODE_H, //G3#
    SDL_SCANCODE_J, //A4#
    SDL_SCANCODE_K, //C4#
    SDL_SCANCODE_L, //D4#
    SDL_SCANCODE_SEMICOLON, //F4#
    SDL_SCANCODE_APOSTROPHE, //G4#
    SDL_SCANCODE_RETURN, //A5#
    SDL_SCANCODE_Z, //C5#
    SDL_SCANCODE_X, //D5#
    SDL_SCANCODE_C, //F5#
    SDL_SCANCODE_V, //G5#
    SDL_SCANCODE_B, //A6#
    SDL_SCANCODE_N, //C6#
    SDL_SCANCODE_M, //D6#
    SDL_SCANCODE_COMMA, //F6#
    SDL_SCANCODE_PERIOD, //G6#
    SDL_SCANCODE_SLASH, //A7#
    SDL_SCANCODE_SPACE //C7#
};

int key_id = 28;

typedef struct
{
    SDL_Rect calc;
    short current_key_available;
    Key *current_key;
    int mouse_x, mouse_y;
    int majorKeys,minorKeys;
    Key **majors;
    Key **minors;
    Key *_tmpKey;
    const Uint8 *state;
    RawStreams *stream;

} RuntimeData;

RuntimeData data;

short containsPoint(SDL_Rect *region,int x,int y){
    if(region->x < x && region->x+region->w > x && region->y < y && region->y + region->h > y)
        return 1;
    return 0;
}

float getFreq(int n){
    return 440*pow(2,(n-49)/12.);
}

Key *create_minor_key(Key *major, int id)
{
    char label[4];
    strcpy(label, major->label);
    label[2] = '#';
    label[3] = '\0';
    Key *key = createKey(label, id, 1);
    key->id = key_id++;
    key->rect.w = (3 * major->rect.w) / 4;
    key->rect.x = major->rect.x + major->rect.w - key->rect.w / 2;
    key->rect.y = major->rect.y;
    key->rect.h = major->rect.h - 150;
    key->active_rect.x = key->rect.x + 2;
    key->active_rect.y = key->rect.y;
    key->active_rect.h = key->rect.h - 10;
    key->active_rect.w = key->rect.w - 4;
    key->freq = getFreq(key->id);
    key->stream = createRawStream(data.stream,key->freq,DURATION);
    key->hover_rect.x = key->rect.x - 5;
    key->hover_rect.y = key->rect.y;
    key->hover_rect.h = key->rect.h + 10;
    key->hover_rect.w = key->rect.w + 10;
    key->current_rect = &key->rect;
    if (createTexture(window, &key->font_texture, font->font12, key->label, (SDL_Color){255, 255, 255, 255}))
    {
        key->font_rect.x = key->rect.x + key->rect.w / 2 - key->font_texture.texture_width / 2;
        key->font_rect.y = key->rect.y + key->rect.h - key->font_texture.texture_height - 5;
        key->font_rect.w = key->font_texture.texture_width;
        key->font_rect.h = key->font_texture.texture_height;
    }

    return key;
}


int create_major_keys(Key **keys, int size, char start1, char start2, Key **minors, int *minorCounter)
{
    int offset = start1 - 'A';
    char label[4];
    label[2] = '\0';
    Key *key;
    int counter = 0;
    for (int i = offset; i < size + offset; i++, counter++)
    {
        if (!(i % 7) && i)
            start2++;
        label[0] = 'A' + i % 7;
        label[1] = ('C' > label[0]) ? start2 -1 : start2 ;
        key = createKey(label, i, 0);
        key->id = key_id++;
        key->rect.x = counter * (1 + key->rect.w);
        key->rect.w -= 2;
        key->active_rect.x = key->rect.x + 2;
        key->active_rect.y = key->rect.y;
        key->active_rect.h = key->rect.h - 10;
        key->active_rect.w = key->rect.w - 4;
        key->hover_rect.x = key->rect.x - 5;
        key->hover_rect.y = key->rect.y;
        key->hover_rect.h = key->rect.h + 10;
        key->hover_rect.w = key->rect.w + 10;
        key->freq = getFreq(key->id);
        key->stream = createRawStream(data.stream,key->freq,DURATION);
        createTexture(window, &key->font_texture, font->font12, label, (SDL_Color){0, 0, 0, 255});
        key->current_rect = &key->rect;
        key->font_rect.w = key->font_texture.texture_width;
        key->font_rect.h = key->font_texture.texture_height;
        key->font_rect.x = key->current_rect->x + key->current_rect->w / 2 - key->font_texture.texture_width / 2;
        key->font_rect.y = key->current_rect->y + key->current_rect->h - key->font_texture.texture_height - 5;

        *(keys++) = key;
        if (label[0] == 'C' || label[0] == 'D' || label[0] == 'F' || label[0] == 'G' || label[0] == 'A')
        {
            (*minors) = create_minor_key(key, key->id + 100);
            key->minor_key = *minors;
            key->minor_pointer_attached = 1;
            minors++;
            (*minorCounter)++;
        }
    }
}

void renderKeys(Window *window, Key **m_keys, int keys)
{
    Key *key;
    for (int i = 0; i < keys; i++)
    {
        key = m_keys[i];
        if(key->state_changed){
            key->current_rect = (key->clicked) ? &key->active_rect : (key->hovered) ? &key->hover_rect : &key->rect;
            key->font_rect.x = key->current_rect->x + key->current_rect->w / 2 - key->font_texture.texture_width / 2;
            key->font_rect.y = key->current_rect->y + key->current_rect->h - key->font_texture.texture_height - 5;
            key->state_changed = 0;
        }
        if(!key->current_rect) key->current_rect = &key->rect;
        SDL_RenderCopy(window->renderer, key->background_texture, NULL, key->current_rect);
        SDL_SetRenderDrawColor(window->renderer,0,0,0,0);
        SDL_RenderDrawRect(window->renderer,key->current_rect);
        SDL_SetRenderDrawColor(window->renderer,255,255,255,255);
        if (key->font_texture.available)
        {
            SDL_RenderCopy(window->renderer, key->font_texture.texture, NULL, &key->font_rect);
        }
    }
}

void handleKeyEvent(SDL_Event *event)
{
    if (event->type == SDL_MOUSEMOTION) 
    {
        SDL_GetMouseState(&data.mouse_x, &data.mouse_y);
        // printf("Mouse Motion");
        if (data.current_key_available)
        {
            // printf("yeah.\n");
            if (!containsPoint(data.current_key->current_rect,data.mouse_x,data.mouse_y))
            {
                // printf("yeah2.\n");
                data.current_key_available = 0;
                data.current_key->hovered = 0;
                data.current_key->clicked = 0;
                data.current_key->state_changed = 1;
            }

        }

        for (int i = 0; (i < data.minorKeys); i++)
        {
            data._tmpKey = data.minors[i];
            // printf("worded.\n");
            if (containsPoint(data._tmpKey->current_rect,data.mouse_x,data.mouse_y))
            {
                if (data.current_key_available && data.current_key != data._tmpKey){
                    data.current_key->hovered = 0;
                    data.current_key->clicked = 0;
                    data.current_key->state_changed = 1;
                }
                data.current_key = data._tmpKey;
                data.current_key_available = 1;
                data.current_key->hovered = 1;
                data.current_key->state_changed = 1;
            }
        }


        for (int i = 0; (i < data.majorKeys) && !data.current_key_available; i++)
        {
            data._tmpKey = data.majors[i];
            // printf("worded.\n");
            if (containsPoint(data._tmpKey->current_rect,data.mouse_x,data.mouse_y))
            {
                data.current_key = data._tmpKey;
                data.current_key_available = 1;
                data.current_key->hovered = 1;
                data.current_key->state_changed = 1;
            }
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN){
        SDL_GetMouseState(&data.mouse_x, &data.mouse_y);
        if (data.current_key_available && containsPoint(data.current_key->current_rect,data.mouse_x,data.mouse_y))
            if(data.current_key->current_rect != &data.current_key->active_rect){
                data.current_key->stream->added = 1;
                data.current_key->current_rect = &data.current_key->active_rect;
                data.current_key->stream->position = 0;
                data.current_key->font_rect.x = data.current_key->current_rect->x + data.current_key->current_rect->w / 2 - data.current_key->font_texture.texture_width / 2;
                data.current_key->font_rect.y = data.current_key->current_rect->y + data.current_key->current_rect->h - data.current_key->font_texture.texture_height - 5;
                // data.current_key->state_changed = 1;
            }
    }

    if (event->type == SDL_MOUSEBUTTONUP){
        if(data.current_key_available && data.current_key->current_rect == &data.current_key->active_rect){
            data.current_key->current_rect = &data.current_key->hover_rect;
            data.current_key->state_changed = 1;
        }
    }
}

int main(int argc, char const *argv[])
{
    //putenv("SDL_AUDIODRIVER=directsound");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_AudioSpec spec;
    SDL_AudioDeviceID dev;

    spec.callback = AudioManager;
    spec.channels = 1;
    spec.format = AUDIO_S32LSB;
    spec.freq = 22100;
    spec.padding = 0;
    spec.samples = 4096;
    spec.silence = 0;
    spec.size = 0;
    if(!dev) {printf("Audio device opening failed.\n") ; return -1;}
    data.stream = createRawStreams(spec.freq);
    // data.stream->added = 0;
    // printf("%d\n",data.stream->added);
    window = createWindow("Piano", WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    initFont();
    font = loadFont("font.ttf");
    spec.userdata = (void *)data.stream;
    dev = SDL_OpenAudioDevice(NULL,0,&spec,NULL,SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    SDL_PauseAudioDevice(dev,0);

    int totalMinors = 0;
    data.current_key_available = 0;
    data.state = SDL_GetKeyboardState(NULL);
    int nMk = 36, nMck = 10, nMfk = 15;
    SDL_Rect src = {0, 0, 100, 100};
    SDL_Rect dst = {0, 0, 100, 100};
    // printf("%d\n",data.stream->added);


    majorDim.w = WIDTH / nMk;
    majorDim.h = HEIGHT - 100;
    majorDim.x = 0;
    majorDim.y = 0;
    minorDim.w = WIDTH / nMk;
    minorDim.h = HEIGHT;
    Texture man = {.available=0};
    initKeyTexture(window);
    createTexture(window,&man,font->font32,"BABY KA PIANO",(SDL_Color){255,255,255,255});
    SDL_Rect m_rect;
    m_rect.w = man.texture_width;
    m_rect.h = man.texture_height;
    m_rect.x = WIDTH/2 - m_rect.w/2;
    m_rect.y = HEIGHT - m_rect.h - 20;

    printf("rect_val:  %d %d\n",m_rect.w,m_rect.h);

    window->background.r = 50;
    window->background.g = 50;
    window->background.b = 100;
    window->background.a = 255;

    Key *major_keys[nMk];
    Key *minor_keys[nMck + nMfk + 1];

    data.majorKeys = nMk;
    data.majors = major_keys;

    create_major_keys(major_keys, 36, 'C', '3', minor_keys, &totalMinors);

    data.minors = minor_keys;
    data.minorKeys = totalMinors;

    Key *active_key;
    Key *current_key;
    Key *key;
    for(int i=0;i<nMk;i++){
        key = major_keys[i];
        printf("%s : %f\n",key->label,key->freq);
    }

    window->eventCallback = handleKeyEvent;
    printf("Render rect: %d %d %d %d\n",m_rect.x,m_rect.y,m_rect.w,m_rect.h);
    while (!window->exit_status)
    {
        for(int i=0;i<nMk;i++){
            if(major_keys[i]->clicked != data.state[keys_major_state[i]] ){
                major_keys[i]->clicked = (data.current_key_available && data.current_key == major_keys[i]) ? major_keys[i]->clicked : data.state[keys_major_state[i]];
                major_keys[i]->state_changed = 1;
                if(major_keys[i]->clicked){
                    major_keys[i]->stream->added = 1;
                    major_keys[i]->stream->position = 0;   
                    major_keys[i]->stream->threshold = 1;
                }else{
                    major_keys[i]->stream->added = 0;
                }
            }
        }
        for(int i=0;i<totalMinors;i++){
            if(minor_keys[i]->clicked != data.state[keys_minor_state[i]] ){
                minor_keys[i]->clicked = (data.current_key_available && data.current_key == minor_keys[i]) ?minor_keys[i]->clicked : data.state[keys_minor_state[i]];
                minor_keys[i]->state_changed = 1;
                if(minor_keys[i]->clicked){
                    minor_keys[i]->stream->added = 1;
                    minor_keys[i]->stream->position = 0;  
                    minor_keys[i]->stream->threshold = 1; 
                }else{
                    minor_keys[i]->stream->added = 0;
                }
            }
        }

        loopEvent(window);
        renderKeys(window, major_keys, nMk);
        renderKeys(window, minor_keys, totalMinors);
        SDL_RenderCopy(window->renderer,man.texture,NULL,&m_rect);
        SDL_RenderPresent(window->renderer);

    }

    SDL_DestroyTexture(man.texture);
    clearAllStreams(data.stream);
    SDL_CloseAudioDevice(dev);
    cleanUp();
    cleanFonts();
    deleteKeys();
    SDL_Quit();
    return 0;
}
