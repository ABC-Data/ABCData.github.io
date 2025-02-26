/*!*************************************************************************
\file	MenuBar.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date	01-11-22
\brief	This header file contains the necessary functionalities to operate
		the MenuBar system. This system is used to display the menu bar
		at the top of the game / level editor window.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#ifndef MENUBAR_H
#define MENUBAR_H

#include "System/System.h"

/*
	The MenuBar is a menu bar usually at the top of the screen
	which contains menus that can be clicked on to perform
	actions.
*/
class MenuBar : public System
{
public:
	MenuBar(bool nOpen = true);
	~MenuBar();

	virtual void Init(); // Initialize the MenuBar system
	virtual void Shutdown(); // Shutdown the MenuBar system
	virtual void Update(float dt); // Update the MenuBar system

	/**************************************************************************/
	/*!
	* \brief
	* 	Set the open state of the MenuBar.
	* \param nOpen
	* 	New open state - whether the menu bar is displayed.
	*/
	/**************************************************************************/
	void SetOpen(bool nOpen);

	void NewEmptyGameObject();

	// Insert a new tetxure into the asset manager's texture list
	void LoadNewTexture(std::string_view textureFilename);

	void LoadNewAudio(std::string_view audioFilename);
private:
	/**************************************************************************/
	/*!
	* \brief Draw the MenuBar, and handle the menu bar's functionalities.
	*/
	/**************************************************************************/
	void DrawMenuBar();
	
private:
	bool open; // Is the menu open?
};

#endif