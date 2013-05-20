#include "Joystick.h"

#define CIRCLE_VERTEX_COUNT 360

Joystick::Joystick():_delegate(NULL),_state(jsNone),_type(jtImage),_normal(NULL),_highlighted(NULL),_target(NULL)
{
}

void Joystick::initImageJoystick(CCNode *normal,JoystickDelegate *delegate)
{
    initImageJoystick(normal, NULL, delegate);
}
void Joystick::initImageJoystick(CCNode *normal,CCNode *highlighted,JoystickDelegate *delegate)
{
    initImageJoystick(normal, highlighted, NULL, delegate);
}
void Joystick::initImageJoystick(CCNode *normal,CCNode *highlighted,CCNode *target,JoystickDelegate *delegate)
{
    _normal=normal;
    _highlighted=highlighted;
    _target=target;
    _delegate=delegate;
    
    _normal->setPosition(ccp(0, 0));
    _normal->setVisible(true);
    this->addChild(_normal);
    _highlighted->setPosition(ccp(0, 0));
    _highlighted->setVisible(false);
    this->addChild(_highlighted);
    _target->setPosition(ccp(0, 0));
    _target->setVisible(false);
    this->addChild(_target);
    
    _maxDistance=std::min(_normal->getContentSize().width/2, _normal->getContentSize().height/2);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void Joystick::initDrawableJoystick(CCRect reactionRect,JoystickDelegate *delegate)
{
    _type=jtDrawable;
    _delegate=delegate;
    _reactionRect=reactionRect;
    _maxDistance=CCDirector::sharedDirector()->getWinSize().height/4;
    
    m_pShaderProgram=CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor);
    m_pShaderProgram->retain();
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void Joystick::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *pTouch=(CCTouch*)pTouches->anyObject();
    if (_reactionRect.containsPoint(this->getParent()->convertToNodeSpace(pTouch->getLocation()))) {
        _state=jsTouched;
        _touchedPoint=pTouch->getLocation();
        if (_type==jtImage) {
            if (_normal) {
                _normal->setVisible(false);
            }
            if (_highlighted) {
                _highlighted->setVisible(true);
            }
            if (_target) {
                _target->setVisible(true);
                _target->setPosition(this->convertToNodeSpace(pTouch->getLocation()));
            }
        }
    }
}

void Joystick::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (_state==jsTouched||_state==jsTouchMoved) {
        CCTouch *pTouch=(CCTouch*)pTouches->anyObject();
        if (ccpDistance(_touchedPoint, pTouch->getLocation())<_maxDistance) {
            _vector=ccpSub(pTouch->getLocation(),_touchedPoint);
            if (_delegate) {
                _delegate->onRock(_vector);
            }
            _state=jsTouchMoved;
            
            if (_type==jtImage) {
                if (_target) {
                    _target->setPosition(this->convertToNodeSpace(pTouch->getLocation()));
                }
            }
        }
        else {
            _state=jsNone;
            if (_delegate) {
                _delegate->onRockEnd();
            }
            if (_type==jtImage) {
                if (_normal) {
                    _normal->setVisible(true);
                }
                if (_highlighted) {
                    _highlighted->setVisible(false);
                }
                if (_target) {
                    _target->setVisible(false);
                }
            }
        }
    }
}

void Joystick::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (_state!=jsNone) {
        _state=jsNone;
        if (_delegate) {
            _delegate->onRockEnd();
        }
        if (_normal) {
            _normal->setVisible(true);
        }
        if (_highlighted) {
            _highlighted->setVisible(false);
        }
        if (_target) {
            _target->setVisible(false);
        }
    }
    
}

void Joystick::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    ccTouchesEnded(pTouches, pEvent);
}

void Joystick::draw()
{
    if (_type==jtDrawable) {
        CC_NODE_DRAW_SETUP();
        switch (_state) {
            case jsTouched:
                drawCircle();
                break;
            case jsTouchMoved:
                drawArrow();
                break;
            default:
                break;
        }
    }
}

void Joystick::drawCircle()
{
    ccVertex2F vertex[CIRCLE_VERTEX_COUNT+2];
    ccColor4F colors[CIRCLE_VERTEX_COUNT+2];
    float coef=M_PI*2/CIRCLE_VERTEX_COUNT;
    int radius=30;
    vertex[0]=vertex2(_touchedPoint.x, _touchedPoint.y);
    colors[0]=ccc4f(0, 0, 0, 1);
    for (int i=0; i<CIRCLE_VERTEX_COUNT; ++i) {
        vertex[i+1].x=cosf(coef*i)*radius+_touchedPoint.x;
        vertex[i+1].y=sinf(coef*i)*radius+_touchedPoint.y;
        colors[i+1]=ccc4f(0, 0, 0, 1);
    }
    
    vertex[CIRCLE_VERTEX_COUNT+1]=vertex2(radius+_touchedPoint.x, _touchedPoint.y);
    colors[CIRCLE_VERTEX_COUNT+1]=ccc4f(0, 0, 0, 1);

    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position|kCCVertexAttribFlag_Color);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertex);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, colors);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_VERTEX_COUNT+2);
}

void Joystick::drawArrow()
{
    CCPoint point=ccpAdd(_touchedPoint, _vector);
    ccVertex2F vertices[4]={{point.x,point.y},{point.x-25,point.y-15},{point.x,point.y+35},{point.x+25,point.y-15}};
    for (int i=0; i<4; ++i) {
        CCPoint tempPoint=ccpRotateByAngle(ccp(vertices[i].x,vertices[i].y), ccp(point.x,point.y),-1*ccpToAngle(ccp(_vector.y,_vector.x)));
        vertices[i]=vertex2(tempPoint.x, tempPoint.y);
    }
    ccColor4F colors[4]={{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1}};
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position|kCCVertexAttribFlag_Color);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
}
