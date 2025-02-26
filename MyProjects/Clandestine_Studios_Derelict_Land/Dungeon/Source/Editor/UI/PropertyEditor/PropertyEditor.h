/*!*************************************************************************
\file	PropertyEditor.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date	27-11-22
\brief  This header file contains the necessary functionalities to operate
		the PropertyEditor system. Views and edits the properties of the
		selected object, as well as view any text files.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#pragma once
#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H

#include "System/System.h"
#include "Transform/Transform.h"
#include "Physics/Physics.h"
#include "Logic/Logic.h"
#include "Logic/Script.h"
#include "GameUI/GameUI.h"
#include "Renderer/Renderer.h"
#include "Animation/Animate.h"

// Script - based Components
#include "Scripts/InventoryComponent.h"
#include "Scripts/CombatComponent.h"
#include "Scripts/EnemyComponent.h"

class Inventory;
class PlayerCombat;
class EnemyCombat;

enum class SelectedType : unsigned int
{
	None,
	Texture,
	Audio,
	Text,
	Font,
	GameObject,
	AssetTypeMax
};

struct PNGHeader
{
	char signature[8];
	// cast to int before using the below two
	char bitDepth, interlace;
	short colorType, compression, filter;
	int width, height; size_t fileSize;

	friend std::ostream& operator<<(std::ostream& os, const PNGHeader& ph);
};

class PropertyEditor : public System
{
private:
	// Ctor + dtor
	PropertyEditor(bool nOpen = true, Vec2 nPos = { 100.0f, 100.0f }, 
		float nWidth = 400.0f, float nHeight = 600.0f); // Constructor
	~PropertyEditor();

	PropertyEditor(const PropertyEditor&) = delete;
	PropertyEditor& operator=(const PropertyEditor&) = delete;
	PropertyEditor(PropertyEditor&&) = delete;
	PropertyEditor& operator=(PropertyEditor&&) = delete;
public:
	// Init, update and shutdown the PropertyEditor system
	virtual void Init();
	virtual void Shutdown();
	virtual void Update(float dt);
	
	/**************************************************************************/
	/*!
	* \brief
	* 	Allocates memory for one property editor.
	* \return
	* 	Returns a pointer to the property editor.
	*/
	/**************************************************************************/
	static PropertyEditor* GetInstance();

	bool GetOpen() const; void SetOpen(const bool nOpen);

	/**************************************************************************/
	/*! \brief	Draws the PropertyEditor and handles its functionalities.
	/**************************************************************************/
	void DrawPropertyEditor();
	
	#pragma region Game Object Selection / Deselection
	
	/**************************************************************************/
	/*!
	* \brief
	*	Gets the selected game object.
	* \return
	* 	Returns a pointer to the selected game object.
	*/
	/**************************************************************************/
	GameObject* GetSelectedGameObject();
	
	/**************************************************************************/
	/*!
	* \brief
	*	Sets the selected game object for the PropertyEditor to show its properties.
	* \param goId
	* 	The ID of the game object to be selected.
	*/
	/**************************************************************************/
	void SetSelectedEntity(const int goId);
	
	/**************************************************************************/
	/*! \brief Deselect the selected game object - blanks the property editor blank.
	/**************************************************************************/
	void DeselectEntity();

	#pragma endregion

	#pragma region Component Displaying
	
	// Display a game object's properties in the property editor.
	void DisplayGameObjectProperties(GameObject& go);
	
	// Display a game object's components in the property editor.
	void DisplayGameObjectComponents(GameObject& go);

	// Display a game object's transform in the property editor.
	void DisplayGameObjectTransform(Transform& transform);

	// Display a game object's RB in the property editor.
	void DisplayGameObjectPhysics(RigidBody& pComp);

	// Display a game object's collider in the property editor.
	void DisplayGameObjectCollider(Collision& cComp);

	// Display a game object's gameUI in the property editor.
	void DisplayGameObjectGameUI(GameUI& uiComp);

	// Display a game object's gameUI in the property editor.
	void DisplayGameObjectRenderer(Renderer& RendererComp);

	void DisplayInventoryComponent(Inventory& inventory);

	void DisplayCombatComponent(PlayerCombat& combat);

	void DisplayEnemyCombatComponent(EnemyCombat& combat);

	void DisplayGameObjectAnimate(Animate& AnimateComp);

	void DisplayGameObjectController(Controller& control);
	void DisplayGameObjectLighting(Lighting& LightingComp);

	void DisplayGameObjectLogic(LogicComponent& logic);
	
	// Display a game object's script in the property editor.
	void DisplayGameObjectScript(Script& script);

#pragma endregion
	
	#pragma region Asset Displaying
	
	// Get string buffer for script / text display
	std::stringstream& GetTextBuffer();

	// Display the text contents of a script in the property editor.
	// Called if user click on a .cs, .cpp, or .h file.
	void DisplayTextFileContents(const std::string_view textFilePath);

	void DisplaySoundData(FMOD::Sound* const);
	
	void DisplayAudioFile(const std::string_view audioFilePath);

	// Display the image of a PNG texture in the property editor.
	void DisplayTextureData(const PNGHeader* texture);
	
	// Draw an image of a sprite in the property editor.
	void DisplayTextureFile(const std::string_view spriteFilePath);

	#pragma endregion

private:
	bool open;
	float size[2];
	Vec2 pos;
	GameObject* selectedGameObject;
	
	SelectedType assetType = SelectedType::None;
	// Used to store the text contents of a script file. (e.g. .cpp, .cs, .h)
	std::string textFileName; std::stringstream scriptTextBuffer;
	
	// To read audio file headers, and display the audio file's properties.
	//char* audioFileBuffer;
};

static PropertyEditor* sPropEditor;

#endif // !PROPERTYEDITOR_H