/*!*************************************************************************
\file   Input.cpp
\author Anderson Phua
\email: a.phua@digipen.edu
\date   27-09-22
\brief  This source file contains the required functionalities in detail to
		read and process the keyboard and mouse input from the user.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#define GLFW_EXPOSE_NATIVE_WIN32 // glfwGetWin32Window
#include "Editor/UI/Editor.h"
#include "Editor/UI/Logger/Logger.h"
#include "Input.h"
#include "CoreSys/Core.h"
#include <GLFW/glfw3native.h> // glfwGetWin32Window

bool keystateR, keystateC, keystateLM, keystateESCAPE,  prevloop_keystateLM = keystateLM;

extern bool show_debug_Data;
extern bool editor_open;
extern bool is_paused;
extern MouseInput GameWindow_Mouse;
// Camera
bool keystateI, keystateK, keystateJ, keystateL;
// Window Fullscreen Toggle
bool keystateF11;

bool lastKeystateR = keystateR, lastKeystateC = keystateC;

MouseInput MyMouse;
KeyInput Input::keyObj;
MouseInput Input::mouseObj;
std::vector<GameObject*> Input::ClickOverlaps;

/**************************************************************************/
/*!
\brief
	Constructor.
*/
/**************************************************************************/
Input::Input() {}

/**************************************************************************/
/*!
\brief
	Destructor.
*/
/**************************************************************************/
Input::~Input() {}

/**************************************************************************/
/*!
\brief
	Initializes the Input system.
*/
/**************************************************************************/
void Input::Init() {}

/**************************************************************************/
/*!
\brief
	Shuts down the Input system.
*/
/**************************************************************************/
void Input::Shutdown() {}

/**************************************************************************/
/*!
\brief
	Updates the Input system.
\param dt
	The delta time - time between frames.
*/
/**************************************************************************/
void Input::Update(float dt) {
	UNREFERENCED_PARAMETER(dt); 
	if (keystateESCAPE)
	{
		if (!editor_open)
		{
			is_paused = !is_paused;
			keystateESCAPE = false;
		}
	}
	if (GetKeyTriggered(Key::Tilde))
	{
		show_debug_Data = !show_debug_Data;
	}
	//std::cout << "----------------------------\n[Input::Update] is_paused =" << std::boolalpha << is_paused << std::endl;
}

#pragma region Key Input

/**************************************************************************/
/*!
 \brief
	Takes in GLFW input and processes it.
 \param window
	Window where input is taken from.
 \param key
	Number of the key pressed.
 \param scancode
	Scan code of the key pressed.
 \param action
	Whether the key is pressed or released.
 \param mods
	Modifier keys. (Shift, Ctrl, Alt, etc.)
*/
/**************************************************************************/
void GetKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)window;
	// Update key input info if entering first key input or holding down next key input
	if (!Input::keyObj.lastKey) { Input::keyObj.lastKey = Input::keyObj.key = key; }
	else if (key != (signed int)Input::keyObj.key) { Input::keyObj.lastKey = Input::keyObj.key; Input::keyObj.key = key; }


	Input::keyObj.action = action; Input::keyObj.mods = mods; Input::keyObj.scanCode = scancode;
	if (Input::keyObj.triggered == false && Input::keyObj.action == GLFW_PRESS) Input::keyObj.triggered = true;
	Input::keyObj.inputMode = static_cast<short>(glfwGetKey(GLSetup::ptr_window, key));

	// Camera
	keystateI = Input::GetKeyDown(Key::I); keystateK = Input::GetKeyDown(Key::K);
	keystateJ = Input::GetKeyDown(Key::J); keystateL = Input::GetKeyDown(Key::L);

	// Window Fullscreen Toggle
	keystateF11 = Input::GetKeyTriggered(Key::F11);
	keystateESCAPE = Input::GetKeyTriggered(Key::Escape);

	KeyProcess(Input::keyObj);
}

/**************************************************************************/
/*!
\brief
	Checks if a key is pressed through GLFW methods.
\param key
	Key to be checked.
\return
	Returns true if the key is pressed, false otherwise.
*/
/**************************************************************************/
bool Input::GetKeyDown(unsigned int key) { return Input::keyObj.action == GLFW_PRESS && Input::keyObj.key == key; }

