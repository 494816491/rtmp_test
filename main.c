#include <stdio.h>
#include <unistd.h>
#include <libavformat/avformat.h>
#include <pthread.h>
#include <libavutil/timestamp.h>
#include "rtmp_h264.h"


#define RTMP_H264_CHN_NUM 1
#define MAX_RTMP_URL_LEN 1000


static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}


int main()
{

#if 1
    av_register_all();
    avformat_network_init();

#if 1 // output init
    rtmp_h264_server_init();

        //初始化rtmp
        struct rtmp_chn_param_st param = {0};
        param.chn_num = 0;
#if 0
        //sprintf(, "rtmp://192.168.22.2/live/chn0");
        //status_get_rtmp_addr(param.ip_addr);
#else
        //sprintf(param.ip_addr, "rtmp://ps3.live.panda.tv/live_panda/fdd3dac64c9f2df18898a695b1b2bfa5?sign=1467df8ab3fe03ec2ce4d29049253f12&time=1513438213&wm=2&wml=1&vr=6&extra=0");
        //sprintf(param.ip_addr, "rtmp://127.0.0.1/live/chn0");
        //sprintf(param.ip_addr, "rtmp://192.168.0.111/live/chn0");
        sprintf(param.ip_addr, "rtmp://192.168.22.2/live/chn0");
#endif
        param.video_frame_rate = 25;
        param.video_w = 1920;
        param.video_h = 1080;
#if 1
        param.audio_enable = 0;
        param.audio_bit_width = 16;
        param.audio_sample_rate = 11025;
#endif
        set_rtmp_chn_param(0, &param);

    open_rtmp_stream(0);
#endif

    AVOutputFormat *ofmt = NULL;
    //AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVFormatContext *ifmt_ctx = NULL;
    AVPacket pkt;

    const char *in_filename, *out_filename;
    int ret, i;

    //input
    in_filename  = "test.h264";
    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        fprintf(stderr, "Could not open input file '%s'", in_filename);
        goto end;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        fprintf(stderr, "Failed to retrieve input stream information");
        goto end;
    }

    av_dump_format(ifmt_ctx, 0, in_filename, 0);

#if 0
    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file '%s'", out_filename);
            goto end;
        }
    }

    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        goto end;
    }
#endif
    int pts = 0;
    while (1) {
        watch_rtmp_video_audio_is_alive(0);
        //AVStream *in_stream, *out_stream;
        AVStream *in_stream;

        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0){
            av_seek_frame(ifmt_ctx, 0, 0,AVSEEK_FLAG_BACKWARD );
            continue;
            //break;
        }

        in_stream  = ifmt_ctx->streams[pkt.stream_index];
#if 0
        out_stream = ofmt_ctx->streams[pkt.stream_index];
#endif

        log_packet(ifmt_ctx, &pkt, "in");
#if 0
        /* copy packet */
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX);
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        log_packet(ofmt_ctx, &pkt, "out");
#else
        pkt.pts = pts;
        pkt.dts = pkt.pts;
        pts += 40;
#endif
#if 0
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
#else
        send_rtmp_video_stream_packet(&pkt, 0);
#endif
        if (ret < 0) {
            fprintf(stderr, "Error muxing packet\n");
            break;
        }
        av_packet_unref(&pkt);
        usleep(40000);
    }
#if 0
    av_write_trailer(ofmt_ctx);
#endif
end:

    avformat_close_input(&ifmt_ctx);
#if 0
    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        return 1;
    }
#endif


#endif

    return 0;
}
