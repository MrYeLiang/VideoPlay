//
// Created by 叶亮 on 2019/1/27.
//

#ifndef VIDEOPLAY_FFDEMUX_H
#define VIDEOPLAY_FFDEMUX_H

#include "IDemux.h"
struct AVFormatContext;

class FFDemux:public  IDemux{
public:
    //打开文件，或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url);

    //获取视频参数
    virtual XParameter GetVPara();

    //获取音频参数
    virtual XParameter GetAPara();

    //读取一帧数据，数据由调用者清理
    virtual XData Read();

    FFDemux();

private:
    AVFormatContext *ic = 0;
    int audioStream = 1;
    int videoStream = 0;
};

#endif //VIDEOPLAY_FFDEMUX_H
