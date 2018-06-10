/*
The zlib/libpng License

Copyright (c) 2018 Osamu Takasugi

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that
		you wrote the original software. If you use this software in a product,
		an acknowledgment in the product documentation would be appreciated but is
		not required.

    2. Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
//////////////////////////////// OS Nuetral Headers ////////////////
#include "OISInputManager.h"
#include "OISException.h"
#include "OISKeyboard.h"
#include "OISMouse.h"
#include "OISJoyStick.h"
#include "OISEvents.h"

//Advanced Usage
#include "OISForceFeedback.h"

#include "OISFFI.h"

#include <sstream>
#include <algorithm>

using namespace OIS;

intptr_t OIS_CreateInputManager(intptr_t wndptr)
{
	ParamList pl;

	std::ostringstream wnd;
	wnd << wndptr;

	pl.insert(std::make_pair(std::string("WINDOW"), wnd.str()));

	return (intptr_t)InputManager::createInputSystem(pl);
}

void OIS_DestroyInputManager(intptr_t inputManager)
{
	if(inputManager) InputManager::destroyInputSystem((InputManager*)inputManager);
}

int OIS_InputManager_GetNumberOfDevices(intptr_t inputManager, int iType)
{
	return ((InputManager*)inputManager)->getNumberOfDevices((Type)iType);
}

void OIS_InputManager_EnableAddOnFactory(intptr_t inputManager)
{
	((InputManager*)inputManager)->enableAddOnFactory(InputManager::AddOn_All);
}

void OIS_InputManager_EnumrateFreeDevice(intptr_t inputManager, OIS_InputManager_EnumrateFreeDeviceCallback cb)
{
	for(auto i : ((InputManager*)inputManager)->listFreeDevices()) {
		cb(i.first, i.second.c_str());
	}
}

intptr_t OIS_InputManager_CreateInputObject(intptr_t inputManager, int iType, const char *vendor)
{
	return (intptr_t)((InputManager*)inputManager)->createInputObject((Type)iType, false, vendor);
}

void OIS_InputManager_DestroyInputObject(intptr_t inputManager, intptr_t inputObject)
{
	((InputManager*)inputManager)->destroyInputObject((Object*)inputObject);
}

bool OIS_InputObject_GetVenderString(intptr_t inputObject, char* buf, size_t bufsize)
{
	auto obj = (Object*)inputObject;
	if(obj->vendor().size() > bufsize) return false;
	for(int i = 0; i < obj->vendor().size(); ++i) {
		buf[i] = obj->vendor()[i];
	}
	return true;
}

intptr_t OIS_InputObject_GetCreator(intptr_t inputObject)
{
	return (intptr_t)((Object*)inputObject)->getCreator();
}

void OIS_InputObject_Capture(intptr_t inputObject)
{
	((Object*)inputObject)->capture();
}

int OIS_InputObject_GetID(intptr_t inputObject)
{
	return ((Object*)inputObject)->getID();
}

bool OIS_Keyboard_IsKeyDown(intptr_t inputObject, int keyCode)
{
	return ((Keyboard*)inputObject)->isKeyDown((KeyCode)keyCode);
}

bool OIS_Keyboard_IsModifierDown(intptr_t inputObject, int modifierKeyCode)
{
	return ((Keyboard*)inputObject)->isModifierDown((Keyboard::Modifier)modifierKeyCode);
}

bool OIS_Mouse_IsButtonDown(intptr_t inputObject, int button)
{
	return ((Mouse*)inputObject)->getMouseState().buttonDown((MouseButtonID)button);
}

int OIS_Mouse_GetPosition(intptr_t inputObject, int axis, int type)
{
	switch(axis + type * 3) {
	case 0:
		return ((Mouse*)inputObject)->getMouseState().X.abs;
	case 1:
		return ((Mouse*)inputObject)->getMouseState().Y.abs;
	case 2:
		return ((Mouse*)inputObject)->getMouseState().Z.abs;
	case 3:
		return ((Mouse*)inputObject)->getMouseState().X.rel;
	case 4:
		return ((Mouse*)inputObject)->getMouseState().Y.rel;
	case 5:
		return ((Mouse*)inputObject)->getMouseState().Z.rel;
	}
	return 0;
}

unsigned long OIS_JoyStick_GetButtons(intptr_t inputObject)
{
	return ((JoyStick*)inputObject)->getJoyStickState().mButtons.to_ulong();
}

int OIS_JoyStick_GetNumberOfComponents(intptr_t inputObject, int componentType)
{
	return ((JoyStick*)inputObject)->getNumberOfComponents((ComponentType)componentType);
}

int OIS_JoyStick_GetAxis(intptr_t inputObject, int index, int type)
{
	switch(type) {
	case 0:
		return ((JoyStick*)inputObject)->getJoyStickState().mAxes[index].abs;
	case 1:
		return ((JoyStick*)inputObject)->getJoyStickState().mAxes[index].rel;
	}
	return 0;
}

int OIS_JoyStick_GetPOV(intptr_t inputObject, int index)
{
	return ((JoyStick*)inputObject)->getJoyStickState().mPOVs[index].direction;
}

int OIS_JoyStick_GetSlider(intptr_t inputObject, int index, int axis)
{
	switch(axis) {
	case 0:
		return ((JoyStick*)inputObject)->getJoyStickState().mSliders[index].abX;
	case 1:
		return ((JoyStick*)inputObject)->getJoyStickState().mSliders[index].abY;
	}
	return 0;
}

float OIS_JoyStick_GetVector(intptr_t inputObject, int index, int axis)
{
	switch(axis)
	{
		case 0:
			return ((JoyStick*)inputObject)->getJoyStickState().mVectors[index].x;
		case 1:
			return ((JoyStick*)inputObject)->getJoyStickState().mVectors[index].y;
		case 2:
			return ((JoyStick*)inputObject)->getJoyStickState().mVectors[index].z;
	}
	return 0;
}
