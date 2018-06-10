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

#ifndef OIS_OISFFI_H
#define OIS_OISFFI_H

#include "OISPrereqs.h"

#if __cplusplus
extern "C" {
#endif

OISExport intptr_t OIS_FFI OIS_InputManager_Create(intptr_t);

OISExport void OIS_FFI OIS_InputManager_Destroy(intptr_t);

OISExport int OIS_FFI OIS_InputManager_GetNumberOfDevices(intptr_t inputManager, int iType);

OISExport void OIS_FFI OIS_InputManager_EnableAddOnFactory(intptr_t inputManager);

typedef void (OIS_InputManager_EnumrateFreeDeviceCallback)(int type, const char *vendor);

OISExport void OIS_FFI OIS_InputManager_EnumrateFreeDevice(intptr_t inputManager, OIS_InputManager_EnumrateFreeDeviceCallback cb);

OISExport intptr_t OIS_FFI OIS_InputManager_CreateInputObject(intptr_t inputManager, int iType, const char *vendor);

OISExport void OIS_FFI OIS_InputManager_DestroyInputObject(intptr_t inputManager, intptr_t inputObject);

OISExport bool OIS_FFI OIS_InputObject_GetVenderString(intptr_t inputObject, char* buf, size_t bufsize);

OISExport intptr_t OIS_FFI OIS_InputObject_GetCreator(intptr_t inputObject);

OISExport void OIS_FFI OIS_InputObject_Capture(intptr_t inputObject);

OISExport int OIS_FFI OIS_InputObject_GetID(intptr_t inputObject);

OISExport bool OIS_FFI OIS_Keyboard_IsKeyDown(intptr_t inputObject, int keyCode);

OISExport bool OIS_FFI OIS_Keyboard_IsModifierDown(intptr_t inputObject, int modifierKeyCode);

OISExport int OIS_FFI OIS_Mouse_GetPosition(intptr_t inputObject, int axis, int type);

OISExport bool OIS_FFI OIS_Mouse_IsButtonDown(intptr_t inputObject, int button);

OISExport unsigned long OIS_FFI OIS_JoyStick_GetButtons(intptr_t inputObject);

OISExport int OIS_FFI OIS_JoyStick_GetNumberOfComponents(intptr_t inputObject, int componentType);

OISExport int OIS_FFI OIS_JoyStick_GetPOV(intptr_t inputObject, int index);

OISExport int OIS_FFI OIS_JoyStick_GetSlider(intptr_t inputObject, int index);

OISExport float OIS_FFI OIS_JoyStick_GetVector(intptr_t inputObject, int index, int axis);

#if __cplusplus
}
#endif

#endif
