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
#ifndef OIS_Joystick_H
#define OIS_Joystick_H
#include "OISObject.h"
#include "OISEvents.h"
#include <bitset>

namespace OIS
{
	/** @remarks default sensitivity for vector3 component of joystick */
	static const float OIS_JOYSTICK_VECTOR3_DEFAULT = 2.28f;

	//! POV / HAT Joystick component
	class OISExport Pov : public Component
	{
	public:
		Pov() : Component(OIS_POV), direction(0) {}

		enum Direction {
			Centered  = 0,
			North	  = 1,
			South	  = 1<<1,
			East	  = 1<<2,
			West	  = 1<<3,
			NorthEast = North | East,
			SouthEast = South | East,
			NorthWest = North | West,
			SouthWest = South | West,
		};

		int direction;
	};

	//! A sliding axis - only used in Win32 Right Now
	class OISExport Slider : public Component
	{
	public:
		Slider() : Component(OIS_Slider), abX(0), abY(0) {};
		//! true if pushed, false otherwise
		int abX, abY;
	};

	/**
		Represents the state of the joystick
		All members are valid for both buffered and non buffered mode
		Sticks with zero values are not present on the device
	*/
	class OISExport JoyStickState
	{
	public:
		//! Constructor
		JoyStickState() { clear(); }

		enum MaxComponentCount {
			MAX_BUTTONS = 32,
			MAX_AXES    = 8,
			MAX_POVS	= 4,
			MAX_SLIDERS	= 4,
		};

		//! Represents all the buttons (uses a bitset)
		std::bitset<MAX_BUTTONS> mButtons;

		//! Represents all the single axes on the device
		Axis mAxes[MAX_AXES];

		//! Represents the value of a POV. Maximum of 4
		Pov mPOVs[MAX_POVS];

		//! Represent the max sliders
		Slider mSliders[MAX_SLIDERS];

		//! Represents all Vector type controls the device exports
		Vector3 mVectors[MAX_AXES];

		//! internal method to reset all variables to initial values
		void clear()
		{
			mButtons.reset();

			for(auto i : mAxes)
			{
				i.absOnly = true; //Currently, joysticks only report Absolute values
				i.clear();
			}

			for(auto i : mVectors) i.clear();

			for(auto i : mPOVs) i.direction = Pov::Centered;

			for(auto i : mSliders) i.abX = i.abY = 0;
		}
	};

	/** Specialised for joystick events */
	class OISExport JoyStickEvent : public EventArg
	{
	public:
		JoyStickEvent(Object* obj, const JoyStickState& st) : EventArg(obj), state(st) {}
		virtual ~JoyStickEvent() = default;

		const JoyStickState& state;

		// Prevent copying.
		JoyStickEvent(const JoyStickEvent&) = delete;
		JoyStickEvent& operator=(JoyStickEvent) = delete;
	};

	/**
		To recieve buffered joystick input, derive a class from this, and implement the
		methods here. Then set the call back to your JoyStick instance with JoyStick::setEventCallback
		Each JoyStick instance can use the same callback class, as a devID number will be provided
		to differentiate between connected joysticks. Of course, each can have a seperate
		callback instead.
	*/
	class OISExport JoyStickListener
	{
	public:
		virtual ~JoyStickListener() = default;

		/** @remarks Joystick button down event */
		virtual bool buttonPressed(const JoyStickEvent& arg, int button) = 0;

		/** @remarks Joystick button up event */
		virtual bool buttonReleased(const JoyStickEvent& arg, int button) = 0;

		/** @remarks Joystick axis moved event */
		virtual bool axisMoved(const JoyStickEvent& arg, int axis) = 0;

		//-- Not so common control events, so are not required --//
		//! Joystick Event, and sliderID
		virtual bool sliderMoved(const JoyStickEvent& arg, int index)
		{
			OIS_UNUSED(arg);
			OIS_UNUSED(index);
			return true;
		}

		//! Joystick Event, and povID
		virtual bool povMoved(const JoyStickEvent& arg, int index)
		{
			OIS_UNUSED(arg);
			OIS_UNUSED(index);
			return true;
		}

		//! Joystick Event, and Vector3ID
		virtual bool vector3Moved(const JoyStickEvent& arg, int index)
		{
			OIS_UNUSED(arg);
			OIS_UNUSED(index);
			return true;
		}
	};

	/**
		Joystick base class. To be implemented by specific system (ie. DirectX joystick)
		This class is useful as you remain OS independent using this common interface.
	*/
	class OISExport JoyStick : public Object
	{
	public:
		virtual ~JoyStick() = default;

		/**
		@remarks
			Returns the number of requested components
		@param cType
			The ComponentType you are interested in knowing about
		*/
		int getNumberOfComponents(ComponentType cType) const;

		/**
		@remarks
			Sets a cutoff limit for changes in the Vector3 component for movement to
			be ignored. Helps reduce much event traffic for frequent small/sensitive
			changes
		@param degrees
			The degree under which Vector3 events should be discarded
		*/
		void setVector3Sensitivity(float degrees = OIS_JOYSTICK_VECTOR3_DEFAULT);

		/**
		@remarks
			Returns the sensitivity cutoff for Vector3 Component
		*/
		float getVector3Sensitivity() const;

		/**
		@remarks
			Register/unregister a JoyStick Listener - Only one allowed for simplicity. If broadcasting
			is neccessary, just broadcast from the callback you registered.
		@param joyListener
			Send a pointer to a class derived from JoyStickListener or 0 to clear the callback
		*/
		virtual void setEventCallback(JoyStickListener* joyListener);

		/** @remarks Returns currently set callback.. or null */
		JoyStickListener* getEventCallback() const;

		/** @remarks Returns the state of the joystick - is valid for both buffered and non buffered mode */
		const JoyStickState& getJoyStickState() const { return mState; }

		//! The minimal axis value
		static const int MIN_AXIS_VALUE = -32768;

		//! The maximum axis value
		static const int MAX_AXIS_VALUE = 32767;

	protected:
		JoyStick(const std::string& vendor, bool buffered, int devID, InputManager* creator);

		//! Number of Buttons
		int mButtonCount;

		//! Number of axes
		int mAxisCount;

		//! Number of sliders
		int mSliderCount;

		//! Number of POVs
		int mPOVCount;

		//! Number of vector
		int mVectorCount;

		//! The JoyStickState structure (contains all component values)
		JoyStickState mState;

		//! The callback listener
		JoyStickListener* mListener;

		//! Adjustment factor for orientation vector accuracy
		float mVector3Sensitivity;
	};
}
#endif
