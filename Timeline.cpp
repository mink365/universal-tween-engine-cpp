#include "Timeline.h"

#include "TweenPool.h"
#include <assert.h>

namespace TweenEngine
{

/**
 * Used for debug purpose. Gets the current number of objects that are
 * waiting in the Timeline pool.
 */
int Timeline::getPoolSize() { return pool.size(); }

/**
 * Increases the minimum capacity of the pool. Capacity defaults to 20.
 */
void Timeline::ensurePoolCapacity(int minCapacity) { pool.ensureCapacity(minCapacity); }

TimelinePool &Timeline::pool = *(new TimelinePool());

Timeline::Timeline()
{
    current = parent = nullptr;

    isBuilt = false;
}

Timeline::~Timeline()
{

}

int Timeline::getTweenCount()
{
    return 0;
}

int Timeline::getTimelineCount()
{
    return 1;
}

Timeline &Timeline::createSequence()
{
    Timeline& tl = *(pool.get());
    tl.setup(Modes::SEQUENCE);
    return tl;
}

Timeline &Timeline::createParallel()
{
    Timeline& tl = *(pool.get());
    tl.setup(Modes::PARALLEL);
    return tl;
}

Timeline &Timeline::push(Tween &tween)
{
    assert(!isBuilt);
//    if (isBuilt) throw new RuntimeException("You can't push anything to a timeline once it is started");
    current->children.push_back(&tween);
    return *this;
}

Timeline &Timeline::push(Timeline &timeline)
{
//    if (isBuilt) throw new RuntimeException("You can't push anything to a timeline once it is started");
//    if (timeline.current != timeline) throw new RuntimeException("You forgot to call a few 'end()' statements in your pushed timeline");
    assert(!isBuilt);
    assert(timeline.current == &timeline);

    timeline.parent = current;
    current->children.push_back(&timeline);
    return *this;
}

Timeline &Timeline::pushPause(float time)
{
    assert(!isBuilt);
//    if (isBuilt) throw new RuntimeException("You can't push anything to a timeline once it is started");
    current->children.push_back(&Tween::mark().delay(time));
    return *this;
}

Timeline &Timeline::beginSequence()
{
    assert(!isBuilt);
//    if (isBuilt) throw new RuntimeException("You can't push anything to a timeline once it is started");
    Timeline& tl = *(pool.get());
    tl.parent = current;
    tl.mode = Modes::SEQUENCE;
    current->children.push_back(&tl);
    current = &tl;
    return *this;
}

Timeline &Timeline::beginParallel()
{
    assert(!isBuilt);
//    if (isBuilt) throw new RuntimeException("You can't push anything to a timeline once it is started");
    Timeline& tl = *(pool.get());
    tl.parent = current;
    tl.mode = Modes::PARALLEL;
    current->children.push_back(&tl);
    current = &tl;
    return *this;
}

Timeline &Timeline::end()
{
    assert(!isBuilt);
    assert(current != this);
//    if (isBuilt) throw new RuntimeException("You can't push anything to a timeline once it is started");
//    if (current == this) throw new RuntimeException("Nothing to end...");
    current = current->parent;
    return *this;
}

BaseTween &Timeline::build()
{
    if (isBuilt)
        return *this;

    duration = 0;

    for (int i=0; i<children.size(); i++) {
        BaseTween* obj = children.at(i);

        if (obj->getRepeatCount() < 0) {
            assert(false);
//            throw new RuntimeException("You can't push an object with infinite repetitions in a timeline");
        }
        obj->build();

        switch (mode) {
            case Modes::SEQUENCE:
        {
                float tDelay = duration;
                duration += obj->getFullDuration();
                obj->delayStart += tDelay;
                break;
        }
        case Modes::PARALLEL:
        {
                duration = std::max(duration, obj->getFullDuration());
                break;
        }
        }
    }

    isBuilt = true;
    return *this;
}

BaseTween &Timeline::start()
{
    BaseTween::start();

    for (int i=0; i<children.size(); i++) {
        BaseTween* obj = children.at(i);
        obj->start();
    }

    return *this;
}

void Timeline::free()
{
    for (BaseTween* obj : children) {
        obj->free();
    }
    children.clear();

    pool.free(this);
}

void Timeline::reset()
{
    BaseTween::reset();

    children.clear();
    current = parent = nullptr;

    isBuilt = false;
}

void Timeline::forceStartValues()
{
    for (int i=children.size()-1; i>=0; i--) {
        BaseTween* obj = children.at(i);
        obj->forceToStart();
    }
}

void Timeline::forceEndValues()
{
    for (int i=0, n=children.size(); i<n; i++) {
        BaseTween* obj = children.at(i);
        obj->forceToEnd(duration);
    }
}

void Timeline::updateOverride(int step, int lastStep, bool isIterationStep, float delta)
{
    if (!isIterationStep && step > lastStep) {
        assert(delta >= 0);
        float dt = isReverse(lastStep) ? -delta-1 : delta+1;
        for (int i=0, n=children.size(); i<n; i++)
            children.at(i)->update(dt);
        return;
    }

    if (!isIterationStep && step < lastStep) {
        assert(delta <= 0);
        float dt = isReverse(lastStep) ? -delta-1 : delta+1;
        for (int i=children.size()-1; i>=0; i--)
            children.at(i)->update(dt);
        return;
    }

    assert(isIterationStep);

    if (step > lastStep) {
        if (isReverse(step)) {
            forceEndValues();
            for (int i=0, n=children.size(); i<n; i++)
                children.at(i)->update(delta);
        } else {
            forceStartValues();
            for (int i=0, n=children.size(); i<n; i++)
                children.at(i)->update(delta);
        }

    } else if (step < lastStep) {
        if (isReverse(step)) {
            forceStartValues();
            for (int i=children.size()-1; i>=0; i--)
                children.at(i)->update(delta);
        } else {
            forceEndValues();
            for (int i=children.size()-1; i>=0; i--)
                children.at(i)->update(delta);
        }

    } else {
        float dt = isReverse(step) ? -delta : delta;
        if (delta >= 0) {
            for (int i=0, n=children.size(); i<n; i++)
                children.at(i)->update(dt);
        }
        else {
            for (int i=children.size()-1; i>=0; i--)
                children.at(i)->update(dt);
        }
    }
}

void Timeline::setup(Modes mode)
{
    this->mode = mode;
    this->current = this;
}

}
