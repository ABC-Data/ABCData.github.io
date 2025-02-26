/* !
@file    GLFWsetup.h
@author  a.warrenjunxuan@digipen.edu
@date	 2/9/2022

\brief
	This file contains declaration of struct GLSetup that encapsulates the functions required to create
	an OpenGL context using GLFW.

	The functions include:
	- init
		Initializes a window screen of size specified by user with a title on the title bar.
	- cleanup
		Destroys and terminate the window created
	- error_callback
		Notifies error during and after initialization in case a GLFW function fails
	- framebuffer_callback
		Notifies when window size is changed
	- fps_counter
		Calculates and updates GLSetup::fps when called
*//*__________________________________________________________________________*/

#ifndef GLFWSETUP_H
#define GLFWSETUP_H

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <string>

struct GLSetup {
	/**************************************************************************/
	/*!
	  \brief
		Initializes a window screen of size specified by user with a title on the title bar.

	  \param w
		Width of window.

	  \param h
		Height of window.

	  \param t
		Title in title bar of window.
	*/
	/**************************************************************************/
	static bool init(GLint w, GLint h, std::string t);
	/**************************************************************************/
	/*!
	  \brief
		Destroys and terminate the window created.
	*/
	/**************************************************************************/
	static void cleanup();

// Callback functions
	/**************************************************************************/
	/*!
	  \brief
		In case a GLFW function fails, an error is reported to the GLFW error callback.
	*/
	/**************************************************************************/
	static void error_callback(int error, char const* description);
	/**************************************************************************/
	/*!
	  \brief
		In case framebuffer fails, an error is reported to the framebuffer callback.
	*/
	/**************************************************************************/
	static void framebuffer_callback(GLFWwindow* ptr_win, int w, int h);
	/**************************************************************************/
	/*!
	  \brief
		Check if window is in focus.
	*/
	/**************************************************************************/
	static void window_focus_callback(GLFWwindow* ptr_win, int focused);
	// Additional functions
	/**************************************************************************/
	/*!
	  \brief
		Calculates and updates GLSetup::fps when called.
	*/
	/**************************************************************************/
	static void fps_counter();

	// Static variables
	static GLint width, height;
	static GLint current_width, current_height;
	static std::string title;
	static GLFWwindow* ptr_window;
	static double fps;
	static double delta_time;
	static long long unsigned int frame_no;
	static float aspectratio;
	static bool IsFocus;
};


#endif