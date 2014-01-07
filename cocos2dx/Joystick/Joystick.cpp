#include "Joystick.h"

#define CIRCLE_VERTEX_COUNT 360

Joystick::Joystick():
    _delegate(NULL),
    _state(jsNone),
    _bgNormal(NULL),
    _bgHighlighted(NULL),
    _touchedJoystick(NULL),
    _movedJoystick(NULL),
    _maxResponseDistance(3000),
    _maxMoveDistance(100),
    _simpleDrawEnabled(false),
    _enabled(true)
{
    _responseRect=CCRectMake(0, 0, CCDirector::sharedDirector()->getWinSize().width/3, CCDirector::sharedDirector()->getWinSize().height/3);
}

void Joystick::initWithImages(CCNode *touchedJoystick,CCNode *movedJoystick)
{
    initWithImages(touchedJoystick, movedJoystick, NULL);
}
void Joystick::initWithImages(CCNode *touchedJoystick,CCNode *movedJoystick,CCNode *bgNormal)
{
    initWithImages(touchedJoystick,movedJoystick,bgNormal,NULL);
}

void Joystick::initWithImages(CCNode *touchedJoystick,CCNode *movedJoystick,CCNode *bgNormal,CCNode *bgHighlighted)
{
    _touchedJoystick=touchedJoystick;
    _movedJoystick=movedJoystick;
    _bgNormal=bgNormal;
    _bgHighlighted=bgHighlighted;
    
    if(_touchedJoystick) {
        _touchedJoystick->setPosition(ccp(0, 0));
        _touchedJoystick->setVisible(false);
        this->addChild(_touchedJoystick);
    }
    if (_movedJoystick) {
        _movedJoystick->setPosition(ccp(0,0));
        _movedJoystick->setVisible(false);
        this->addChild(_movedJoystick);
    }
    if (_bgNormal) {
        _bgNormal->setPosition(ccp(0, 0));
        _bgNormal->setVisible(true);
        this->addChild(_bgNormal);
    }
    if (_bgHighlighted) {
        _bgHighlighted->setPosition(ccp(0, 0));
        _bgHighlighted->setVisible(false);
        this->addChild(_bgHighlighted);
    }
}

void Joystick::EnableSimpleDraw()
{
    if (!_simpleDrawEnabled) {
        _simpleDrawEnabled=true;
        m_pShaderProgram=CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor);
        m_pShaderProgram->retain();
    }
}

void Joystick::DisableSimpleDraw()
{
    if (_simpleDrawEnabled) {
        _simpleDrawEnabled=false;
        m_pShaderProgram->release();
    }
}

void Joystick::setDelegate(JoystickDelegate *delegate)
{
    _delegate=delegate;
}

bool Joystick::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!_enabled) {
        return false;
    }
    
    CCNode *parent=this->getParent();
    if (!parent) {
        return false;
    }
    
    if (_responseRect.containsPoint(pTouch->getLocation())) {
        _state=jsTouched;
        _touchedPoint=pTouch->getLocation();
        if (_touchedJoystick) {
            _touchedJoystick->setVisible(true);
            _touchedJoystick->setPosition(this->convertToNodeSpace(pTouch->getLocation()));
        }
        if (_movedJoystick) {
            _movedJoystick->setVisible(false);
        }
        if (_bgNormal) {
            _bgNormal->setVisible(false);
        }
        if (_bgHighlighted) {
            _bgHighlighted->setVisible(true);
        }
        return true;
    }
    return false;
}

