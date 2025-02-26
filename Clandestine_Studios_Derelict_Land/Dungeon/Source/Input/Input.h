/*!*************************************************************************
\file   Input.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date   27-09-22
\brief  This header file contains the necessary functionalities to handle
		keyboard and mouse input from the user. This makes use of the 
		GLFW API.
		
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#ifndef INPUT_H
#define INPUT_H

#include "System/System.h"
#include "Graphics/GLFWsetup.h" // GLSetup, GLWindow
#include "KeyCodes.h"
#include "MouseCodes.h"

#include <Windows.h> // HWND

class Input : public System
{
public:
	/**************************************************************************/
	/*!
	\brief
		Constructor.
	*/
	/**************************************************************************/
	Input();

	/**************************************************************************/
	/*!
	\brief
		Destructor.
	*/
	/**************************************************************************/
	virtual ~Input();

	/**************************************************************************/
	/*!
	\brief
		Initializes the Input system.
	*/
	/**************************************************************************/
	virtual void Init();

	/**************************************************************************/
	/*!
	\brief
		Shuts down the Input system.
	*/
	/**************************************************************************/
	virtual void Shutdown();

	/**************************************************************************/
	/*!
	\brief
		Updates the Input system.
	\param dt
		The delta time - time between frames.
	*/
	/**************************************************************************/
	virtual void Update(float dt);

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
	static bool GetKeyDown(unsigned int key); // KeyCodes can be used
	
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
	static bool GetKeyUp(unsigned int key);
	
	/**************************************************************************/
	/*!
	\brief
		Checks if a particular key is held down. (GLFW_RELEASE)
	\param key
		Value of the key to be checked.
	\return
		Returns true if the key is held down, false otherwise.
	*/
	/**************************************************************************/
	static bool GetKeyHeldDown(unsigned int key);

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
	static bool GetKeyTriggered(unsigned int key);

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
	static bool CheckKeyTriggered(unsigned int key);

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
	static bool GetMouseButtonDown(MouseButton button);

	/**************************************************************************/
	/*!
	* \brief
	*	Returns all modifier keys pressed.
	* \return
	* 	Modifier keys pressed as an integer.
	/**************************************************************************/
	static int GetModifierKeys();

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
	static bool GetMouseButtonUp(MouseButton button);

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
	static bool GetMouseTriggered(MouseButton button);

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
	static bool GetMouseTriggeredDepth(MouseButton button, GameObject* GO);
	
	// Checks if a particular mouse button is held down. (Unused)
	// static bool GetMouseButton(MouseButton button);
	
	/**************************************************************************/
	/*!
	\brief
		Gets the struct containing the mouse input data.
	\return
		Returns the struct containing the mouse input data.
	*/
	/**************************************************************************/
	static MouseInput GetMouse();

	// Struct to hold the input data for the mouse
	static MouseInput mouseObj;
	static KeyInput keyObj;
	static std::vector<GameObject*> ClickOverlaps;
};



/**************************************************************************/
/*!
\brief
	Receives GLFW keyboard input and puts in an input object. Said object is
	then passed to the input processing function.
\param	window
	Window to receive input from.
\param	key
	Key pressed.
\param	scancode
	Scancode of the key pressed. Platform-specific.
\param	action
	What action is being performed on the key. 
	(GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT)
\param	mods
	Modifier keys pressed. (GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, GLFW_MOD_ALT,
	GLFW_MOD_SUPER, etc.)
*/
/**************************************************************************/
void GetKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
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
void GetMouseInput(GLFWwindow* window, int button, int action, int mods);
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
void GetHoverInput(GLFWwindow* window, double xpos, double ypos);
/**************************************************************************/
/*!
 \brief
	Processes the input from the user and displays the key pressed as debug
 \param wnd
	Window where input was recorded.
 \param msg
	Key message action - pressed, released.
 \param wParam
	Number of the key input.
 \param lParam
	Scancode of the key input.
*/
/**************************************************************************/
void KeyProcess(KeyInput& keyObj);
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
void MouseProcess(HWND wnd, MouseInput mouse);

/**************************************************************************/
/*!
* \brief
*	Log the current and last key pressed.
* \param key
* 	Key object.
/**************************************************************************/
std::string LogKeyInput();

/**************************************************************************/
/*!
* \brief
*	Log the current and last mouse input.
* \param mouse
* 	Mouse object with the mouse input data.
/**************************************************************************/
std::string LogMouseInput();

void AltTab();

#endif