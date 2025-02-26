/******************************************************************************/
/*!
\file    Physics.h
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function declarations to implement physics movement that will be
		 used to update the positions of the game object instances.
		 The functions include:
		 - Physics
		 Default constructor.
		 - Init
		 Initialize the Physics system
		 - Shutdown
		 Shut down the Physics system
		 - Update
		 Updates all game objects

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once
#include "Essential.h"
#include "System/System.h"
#include "GameObject/GameObject.h"
#include "Transform/Transform.h"
#include "RigidBody/RigidBody.h"
#include "Collision/Collision.h"

#include "Input/MouseCodes.h"

class Collision;

class Physics :public System
{
public:
	/*!*************************************************************************
	****
	\brief
		Default constructor.
	****************************************************************************
	***/
	Physics();
	/*!*************************************************************************
	****
	\brief
		Initialize the Physics system
	****************************************************************************
	***/
	virtual void Init();
	/*!*************************************************************************
	****
	\brief
		Shut down the Physics system
	****************************************************************************
	***/
	virtual void Shutdown();
	/*!*************************************************************************
	****
	\brief
		Updates all game objects
	\param[in] dt
		delta time
	****************************************************************************
	***/
	virtual void Update(float dt);

	constexpr static float GRAVITY = 9.81f;
};
