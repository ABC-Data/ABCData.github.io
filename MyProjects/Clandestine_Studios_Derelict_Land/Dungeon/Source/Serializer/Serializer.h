/******************************************************************************/
/*!
\file		Serializer.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 1, 2022
\brief		Serialization system

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include <cstdio>
#include <string>
#include <stdexcept>
#include <sstream>
#include "Essential.h"
#include "System/System.h"
#include "Level/Level.h"
#include "GameObject/GameObject.h"
#include "Transform/Transform.h"
#include "RigidBody/RigidBody.h"
#include "Collision/Collision.h"
#include "Graphics/Graphics.h"
#include "Audio/Audio.h"
#include "Logic/Logic.h"
#include "Factory/Factory.h"



class FileSerializer
{
	public:
		/**************************************************************************/
		/*!
		  \brief
			Serializes the current Level to a file (.scn)
		  \param CurrentLevel
			The level to serialize
		*/
		/**************************************************************************/
		static void SeralizeLevelToSceneFile(Level* CurrentLevel);
};