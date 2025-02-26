/******************************************************************************/
/*!
\file    Transform.cpp
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function definitions to implement transform physics movement that
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

#include "Transform.h"
#include "Graphics/Graphics.h"
extern bool fullscreen;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
/*!*************************************************************************
****
\brief
	Default constructor.
****************************************************************************
***/
Transform::Transform() :Position(0.0f, 0.0f), RotationAngle(0.0f), Scale(100.0f, 100.0f), RotationSpeed(0.0f), Dimensions(100.0f, 100.0f), Depth(0.f) {
	SetType(ComponentType::Transform);
}

void Transform::ShakeObject(float magnitude) {
	float x = (-1.f + (rand() / (RAND_MAX / (1.f - (-1.f))))) * magnitude;
	Position.x += x;
}

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
void Transform::SetTransform(Vec2 PositionVector, float RotationAngleValue, float RotationSpeedValue, Vec2 ScaleValue,
	Vec2 DimensionValue, float DepthValue) {
	Scale = ScaleValue;
	Dimensions = DimensionValue;
	Position = PositionVector;
	RotationAngle = RotationAngleValue;
	RotationSpeed = RotationSpeedValue;
	Depth = DepthValue;
}

/*!*************************************************************************
****
\brief
	Copies data of component.
\param[in] Target
	Target component to copy
****************************************************************************
***/
void Transform::CopyData(Component* Target) {
	Transform* t = dynamic_cast<Transform*>(Target);
	if (t != nullptr) {
		this->Scale = t->Scale;
		this->Position = t->Position;
		this->RotationAngle = t->RotationAngle;
		this->RotationSpeed = t->RotationSpeed;
		this->Dimensions = t->Dimensions;
		this->Depth = t->Depth;
	}
}

/*!*************************************************************************
****
\brief
	Sets only position.
\param[in] SetPosition
	Position to set to
****************************************************************************
***/
void Transform::SetPosition(Vec2 SetPosition)
{
	Position = SetPosition;
}

/*!*************************************************************************
****
\brief
	Serializes Transform component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void Transform::Serialize(FILE* fp) {
	//std::cout << "[Transform::Serialize] called " << std::endl;

	Vec2 towrite_position = Position;
	Vec2 towrite_dimension = Dimensions;

	if (this->GetComponentOwner()->layer == "UI")
	{
		towrite_position = { Position.x * ((float)WINDOW_WIDTH / (float)GLSetup::current_width) , Position.y * ((float)WINDOW_HEIGHT / (float)GLSetup::current_height) };
		towrite_dimension = { Dimensions.x * ((float)WINDOW_WIDTH / (float)GLSetup::current_width) , Dimensions.y * ((float)WINDOW_HEIGHT / (float)GLSetup::current_height) };
	}

	fprintf(fp, "Transform\n");

	fprintf(fp, "Position: %.2f, %.2f, %f\n", towrite_position.x, towrite_position.y, Depth);
	fprintf(fp, "Rotation Angle: %f\n", RotationAngle);
	fprintf(fp, "Rotation Speed: %f\n", RotationSpeed);
	fprintf(fp, "Scale: %f, %f\n", Scale.x, Scale.y);
	fprintf(fp, "Dimensions: %.2f, %.2f\n", towrite_dimension.x, towrite_dimension.y);
}

/*!*************************************************************************
****
\brief
	Serializes Collision component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void Transform::Deserialize(FILE* fp) {
	float x, y, z, ra, rs, sx, sy, dx, dy;
	int success = 1;
	success *= fscanf(fp, "Position: %f, %f, %f\n", &x, &y, &z);
	success *= fscanf(fp, "Rotation Angle: %f\n", &ra);
	success *= fscanf(fp, "Rotation Speed: %f\n", &rs);
	success *= fscanf(fp, "Scale: %f, %f\n", &sx, &sy);
	success *= fscanf(fp, "Dimensions: %f, %f\n", &dx, &dy);
	if (!success) {
		std::cout << "Serialize Transform Failed!";
		SetTransform({ 0,0 }, 0, 0, { 0, 0 }, { 0,0 }, 0);
		return;
	}
	SetTransform({ x,y }, ra, rs, { sx,sy }, { dx, dy }, z);
}