/**************************************************************************/
/*!
\brief
	Checks if a key is released.
\param	key
	Value of the key to be checked.
\return
	Returns true if the key is released, false otherwise.
*/
/**************************************************************************/
bool Input::GetKeyUp(unsigned int key) { return Input::keyObj.action == GLFW_RELEASE && Input::keyObj.key == key; }

/**************************************************************************/
/*!
\brief
	Checks if a particular key is held down. (GLFW_REPEAT)
\param key
	Value of the key to be checked.
\return
	Returns true if the key is held down, false otherwise.
*/
/**************************************************************************/
bool Input::GetKeyHeldDown(unsigned int key) { return (Input::keyObj.action == GLFW_REPEAT) && (Input::keyObj.key == key); }

/**************************************************************************/
/*!
	\brief
	Checks if a key has been pressed once.
	\param key
	Value of the key to be checked.
	\return
	Returns true if the key has been pressed once, false otherwise.
*/
/**************************************************************************/
bool Input::GetKeyTriggered(unsigned int key) {
	if (Input::keyObj.key == key && Input::keyObj.triggered) {
		Input::keyObj.triggered = false;
		return true;
	}
	return false;
}

/**************************************************************************/
/*!
	\brief
	Checks if a key has been pressed once.
	\param key
	Value of the key to be checked.
	\return
	Returns true if the key has been pressed once, false otherwise.
*/
/**************************************************************************/
bool Input::CheckKeyTriggered(unsigned int key)
{
	if (Input::keyObj.key == key && Input::keyObj.triggered) {
		return true;
	}
	return false;
}

/**************************************************************************/
/*!
 \brief
	Processes the input from the user and displays the key pressed as debug
 \param wnd
	Window where input was recorded.
 \param msg
	Key message action - pressed, released.
 \param key
	Number of the key input.
 \param lParam
	Scancode of the key input.
*/
/**************************************************************************/
void KeyProcess(KeyInput& ko)
{
	switch (ko.inputMode)
	{
	case GLFW_PRESS:
	{
		switch (ko.key)
		{
		case Key::Space:
		{
			//editor->Log("Space1");
			break;	
		}
		case Key::LeftAlt:
		case Key::RightAlt:
		{
			// F4 + LAlt / RAlt to quit
			if (glfwGetKey(GLSetup::ptr_window, Key::F4) == GLFW_PRESS)
			{
				// Cuts the engine's main update loop - it depends on this.
				glfwSetWindowShouldClose(GLSetup::ptr_window, true);
				// PostQuitMessage(0); DestroyWindow(wnd); // Formerly the Win32 way --> quit app, close window
			}

			// Testing
			// if (glfwGetKey(GLSetup::ptr_window, Key::Tab) == GLFW_PRESS) { AltTab(); }
			break;
		}
		}
		break;
	}
	}
}

/**************************************************************************/
/*!
* \brief
*	Log the current and last key pressed.
* \param key
* 	Key object.
/**************************************************************************/
std::string LogKeyInput()
{
	// Debug output only if the key is pressed down for the first frame
	if (Input::keyObj.key != 0)
	{
		std::stringstream keyStream;
		keyStream << "Last Key: " << Key::Keymap[(Keycode)Input::keyObj.lastKey] << " | ";
		keyStream << "Current Key: " << Key::Keymap[(Keycode)Input::keyObj.key] << std::endl;
		return keyStream.str();
	}
	return std::string("No key input yet.");
}

#pragma endregion

#pragma region Mouse Input

/**************************************************************************/
/*!
\brief
	Checks if a particular mouse button is pressed.
\param	button
	MouseCode enum of the mouse button to be checked.
\return
	Returns true if the mouse button is pressed, false otherwise.
*/
/**************************************************************************/
bool Input::GetMouseButtonDown(MouseButton button) { return Input::mouseObj.action == GLFW_PRESS && Input::mouseObj.buttonPressed == button; }

