/******************************************************************************/
/*!
\file    RigidBody.h
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function declarations to implement rigid body physics movement that
		 will be used to update the positions of the game object instances.
		 The functions include:
		  - RigidBody
		 Default constructor.
		 - Move
		 Moves game object.
		 - Accelerate
		 Accelerates game object.
		 - operator=
		 Assignment operator overload.
		 - CopyData
		 Copies data for rigid body.
		 - SetRigidBody
		 Sets property data for rigid body.
		 - BuildDistanceVector
		 Builds distance vector used for calculating distance between current
		 position and mouse position.
		 - Serialize
		 Serializes Rigid Body component.
		 - Deserialize
		 Deserializes Rigid Body component.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once
#include "Essential.h"
#include "GameObject/Component.h"
#include "Transform/Transform.h"

class RigidBody :public Component {
public:
	bool Solid;
	float InverseMass, Force;
	Vec2 Velocity, Acceleration;
	/*!*************************************************************************
	****
	\brief
		Default constructor.
	****************************************************************************
	***/
	RigidBody();
	/*!*************************************************************************
	****
	\brief
		Moves game object.
	\param[in] TransformPtr
		Transform pointer that provides position
	\param[in] dt
		delta time
	****************************************************************************
	***/
	void Move(Vec2& Position, const float fixedDeltaTime);

	/*!*************************************************************************
	****
	\brief
		Moves game object to specified location
	\param[in] source_pos
		Object's transform's position
	\param[in] dest_pos
		Target location
	\param[in] fixeddt
		delta time
	****************************************************************************
	***/
	void MoveTo(Vec2& source_pos, Vec2 dest_pos, const float fixeddt);
	/*!*************************************************************************
	****
	\brief
		Accelerates game object.
	\param[in] TransformPtr
		Transform pointer that provides position
	\param[in] dt
		delta time
	****************************************************************************
	***/
	void Accelerate(Vec2 UnitDirectionVector, const float fixedDeltaTime);

	/*!*************************************************************************
	****
	\brief
		Assignment operator overload
	\param[in] rhs
		RigidBody component to assign to
	****************************************************************************
	***/
	RigidBody& operator=(RigidBody& rhs);

	/*!*************************************************************************
	****
	\brief
		Copies data for rigid body
	\param[in] Target
		Target component to copy
	****************************************************************************
	***/
	virtual void CopyData(Component* Target) override;

	/*!*************************************************************************
	****
	\brief
		Sets property data for rigid body.
	\param[in] SetSolid
		Solid state to set to.
	\param[in] SetVelocity
		Velocity to set to.
	\param[in] SetSolid
		Acceleration to set to.
	****************************************************************************
	***/
	void SetRigidBody(bool SetSolid, float SetForce, float SetInverseMass);

	/*!*************************************************************************
	****
	\brief
		Sets property data for velocity only.
	\param[in] SetVel
		Velocity to set to.
	****************************************************************************
	***/
	void SetVelocity(Vec2 SetVel);

	/*!*************************************************************************
	****
	\brief
		Builds distance vector to be used for calculating distance between
		current position and mouse position.
	\param[in] Source
		Current position of game object.
	\param[in] Destination
		Mouse position.
	\return
		Difference of the two vectors.
	****************************************************************************
	***/
	Vec2 BuildDistanceVector(Vec2 Source, Vec2 Destination);

	/*!*************************************************************************
	****
	\brief
		Serializes Rigid Body component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
	void Serialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Deserializes Rigid Body component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
	void Deserialize(FILE* fp) override;
};