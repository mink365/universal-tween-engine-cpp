//
//  TweenPool.h
//
//  This code is derived from Universal Tween Engine
//  Licensed under Apache License 2.0 - http://www.apache.org/licenses/LICENSE-2.0
//

#ifndef __TweenPool__
#define __TweenPool__

#include "Pool.h"
#include "Tween.h"
#include "Timeline.h"

namespace TweenEngine
{
    class TweenPoolCallback : public PoolCallback<Tween>
    {
    public:
        void onPool(Tween *obj);
        void onUnPool(Tween *obj);
    };
    
    class TweenPool : public Pool<Tween>
    {
    protected:
        Tween *create();
    public:
        TweenPool();
        
    };

    class TimelinePoolCallback : public PoolCallback<Timeline>
    {
    public:
        void onPool(Timeline *obj);
        void onUnPool(Timeline *obj);
    };

    class TimelinePool : public Pool<Timeline>
    {
    protected:
        Timeline *create();
    public:
        TimelinePool();

    };
}
#endif /* defined(__TweenPool__) */
