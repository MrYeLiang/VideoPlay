//
// Created by 叶亮 on 2018/12/6.
//
#include "IPlayer.h"
#include "IAudioPlay.h"
#include "IDecode.h"

IPlayer *IPlayer::Get(unsigned char index)
{
    static IPlayer p[256];
    return &p[index];
}

void IPlayer::Main()
{
    while(!isExit)
    {
        mux.lock();
        if(!audioPlay||!vdecode){
            mux.unlock();
            XSleep(2);
            continue;
        }

        //获取音频pts 告诉视频
        int apts = audioPlay->pts;
        vdecode->synPts = apts;
        mux.unlock();
        XSleep(2);
    }
}

void IPlayer::Close()
{
    mux.lock();
    //2 先关闭主体线程，再清理观察者
    //同步线程
    XThread::Stop();
    //解封装
    if(demux){
        demux->Stop();
    }
}