//
// Created by 叶亮 on 2019/1/25.
//

#include <jni.h>
#include "FFDemux.h"
#include "Xlog.h"
#include "IObserver.h"
#include "XData.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "XEGL.h"
#include "XShader.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "GLVideoView.h"
#include "IResample.h"
#include "FFResample.h"
#include "IAudioPlay.h"
#include "SLAudioPlay.h"

IVideoView *   view = NULL;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_videoplay_XPlay_decode(JNIEnv *env, jobject instance) {

}

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res)
{
    FFDecode::InitHard(vm);
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_videoplay_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    //解封装对象
    IDemux *demux = new FFDemux();
    demux ->Open("/storage/emulated/0/video.mp4");

    //音频解码器
    IDecode *aDecode = new FFDecode();
    XParameter outPara = demux->GetAPara();
    aDecode->Open(outPara);
    IResample *resample = new FFResample(); //音频重采样
    resample->Open(demux->GetAPara());
    aDecode->AddObs(resample);
    demux->AddObs(aDecode);

    IAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->StartPlay(outPara);
    resample->AddObs(audioPlay);

    aDecode->Start();



    //视频解码器
    IDecode *vdecode = new FFDecode();
    vdecode->Open(demux->GetVPara(),true);
    view = new GLVideoView();
    vdecode->AddObs(view);
    demux->AddObs(vdecode);
    vdecode->Start();

    demux->Start();

    //视频显示
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    view->SetRender(win);
}