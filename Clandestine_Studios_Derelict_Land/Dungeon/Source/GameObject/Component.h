/******************************************************************************/
/*!
\file		Component.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Sep 14, 2022
\brief		Base Component class header

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
//Component
//Gets its behaviour from System


//List of components that exist
enum class ComponentType {
	Transform,	//1
	RigidBody,	//2
	Collision,	//3
	Logic,		//4
	GameUI,
	Renderer,
	Controller,
	Animate,
	Lighting,

	//BELOW ARE SCRIPT ONLY COMPONENTS
	LogicTargets,
	PlayerCombat,
	Inventory,
	EnemyCombat,
	Lifetime,

	//Max
	ComponentTypeSize
};

static std::vector<std::string> ComponentTypeStrings{
	"Transform",
	"RigidBody",
	"Collision",
	"Logic",
	"GameUI",
	"Renderer",
	"Controller",
	"Animate",
	"Lighting",
	"LogicTargets",
	"PlayerCombat",
	"Inventory",
	"EnemyCombat",
	"Lifetime"
};

class Component {
public:

	virtual ~Component() = default;
	//For linkage
	friend class GameObject;

	virtual void Init() {};

	//Serialize TO file
	virtual void Serialize(FILE* fp) = 0;

	//Deserialize FROM file
	virtual void Deserialize(FILE* fp) = 0;

	//Copy data from Target to this
	virtual void CopyData(Component* Target) = 0;
	
	//Get Owner Game Object
	GameObject* GetComponentOwner() { return Owner; }

	//Set Owner Game Object
	void SetComponentOwner(GameObject* NewOwner) { Owner = NewOwner; }

	//Get Type of this component
	ComponentType GetType() { return Type; }

	//Set Type of this component
	void SetType(ComponentType type) { Type = type; }

private:
	//The Game Object that owns this component
	GameObject* Owner{};
	//The type of the component
	ComponentType Type{};
};