/******************************************************************************/
/*!
\file    Collision.cpp
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function definitions to implement physics movement that will be used
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

#include "Collision.h"

/*!*************************************************************************
****
\brief
	Finds maximum value.
\param[in] a
	First float.
\param[in] b
	Second float.
\return
	Bigger float.
****************************************************************************
***/
float Max(float a, float b) {
	float max = a;
	max >= b ? max = a : max = b;
	return max;
}

/*!*************************************************************************
****
\brief
	Finds minumum value.
\param[in] a
	First float
\param[in] b
	Second float
\return
	Smaller float
****************************************************************************
***/
float Min(float a, float b) {
	float min = a;
	min <= b ? min = a : min = b;
	return min;
}

/*!*************************************************************************
****
\brief
	Default constructor.
****************************************************************************
***/
Collision::Collision() :go_is_colliding(false) {
	SetType(ComponentType::Collision); showBox = false;
}

/*!*************************************************************************
****
\brief
	Sets class velocity.
\param[in] SetVel
	Velocity to set for Vel. ( from Rigid Body; if no RB, = 0 )
****************************************************************************
***/
void Collision::SetVelocity(Vec2 SetVel) {
	Vel = SetVel;
}

/*!*************************************************************************
****
\brief
	Sets bounding box parameters.
\param[in] BOUNDING_RECT_SIZE
	Size of bounding box ( to be obtained from class BoundingRectSize member )
\param[in] PositionInst
	Position of game object ( from Transform )
\param[in] ScaleInst
	Scale of game object ( from Transform )
****************************************************************************
***/
void Collision::SetBoundingBox(Vec2 PositionInst, Vec2 ScaleInst, Vec2 DimensionInst) {
	BoundingBox.min.x = -0.5f * DimensionInst.x * (ScaleInst.x / 100.f) + PositionInst.x;
	BoundingBox.min.y = -0.5f * DimensionInst.y * (ScaleInst.y / 100.f) + PositionInst.y;
	BoundingBox.max.x = 0.5f * DimensionInst.x * (ScaleInst.x / 100.f) + PositionInst.x;
	BoundingBox.max.y = 0.5f * DimensionInst.y * (ScaleInst.y / 100.f) + PositionInst.y;
}

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
Collision& Collision::operator=(Collision& rhs) {
	this->BoundingBox = rhs.BoundingBox;
	this->Vel = rhs.Vel;
	return *this;
}

/*!*************************************************************************
****
\brief
	Copies data of component.
\param[in] Target
	Component to be copied.
****************************************************************************
***/
void Collision::CopyData(Component* Target) {
	Collision* t = dynamic_cast<Collision*>(Target);
	if (t != nullptr) {
		this->BoundingBox = t->BoundingBox;
		this->Vel = t->Vel;
	}
}

/*!*************************************************************************
****
\brief
	Serializes Collision component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void Collision::Serialize(FILE* fp)
{
	fprintf(fp, "Collision\n");
}

/*!*************************************************************************
****
\brief
	Deserializes Collision component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void Collision::Deserialize(FILE* fp)
{
	(void)fp;
	//Do nothing
}

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
	delta time.
\return
	True if collision detected; false if none detected.
****************************************************************************
***/
bool CollisionIntersection_RectRect(const AABB& aabb1, Vec2& vel1, const AABB& aabb2, Vec2& vel2, float dt) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.min.y <= aabb2.max.y) &&
		(aabb1.max.x >= aabb2.min.x && aabb1.max.y >= aabb2.min.y))
	{
		return 1;
	}

	Vec2 Vb = Vec2{ vel2.y - vel1.y,vel2.x - vel1.x }; // vector Vb 

	float tFirst = 0; // Initialize and calculate the new velocity of Vb 
	float tLast = dt;

	if (Vb.x < 0) // for x axis  
	{
		if (aabb1.min.x > aabb2.max.x) // case 1 
		{
			return 0;
		}

		if (aabb1.max.x < aabb2.min.x) // case 4 (max1 with min2) 
		{
			tFirst = Max((aabb1.max.x - aabb2.min.x) / Vb.x, tFirst);
		}

		if (aabb1.min.x < aabb2.max.x) // case 4 (2/2) 
		{
			tLast = Min((aabb1.min.x - aabb2.max.x) / Vb.x, tLast);
		}
	}

	if (Vb.x > 0) // for x axis  
	{
		if (aabb1.min.x > aabb2.max.x) // case 2 (1/2) 
		{
			tFirst = Max((aabb1.min.x - aabb2.max.x) / Vb.x, tFirst);
		}

		if (aabb1.max.x > aabb2.min.x) // case 2 (2/2) 
		{
			tLast = Min((aabb1.max.x - aabb2.min.x) / Vb.x, tLast);
		}

		if (aabb1.max.x < aabb2.min.x) // case 3 
		{
			return 0;
		}
	}

	if (Vb.y < 0) // for y axis  
	{
		if (aabb1.min.y > aabb2.max.y) // case 1 
		{
			return 0;
		}

		if (aabb1.max.y < aabb2.min.y) // case 4 (1/2) 
		{
			tFirst = Max((aabb1.max.y - aabb2.min.y) / Vb.y, tFirst);
		}

		if (aabb1.min.y < aabb2.max.y) // case 4 (2/2) 
		{
			tLast = Min((aabb1.min.y - aabb2.max.y) / Vb.y, tLast);
		}
	}

	if (Vb.y > 0) // for y axis  
	{
		if (aabb1.min.y > aabb2.max.y) // case 2 (1/2) 
		{
			tFirst = Max((aabb1.min.y - aabb2.max.y) / Vb.y, tFirst);
		}

		if (aabb1.max.y > aabb2.min.y) // case 2 (2/2) 
		{
			tLast = Min((aabb1.max.y - aabb2.min.y) / Vb.y, tLast);
		}

		if (aabb1.max.y < aabb2.min.y) // case 3 
		{
			return 0;
		}
	}

	if (tFirst > tLast) // case 5 
	{
		return 0;
	}

	return 0;
}
