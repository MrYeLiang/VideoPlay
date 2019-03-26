//
// Created by 叶亮 on 2019/3/4.
//

#include "IPlayer.h"
#include "Xlog.h"
#include "IPlayer.h"
#include "IAudioPlay.h"

IPlayer *IPlayer::Get(unsigned char index)
{
    static IPlayer p[256];
    return &p[index];
}

void IPlayer::Main()
{
    while (!isExit)
    {
        mux.lock();
        if(!audioPlay || !vdecode)
        {
            mux.unlock();
            XSleep(2);
            continue;
        }

        //同步
        //获取音频的pts 告诉音频
        int apts = audioPlay->pts;
        vdecode->synPts = apts;

        mux.unlock();
        XSleep(2);
    }
}

void IPlayer::Close()
{
    mux.lock();

    //先关闭主题线程，再清理观察者
    //同步线程
    XThread::Stop();

    //解封装
    if(demux)
        demux->Stop();

    //解码
    if(vdecode)
        vdecode->Stop();
    if(adecode)
        adecode->Stop();

    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear();
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();

    //3 清理资源
   if(audioPlay)
        audioPlay->Close();
    if(videoView)
        videoView->Close();
    if(adecode)
        adecode->Close();
    if(vdecode)
        vdecode->Close();
    if(demux)
        demux->Close();
    mux.unlock();

}

bool IPlayer::Open(const char *path)
{
    Close();
    //解封装
    if(!demux || !demux->Open(path)){
        XLOGE("demux->Open %s failed!", path);
        return false;
    }

    //解码 如果解封装之后是原始数据解码可能不需要
    if(!vdecode || !vdecode->Open(demux->GetVPara(),isHardDecode)){
        XLOGE("vdecode->Open %s failed!",path);
        //return false;
    }

    if(!vdecode || !adecode->Open(demux->GetAPara()))
    {
        XLOGE("adecode->Open %s failed!", path);
    }

    //重采样，有可能不需要  解码或者解封装后可能是直接能播放的数据
    if(outPara.sample_rate <= 0) outPara = demux->GetAPara();

    if(!resample || !resample->Open(demux->GetAPara(),outPara))
    {
        XLOGE("resample->Open %s failed!",path);
    }

    return true;
}

bool IPlayer::Start()
{

    mux.lock();
    if(vdecode){
        vdecode->Start();
    }

    if(!demux || !demux->Start())
    {
        XLOGE("demux->Start failed!");
        return false;
    }

    if(adecode)
    {
        adecode->Start();
    }

    if(audioPlay){
        audioPlay->StartPlay(outPara);
    }
    XThread::Start();
    mux.unlock();

    return true;
}

void IPlayer::InitView(void *win)
{
    if(videoView)
    {
        videoView->Close();
        videoView->SetRender(win);
    }
}

//获取当前的播放进度
double  IPlayer::PlayPos(){
    double pos = 0.0;
    mux.lock();
    int total = 0;
    if(demux)
    {
        total = demux->totalMs;
    }

    if(total > 0)
    {
        if(vdecode)
        {
            pos = (double)vdecode->pts/(double)total;
        }
    }

    mux.unlock();

    //XLOGI("IPlayer--> pos = %lf",pos);
    return pos;
}