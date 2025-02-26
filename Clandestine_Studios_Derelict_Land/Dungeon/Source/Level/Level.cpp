/************************************************************************ /
/*!
\file    Level.cpp
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    January 5, 2023
\brief   this file contains the function definitions for the in-place layering
		 system

		Copyright (C) 2023 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Level/Level.h"
#include "Transform/Transform.h"
#include "RigidBody/RigidBody.h"
#include "Collision/Collision.h"
#include "Logic/Logic.h"
#include "GameUI/GameUI.h"
#include "Renderer/Renderer.h"


/**************************************************************************/
	/*!
	\brief
		this function adds the game object, to its respective layers,
		according to the respective components that it owns
	\param item
		the GO to be added
	*/
/**************************************************************************/

void Level::AddGOsPtrsToRespectiveLayers(GameObject* GO)
{

	//std::cout << "BEFORE:" << std::endl;
	//PrintLayerSizes();
	//std::cout << "======================" << std::endl;

	if (dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform)) != nullptr)
	{
		Transform_LAYER.push_back(GO);
	}

	if (dynamic_cast<RigidBody*>(GO->GetComponent(ComponentType::RigidBody)) != nullptr)
	{
		RigidBody_LAYER.push_back(GO);
	}

	if (dynamic_cast<Collision*>(GO->GetComponent(ComponentType::Collision)) != nullptr)
	{
		Collision_LAYER.push_back(GO);
	}

	if (dynamic_cast<Logic*>(GO->GetComponent(ComponentType::Logic)) != nullptr)
	{
		Logic_LAYER.push_back(GO);
	}

	if (dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI)) != nullptr)
	{
		GameUI_LAYER.push_back(GO);
	}

	if (dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer)) != nullptr)
	{
		Renderer_LAYER.push_back(GO);
	}
	//std::cout << "AFTER:" << std::endl;
	//PrintLayerSizes();
	//std::cout << "======================" << std::endl;
}


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

void Level::RemoveGOsPtrsFromSpecificLayers(GameObject* item, ComponentType  comp_type)
{
	/*
	std::cout << "BEFORE:" << std::endl;
	PrintLayerSizes();
	std::cout << "======================" << std::endl;
	*/
	std::vector<GameObject*>::iterator it;

	switch (comp_type)
	{
		case ComponentType::Transform:
			it = std::find(Transform_LAYER.begin(), Transform_LAYER.end(), item);
			if (it != Transform_LAYER.end())
			{
				std::cout << "GO found in Transform Layer" << std::endl;
				Transform_LAYER.erase(it);
				std::cout << "GO erased from Transform Layer" << std::endl;
			}
			break;

		case ComponentType::RigidBody:
			it = std::find(RigidBody_LAYER.begin(), RigidBody_LAYER.end(), item);
			if (it != RigidBody_LAYER.end())
			{
				std::cout << "GO found in RigidBody Layer" << std::endl;
				RigidBody_LAYER.erase(it);
				std::cout << "GO erased from RigidBody Layer" << std::endl;
			}
			break;

		case ComponentType::Collision:
			it = std::find(Collision_LAYER.begin(), Collision_LAYER.end(), item);
			if (it != Collision_LAYER.end())
			{
				std::cout << "GO found in Collision Layer" << std::endl;
				Collision_LAYER.erase(it);
				std::cout << "GO erased from Collision Layer" << std::endl;
			}
			break;

		case ComponentType::Logic:

			it = std::find(Logic_LAYER.begin(), Logic_LAYER.end(), item);
			if (it != Logic_LAYER.end())
			{
				std::cout << "GO found in Logic Layer" << std::endl;
				Logic_LAYER.erase(it);
				std::cout << "GO erased from Logic Layer" << std::endl;
			}
			break;

		case ComponentType::GameUI:

			it = std::find(GameUI_LAYER.begin(), GameUI_LAYER.end(), item);
			if (it != GameUI_LAYER.end())
			{
				std::cout << "GO found in GameUI Layer" << std::endl;
				GameUI_LAYER.erase(it);
				std::cout << "GO erased from GameUI Layer" << std::endl;
			}
			break;

		case ComponentType::Renderer:

			it = std::find(Renderer_LAYER.begin(), Renderer_LAYER.end(), item);
			if (it != Renderer_LAYER.end())
			{
				std::cout << "GO found in Renderer Layer" << std::endl;
				Renderer_LAYER.erase(it);
				std::cout << "GO erased from Renderer Layer" << std::endl;
			}
			break;
	}

	//std::cout << "AFTER:" << std::endl;
	//PrintLayerSizes();
	//std::cout << "======================" << std::endl;
}
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
void Level::AddGOsPtrToSpecificLayer(GameObject* item, ComponentType comp_type)
{
	//std::cout << "BEFORE:" << std::endl;
	//PrintLayerSizes();
	//std::cout << "======================" << std::endl;


	switch (comp_type)
	{
	case ComponentType::Transform:
		Transform_LAYER.push_back(item);
		break;

	case ComponentType::RigidBody:
		RigidBody_LAYER.push_back(item);
		break;

	case ComponentType::Collision:
		Collision_LAYER.push_back(item);
		break;

	case ComponentType::Logic:
		Logic_LAYER.push_back(item);
		break;

	case ComponentType::GameUI:
		GameUI_LAYER.push_back(item);
		break;

	case ComponentType::Renderer:
		Renderer_LAYER.push_back(item);
		break;
	}

	//std::cout << "AFTER:" << std::endl;
	//PrintLayerSizes();
	//std::cout << "======================" << std::endl;
}

