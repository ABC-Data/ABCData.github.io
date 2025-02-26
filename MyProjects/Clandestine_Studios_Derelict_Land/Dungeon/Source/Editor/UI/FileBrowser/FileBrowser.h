/*!*************************************************************************
\file   FileBrowser.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date   26-11-22
\brief	Functionalities for the FileBrowser system. This system is used to
		allow the user to browse through and interact with the files in the
		project directory.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#pragma once
#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include "System/System.h"
#include "Graphics/GLFWsetup.h" // GLSetup, GLWindow

#include <filesystem>

class FileBrowser : public System
{
	// Ctor + Dtor
	FileBrowser(bool nOpen = true, float nWidth = 400.0f, float nHeight = 600.0f);
	~FileBrowser();
public:
	// Init, update & shut down the FileBrowser system
	virtual void Init();
	virtual void Shutdown();
	virtual void Update(float dt);

	/**************************************************************************/
	/*!
	* \brief
	*	Retrieves open status of the FileBrowser system.
	* \return
	* 	True if the FileBrowser system is open, false otherwise.
	*/
	/**************************************************************************/
	bool GetOpen() const;
	
	/**************************************************************************/
	/*!
	* \brief
	* 	Sets the open status of the FileBrowser system.
	* \param nOpen
	* 	The new open status.
	*/
	/**************************************************************************/
	void SetOpen(const bool nOpen);

	/**************************************************************************/
	/*! \brief Produces and / or returns the only instance of the FileBrowser.
	/**************************************************************************/
	static FileBrowser* GetInstance();

	/**************************************************************************/
	/*! \brief Draws the FileBrowser system, and handles its functionalities.
	/**************************************************************************/
	void DrawFileBrowser();
	
	/**************************************************************************/
	/*!
	* \brief
	*	If the path is invalid, the current path will not be changed.
		Otherwise, the current path will be set to the path specified.
	* \param nPath
	*	The path to set the current path to.
	*/
	/**************************************************************************/
	void SetCurrentPath(const std::string_view nPath);

	/**************************************************************************/
	/*!
	* \brief
	*	Returns total number of child files & directories under the input path.
	* \param nPath
	*	The path to count the number of child files & directories under.
	* \return
	*	A pair of 2 int's. First number's the file count, second's the number of
	*	directories / folders.
	*/
	/**************************************************************************/
	std::pair<int,int> ChildrenCount(const std::string_view nPath) const;
	
	/**************************************************************************/
	/*!
	* \brief
	*	Draws, and operates the context menu operations for the file / directory
	*	specified by the path, when the user right-clicks on it.
	* \param path
	*	The path to the file / directory to draw the context menu for.
	*/
	/**************************************************************************/
	void UseContextMenu(const std::filesystem::directory_entry& entry);

	/**************************************************************************/
	/*!
	* \brief
	*	Allows the user to rename the file / directory specified by the path
	*	from within the FileBrowser editor subsystem.
	* \param entry
	*	The file / directory to rename.
	*/
	/**************************************************************************/
	void RenameFile(const std::filesystem::directory_entry& entry);

private:

	void DrawDirectoryTreeView(const std::filesystem::directory_entry& root);

	bool open = true, focused = true, renameFlag = false;
	float size[2]{ 0 };
	std::string currentPath, currentDirectory;
	const std::string projectDir;
	std::string fileToRename;

	// list of paths traversed by the user.
	std::vector<std::string> pathHistory;
	unsigned short pathListIndex = 0;
};	

namespace { bool instanced = false; }
#endif // !FILEBROWSER_H
