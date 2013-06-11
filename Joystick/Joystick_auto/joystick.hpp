#ifndef __joystick_h__
#define __joystick_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_Joystick_class;
extern JSObject *jsb_Joystick_prototype;

JSBool js_joystick_Joystick_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_joystick_Joystick_finalize(JSContext *cx, JSObject *obj);
void js_register_joystick_Joystick(JSContext *cx, JSObject *global);
void register_all_joystick(JSContext* cx, JSObject* obj);
JSBool js_joystick_Joystick_getResponseRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_EnableSimpleDraw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_onEnter(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_getMaxMovedDistance(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_DisableSimpleDraw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_ccTouchBegan(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_ccTouchEnded(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_ccTouchCancelled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_ccTouchMoved(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_setResponseRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_getMaxResponseDistance(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_setMaxResponseDistance(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_setMaxMovedDistance(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_initWithImages(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_AdjustResponseRectToBackground(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_joystick_Joystick_Joystick(JSContext *cx, uint32_t argc, jsval *vp);
#endif

