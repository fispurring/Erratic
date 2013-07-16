//
//  joystick_manual.cpp
//  DemoJSB
//
//  Created by LiXinyu on 13-5-28.
//
//

#include "joystick_manual.h"
#include "Joystick.h"
#include "ScriptingCore.h"

class JSB_JoystickDelegate : public JoystickDelegate
{
public:
    virtual void onRock(ysDirection director,CCPoint vector)
    {
        jsval arg[2];
        arg[0]=INT_TO_JSVAL(director);
        arg[1]=ccpoint_to_jsval(ScriptingCore::getInstance()->getGlobalContext(), vector);
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate), "onRock",2,arg,NULL);
    }

    virtual void onRockEnd()
    {
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(m_pJSDelegate),"onRockEnd");
    }
    
    void setJSDelegate(JSObject *sender)
    {
        m_pJSDelegate=sender;
        JS_AddObjectRoot(ScriptingCore::getInstance()->getGlobalContext(), &m_pJSDelegate);
    }
    ~JSB_JoystickDelegate() {
        JS_RemoveObjectRoot(ScriptingCore::getInstance()->getGlobalContext(), &m_pJSDelegate);
    }
private:
    JSObject* m_pJSDelegate;
};

static JSBool js_joystick_Joystick_setDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy; JS_GET_NATIVE_PROXY(proxy, obj);
    Joystick* cobj = (Joystick *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, JS_FALSE, "Invalid Native Object");
    
    if (argc == 1) {
        // save the delegate
        JSObject *jsDelegate = JSVAL_TO_OBJECT(argv[0]);
        JSB_JoystickDelegate* nativeDelegate = new JSB_JoystickDelegate();
        nativeDelegate->setJSDelegate(jsDelegate);
        cobj->setDelegate(nativeDelegate);
        
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return JS_TRUE;
    }
    JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
    return JS_FALSE;
}

extern JSObject *jsb_Joystick_prototype;
void register_all_joystick_manual(JSContext *cx,JSObject *global)
{
    JS_DefineFunction(cx, jsb_Joystick_prototype, "setDelegate", js_joystick_Joystick_setDelegate, 1, JSPROP_READONLY|JSPROP_PERMANENT);
}