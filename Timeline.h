#ifndef TIMELINE_H
#define TIMELINE_H

#include "BaseTween.h"
#include "Tween.h"

namespace TweenEngine
{

enum class Modes
{
    SEQUENCE,
    PARALLEL
};

class TimelinePool;
class TimelinePoolCallback;

class Timeline : public BaseTween
{
    friend class TimelinePoolCallback;

private:
    std::vector<BaseTween *> children;
    Timeline* current;
    Timeline* parent;
    Modes mode;
    bool isBuilt;

public:
    static TimelinePool &pool;

    static int getPoolSize();
    static void ensurePoolCapacity(int minCapacity);

    static Timeline& createSequence();
    static Timeline& createParallel();

public:
    Timeline();
    ~Timeline();

    virtual int getTweenCount();
    virtual int getTimelineCount();

    Timeline& push(Tween &tween);
    Timeline& push(Timeline& timeline);
    Timeline& pushPause(float time);
    Timeline& beginSequence();
    Timeline& beginParallel();
    Timeline& end();

    virtual BaseTween &build();
    virtual BaseTween &start();
    virtual void free();

protected:
    virtual void reset();
    virtual void forceStartValues();
    virtual void forceEndValues();
    virtual void updateOverride(int step, int lastStep, bool isIterationStep, float delta);

private:
    void setup(Modes mode);
};

}

#endif // TIMELINE_H