/**************************************************************************/
/*!
* \brief
*	Returns all modifier keys pressed.
* \return
* 	Modifier keys pressed as an integer.
/**************************************************************************/
int Input::GetModifierKeys() { return Input::keyObj.mods; }

/**************************************************************************/
/*!
\brief
	Checks if a particular mouse button is released.
\param	button
	Mousecode enum - of the button to be checked.
\return
	Returns true if the mouse button is released, false otherwise.
*/
/**************************************************************************/
bool Input::GetMouseButtonUp(MouseButton button) { return Input::mouseObj.action == GLFW_RELEASE && Input::mouseObj.buttonPressed == button; }

/**************************************************************************/
/*!
	\brief
	Checks if a mouse key has been pressed once.
	\param key
	Value of the key to be checked.
	\return
	Returns true if the key has been pressed once, false otherwise.
*/
/**************************************************************************/
bool Input::GetMouseTriggered(MouseButton button) {
	return (Input::mouseObj.triggered && Input::mouseObj.buttonPressed == button);
}

/**************************************************************************/
/*!
	\brief
	Checks if a mouse key has been pressed once. If so, consume trigger check
	\param key
	Value of the key to be checked.
	\return
	Returns true if the key has been pressed once, false otherwise.
*/
/**************************************************************************/
bool Input::GetMouseTriggeredDepth(MouseButton button, GameObject* GO) {
	if (Input::mouseObj.triggered && Input::mouseObj.buttonPressed == button) {
		if (Input::ClickOverlaps.size() > 0) {
			if (Input::ClickOverlaps.back() == GO || Input::ClickOverlaps.back()->layer != "UI") {
				Input::mouseObj.triggered = false;
				return true;
			}
		}
	}
	return false;
}

/**************************************************************************/
/*!
\brief
	Gets the struct containing the mouse input data.
\return
	Returns the struct containing the mouse input data.
*/
/**************************************************************************/
MouseInput Input::GetMouse() { return Input::mouseObj; }

/**************************************************************************/
/*!
\brief
	Receives GLFW mouse input and puts in an MouseInput object. Said object is
	then passed to the relevant input processing function.
\param window
	Window to receive input from.
\param button
	Mouse button pressed.
\param action
	What action is being performed on the mouse button.
	(GLFW_PRESS, GLFW_RELEASE)
\param mods
	Modifier keys pressed. (GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, GLFW_MOD_ALT,
	GLFW_MOD_SUPER, etc.)
*/
/**************************************************************************/
void GetMouseInput(GLFWwindow* window, int button, int action, int mods)
{
	//std::cout << " Get mouse input called" << std::endl;

	Input::mouseObj.buttonPressed = button; Input::mouseObj.action = action; Input::mouseObj.mods = mods;
	//std::cout << "B: " << Input::mouseObj.buttonPressed << " A: " << Input::mouseObj.action << " Tb: " << Input::mouseObj.triggered;
	if (Input::mouseObj.triggered == false && Input::mouseObj.action == GLFW_PRESS) Input::mouseObj.triggered = true;
	//std::cout << " Ta: " << Input::mouseObj.triggered << std::endl;
	if (Input::GetMouseButtonDown(MouseButton::Button0)) {
		prevloop_keystateLM = keystateLM;
		keystateLM = TRUE;
		if (!editor_open) // if the editor isn't open, use the GLFW mouse object
		{
			MyMouse = Input::GetMouse();
		}
		else // editor is open, use the editor side mouse input
		{
			MyMouse = GameWindow_Mouse;
			GameWindow_Mouse.buttonPressed = 0;
			GameWindow_Mouse.action = 1;

		}

		// Mouse
		if (Input::GetMouseButtonDown(MouseButton::ButtonLeft))
		{
			//std::cout << ("\nGLFW Mouse Position " + std::to_string(mouseObj.x) + " "
			//	+ std::to_string(mouseObj.y) + "\n");
		}
		Input::ClickOverlaps.clear();
		Vec2 clicked_pos;
		clicked_pos.x = Batch::camera.GetCameraPosition().x + ((float)MyMouse.x / ((float)GLSetup::current_width / (float)GLSetup::width));
		clicked_pos.y = Batch::camera.GetCameraPosition().y + (float)((GLSetup::current_height - MyMouse.y) / ((float)GLSetup::current_height / (float)GLSetup::height));
		Engine->GetAllObjectsAtLocation(Input::ClickOverlaps, clicked_pos);

		std::sort(Input::ClickOverlaps.begin(), Input::ClickOverlaps.end(), [](GameObject* a, GameObject* b) {return a->GetComponent<Transform>()->Depth < b->GetComponent<Transform>()->Depth; });
		
		/*std::cout << clicked_pos.x << " " << clicked_pos.y << std::endl;
		for (GameObject* GO : Input::ClickOverlaps) {
			std::cout << GO->GetName() << " ";
		}
		std::cout << std::endl;*/
		//std::cout << "MyMouse MousePos.x :" << MyMouse.x << std::endl;
		//std::cout << "MyMouse mousePos.y :" << MyMouse.y << std::endl;
		//std::cout << "GameWindow MousePos.x :" << GameWindow_Mouse.x << std::endl;
		//std::cout << "GameWindow mousePos.y :" << GameWindow_Mouse.y << std::endl;
		//std::cout << "GameWindow mousePos.y translated :" << 900 - GameWindow_Mouse.y << std::endl;
	}
	if (Input::GetMouseButtonUp(MouseButton::Button0)) {
		//std::cout << " MOUSE IS UP" << std::endl;
		prevloop_keystateLM = keystateLM;
		keystateLM = FALSE;

		if (editor_open)
		{
			GameWindow_Mouse.buttonPressed = 0;
			GameWindow_Mouse.action = 0;
		}
	}
	MouseProcess(glfwGetWin32Window(window), Input::mouseObj);
}

