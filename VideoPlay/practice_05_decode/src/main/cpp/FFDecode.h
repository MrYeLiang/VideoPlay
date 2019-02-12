//
// Created by 叶亮 on 2019/2/12.
//

#ifndef VIDEOPLAY_FFDECODE_H
#define VIDEOPLAY_FFDECODE_H

#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode:public IDecode
{
public:
    virtual bool Open(XParameter para);

    //future模型 发送数据到线程解码
    virtual bool SendPacket(XData pkt);

    //从线程中获取解码结果
    virtual XData RecvFrame();
protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
};
#endif //VIDEOPLAY_FFDECODE_H
