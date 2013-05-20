#ifndef __motion__Joystick__
#define __motion__Joystick__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

typedef enum {
    jsNone,
    jsTouched,
    jsTouchMoved
}JoystickState;

class JoystickDelegate {
public:
    virtual void onRock(CCPoint vector)=0;
    virtual void onRockEnd()=0;
};

typedef enum {
    jtImage,
    jtDrawable
}JoystickType;

class Joystick:public CCNode,public CCStandardTouchDelegate {
    CCRect _reactionRect;
    JoystickState _state;
    CCPoint _touchedPoint;
    JoystickDelegate *_delegate;
    CCPoint _vector;
    JoystickType _type;
    CCNode *_normal;
    CCNode *_highlighted;
    CCNode *_target;
    float _maxDistance;
    void drawCircle();
    void drawArrow();
public:
    Joystick();
    CREATE_FUNC(Joystick);
    void initImageJoystick(CCNode *normal,JoystickDelegate *delegate);
    void initImageJoystick(CCNode *normal,CCNode *highlighted,JoystickDelegate *delegate);
    void initImageJoystick(CCNode *normal,CCNode *highlighted,CCNode *target,JoystickDelegate *delegate);
    void initDrawableJoystick(CCRect reactionRect,JoystickDelegate *delegate);
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    virtual void draw();
};

#endif
