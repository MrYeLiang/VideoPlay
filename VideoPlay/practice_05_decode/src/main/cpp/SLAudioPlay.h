//
// Created by 叶亮 on 2019/2/28.
//

#ifndef VIDEOPLAY_SLAUDIOPLAY_H
#define VIDEOPLAY_SLAUDIOPLAY_H

#include "IAudioPlay.h"

class SLAudioPlay: public IAudioPlay
{
public:
    virtual bool StartPlay(XParameter out);
    void PlayCall(void *bufq);
};
#endif //VIDEOPLAY_SLAUDIOPLAY_H
