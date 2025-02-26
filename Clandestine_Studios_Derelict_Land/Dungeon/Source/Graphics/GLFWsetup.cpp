/* !
@file    GLFWsetup.cpp
@author  a.warrenjunxuan@digipen.edu
@date	 2/9/2022

@brief	 This file contains declaration of struct GLSetup that encapsulates the functions required to create
		 an OpenGL context using GLFW.
*//*__________________________________________________________________________*/
#include <iostream>
#include <sstream>
#include "Graphics/GLFWsetup.h"
#include "Input/Input.h" // GetKeyInput

// Static variables
GLint GLSetup::width;
GLint GLSetup::height;
GLint GLSetup::current_width;
GLint GLSetup::current_height;
std::string GLSetup::title;
GLFWwindow* GLSetup::ptr_window;
double GLSetup::fps;
double GLSetup::delta_time;
long long unsigned int GLSetup::frame_no;
float GLSetup::aspectratio;
bool GLSetup::IsFocus;

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
bool GLSetup::init(GLint w, GLint h, std::string t) {
	GLSetup::width = w ;
	GLSetup::height = h;
	GLSetup::current_width = w;
	GLSetup::current_height = h;
	GLSetup::title = t;
	GLSetup::frame_no = 0;
	GLSetup::aspectratio = (float)(w / h);
	GLSetup::IsFocus = true;

	// Check GLFW init
	if (!glfwInit()) {
		std::cout << "GLFW init has failed - abort program!!!" << std::endl;
		return false;
	}
	// Report error to callback function if GLFW fails
	glfwSetErrorCallback(GLSetup::error_callback);

	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);

	glfwWindowHint(GLFW_STENCIL_BITS, 8);;// used for FrameBuffer
	
	// Creates Width by Height windowed mode window specified by perimeter with opengl context.
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
#ifndef _DEBUG		
	GLSetup::ptr_window = glfwCreateWindow(glfwGetVideoMode(monitor)->width, glfwGetVideoMode(monitor)->height, GLSetup::title.c_str(), glfwGetPrimaryMonitor(), NULL);
	if (!GLSetup::ptr_window) {
		std::cerr << "GLFW unable to create OpenGL context - abort program\n";
		GLSetup::cleanup();
		return false;
	}
#else
	(void)monitor;
	GLSetup::ptr_window = glfwCreateWindow(GLSetup::current_width, GLSetup::current_height, GLSetup::title.c_str(), NULL, NULL);
	if (!GLSetup::ptr_window) {
		std::cerr << "GLFW unable to create OpenGL context - abort program\n";
		GLSetup::cleanup();
		return false;
	}
#endif

	glfwMakeContextCurrent(GLSetup::ptr_window);

	glfwSetFramebufferSizeCallback(ptr_window, GLSetup::framebuffer_callback);

	// Setting functions to take in key-mouse input from GLFW
	glfwSetKeyCallback(ptr_window, GetKeyInput);
	glfwSetMouseButtonCallback(ptr_window, GetMouseInput);
	glfwSetCursorPosCallback(ptr_window, GetHoverInput);
	glfwSetWindowFocusCallback(ptr_window, window_focus_callback);
	
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Unable to initialize GLEW - error: "
			<< glewGetErrorString(err) << " abort program" << std::endl;
		return false;
	}
	if (GLEW_VERSION_4_5) {
		std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
	}
	else {
		std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
		return false;
	}

	return true;
}

/**************************************************************************/
/*!
  \brief
	Destroys and terminate the window created.
*/
/**************************************************************************/
void GLSetup::cleanup() {
	glfwDestroyWindow(GLSetup::ptr_window);
	glfwTerminate();
}

// In case a GLFW function fails, an error is reported to the GLFW error callback.
// You can receive these reports with an error callback.
// This function must have the signature below but may do anything permitted in other callbacks.
/**************************************************************************/
	/*!
	  \brief
		In case a GLFW function fails, an error is reported to the GLFW error callback.
	*/
/**************************************************************************/
void GLSetup::error_callback(int error, char const* description) {
	error;

	std::cerr << "GLFW error: " << description << std::endl;
}
/**************************************************************************/
	/*!
	  \brief
		In case framebuffer fails, an error is reported to the framebuffer callback.
	*/
/**************************************************************************/
void GLSetup::framebuffer_callback(GLFWwindow* ptr_win, int w, int h) {
	glfwGetFramebufferSize(ptr_win, &w, &h);
	glViewport(0, 0, w, h);
}
/**************************************************************************/
/*!
  \brief
	Check if window is in focus.
*/
/**************************************************************************/
void GLSetup::window_focus_callback(GLFWwindow* ptr_win, int focused)
{
	(void)ptr_win;
	if (!focused) {
#ifndef _DEBUG
		glfwIconifyWindow(ptr_win);
#endif
		IsFocus = false;
	}
	else
		IsFocus = true;
}
/**************************************************************************/
	/*!
	  \brief
		Calculates and updates GLSetup::fps when called.
	*/
/**************************************************************************/
void GLSetup::fps_counter() {
	static double previous_time = glfwGetTime();
	double current_time = glfwGetTime();
	GLSetup::delta_time = current_time - previous_time;
	previous_time = current_time;
	
	static double frame = 0.0;
	static double start_time = glfwGetTime();
	double elapsed_time = current_time - start_time;
	frame++;

	double fps_interval = 1.0;
	fps_interval = (fps_interval < 0.0) ? 0.0 : fps_interval;

	if (elapsed_time > fps_interval) {
		GLSetup::fps = frame/ elapsed_time;
		start_time = current_time;
		frame = 0.0;		

		std::stringstream ss;
		ss << GLSetup::title << " | FPS: " << GLSetup::fps;
		glfwSetWindowTitle(GLSetup::ptr_window, ss.str().c_str());
		ss.str("");
		ss.clear();
	}
}

