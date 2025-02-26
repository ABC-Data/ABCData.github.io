/******************************************************************************/
/*!
\file    Factory.h
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function declarations to implement game object factory.
		 The functions include:
		 - Factory
		 Default constructor.
		 - ~Factory
		 Default constructor.
		 - Init
		 Initializes Factory system.
		 - AddGameObject
		 Add a new game object with serialization.
		 - BuildAndSerialize
		 Get which components are to be in game object and build it.
		 - Shutdown
		 Shuts down Factory system by deleting all game objects.
		 - Destroy
		 To destroy game object.
		 - Update
		 To update game objects.
		 - Clone
		 Make a copy of a game object.
		 - FindObject
		 Finds object given its id.
		 - Instance
		 Needed for GO creation outside of main.
		 - DeleteStaticFactory
		 Delete static factory.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "System/System.h"
#include "GameObject/GameObject.h"
#include "Deserializer/Deserializer.h"
#include "Serializer/Serializer.h"
#include "Transform/Transform.h"
#include "Collision/Collision.h"
#include "RigidBody/RigidBody.h"
#include "Logic/Logic.h"
#include "Scripts/InventoryComponent.h"
#include "Scripts/CombatComponent.h"
#include "Renderer/Renderer.h"
#include "Level/ControllerComponent.h"
#include "Scripts/EnemyComponent.h"
#include "Scripts/LifetimeComponent.h"

class Factory : public System
{
public:
	/*!*************************************************************************
	****
	\brief
		Default constructor.
	****************************************************************************
	***/
	Factory();
	/*!*************************************************************************
	****
	\brief
		Default destructor.
	****************************************************************************
	***/
	~Factory();

	/*!*************************************************************************
	****
	\brief
		To add a new game object.
	\param[in] filename
		File to open.
	\return
		Pointer to addded game object.
	****************************************************************************
	***/
	GameObject* AddGameObject(const std::string& filename);

	/*!*************************************************************************
	****
	\brief
		To add a new game object.
	\param[in] filename
		File to open.
	\return
		Pointer to addded game object.
	****************************************************************************
	***/
	GameObject* AddGameObject(FILE* filename);

	/*!*************************************************************************
	****
	\brief
		To destroy game object
	\param[in] gameObject
		Game object to destroy
	****************************************************************************
	***/
	void Destroy(GameObject* GameObject);

	/*!*************************************************************************
	****
	\brief
		Initializes Factory system.
	****************************************************************************
	***/
	virtual void Init();

	/*!*************************************************************************
	****
	\brief
		To update game objects
	\param[in] dt
		delta time
	****************************************************************************
	***/
	virtual void Update(float dt);

	/*!*************************************************************************
	****
	\brief
		Shuts down Factory system by deleting all game objects.
	****************************************************************************
	***/
	virtual void Shutdown();

	/*!*************************************************************************
	****
	\brief
		Get which components are to be in game object and build it.
	\param[in] filename
		File to open
	\return
		Pointer to built game object
	****************************************************************************
	***/
	GameObject* BuildAndSerialize(const std::string& filename);

	/*!*************************************************************************
	****
	\brief
		Get which components are to be in game object and build it.
	\param[in] fp
		File to open
	\return
		Pointer to built game object
	****************************************************************************
	***/
	GameObject* BuildAndSerialize(FILE* fp);

	/*!*************************************************************************
	****
	\brief
		Make a copy of a game object
	\param[in] GameObj
		Game object to be copied
	****************************************************************************
	***/
	GameObject* Clone(GameObject* GameObj);

	void QueueDelete(GameObject* GO);

	void QueueAdd(GameObject* GO);

	// new Factory
	/*!*************************************************************************
	****
	\brief
		Needed for GO creation outside of main.
	\return
		Created Factory.
	****************************************************************************
	***/
	static Factory* Instance();
	/*!*************************************************************************
	****
	\brief
		Delete static factory above.
	****************************************************************************
	***/
	static void DeleteStaticFactory();

	void ResetLastGOID() { LastGameObjectId = 0; }
	void DecrementGOID() { --LastGameObjectId; }
	

	/**************************************************************************/
	/*!
	* \brief
	*	This function is called in factory::update when the GLFW window has
	*	changed sizes, and it scales up if the window is resized from
	*	windowed to fullscreen. It will do the opposite for the vice-versa
	*	scenario.
	*
	* \param GO_xform_component
	*	Transform component of the selected UI GameObject that is going to be
	*	scaled to the repsective screen resolution
	* \param prev_width
	*	the previous width of the screen to scale from
	* \param prev_heighth
	*	the previous height of the screen to scale from
	* \param current_width
	*	the current width of the screen to scale to.
	* \param current_height
	*	the current height of the screen to scale to.
	*/
	/**************************************************************************/
	void TransScaleUIGO(Transform* GO_xform_component, int prev_width, int prev_height, int current_width, int current_height);
	/*!*************************************************************************
	****
	\brief
		GameObjects that are GAMEUI will be scaled back down when closed on 
		fullscreen mode, going back into the coord system of the default window
		size
	****************************************************************************
	***/


	/**************************************************************************/
	/*!
	* \brief
	*	This function is used to read a txt file and add multiple gameobjects 
	*	in runtime of the game. (Purpose of creating this : Louis needed to add
	*	Options menu game objects during runtime and not have them in the scene
	*	file)
	*
	* \param filename
	*	The file containing the various game objects that are needed to
	*	for the specific purpose.
	* \return
	*	A map of gameobject names and their GO pointers.
	*/
	/**************************************************************************/
	std::map<std::string,GameObject*> AddMultipleGameObjects(const std::string& filename);

	/*!*************************************************************************
	****
	\brief
		Using the parent ID set in the GOs, sets pointers of the children GO vector
		to the parents, and a pointer to the parent in each of the child GO(s).
	\param level
		Level w/ GOs to have parents / children GO's set.
	****************************************************************************
	***/
	void BindGameObjectParents(Level* level);

private:
	unsigned LastGameObjectId; 	// Increment game object ids to make them unique
	std::vector<GameObject*> DeleteQueue;
	std::vector<GameObject*> AddQueue;
};

static Factory* factory = Factory::Instance(); // needed for GO creation outside of main
