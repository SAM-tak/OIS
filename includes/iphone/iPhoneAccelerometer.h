/*
The zlib/libpng License

Copyright (c) 2018 Arthur Brainville
Copyright (c) 2015 Andrew Fenn
Copyright (c) 2005-2010 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no
event will the authors be held liable for any damages arising from the use of this
software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to the
following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that
        you wrote the original software. If you use this software in a product,
        an acknowledgment in the product documentation would be appreciated
        but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#ifndef OIS_iPhoneAccelerometer_H
#define OIS_iPhoneAccelerometer_H

#include "OISJoystick.h"
#include "iphone/iPhonePrereqs.h"

#import <UIKit/UIKit.h>
@class iPhoneAccelerometerDelegate;

class JoyStickState;

namespace OIS
{
	class iPhoneAccelerometer : public JoyStick
	{
	public:
		iPhoneAccelerometer(InputManager* creator, bool buffered);
		virtual ~iPhoneAccelerometer();

		/** @copydoc Object::setBuffered */
		virtual void setBuffered(bool buffered);

		void setUpdateInterval(float interval)
		{
			mUpdateInterval = interval;
			[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0f / mUpdateInterval)];
		}

		Vector3 getAccelerometerVector3(void) { return mState.mVectors[0]; }
		/** @copydoc Object::capture */
		virtual void capture();

		/** @copydoc Object::queryInterface */
		virtual Interface* queryInterface(Interface::Type type) { return 0; }

		/** @copydoc Object::_initialize */
		virtual void _initialize();

		void didAccelerate(UIAcceleration* acceleration);

	protected:
		iPhoneAccelerometerDelegate* accelerometerDelegate;

		/** The update frequency of the accelerometer.  Represented in times per second. */
		float mUpdateInterval;
		Vector3 mTempState;
	};
}

#endif // OIS_iPhoneAccelerometer_H