/**************************************************************************/
/*!
\brief
	Receives GLFW mouse movement input and puts it in an MouseInput object.
	Said object is then passed to the relevant input processing function.
\param window
	Window to receive input from.
\param xpos
	X position of the mouse cursor.
\param ypos
	Y position of the mouse cursor.
*/
/**************************************************************************/
void GetHoverInput(GLFWwindow* window, double xpos, double ypos)
{
	(void)window;
	Input::mouseObj.x = (int)xpos; Input::mouseObj.y = (int)ypos;
	// MouseProcess(glfwGetWin32Window(window), mouseObj);
}

/**************************************************************************/
/*!
\brief
	A function to handle the mouse input on the window.
\param wnd
	Handle to the main window where input is recorded and processed.
\param mouse
	Mouse object with the mouse input data.
*/
/**************************************************************************/
void MouseProcess(HWND wnd, MouseInput mouse)
{
	UNREFERENCED_PARAMETER(wnd);
	switch (mouse.action)
	{
	case GLFW_PRESS:
	{
		switch (mouse.buttonPressed)
		{
		case Button0: // LMB
		{
			break;
		}
		case Button1: // RMB
		{
			break;
		}
		case Button2: // MMB
		{
			break;
		}
		}
	}
	}
}

/**************************************************************************/
/*!
* \brief
*	Log the current and last mouse input.
* \param mouse
* 	Mouse object with the mouse input data.
/**************************************************************************/
std::string LogMouseInput()
{
	std::stringstream mouseStream;
	mouseStream << "Mouse Position: X: " << Input::mouseObj.x << ", Y: " << Input::mouseObj.y << " | ";
	mouseStream << "Mouse Button: " << Mousemap[(MouseCode)Input::mouseObj.buttonPressed] << " | ";
	mouseStream << "Mouse Action: " << glfwGetMouseButton(GLSetup::ptr_window, Input::mouseObj.buttonPressed) << std::endl;

	return mouseStream.str();
}

#pragma endregion

void AltTab(void)
{
	if (glfwGetWindowAttrib(GLSetup::ptr_window, GLFW_VISIBLE))
	{
		glfwIconifyWindow(GLSetup::ptr_window); // Minimizes the window
	}
	else
	{
		glfwRestoreWindow(GLSetup::ptr_window); // Restores the window
	}
}
