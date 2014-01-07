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

typedef enum {
    ysdLeft=1,
    ysdLeftUp,
    ysdUp,
    ysdRightUp,
    ysdRight,
    ysdRightDown,
    ysdDown,
    ysdLeftDown
}ysDirection;

class JoystickDelegate {
public:
    virtual void onRock(ysDirection direction,CCPoint vector)=0;
    virtual void onRockEnd()=0;
};

class Joystick:public CCNode,public CCTargetedTouchDelegate {
    CCRect _responseRect;
    JoystickState _state;
    CCPoint _touchedPoint;
    JoystickDelegate *_delegate;
    CCPoint _vector;
    CCNode *_bgNormal;
    CCNode *_bgHighlighted;
    CCNode *_touchedJoystick;
    CCNode *_movedJoystick;
    float _maxResponseDistance;
    float _maxMoveDistance;
    bool _simpleDrawEnabled;
    bool _enabled;
    void drawCircle();
    void drawArrow();
public:
    Joystick();
    CREATE_FUNC(Joystick);
    void initWithImages(CCNode *touchedJoystick,CCNode *movedJoystick);
    void initWithImages(CCNode *touchedJoystick,CCNode *movedJoystick,CCNode *bgNormal);
    void initWithImages(CCNode *touchedJoystick,CCNode *movedJoystick,CCNode *bgNormal,CCNode *bgHighlighted);
    void EnableSimpleDraw();
    void DisableSimpleDraw();
    void setResponseRect(CCRect responseRect) {_responseRect=responseRect;}
    CCRect getResponseRect() {return _responseRect;}
    void setDelegate(JoystickDelegate *delegate);
    void setMaxResponseDistance(float maxResponseDistance) {_maxResponseDistance=maxResponseDistance;}
    float getMaxResponseDistance() {return _maxResponseDistance;}
    void setMaxMovedDistance(float maxMovedDistance) {_maxMoveDistance=maxMovedDistance;}
    float getMaxMovedDistance() {return _maxMoveDistance;}
    void AdjustResponseRectToBackground(float padding);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void draw();
    virtual void onEnter();
    void setEnabled(bool enabled);
};

#endif
