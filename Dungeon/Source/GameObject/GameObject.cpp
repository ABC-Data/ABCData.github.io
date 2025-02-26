/******************************************************************************/
/*!
\file		GameObject.cpp
\author 	Brandon Hsu, Anderson Phua
\par    	email: b.hsu\@digipen.edu , 620007421
\par    	email: a.phua\@digipen.edu , 620009021
\date   	Sep 14, 2022
\brief		Game Object implementation

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "GameObject.h"
#include "Transform/Transform.h"
#include "Collision/Collision.h"
#include "RigidBody/RigidBody.h"
#include "GameUI/GameUI.h"
#include "Editor/UI/Editor.h"
#include "CoreSys/Core.h"

std::vector<GameObject*> GameObject::GameObjectArray;
std::map<unsigned int, std::string> GameObject::Tags;

std::vector<std::string> GameObject::Layers;
std::bitset<32> GameObject::layerMask;

/******************************************************************************/
/*!
* \brief
* Initializes the GameObject's components, ID. By default, it is set to active,
* and its layer as "Default".
*/
/******************************************************************************/
void GameObject::Init(const int ObjId)
{
	for (Component* comp : Components) {
		comp->SetComponentOwner(this);
		comp->Init();
	}
	ObjectId = ObjId;

	if (tag.empty()) {tag = "Untagged";}
	if (layer.empty()) { layer = "Default"; }
}

/******************************************************************************/
/* \brief Destroys the GameObject's components, from the back to the front.
/******************************************************************************/
void GameObject::Destroy()
{
	if (this == nullptr) { return; }

	// if this gameobject has any child gameobjects, destroy them first
	if (HasChildren())
	{
		for (GameObject* child : GetChildren()) { child->Destroy(); }
		children.clear();
	}
	
	for (int i = 0; i < Components.size(); ++i) { delete Components[Components.size() - i - 1]; }
	factory->DecrementGOID();
}

