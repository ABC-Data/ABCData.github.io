/******************************************************************************/
/*!
\file    Transform.h
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function declarations to implement transform physics movement that
		 will be used to update the positions of the game object instances.
		  The functions include:
		 - Transform
		 Default constructor.
		 - SetTransform
		 Set transform parameters.
		 - CopyData
		 Copies data for transform.
		 - SetPosition
		 Sets only position.
		 - Serialize
		 Serializes transform component.
		 - Deserialize
		 Deserializes transform component.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once
#include "Essential.h"
#include "GameObject/Component.h"


class Transform :public Component {
public:
	Vec2 Position, Scale, Dimensions;
	float RotationAngle, RotationSpeed, Depth;
	/*!*************************************************************************
	****
	\brief
	Default constructor.
	****************************************************************************
	***/
	Transform();

	void ShakeObject(float magnitude);

	/*!*************************************************************************
	****
	\brief
		Set transform parameters.
	\param[in] PositionVector
		Position to set to
	\param[in] RotationAngleValue
		Rotation angle to set to
	\param[in] RotationSpeedValue
		Rotation speed to set to
	\param[in] ScaleValue
		Scale to set to
	\param[in] DimensionValue
		Dimensions to set to
	\param[in] DepthValue
		Depth to set to
	****************************************************************************
	***/
	void SetTransform(Vec2 PositionVector, float RotationAngleValue, float RotationSpeedValue, Vec2 ScaleValue, Vec2 DimensionValue, float DepthValue);
	/*!*************************************************************************
	****
	\brief
	Copies data for transform
	\param[in] Target
	Target component to copy
	****************************************************************************
	***/
	virtual void CopyData(Component* Target) override;

	/*!*************************************************************************
	****
	\brief
	Sets only position.
	\param[in] SetPosition
	Position to set to
	****************************************************************************
	***/
	void SetPosition(Vec2 SetPosition);

	/*!*************************************************************************
	****
	\brief
		Serializes Transform component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
	virtual void Serialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Serializes Collision component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
	virtual void Deserialize(FILE* fp) override;
};