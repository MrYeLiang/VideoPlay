//
// Created by 叶亮 on 2018/12/4.
//

#ifndef VIDEOPLAY_IAUDIOPLAY_H
#define VIDEOPLAY_IAUDIOPLAY_H

#include "IObserver.h"
#include "XParameter.h"
#include <list>

class IAudioPlay: public IObserver
{
public:
    //缓冲满后阻塞
    virtual void Update(XData data);

    //获取缓冲数据 如没有则阻塞
    virtual XData GetData();

    virtual bool StartPlay(XParameter out) = 0;
    virtual void Close() = 0;
    virtual void Clear();

    //最大缓冲
    int maxFrame = 100;
    int pts = 0;

protected:
    std::list <XData> frames;
    std::mutex framesMutext;
};
#endif //VIDEOPLAY_IAUDIOPLAY_H
