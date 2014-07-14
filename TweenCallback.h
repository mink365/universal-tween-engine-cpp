//
//  TweenCallback.h
//
//  This code is derived from Universal Tween Engine
//  Licensed under Apache License 2.0 - http://www.apache.org/licenses/LICENSE-2.0
//

/**
 * TweenCallbacks are used to trigger actions at some specific times. They are
 * used in both Tweens and Timelines. The moment when the callback is
 * triggered depends on its registered triggers:
 * <p/>
 *
 * <b>BEGIN</b>: right after the delay (if any)<br/>
 * <b>START</b>: at each iteration beginning<br/>
 * <b>END</b>: at each iteration ending, before the repeat delay<br/>
 * <b>COMPLETE</b>: at last END event<br/>
 * <b>BACK_BEGIN</b>: at the beginning of the first backward iteration<br/>
 * <b>BACK_START</b>: at each backward iteration beginning, after the repeat delay<br/>
 * <b>BACK_END</b>: at each backward iteration ending<br/>
 * <b>BACK_COMPLETE</b>: at last BACK_END event
 * <p/>
 *
 * <pre> {@code
 * forward :      BEGIN                                   COMPLETE
 * forward :      START    END      START    END      START    END
 * |--------------[XXXXXXXXXX]------[XXXXXXXXXX]------[XXXXXXXXXX]
 * backward:      bEND  bSTART      bEND  bSTART      bEND  bSTART
 * backward:      bCOMPLETE                                 bBEGIN
 * }</pre>
 *
 * @see Tween
 * @see Timeline
 * @author Aurelien Ribon | http://www.aurelienribon.com/
 */

#ifndef __TweenCallback__
#define __TweenCallback__

#include <functional>

namespace TweenEngine
{
    class BaseTween;
    
    enum class TweenCallbackType
    {
        BEGIN = 0x01,
        START = 0x02,
        END = 0x04,
        COMPLETE = 0x08,
        BACK_BEGIN = 0x10,
        BACK_START = 0x20,
        BACK_END = 0x40,
        BACK_COMPLETE = 0x80,
        ANY_FORWARD = 0x0F,
        ANY_BACKWARD = 0xF0,
        ANY = 0xFF,
    };

    typedef std::function<void(TweenCallbackType type, BaseTween *source)> TweenCallbackFunc;
    
}
#endif /* defined(__TweenCallback__) */
