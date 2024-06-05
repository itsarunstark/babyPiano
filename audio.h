#ifndef _AUDIO_H
#define _AUDIO_H

#include<SDL2/SDL.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct{
    Uint16 *stream;
    Uint8 added,index;
    Uint32 position;
    Uint32 length;

} PlayableChannel;

typedef struct{
    PlayableChannel **streams;
    Sint8 count,loaded,added;
    Uint32 position;
} AudioData;

typedef struct{
    Uint32 position;
    double freq;
    short added;
    short index;
    int amplitude;
    double duration;
    double length,threshold;
} AudioRaw;


typedef struct{
    Uint32 base_freq;
    AudioRaw *streams[88];
    Uint8 index,count,added;
    int time_counter;
} RawStreams;

Uint32 loadStreams(AudioData *data,FILE *fp);
PlayableChannel *LoadFromFile(AudioData *data,FILE *fp);
void releaseAllStreams(AudioData *data);
AudioData *audioInit();
void AudioManager(void *,Uint8 *stream,int len);
RawStreams *createRawStreams(Uint32 freq);
int removeRawStream(RawStreams *stream,AudioRaw *raw);
AudioRaw *createRawStream(RawStreams *stream, double freq,double duration);
int addRawStream(RawStreams *stream, AudioRaw *raw);
int clearAllStreams(RawStreams *stream);
#endif