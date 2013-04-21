#include "KeyboardDispatcher.h"

void wndProcHook(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		KeyboardDispatcher::sharedDispatcher()->KeyDown(wParam);
		break;
	case WM_KEYUP:
		KeyboardDispatcher::sharedDispatcher()->KeyUp(wParam);
		break;
	}
}

KeyboardDispatcher *KeyboardDispatcher::s_singleton=NULL;

KeyboardDispatcher *KeyboardDispatcher::sharedDispatcher()
{
	if(!s_singleton)
	{
		s_singleton=new KeyboardDispatcher();
	}
	return s_singleton;
}

KeyboardDispatcher::KeyboardDispatcher()
{
}

void KeyboardDispatcher::setEnabled(bool enabled)
{
	if(enabled)
	{
		CCDirector::sharedDirector()->getOpenGLView()->setAccelerometerKeyHook(&wndProcHook);
	}
	else
	{
		CCDirector::sharedDirector()->getOpenGLView()->setAccelerometerKeyHook(NULL);
	}
}

void KeyboardDispatcher::addListener(KeyboardListener *listener)
{
	_listeners.push_back(listener);
}

void KeyboardDispatcher::removeListener(KeyboardListener *listener)
{
	_listeners.remove(listener);
}

void KeyboardDispatcher::KeyDown(WPARAM wParam)
{
	for(list<KeyboardListener*>::iterator it=_listeners.begin();it!=_listeners.end();++it)
	{
		(*it)->onKeyDown(wParam);
	}
}

void KeyboardDispatcher::KeyUp(WPARAM wParam)
{
	for(list<KeyboardListener*>::iterator it=_listeners.begin();it!=_listeners.end();++it)
	{
		(*it)->onKeyUp(wParam);
	}
}