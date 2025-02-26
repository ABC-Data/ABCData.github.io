/******************************************************************************/
/*!
\file		GameObject.h
\author 	Brandon Hsu
\author		Anderson Phua
\par    	email: b.hsu\@digipen.edu , 620007421
\par    	email: a.phua\@digipen.edu , 620009021
\date   	Sep 14, 2022
\brief		Game Object header

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Component.h"
#include "Essential.h"
#include <bitset>

class GameObject {
public:	
	
	void Init(int ObjId);

	void Destroy();
	
	/******************************************************************************/
	/*!
	* \brief
	*	Gets the component of the specified type.
	* \param type
	*	The type of the component to get.
	* \return
	*	The component of the specified type, if it exists. Otherwise, a nullptr is
	*	returned.
	*/
	/******************************************************************************/
	Component* GetComponent(const ComponentType type);

	/******************************************************************************/
	/*!
	* \brief
	* Checks if this GO has a specific component.
	* \param type
	* The type of component to check for.
	* \return
	* True if the component exists, false otherwise.
	*/
	/******************************************************************************/
	bool HasComponent(const ComponentType type);
	
	/*!*************************************************************************
	* \brief
	* Templated overload of GetComponent. Returns a pointer to the component
	* of the specified type.
	* \param[in] T
	* The type of component to get.
	* \return
	* A pointer to the component of the specified type. Returns nullptr if
	* the component does not exist.
	/***************************************************************************/
	template <typename T> T* GetComponent() 
	{
		for (Component* comp : Components) {if (dynamic_cast<T*>(comp) != nullptr) return dynamic_cast<T*>(comp);}
		return nullptr;
	}
	
	/******************************************************************************/
	/*!
	* \brief
	* Adds a component of the specified type to the GameObject.
	* \param type
	* The type of the component to add.
	* \param comp
	* The component to add.
	*/
	/******************************************************************************/
	void AddComponent(const ComponentType type, Component* comp);
	
	/*!*************************************************************************
	* \brief
	* Templated overload of AddComponent. Adds a component to the game object.
	* \param[in] T
	* The type of component to add.
	/***************************************************************************/
	template <typename T> void AddComponent()
	{
		for (Component* c : Components) {if (dynamic_cast<T*>(c) != nullptr) return;}
		Components.push_back(new T());
	}

	/******************************************************************************/
	/*!
	* \brief
	* Removes the component of the specified type from the GameObject. If the component
	* does not exist, nothing happens.
	* \param type
	* The type of the component to remove.
	*/
	/******************************************************************************/
	void RemoveComponent(const ComponentType type);
	std::vector<Component*>& GetComponentList();

	bool& GetActive();
	void SetActive(const bool active);

	const std::string& GetName();
	void SetName(const std::string nName);

	const int& GetObjectId() { return ObjectId; }
	void SetObjectId(int id) { ObjectId = id; }

	/*!*************************************************************************
	* \brief
	* Checks whether the GameObjects are the same via their ObjectIds.
	* \param[in] other
	* The GameObject to compare to.
	* \return
	* True if the GameObjects are the same, false otherwise.
	/***************************************************************************/
	bool operator==(const GameObject& other) const { return ObjectId == other.ObjectId; }
	bool operator!=(const GameObject& other) const { return !(ObjectId == other.ObjectId); }

	// Is this object's tag the same as t?
	bool CompareTag(const std::string_view t) const;
	static void AddTag(const std::string nTag);
	static void RemoveTag(const std::string_view nTag);

	static void AddLayer(const std::string nLayer, const unsigned int index);
	static void UpdateLayer(const std::string nLayer, const unsigned int index);
	static void RemoveLayer(const std::string nLayer);
	static int GetLayerIndex(const std::string nLayer);
	
	// Finds the first active GameObject by its name, inside the GOA.
	static GameObject* Find(const char* name);
	static GameObject* Find(const char* name, std::vector<GameObject*>& gObjects);
	
	/* 
		Returns one active GameObject tagged tag. Returns null if no GameObject was found.
		Tags must be declared in the tag manager before using them. An exception is thrown 
		if the tag does not exist or if an empty string or null is passed as the tag.
	*/
	static GameObject* FindWithTag(const std::string& fTag, std::vector<GameObject*>& gObjects);
	static GameObject* FindWithLayer(const std::string& fLayer, std::vector<GameObject*>& gObjects);

	// Returns the ordered tag map
	static std::map<unsigned int, std::string>& GetTagMap();
	static std::vector<std::string>& GetLayers();
	static std::bitset<32>& GetLayerMask();
	static void SetLayerMask(std::bitset<32> mask);

	std::string tag, layer;
	std::string sortingLayer;
	
	int parentID = 0; // ID's start from 1
	GameObject* parent = nullptr;
	std::vector<GameObject*>& GetChildren();
	bool HasChildren() { return children.size() > 0; }

private:
	static std::vector<GameObject*>GameObjectArray;
	static std::map<unsigned int, std::string> Tags;
	
	static std::vector<std::string> Layers;
	static std::bitset<32> layerMask;
	
	std::vector<Component*> Components;
	std::string name;
	bool active = true;

	std::vector<GameObject*> children;
public:
	int ObjectId = 0;
	
	/**************************************************************************/
	/*!
	* \brief
	*	Gets an vector of Components of the specified type in the GameObject's children.
	* \return
	*	Vector of Component pointers of the specified type.
	*/	
	/**************************************************************************/
	template <typename T> std::vector<T*> GetComponentsInChildren()
	{
		std::vector<T*> childrenComponents;
		for (int j = 0; j < children.size(); ++j) { childrenComponents.push_back(children[j]->GetComponent<T>()); }
		return childrenComponents;
	}

	/**************************************************************************/
	/*!
	* \brief
	*	Gets an vector of Components of the specified type in the GameObject's parent(s
	* \return
	*	Vector of Component pointers of the specified type.
	*/
	/**************************************************************************/
	template <typename T> std::vector<T*> GetComponentsInParent()
	{
		std::vector<T*> parentComponents;
		for (int j = 0; j < parent->children.size(); ++j) { parentComponents.push_back(parent->children[j]->GetComponent<T>()); }
		return parentComponents;
	}

};

#endif // !GAMEOBJECT_H