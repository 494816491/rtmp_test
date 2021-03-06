#ifndef UDP_TS_H
#define UDP_TS_H
#include "streamblock.h"

#ifdef __cplusplus
extern "C"{
#endif
#define MAX_RTMP_URL_LEN 1000

#include <libavcodec/avcodec.h>

typedef struct hiSIZE_S
{
    uint32_t u32Width;
    uint32_t u32Height;
} SIZE_S;

struct rtmp_chn_param_st{
    int chn_num;
    int video_frame_rate;
    int video_w;
    int video_h;
    char ip_addr[MAX_RTMP_URL_LEN];


    int audio_enable;
    int audio_sample_rate;
    int audio_bit_width;
    int audio_sound_mode;
    int audio_num_pre_frame;
};


int rtmp_h264_server_init();

int watch_rtmp_video_audio_is_alive(int chn);
int open_rtmp_stream(int chn);
int close_rtmp_stream(int chn);
int set_rtmp_chn_param(int chn, struct rtmp_chn_param_st *param);

int send_rtmp_video_stream(Mal_StreamBlock *block);
int send_rtmp_audio_stream( Mal_StreamBlock *block);

int send_rtmp_video_stream_packet(AVPacket *pkt, int chn_num);

#ifdef __cplusplus
}
#endif

#endif // UDP_TS_H