#pragma region Components

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
Component* GameObject::GetComponent(const ComponentType type)
{
	for (Component* comp : Components) {
		if (type == comp->GetType()) return comp;
	}
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
void GameObject::AddComponent(const ComponentType type, Component* comp)
{
	for (int i = 0; i < Components.size(); ++i) {
		if (Components[i]->GetType() == type) {
			//Component already exists, do not add!
			return;
		}
	}
	Components.push_back(comp);
	//Level* level = Engine->GetLevel();
	//level->AddGOsPtrToSpecificLayer(this, type);
	//Level::PushGOsPtrToSpecificLayer(this, type);
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
void GameObject::RemoveComponent(const ComponentType type)
{
	for (int i = 0; i < Components.size(); ++i) {
		if (Components[i]->GetType() == type) {
			delete Components[i];
			Components.erase(Components.begin() + i);
			break;
		}
	}
	Engine->GetLevel()->RemoveGOsPtrsFromSpecificLayers(this, type);
}

/******************************************************************************/
/*!
* \brief
* Returns the list of components found in this GameObject.
* \return
* The list of components.
*/
/******************************************************************************/
std::vector<Component*>& GameObject::GetComponentList() { return Components; }

#pragma endregion

/******************************************************************************/
/*!
* \brief
* Returns the active state of the GameObject, or that of its parent if any.
* \return
* The active state of the GameObject, or its parent's.
*/
/******************************************************************************/
bool& GameObject::GetActive() { return parent ? parent->GetActive() : active; }

/******************************************************************************/
/*!
* \brief
* Sets the active state of the GameObject.
* \param isactive
* The new active state to set.
*/
/******************************************************************************/
void GameObject::SetActive(const bool isactive) { this->active = isactive; }

#pragma region Names

/******************************************************************************/
/*!
* \brief
* Returns the name of the GameObject.
* \return
* The name of the GameObject.
*/
/******************************************************************************/
const std::string& GameObject::GetName() { return name; }

/******************************************************************************/
/*!
* \brief
* Sets the name of the GameObject.
* \param name
* The new name to set.
*/
/******************************************************************************/
void GameObject::SetName(const std::string nName) { name = nName; }

/******************************************************************************/
/*!
* \brief
* Goes through the list of GameObjects and returns the one with the specified name.
* \param name
* The name of the GameObject to find.
* \return
* The GameObject with the specified name, if it exists. Otherwise, a nullptr is
* returned.
*/
/******************************************************************************/
GameObject* GameObject::Find(const char* name) 
{
	for (GameObject*& go : GameObjectArray)
	{
		if (strcmp(go->GetName().c_str(), name) == 0)
			return go;
	} 
	return nullptr; 
}

/******************************************************************************/
/*!
* \brief
* Goes through the list of GameObjects and returns the one with the specified name.
* \param name
* The name of the GameObject to find - case insensitive.
* \param gObjects
* The list of GameObjects to search through.
* \return
* The GameObject with the specified name, if it exists. Otherwise, a nullptr is
* returned.
*/
/******************************************************************************/
GameObject* GameObject::Find(const char* name, std::vector<GameObject*>& gObjects)
{
	for (GameObject*& go : gObjects)
	{
		if (strcmp(go->GetName().c_str(), name) == 0)
			return go;
	}
	return nullptr;
}

#pragma endregion

#pragma region Tags & Layers

/******************************************************************************/
/*!
* \brief
* Compares this GameObject's tag with the specified input tag. Case sensitive.
* \param t
* The tag to compare with.
* \return
* True if the tags are the same. Otherwise, false.
*/
/******************************************************************************/
bool GameObject::CompareTag(const std::string_view t) const { return t == tag; }

/******************************************************************************/
/*!
* \brief
* Adds a tag to the end of the current list of tags. Shown in the tag manager.
* \param nTag
* The tag to add.
*/
/******************************************************************************/
void GameObject::AddTag(const std::string nTag) { Tags[(unsigned int)Tags.size()] = nTag; }

/******************************************************************************/
/*!
* \brief
* Marks the tag for removal from the list of tags. Restart the application to 
* completely delete it.
* \param nTag
* The tag to remove.
*/
/******************************************************************************/
void GameObject::RemoveTag(const std::string_view nTag)
{
	/* Removing tags does not directly remove the tag from the game objects,
	but instead should mark them as "Deleted". Upon safely leaving the application,
	the tags marked as "Deleted" should not be saved back into the Tags text file.
	Thus, said tags are considered to be "deleted" in a sense. */

	// Check if any GameObjects are using this tag
	for (GameObject* obj : GameObjectArray) 
	{
		if (obj->CompareTag(nTag)) 
		{
			editor->Log("[GameObject::RemoveTag] Error: Cannot delete tag because "
				"it is in use by GameObject \"" + obj->GetName() + "\"", Logger::LogType::Error);
			return;
		}
	}

	// Remove tag since no gameObjects are using it
	for (auto it = Tags.begin(); it != Tags.end(); ++it) 
	{ 
		if (it->second == nTag)
		{
			// Rename this tag as (Removed) AKA this tag is marked for deletion
			// It should not be seen the next time this program is run
			it->second = "(Removed)"; break;
		}
	}
}

/******************************************************************************/
/*!
* \brief
* Adds a layer to an input index in the current list of layers. This is only
* done for initializing the list of layers. To add a layer to the end of the list,
* use UpdateLayer instead.
* \param nLayer
* The name of the new layer to add. Case sensitive.
* \param index
* The index to add the layer to. If the index is out of bounds, nothing happens.
*/
/******************************************************************************/
void GameObject::AddLayer(const std::string nLayer, const unsigned int index) { if (index < 0 || index > 31) return; Layers.emplace_back(nLayer); }

/******************************************************************************/
/*!
* \brief
* Updates the name of a layer at the specified index. If the index is out of
* bounds, nothing happens.
* \param nLayer
* The new name of the layer. If the name is empty, nothing happens.
* \param index
* The index of the layer to update. If the index is out of bounds, nothing happens.
*/
/******************************************************************************/
void GameObject::UpdateLayer(const std::string nLayer, const unsigned int index) { if (index < 0 || index > 31) return; GameObject::GetLayers()[index] = nLayer; }

/******************************************************************************/
/*!
* \brief
* Removes a layer from the list of layers, assuming that the layer is not in use.
* \param nLayer
* The name of the layer to remove. Case sensitive.
*/
/******************************************************************************/
void GameObject::RemoveLayer(const std::string nLayer)
{
	// Check if any GameObjects are using this layer
	for (GameObject* obj : GameObjectArray)
	{
		if (obj->layer == nLayer) { editor->Log("[GameObject::RemoveLayer] Error: Cannot remove layer! " + obj->name + " is using this layer!", Logger::LogType::Error); return; }
	}

	// Remove layer since no gameObjects are using it
	for (int i = 0; i < Layers.size(); ++i) { if (Layers[i] == nLayer) { Layers[i] = ""; break; } }
}

/******************************************************************************/
/*!
* \brief
* Retrieves the index of the specified layer. If the layer does not exist, -1 is
* returned.
* \param nLayer
* The name of the layer to find. Case sensitive.
* \return
* The index of the layer. If the layer does not exist, -1 is returned.
*/
/******************************************************************************/
int GameObject::GetLayerIndex(const std::string nLayer)
{
	for (int i = 0; i < GameObject::GetLayers().size(); ++i) 
	{ if (GameObject::GetLayers()[i] == nLayer) { return i; } }
	return -1;
}

/******************************************************************************/
/*!
* \brief
* Finds the first GameObject with the specified tag. If no GameObjects are found,
* a nullptr is returned. But if the tag does not exist or no GameObjects are using
* the tag, an error is logged into the logger.
* \param fTag
* The tag to find. Case sensitive.
* \param gObjects
* The game object array of the scene to search through.
* \return
* The first GameObject with the specified tag. If no GameObjects are found, a
* nullptr is returned and an exception is thrown.
/
/******************************************************************************/
GameObject* GameObject::FindWithTag(const std::string& fTag, std::vector<GameObject*>& gObjects)
{
	try {
		// If the input is empty / null, throw an error.
		if (fTag.empty()) throw std::invalid_argument("[GameObject::FindWithTag] Error: Input tag is empty.");
		
		for (GameObject*& go : gObjects) {if (go->CompareTag(fTag)) return go;}
		
		// If no GameObject with the specified tag is found, throw an exception.
		throw std::exception("[GameObject::FindWithTag] Error: No GameObject with the specified tag was found.");
	}
	catch (std::exception e)
	{
		editor->Log(e.what(), Logger::LogType::Error);
	}

	return nullptr;
}

/******************************************************************************/
/*!
* \brief
* Gets the first GameObject with the specified layer. If no GameObjects are found,
* a nullptr is returned. But if the layer does not exist or no GameObjects are
* using the layer, an error is logged into the logger.
* \param fLayer
* The layer to find. Case sensitive.
* \param gObjects
* The game object array of the scene to search through.
* \return
* The first GameObject with the specified layer. If no GameObjects are found, a
* nullptr is returned and an exception is thrown.
*/
/******************************************************************************/
GameObject* GameObject::FindWithLayer(const std::string& fLayer, std::vector<GameObject*>& gObjects)
{
	try {
		// If the input is empty / null, throw an error.
		if (fLayer.empty()) throw std::invalid_argument("[GameObject::FindWithLayer] Error: Input layer is empty.");

		for (GameObject*& go : gObjects) { if (go->layer == fLayer) return go; }

		// If no GameObject with the specified layer is found, throw an exception.
		throw std::exception("[GameObject::FindWithLayer] Error: No GameObject with the specified layer was found.");
	}
	catch (std::exception& e)
	{
		editor->Log(e.what(), Logger::LogType::Error);
	}

	return nullptr;
}

/******************************************************************************/
/*!
* \brief
* Retreives the ordered map of tags. The key is the index of the tag, and the
* value is the name of the tag.
* \return
* The ordered map of tags.
*/
/******************************************************************************/
std::map<unsigned int, std::string>& GameObject::GetTagMap() { return Tags; }

/******************************************************************************/
/*!
* \brief
* Retrives the vector of layers.
* \return
* The vector of layers.
*/
/******************************************************************************/
std::vector<std::string>& GameObject::GetLayers() { return Layers; }

/******************************************************************************/
/*!
* \brief
* Retrieves the 32-bit bitset layer mask.
* \return
* The 32-bit bitset layer mask.
*/
/******************************************************************************/
std::bitset<32>& GameObject::GetLayerMask() { return layerMask; }

/******************************************************************************/
/*!
* \brief
* Sets the 32-bit bitset layer mask.
* \param nLayerMask
* The new 32-bit bitset layer mask.
*/
/******************************************************************************/
void GameObject::SetLayerMask(std::bitset<32> nLayerMask) { layerMask = nLayerMask; }

/******************************************************************************/
/*!
* \brief
* Returns the vector of child GO's under this GO.
* \return
* The vector of child GO's under this GO.
*/
/******************************************************************************/
std::vector<GameObject*>& GameObject::GetChildren() { return children; }

#pragma endregion

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
bool GameObject::HasComponent(const ComponentType type) {
	for (Component* comp : Components) {
		if (comp->GetType() == type) return true;
	}
	return false;
}
