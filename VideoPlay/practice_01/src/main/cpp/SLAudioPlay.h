//
// Created by 叶亮 on 2018/12/4.
//

#ifndef VIDEOPLAY_SLAUDIOPLAY_H
#define VIDEOPLAY_SLAUDIOPLAY_H

#include "IAudioPlay.h"

class SLAudioPlay: public IAudioPlay
{
public:
    virtual bool StartPlay(XParameter out);
    virtual void Close();
    void PlayCall(void *bufq);

    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf = 0;
    std::mutex mutex;
};
#endif //VIDEOPLAY_SLAUDIOPLAY_H
