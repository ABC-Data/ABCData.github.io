/******************************************************************************/
/*!
\file    Deserializer.h
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function declarations to implement data driven features of game object
		 properties or components. The functions include:
		 - FilePtr
		 Copy Constructor.
		 - FilePtr
		 Delegating Constructor.
		 - ~FilePtr
		 Default destructor.
		 - ReadInt
		 Read ints from file.
		- SerializeTransform
		Serializes transform properties from file.
		- SerializeRigidBody
		Serializes RigidBody properties from file.
		- SerializeWindowParams
		Serializes window properties from file.
		- ReadString
		Read strings from file.
		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once
#include <cstdio>
#include <string>
#include <stdexcept>
#include "Factory/Factory.h"
#include "Transform/Transform.h"
#include "RigidBody/RigidBody.h"
#include "Collision/Collision.h"
#include "Graphics/Graphics.h"
#include "Audio/Audio.h"
#include "Level/Level.h"
#include "Logic/Logic.h"
#include "GameUI/GameUI.h"
#include "Renderer/Renderer.h"

class Graphics;

class FilePtr {
public:
	FILE* p;

	/*!*************************************************************************
	****
	\brief
		Copy Constructor.
	\param[in] n
		Filename.
	\param[in] a
		mode of opening file.
	****************************************************************************
	***/
	FilePtr(char const* n, char const* a);

	/*!*************************************************************************
	****
	\brief
		Delegating Constructor
	\param[in] n
		Filename.
	\param[in] a
		Mode of opening file.
	****************************************************************************
	***/
	FilePtr(std::string const& n, char const* a); // delegating ctor (calls 1st constructor)

	/*!*************************************************************************
	****
	\brief
		Default destructor.
	****************************************************************************
	***/
	~FilePtr(); // dtor

	/*!*************************************************************************
	****
	\brief
		Read ints from file
	\param[in] p
		File to open
	****************************************************************************
	***/
	static int* ReadInt(FILE* ptr);

	/*!*************************************************************************
	****
	\brief
		Serializes transform properties from file.
	\param[in] ptr
		File to open.
	\param[in] tInst
		Pointer to transform component to serialize.
	****************************************************************************
	***/
	static void SerializeTransform(FILE* ptr, Transform* tInst);

	/*!*************************************************************************
	****
	\brief
		Serializes rigidbody properties from file.
	\param[in] ptr
		File to open.
	\param[in] rbInst
		Pointer to rigidbody component to serialize.
	****************************************************************************
	***/
	static void SerializeRigidBody(FILE* ptr, RigidBody* rbInst);

	/*!*************************************************************************
	****
	\brief
		Serializes window properties from file.
	\param[in] Filename
		File to open.
	\param[in] gInst
		Pointer to graphics component to serialize.
	****************************************************************************
	***/
	bool SerializeWindowParams(std::string Filename, Graphics* gInst);

	/*!*************************************************************************
	****
	\brief
		Serializes an entire level
	\param[in] Filename
		File to open.
	\return
		Properly built level object
	****************************************************************************
	***/
	static Level* SerializeLevel(std::string Filename);

	/*!*************************************************************************
	****
	\brief
		Saves a level to file
	\param[in] level
		Level to save
	****************************************************************************
	***/
	static void SaveLevel(Level* level);

	/*!*************************************************************************
	****
	\brief
		Reads tags from file
	\param[in] Filename
		Filepath
	\return
		True if serialized, false if could not
	****************************************************************************
	***/
	static bool SerializeTags(std::string Filename);

	/*!*************************************************************************
	****
	\brief
		Saves tags to file
	\param[in] Filename
		Filepath
	\return
		True if deserialized, false if could not
	****************************************************************************
	***/
	static bool DeserializeTags(std::string Filename);

	/*!*************************************************************************
	****
	\brief
		Reads layers from file
	\param[in] Filename
		Filepath
	\return
		True if serialized, false if could not
	****************************************************************************
	***/
	static bool SerializeLayers(std::string Filename);

	/*!*************************************************************************
	****
	\brief
		Writes layers to file
	\param[in] Filename
		Filepath
	\return
		True if deserialized, false if could not
	****************************************************************************
	***/
	static bool DeserializerLayers(std::string Filename);

	/*!*************************************************************************
	****
	\brief
		Reads Logic component data
	\param[in] Filename
		Filepath
	\param[in] lInst
		Logic Component to store data to
	****************************************************************************
	***/
	static void SerializeLogicComponent(FILE* ptr, LogicComponent* lInst);

	/*!*************************************************************************
	****
	\brief
		Reads GameUI component data
	\param[in] Filename
		Filepath
	\param[in] G_UI_Inst
		GameUI component to store data to
	****************************************************************************
	***/
	static void SerializeGameUI(FILE* ptr, GameUI* G_UI_Inst);

	/*!*************************************************************************
	****
	\brief
		Reads Logic Target component data
	\param[in] Filename
		Filepath
	\param[in] LT
		Logic Target Component to store data to
	****************************************************************************
	***/
	static void SerializeLogicTargets(FILE* ptr, LogicTargets* LT);

	/*!*************************************************************************
	****
	\brief
		Reads Renderer	component data
	\param[in] Filename
		Filepath
	\param[in] LT
		Logic Target Component to store data to
	****************************************************************************
	***/
	static void SerializeRenderer(FILE* ptr, Renderer* Renderer_Inst);

	/*!*************************************************************************
	****
	\brief
		Reads Textures	component data
	\param[in] Filename
		Filepath
	\param[in] LT
		Logic Target Component to store data to
	****************************************************************************
	***/
	static void SerializeTextures(FILE* ptr, Assets& assets);

	static void SerializeAudio(FILE* ptr, Assets& assets);

	/**************************************************************************/
	/*!
	  \brief
		Deserializes textures read in Texture.txt.

	  \param Filename
		File path of Texture.txt.

	  \param AssetsManager
		AssetsManager to keep textures.
	*/
	/**************************************************************************/
	static void DeserializeTextures(std::string Filename, Assets& AssetsManager);
	/**************************************************************************/
	/*!
	  \brief
		Deserializes shaders read in Shaders.txt.

	  \param Filename
		File path of Shaders.txt.

	  \param AssetsManager
		AssetsManager to keep shaders.
	*/
	/**************************************************************************/
	static void DeserializeShaders(std::string Filename, Assets& AssetsManager);
	/**************************************************************************/
	/*!
	  \brief
		Deserializes sprites read in Sprites.txt.

	  \param Filename
		File path of Sprites.txt.

	  \param AssetsManager
		AssetsManager to keep sprites.
	*/
	/**************************************************************************/
	static void DeserializeSprites(std::string Filename, Assets& AssetsManager);

	static void DeserializeSpriteSheet(std::string Filename, Assets& AssetsManager);
	/**************************************************************************/
	/*!
	  \brief
		Deserializes fonts read in Fonts.txt.

	  \param Filename
		File path of Fonts.txt.

	  \param AssetsManager
		AssetsManager to keep fonts.
	*/
	/**************************************************************************/
	static void DeserializeFonts(std::string Filename, Assets& AssetsManager);
};
/*!*************************************************************************
****
\brief
	Read strings from file.
\param[in] p
	File
****************************************************************************
***/
std::string* ReadString(std::string Filename);