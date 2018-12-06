//
// Created by 叶亮 on 2018/12/6.
//

#include "IObserver.h"

//主题函数 添加观察者
void IObserver::AddObs(IObserver *obs)
{
    if(!obs) return;
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}

//通知所有观察者
void IObserver::Notify(XData data)
{
    mux.lock();
    for(int i=0;i<obss.size();i++)
    {
        obss[i]->Update(data);
    }
    mux.unlock();
}