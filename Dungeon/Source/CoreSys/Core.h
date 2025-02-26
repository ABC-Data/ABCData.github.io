/******************************************************************************/
/*!
\file		Core.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Sep 14, 2022
\brief		Core System header

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
// Include guards
#ifndef CORE_H
#define CORE_H
//Core system
//Manages all SYSTEMS
//Will initialise them
//Will update them
#include "Essential.h"
#include "../System/System.h"
#include "GameObject/GameObject.h"
#include "Level/Level.h"
#include "CoreSys/PerformanceViewer.h"
#include "SceneManager/SceneManager.h"
//#include "GameUI/GameUI.h"

typedef std::chrono::time_point<std::chrono::high_resolution_clock> ClockPoint;

enum class GOSearchPattern {
	ID,
	NAME,
	PREFIX,
	SUFFIX,
	PREFIX_AND_SUFFIX,
	VEC2,
	CLOSEST_FROM_OBJECT,
	PROXIMITY_PREFIX
};

enum class GOSearchComponent {
	NO_COMPONENT,
	WITH_COMPONENT,
};

class Core {
public:
	/**************************************************************************/
	/*!
	  \brief
		Constructor for Core
	*/
	/**************************************************************************/
	Core();

	/**************************************************************************/
	/*!
	  \brief
		Destructor for Core. Doesn't do anything yet
	*/
	/**************************************************************************/
	~Core();

	/**************************************************************************/
	/*!
	  \brief
		Initialises Core. Use after setting up
	*/
	/**************************************************************************/
	void Init();

	/**************************************************************************/
	/*!
	  \brief
		Shuts down Core. Kind of like a destructor
	*/
	/**************************************************************************/
	void Shutdown();

	/**************************************************************************/
	/*!
	  \brief
		Update loop for Core, aka the main game loop
	*/
	/**************************************************************************/
	void Update();

	/**************************************************************************/
	/*!
	  \brief
		Adds a system to the core, do only before initialising

	  \param s
		System to add
	*/
	/**************************************************************************/
	void AddSystem(System* s);

	/**************************************************************************/
	/*!
	  \brief
		Sets the current level to another level

	  \param lvl
		Level to change to
	*/
	/**************************************************************************/
	void SetLevel(Level* lvl);
	
	/**************************************************************************/
	/*!
	  \brief
		Gets the App Time Elapsed

	  \return
		App Time Elapsed
	*/
	/**************************************************************************/
	long double GetAppTimeElapsed();

	/**************************************************************************/
	/*!
	  \brief
		Gets the current level

	  \return
		CurrentLevel
	*/
	/**************************************************************************/
	Level* GetLevel();

	void SetNextLevel(Level* lvl);

	Level* GetNextLevel();

	//Get Game Object tools
	//For all: Return nullptr if not found
	//For all: Only returns active GOs

	/**************************************************************************/
	/*!
	  \brief
		Gets game object by ID.

	  \param id
		ID of object to search

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetGameObject(int id);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object by id and component.

	  \param name
		ID of object to search

	  \param component
		Object has this component?

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetGameObject(int id, ComponentType component);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object by name. Returns first with matching name

	  \param name
		Name of object to search

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetGameObject(std::string name);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object by name and component.

	  \param name
	    Name of object to search

	  \param component
		Object has this component?

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetGameObject(std::string name, ComponentType component);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object closest to a point.

	  \param source
		Point of origin

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetClosestGameObject(Vec2 source, ComponentType component = ComponentType::ComponentTypeSize);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object closest to another game object that is not itself.

	  \param source
		Game object of origin

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetClosestGameObject(GameObject* source, ComponentType component = ComponentType::ComponentTypeSize);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object whose name starts with a certain prefix

	  \param prefix
		Name prefix

	  \param component
		Component to search for

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetObjectByPrefix(std::string prefix, ComponentType component = ComponentType::ComponentTypeSize);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object whose name ends with a certain suffix

	  \param suffix
		Name suffix

	  \param component
		Component to search for

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetObjectBySuffix(std::string suffix, ComponentType component = ComponentType::ComponentTypeSize);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object whose name starts with a certain prefix and ends with a certain suffix

	  \param prefix
	    Name prefix

	  \param suffix
		Name suffix

	  \param component
		Component to search for

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetObjectByPrefixSuffix(std::string prefix, std::string suffix, ComponentType component = ComponentType::ComponentTypeSize);

	/**************************************************************************/
	/*!
	  \brief
		Gets game object whose name starts with a certain prefix and is within a certain proximity to a source

	  \param source
		Vec2 source

	  \param prefix
		Name prefix

	  \param proximity
		Distance between source and GO

	  \param component
		Component to search for

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetObjectInProximityByPrefix(Vec2 source, std::string prefix, float proximity, ComponentType component = ComponentType::ComponentTypeSize);

	/**************************************************************************/
	/*!
	  \brief
		Gets all game objects whose name starts with a certain prefix

	  \param GOA
		Vector container to store results in

	  \param prefix
		Name prefix to search for

	  \param component
		Component to search for
	*/
	/**************************************************************************/
	void GetAllObjectsByPrefix(std::vector<GameObject*>& GOA, std::string prefix, ComponentType component = ComponentType::ComponentTypeSize);

	/**************************************************************************/
	/*!
	  \brief
		Gets all game objects that overlap a certain location

	  \param GOA
		Vector container to store results in

	  \param position
		Position to check

	  \param component
		Component to search for
	*/
	/**************************************************************************/
	void GetAllObjectsAtLocation(std::vector<GameObject*>& GOA, Vec2 position, ComponentType component = ComponentType::ComponentTypeSize);

	/**************************************************************************/
	/*!
	  \brief
		Gets a specified component of a GO.

	  \param T
		Component

	  \param GO
		Game Object to search

	  \return
		Dynamic casted component
	*/
	/**************************************************************************/
	template <typename T>
	T* GetObjectComponent(GameObject* GO) {
		if (GO == nullptr) return nullptr;
		for (Component* C : GO->GetComponentList()) {
			if (dynamic_cast<T*>(C) != nullptr) return dynamic_cast<T*>(C);
		}
		return nullptr;
	}

	/**************************************************************************/
	/*!
	  \brief
		Advanced Game Object Searching function. Covers basically all supported cases.
		!!!PUT COMPONENT FIRST IF SEARCHING WITH COMPONENTS!!!
		WARNING: Typing is REALLY strict for params in pattern. Example: Use std::string("Myname") instead of "Myname"

	  \param pattern
		Search pattern to search for objects. Supply these params for these patterns:
		ID: Object ID (int)
		NAME: Object Name (std::string)
		PREFIX: Name prefix (std::string)
		SUFFIX: Name suffix (std::string)
		PREFIX_AND_SUFFIX: Name prefix (std::string), Name suffix (std::string)
		VEC2: Point (Vec2)
		CLOSEST_FROM_OBJECT: Source Game Object (GameObject*)

	  \param comppattern
		Search pattern for components within objects. Supply these params for these patterns:
		NO_COMPONENT: None - Do not search for components
		WITH_COMPONENT: Component type to search for (ComponentType)

	  \return
		Found game object
	*/
	/**************************************************************************/
	GameObject* GetGameObjectAdvanced(GOSearchPattern pattern, GOSearchComponent comppattern, ...);

	std::vector<System*>& GetSystem();

	/*!*************************************************************************
	****
	\brief
		Instance of Core
	\return
		Created Core.
	****************************************************************************
	***/
	static Core* Instance();
	/*!*************************************************************************
	****
	\brief
		Delete static Core above.
	****************************************************************************
	***/
	static void DeleteStaticCore();
	

private:
	Level* CurrentLevel;

	Level* NextLevel;

	std::vector<System*> Systems;

	bool GameRunning;

	ClockPoint LastTick;

	std::shared_ptr<PerformanceViewer> PV;

	long double AppTimeElapsed;

	//PerformanceViewer* PV;


};

static Core* Engine = Core::Instance();
#endif
