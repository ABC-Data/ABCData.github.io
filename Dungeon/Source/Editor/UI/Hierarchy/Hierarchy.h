/*!*************************************************************************
\file	Hierarchy.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date	01-11-2022
\brief	This header file contains the necessary functionalities to operate
		the Hierarchy system. This system is used to display the hierarchy
		of the game objects in the current level scene.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#pragma once
#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "System/System.h"
#include "GameObject/GameObject.h"

enum class SearchFilter : unsigned short { Name = 0, Component = 1, ID = 2, Tag, SearchFilterCount };

class Hierarchy : public System
{
public:
	// Ctor & dtor
	Hierarchy(bool nOpen = true, Vec2 nPos = { 0.0f, 0.0f });
	~Hierarchy();

	Hierarchy(const Hierarchy&) = delete;				// Copy Constructor
	Hierarchy& operator=(const Hierarchy&) = delete;	// Copy Assignment Operator
	Hierarchy(Hierarchy&&) = delete;					// Move Constructor
	Hierarchy& operator=(Hierarchy&&) = delete;		// Move Assignment Operator

	// Init, update and shut the Hierarchy system down.
	virtual void Init();
	virtual void Update(float dt);
	virtual void Shutdown();

	/**************************************************************************/
	/*! \brief
	*	Returns the open state of the Hierarchy window.
	* \return
	* 	True if the Hierarchy window is open, false otherwise.
	*/
	/**************************************************************************/
	bool GetOpen() const;

	/**************************************************************************/
	/*! \brief Sets the open state of the Hierarchy window.
	/**************************************************************************/
	void SetOpen(const bool nOpen);

	/**************************************************************************/
	/*!
	* \brief
	* 	Updates the gameObjects list in the game level, when one is added or
	*	deleted.
	*/
	/**************************************************************************/
	void RefreshHierarchy();

	/**************************************************************************/
	/*!
	* \brief
	* 	Display the game objects in this game scene, and handle related functionalities.
	*/
	/**************************************************************************/
	void DrawHierarchy();

	/**************************************************************************/
	/*!
	* \brief
	*	This section draws and handles filtering mechanisms for the hierarchy list.
	*/
	/**************************************************************************/
	void DisplayGameObjectFilter();

	/**************************************************************************/
	/*!
	* \brief
	*   Display the GO's name in the hierarchy list.
	* \param gameObject
	*  The game object to display.
	* \param underParent
	* True if the game object is under a parent game object, false otherwise.
	/**************************************************************************/
	void DisplayGameObject(GameObject* gameObject, const bool underParent = false);

	/**************************************************************************/
	/*!
	* \brief
	*	Display the game objects' names, and provide handles for functionalities
	*	such as reordering, adding and removing game objects.
	*/
	/**************************************************************************/
	void DisplayHierarchy();

	/**************************************************************************/
	/*!
	* \brief
	*	Search for game object(s) by the name of the filter type object.
	*	Game object(s) that have the filter type object's name will be displayed.
	*
	*	E.g. if the filter type is "Name", and the filter object's name is "Player",
	*	then all game objects that have "Player" in their name will be displayed.
	*	An empty filter object name will display all game objects.
	* \param filter
	*	The search filter to use.
	* \param searchStr
	*	The search string to use.
	* \param cType
	*	The component type enum to find its corresponding component object with.
	*/
	/**************************************************************************/
	void FilterHierarchy(const SearchFilter filter, std::string searchStr, const ComponentType compType);

	/**************************************************************************/
	/*!
	* \brief
	*	Sets the selected game object to the one at	the specified index in the
	*	hierarchy list. Upon clicking on a game object in the hierarchy, this
	*	function should display the game object's properties in the property editor.
	* \param index
	*	The index of the game object in the hierarchy list.
	* \param go
	*	Reference to the game object to set as selected. Used for debug purposes.
	*/
	/**************************************************************************/
	void SelectGameObject(GameObject& go, const int index);

	/**************************************************************************/
	/*!
	* \brief
	*	Finds the index of the GameObject inside the hierarchy
	* \param id
	*	Unique ID of the GameObject
	* \return
	*	Index of the GameObject in the hierarchy
	*/
	/**************************************************************************/
	int GetHierarchyListIndex(const size_t id) const;

	/**************************************************************************/
	/*!
	* \brief
	*	Sets a GameObject up for renaming.
	* \param go
	*	Pointer to the GameObject
	*/
	/**************************************************************************/
	void SetRenameTarget(GameObject* go);

	/**************************************************************************/
	/*!
	* \brief
	*  Returns whether a GameObject is currently being renamed.
	* \return
	* True if a GameObject is being renamed, false otherwise.
	*/
	/**************************************************************************/
	bool IsRenaming() const;
private:
	// List of game objects in level
	std::vector<GameObject*> hierarchyList; GameObject* renameTarget = nullptr;

	// Vector of pairs. First value represents the object ID itself.
	// The other clicks as true if the related object passes the 
	// filter settings entered, if any.
	std::vector<std::pair<int, bool>> hierarchyIds;

	// Whether or not the hierarchy is open
	bool open;

	// Position of hierarchy window
	Vec2 pos;

	bool renaming = false;
};

#endif // !HIERARCHY_H