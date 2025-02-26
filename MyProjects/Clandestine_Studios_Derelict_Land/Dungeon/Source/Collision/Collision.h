/******************************************************************************/
/*!
\file    Collision.h
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function declarations to implement physics movement that will be used
		 to update the positions of the game object instances and collision
		 detection. The functions include:
		 - max
		 Finds maximum value.
		 - min
		 Finds minimum value.
		 - Collision
		 Default constructor.
		 - SetVelocity
		 Sets class velocity.
		 -SetBoundingBox
		 Sets bounding box.
		 - CollisionIntersection_RectRect
		 Detects collision by AABB.
		  - operator=
		 Assignment operator overload.
		 - CopyData
		 Copies data of component.
		  - Serialize
		 Serializes collision component.
		 - Deserialize
		 Deserializes collision component.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once
#include "Essential.h"
#include "GameObject/Component.h"
#include "Transform/Transform.h"
#include "Physics/Physics.h"

class Physics;

struct AABB {
	Vec2 min, max;
};

/*!*************************************************************************
****
\brief
	Finds larger of 2 values.
\param[in] a
	First value.
\param[in] b
	Second value.
\return
	Larger value.
****************************************************************************
***/
float Max(float a, float b);

/*!*************************************************************************
****
\brief
	Finds smaller of 2 values.
\param[in] a
	First value.
\param[in] b
	Second value.
\return
	Smaller value.
****************************************************************************
***/
float Min(float a, float b);

class Collision : public Component {
public:
	AABB BoundingBox;
	Vec2 Vel;
	bool go_is_colliding, showBox;
	/*!*************************************************************************
	****
	\brief
		Default constructor.
	****************************************************************************
	***/
	Collision();
	/*!*************************************************************************
	****
	\brief
		Sets class velocity.
	\param[in] SetVel
		First velocity for Vel.
	****************************************************************************
	***/
	void SetVelocity(Vec2 SetVel);

	/*!*************************************************************************
	****
	\brief
		Sets bounding box parameters.
	\param[in] BOUNDING_RECT_SIZE
		Size of bounding box.
	\param[in] transformPointer
		Pointer to transform component for game object.
	****************************************************************************
	***/
	void SetBoundingBox(Vec2 PositionInst, Vec2 ScaleInst, Vec2 DimensionInst);

	/*!*************************************************************************
	****
	\brief
		Assignment operator overload.
	\param[in] rhs
		Collision component.
	\return
		Reference to Collision component.
	****************************************************************************
	***/
	Collision& operator=(Collision& rhs);

	/*!*************************************************************************
	****
	\brief
		Copies data of component.
	\param[in] Target
		Component to be copied.
	****************************************************************************
	***/
	virtual void CopyData(Component* Target) override;

	/*!*************************************************************************
	****
	\brief
		Serializes Collision component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
	virtual void Serialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Deserializes Collision component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
	virtual void Deserialize(FILE* fp) override;
};

/*!*************************************************************************
****
\brief
	Detects collision by AABB.
\param[in] aabb1
	Bounding box for game object 1.
\param[in] vel1
	Velocity of game object 1.
\param[in] aabb2
	Bounding box for game object 2.
\param[in] vel2
	Velocity of game object 2.
\param[in] dt
	delta time
\return
	True if collision detected; false if none detected.
****************************************************************************
***/
bool CollisionIntersection_RectRect(const AABB& aabb1, Vec2& vel1, const AABB& aabb2, Vec2& vel2, float dt); // non-member function