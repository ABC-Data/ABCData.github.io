#pragma once
/************************************************************************ /
/*!
\file    Level.h
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    January 5, 2023
\brief   Class for storing the gameobjects and level's data as the scene manager
		 switches betweens scenes, thus changing levels

		Copyright (C) 2023 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Essential.h"
#include "GameObject/GameObject.h"
#include "Graphics/AssetManager.h"


class Level {
public:
	/**************************************************************************/
		/*!
		\brief
			Default Constructor of Level
		*/
	/**************************************************************************/
	Level() : LevelId{ "Untitled" }, LevelMinBound{ 0.f }, LevelMaxBound{ 0.f }{};
	/**************************************************************************/
		/*!
		\brief
			Constructor of Renderer component
		\param id
			this param is to set the id of the level
		*/
	/**************************************************************************/
	Level(const char* Id) : LevelId{ Id }, LevelMinBound{ 0.f }, LevelMaxBound{ 0.f } {};
	~Level() = default;

	/**************************************************************************/
		/*!
		\brief
			adds GO to the current level's GO array
		\param item
			the GO to be added
		*/
	/**************************************************************************/
	void AddGameObject(GameObject* item) { GameObjectArray.push_back(item); AddGOsPtrsToRespectiveLayers(item); };
	/**************************************************************************/
		/*!
		\brief
			getter func to return this level's GOA
		*/
	/**************************************************************************/
	std::vector<GameObject*>& GetGameObjectArray() { return GameObjectArray; };
	/**************************************************************************/
		/*!
		\brief
			getter func to return this level's GOA size
		*/
	/**************************************************************************/
	size_t GetGOASize() { return GameObjectArray.size(); };
	/**************************************************************************/
		/*!
		\brief
			getter func to return this level's Id
		*/
	/**************************************************************************/
	std::string GetId() { return LevelId; };
	/**************************************************************************/
		/*!
		\brief
			getter func to return this level's assets
		*/
	/**************************************************************************/
	Assets* &GetAssets() { return AssetsManager; };
	/**************************************************************************/
		/*!
		\brief
			setter func to set the bounds of the level's min and max bound
		\param min
			the min bound float value
		\param max
			the max bound float value
		*/
	/**************************************************************************/
	void SetLevelBounds(float min, float max) { LevelMinBound = min; LevelMaxBound = max; };
	/**************************************************************************/
		/*!
		\brief
			getter func to return this level's min bounds
		*/
	/**************************************************************************/
	float GetLevelMin() { return LevelMinBound; };
	/**************************************************************************/
		/*!
		\brief
			getter func to return this level's max bounds
		*/
	/**************************************************************************/
	float GetLevelMax() { return LevelMaxBound; };


	//For LAYERING
	/**************************************************************************/
		/*!
		\brief
			this function adds the game object, to its respective layers,
			according to the respective components that it owns
		\param item
			the GO to be added
		*/
	/**************************************************************************/
	void AddGOsPtrsToRespectiveLayers(GameObject* item);
	/**************************************************************************/
		/*!
		\brief
			this function adds the game object, to a specific layer,
		\param item
			the GO to be added
		\param comp_type
			the component layer to be added into
		*/
	/**************************************************************************/
	void AddGOsPtrToSpecificLayer(GameObject* item, ComponentType comp_type);
	/**************************************************************************/
		/*!
		\brief
			this function removes the game object, from its specific layer,
			according to the respective components that it owns
		\param item
			the GO to be removed
		\param comp_type
			the component layer to be removed from
		*/
	/**************************************************************************/
	void RemoveGOsPtrsFromSpecificLayers(GameObject* item, ComponentType comp_type);
	/**************************************************************************/
		/*!
		\brief
			this function removes the game object, from its respective layers,
			according to the respective components that it owns
		\param item
			the GO to be removed
		*/
	/**************************************************************************/
	void RemoveGOsPtrsFromAllLayers(GameObject* item);
	/**************************************************************************/
		/*!
		\brief
			this function prints the sizes of the respective layers. This is 
			used in debugging purposes.
		*/
	/**************************************************************************/
	void PrintLayerSizes();
	/**************************************************************************/
		/*!
		\brief
			the following 6 functions are used to get the specific layer from the
			level
		*/
	/**************************************************************************/
	std::vector<GameObject*>& GetTransformLayer();
	std::vector<GameObject*>& GetRigidBodyLayer();
	std::vector<GameObject*>& GetCollisionLayer();
	std::vector<GameObject*>& GetLogicLayer();
	std::vector<GameObject*>& GetGameUILayer();
	std::vector<GameObject*>& GetRendererLayer();

private:
	std::string LevelId;
	std::vector<GameObject*> GameObjectArray;
	float LevelMinBound;
	float LevelMaxBound;

	//For LAYERING
	std::vector<GameObject*> Transform_LAYER;
	std::vector<GameObject*> RigidBody_LAYER;
	std::vector<GameObject*> Collision_LAYER;
	std::vector<GameObject*> Logic_LAYER;
	std::vector<GameObject*> GameUI_LAYER;
	std::vector<GameObject*> Renderer_LAYER;


	//TODO: Other things (scripts?)
	Assets* AssetsManager{};
};