void Joystick::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!_enabled) {
        return;
    }

    if (_state==jsTouched||_state==jsTouchMoved) {
        if (ccpDistance(_touchedPoint, pTouch->getLocation())<_maxResponseDistance) {
            _vector=ccpSub(pTouch->getLocation(),_touchedPoint);
            if (_delegate) {
                float angle=CC_RADIANS_TO_DEGREES(ccpToAngle(_vector));
                ysDirection direction=ysdLeft;
                if ((angle>=157.5&&angle<=180)||(angle>=-180&&angle<=-157.5)) {
                    direction=ysdLeft;
                }
                else if (angle<=157.5&&angle>=112.5) {
                    direction=ysdLeftUp;
                }
                else if (angle>=67.5&&angle<=112.5) {
                    direction=ysdUp;
                }
                else if (angle>=22.5&&angle<=67.5) {
                    direction=ysdRightUp;
                }
                else if(angle>=-22.5&&angle<=22.5) {
                    direction=ysdRight;
                }
                else if (angle>=-67.5&&angle<=-22.5) {
                    direction=ysdRightDown;
                }
                else if (angle>=-112.5&&angle<=-67.5) {
                    direction=ysdDown;
                }
                else if (angle>=-157.5&&angle<=112.5) {
                    direction=ysdLeftDown;
                }
                _delegate->onRock(direction,_vector);
            }
            _state=jsTouchMoved;
            
            if (_touchedJoystick) {
                _touchedJoystick->setVisible(false);
            }
            if (_movedJoystick) {
                _movedJoystick->setVisible(true);
                CCPoint point=pTouch->getLocation();
                if (ccpDistance(_touchedPoint, pTouch->getLocation())>_maxMoveDistance) {
                    point=ccpMult(ccpNormalize(pTouch->getLocation()),_maxMoveDistance);
                }
                _movedJoystick->setPosition(this->convertToNodeSpace(point));
                _movedJoystick->setRotation(CC_RADIANS_TO_DEGREES(ccpToAngle(ccp(_vector.y,_vector.x))));
            }
        }
        else {
            _state=jsNone;
            if (_delegate) {
                _delegate->onRockEnd();
            }
            if (_bgNormal) {
                _bgNormal->setVisible(true);
            }
            if (_bgHighlighted) {
                _bgHighlighted->setVisible(false);
            }
            if (_touchedJoystick) {
                _touchedJoystick->setVisible(false);
            }
        }
    }
}

void Joystick::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!_enabled) {
        return;
    }
    if (_state!=jsNone) {
        _state=jsNone;
        if (_delegate) {
            _delegate->onRockEnd();
        }
        if (_touchedJoystick) {
            _touchedJoystick->setVisible(false);
        }
        if (_movedJoystick) {
            _movedJoystick->setVisible(false);
        }
        if (_bgNormal) {
            _bgNormal->setVisible(true);
        }
        if (_bgHighlighted) {
            _bgHighlighted->setVisible(false);
        }
    }
    
}

void Joystick::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    ccTouchEnded(pTouch, pEvent);
}

void Joystick::draw()
{
    if (_simpleDrawEnabled) {
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

void Joystick::onEnter()
{
    CCNode::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void Joystick::AdjustResponseRectToBackground(float padding)
{
    if (_bgNormal) {
        CCPoint point=this->convertToWorldSpace(ccp(_bgNormal->boundingBox().getMinX(),_bgNormal->boundingBox().getMinY()));
        CCSize size=_bgNormal->getContentSize();
        _responseRect=CCRectMake(point.x-padding, point.y-padding, size.width+2*padding, size.height+2*padding);
    }
}

void Joystick::setEnabled(bool enabled)
{
    if (_enabled!=enabled) {
        _enabled=enabled;
        if (!_enabled) {
            _state=jsNone;
            if(_touchedJoystick) {
                _touchedJoystick->setPosition(ccp(0, 0));
                _touchedJoystick->setVisible(false);
            }
            if (_movedJoystick) {
                _movedJoystick->setPosition(ccp(0,0));
                _movedJoystick->setVisible(false);
            }
            if (_bgNormal) {
                _bgNormal->setPosition(ccp(0, 0));
                _bgNormal->setVisible(true);
            }
            if (_bgHighlighted) {
                _bgHighlighted->setPosition(ccp(0, 0));
                _bgHighlighted->setVisible(false);
            }
        }
    }
}