/**************************************************************************/
	/*!
	\brief
		this function removes the game object, from its respective layers,
		according to the respective components that it owns
	\param item
		the GO to be removed
	*/
/**************************************************************************/
void Level::RemoveGOsPtrsFromAllLayers(GameObject* item)
{
	//std::cout << "BEFORE:" << std::endl;
	//PrintLayerSizes();
	//std::cout << "======================" << std::endl;

	//std::cout << "items component size " << item->GetComponentList().size()<< std::endl;
	for (auto& comp : item->GetComponentList())
	{
		RemoveGOsPtrsFromSpecificLayers(item ,comp->GetType());
	}

	//std::cout << "AFTER:" << std::endl;
	//PrintLayerSizes();
	//std::cout << "======================" << std::endl;
}
/**************************************************************************/
	/*!
	\brief
		this function prints the sizes of the respective layers. This is
		used in debugging purposes.
	*/
/**************************************************************************/
void Level::PrintLayerSizes()
{
	std::cout << "TransformLayer size: " << Transform_LAYER.size() << std::endl;
	std::cout << "RigidBodyLayer size: " << RigidBody_LAYER.size() << std::endl;
	std::cout << "CollisionLayer size: " << Collision_LAYER.size() << std::endl;
	std::cout << "LogicLayer size: " << Logic_LAYER.size() << std::endl;
	std::cout << "GameUILayer size: " << GameUI_LAYER.size() << std::endl;
	std::cout << "RendererLayer size: " << Renderer_LAYER.size() << std::endl;
}

/**************************************************************************/
		/*!
		\brief
			the following 6 functions are used to get the specific layer from the
			level
		*/
/**************************************************************************/

std::vector<GameObject*>& Level::GetTransformLayer()
{
	return Transform_LAYER;
}

std::vector<GameObject*>& Level::GetRigidBodyLayer()
{
	return RigidBody_LAYER;
}

std::vector<GameObject*>& Level::GetCollisionLayer()
{
	return Collision_LAYER;
}

std::vector<GameObject*>& Level::GetLogicLayer()
{
	return Logic_LAYER;
}

std::vector<GameObject*>& Level::GetGameUILayer()
{
	return GameUI_LAYER;
}

std::vector<GameObject*>& Level::GetRendererLayer()
{
	return Renderer_LAYER;
}
