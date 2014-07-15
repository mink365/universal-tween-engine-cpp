//
//  TweenPool.cpp
//
//  This code is derived from Universal Tween Engine
//  Licensed under Apache License 2.0 - http://www.apache.org/licenses/LICENSE-2.0
//

#include "TweenPool.h"
#include "Tween.h"
#include "Timeline.h"

namespace TweenEngine
{
    void TweenPoolCallback::onPool(Tween *obj)
    {
        obj->reset();
    }
    
    void TweenPoolCallback::onUnPool(Tween *obj)
    {
        obj->reset();
    }
    
    TweenPool::TweenPool()
        : Pool<TweenEngine::Tween>(20, new TweenPoolCallback())
    {
    }
    
    Tween *TweenPool::create() {
        return new Tween();
    }

    void TimelinePoolCallback::onPool(Timeline *obj)
    {
        obj->reset();
    }

    void TimelinePoolCallback::onUnPool(Timeline *obj)
    {
        obj->reset();
    }

    Timeline *TimelinePool::create()
    {
        return new Timeline();
    }

    TimelinePool::TimelinePool()
        : Pool<TweenEngine::Timeline>(20, new TimelinePoolCallback())
    {

    }
}
