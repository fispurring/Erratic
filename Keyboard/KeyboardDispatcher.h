#ifndef __KEYBOARD_DISPATCHER_H__
#define __KEYBOARD_DISPATCHER_H__

#include "cocos2d.h"
#include "windows.h"

using namespace std;

using namespace cocos2d;

class KeyboardListener
{
public:
	virtual void onKeyDown(WPARAM wParam) {}
	virtual void onKeyUp(WPARAM wParam) {}
};

class KeyboardDispatcher
{
	static	KeyboardDispatcher *s_singleton;
	list<KeyboardListener*> _listeners;
	KeyboardDispatcher();
public:
	static KeyboardDispatcher *sharedDispatcher();
	void setEnabled(bool enabled);
	void addListener(KeyboardListener *listener);
	void removeListener(KeyboardListener *listener);
	void KeyDown(WPARAM wParam);
	void KeyUp(WPARAM wParam);
};

#endif