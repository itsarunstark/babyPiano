#include "audio.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
int init = 0;


void AudioManager(void *userdata,Uint8 *stream,int len){

    //printf("Audio Manager ki jay.\n");
    RawStreams *data = (RawStreams *)userdata;
    // printf("%p\n",data);
    AudioRaw *raw;
    Sint32 *p= (Sint32 *)stream;
    double x;
    // printf("Reached here.\n");
    // printf("LENGTH OF clsRAW_AUDIO: %d\n",data->added);
    for(int i=0;i<len/4;i++){
        p[i] = 0;
        //printf("%p\n",p+i);
        for(int j=0;j<data->count;j++){
            // printf("%d\n",x);
            raw = data->streams[j];
            x = raw->position/raw->length;
            if(raw->added){
                p[i] += raw->amplitude*cos(2*M_PI*raw->freq*x*raw->duration)*exp(-x*14);
                raw->position += 1;
            }else if (raw->threshold > 0){
                p[i] += raw->threshold*raw->amplitude*cos(2*M_PI*raw->freq*x*raw->duration)*exp(-x*20);
                raw->position += 1;
                // printf("Raw threshold: %f\n",raw->threshold);
                raw->threshold -= 0.0001;
            }else{
                raw->threshold = 0;
            }
            if (raw->position>=raw->length){
                    raw->added = 0;
                    raw->threshold = 0;
                    raw->position = 0;
            }
        }
    }
    // printf("This is work.\n");
}


RawStreams *createRawStreams(Uint32 freq){
    RawStreams *stream = (RawStreams *)malloc(sizeof(RawStreams));
    if(!stream) return NULL;
    stream->base_freq = freq;
    stream->count = 0;
    stream->index = 0;
    stream->added = 0;
    stream->time_counter = 0;
    return stream;
}

AudioRaw *createRawStream(RawStreams *stream, double freq,double duration){
    AudioRaw *raw = (AudioRaw *)malloc(sizeof(AudioRaw));
    raw->added = 0;
    raw->freq = freq;
    raw->duration = duration;
    raw->length = stream->base_freq*raw->duration;
    raw->amplitude = INT32_MAX/4;
    raw->index = 0;
    raw->position = 0;
    stream->streams[stream->count] = raw;
    stream->count++;
    raw->index++;
    return raw;
}

int addRawStream(RawStreams *stream, AudioRaw *raw){
    // printf("Added to stream.\n");
    if(!raw->added)
        stream->added += 1;
    raw->added = 1;
    return 1;
}


int removeRawStream(RawStreams *stream,AudioRaw *raw){
    // printf("Removed from stream\n");
    if(raw->added)
        stream->added--;
    raw->added = 0;
    raw->position = 0;
    return raw->added;
}

int clearAllStreams(RawStreams *stream){
    for(int i=0;i<stream->count;i++){
        free(stream->streams[i]);
        printf("Oh stream #%d cleared.\n",i);
    }
    free(stream);
    printf("Freed every stream.\n");
    return 1;
}

AudioData *audioInit(){
    if (init) return NULL;
    init = 1;
    AudioData *data = (AudioData *)malloc(sizeof(AudioData));
    data->count = 0;
    data->position = 0;
    data->loaded = 0;
    data->added = 0;
    return data;
}

Uint32 loadStreams(AudioData *data,FILE *fp){
    if(fread(&data->count,sizeof(Uint32),1,fp)) {
        data->streams = (PlayableChannel **)malloc(sizeof(PlayableChannel *)*(data->count));
        return data->count;
    }
    else return 0;
}


PlayableChannel *LoadFromFile(AudioData *data,FILE *fp){
    PlayableChannel *channel = (PlayableChannel *)malloc(sizeof(PlayableChannel));
    if(!channel) return NULL;
    if(!fread(&channel->length,sizeof(Uint32),1,fp)) {free(channel);return NULL;}
    channel->length /= 2;
    channel->stream = (Uint16 *)calloc(channel->length,sizeof(Uint16));
    if(!channel->stream)  {free(channel);return NULL;}
    fread(channel->stream,sizeof(Uint16),channel->length,fp);
    data->streams[data->loaded] = channel;
    channel->index = data->loaded;
    channel->position = 0;
    channel->added = 0;
    data->loaded++;
    return channel;
}

void releaseAllStreams(AudioData *data){
    for(int i=0;i<data->loaded;i++){
        free(data->streams[i]->stream);
        free(data->streams[i]);
        printf("Freed channel #%d\n",i);
    }
    free(data->streams);
    free(data);
    printf("Freed streamer");
}
