/*!*************************************************************************
\file   MouseCodes.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date   27-09-22
\brief  This file contains the code enums for the mouse buttons.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#pragma once
#include <map>

using MouseCode = short;

struct MouseInput
{
	int buttonPressed = 0; // button pressed - e.g. LMB (0),RMB (1),MMB (2)
	int action = 0; // pressed / released  // 1 is pressed , 0 is released ~ Louis's temp
	// marks the mouse cursor's hotspot's position from the top left
	// of the window screen
	int x = 0, y = 0;
	int mods = 0; // checks if the SHIFT / CTRL / ALT / SUPER keys are pressed.
	bool triggered{ false };
};

// Mouse buttons enums
enum MouseButton : MouseCode
{
	// From glfw3.h
	Button0 = 0, // LMB
	Button1, // RMB
	Button2, // MMB
	Button3,
	Button4,
	Button5,
	Button6,
	Button7,

	ButtonLast = Button7,
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2,

	ButtonCount = 8
};

static std::map<MouseCode, const char*> Mousemap
{
	{MouseButton::Button0, "LMB"},
	{MouseButton::Button1, "RMB"},
	{MouseButton::Button2, "MMB"},
	{MouseButton::Button3, "MB3"},
	{MouseButton::Button4, "MB4"},
	{MouseButton::Button5, "MB5"},
	{MouseButton::Button6, "MB6"},
	{MouseButton::Button7, "MB7"},
};
