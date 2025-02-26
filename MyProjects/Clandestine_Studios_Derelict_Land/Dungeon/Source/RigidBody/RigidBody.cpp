/******************************************************************************/
/*!
\file    RigidBody.cpp
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

#include "RigidBody.h"

/*!*************************************************************************
****
\brief
	Default constructor.
****************************************************************************
***/
RigidBody::RigidBody() :Velocity(0.0f, 0.0f), Solid(false), Acceleration(0.0f, 0.0f), Force(0.0f), InverseMass(0.0f) {
	SetType(ComponentType::RigidBody);
}

/*!*************************************************************************
****
\brief
	Moves game object.
\param[in] Position
	Position of game object.
\param[in] fixedDeltaTime
	User-defined fixed delta time. (1/60)
****************************************************************************
***/
void RigidBody::Move(Vec2& Position, const float fixedDeltaTime) {
	Position.x += Velocity.x * fixedDeltaTime;
	Position.y += Velocity.y * fixedDeltaTime;
}



void RigidBody::MoveTo(Vec2& source_pos, Vec2 dest_pos, const float fixeddt)
{
	Vec2 NDV;
	Vec2 DV = BuildDistanceVector(source_pos, dest_pos);
	Vec2Normalize(NDV, DV);
	Accelerate(NDV, fixeddt);
	source_pos.x += Velocity.x * fixeddt;
	source_pos.y += Velocity.y * fixeddt;
}

/*!*************************************************************************
****
\brief
	Accelerates game object.
\param[in] UnitDirectionVector
	Normalized distance vector.
\param[in] fixedDeltaTime
	User-defined fixed delta time. (1/60)
****************************************************************************
***/
void RigidBody::Accelerate(Vec2 UnitDirectionVector, const float fixedDeltaTime) {
	Velocity = { 0.0f,0.0f };
	Vec2 ForceVector = UnitDirectionVector * Force;
	Acceleration = ForceVector * InverseMass;
	Velocity = Acceleration * fixedDeltaTime + Velocity;
}

/*!*************************************************************************
****
\brief
	Assignment operator overload
\param[in] rhs
	RigidBody component to assign to
****************************************************************************
***/
RigidBody& RigidBody::operator=(RigidBody& rhs) {
	this->Solid = rhs.Solid;
	this->Force = rhs.Force;
	this->Velocity = rhs.Velocity;
	this->InverseMass = rhs.InverseMass;
	this->Acceleration = rhs.Acceleration;
	return *this;
}

/*!*************************************************************************
****
\brief
	Copies data for rigid body
\param[in] Target
	Target component to copy
****************************************************************************
***/
void RigidBody::CopyData(Component* Target) {
	RigidBody* t = dynamic_cast<RigidBody*>(Target);
	if (t != nullptr) {
		this->Solid = t->Solid;
		this->Force = t->Force;
		this->Velocity = t->Velocity;
		this->InverseMass = t->InverseMass;
		this->Acceleration = t->Acceleration;
	}
}

/*!*************************************************************************
****
\brief
	Sets property data for rigid body.
\param[in] SetSolid
	Solid state to set to.
\param[in] SetForce
	Force to set to.
\param[in] SetInverseMass
	InverseMass to set to.
****************************************************************************
***/
void RigidBody::SetRigidBody(bool SetSolid, float SetForce, float SetInverseMass) {
	Solid = SetSolid;
	Force = SetForce;
	InverseMass = SetInverseMass;
}

/*!*************************************************************************
****
\brief
	Sets property data for velocity only.
\param[in] SetVel
	Velocity to set to.
****************************************************************************
***/
void RigidBody::SetVelocity(Vec2 SetVel)
{
	Velocity = SetVel;
}

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
Vec2 RigidBody::BuildDistanceVector(Vec2 Source, Vec2 Destination)
{
	return (Destination - Source);
}

/*!*************************************************************************
****
\brief
	Serializes Rigid Body component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void RigidBody::Serialize(FILE* fp)
{
	fprintf(fp, "RigidBody\n");
	fprintf(fp, "Solid: %d\n", Solid);
	fprintf(fp, "Inverse Mass: %f\n", InverseMass);
	fprintf(fp, "Force: %f\n", Force);
}

/*!*************************************************************************
****
\brief
	Deserializes Rigid Body component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void RigidBody::Deserialize(FILE* fp)
{
	int State;
	float F, IM;
	int success = 1;
	success *= fscanf(fp, "Solid: %i\n", &State);
	success *= fscanf(fp, "Inverse Mass: %f\n", &IM);
	success *= fscanf(fp, "Force: %f\n", &F);

	if (!success) {
		std::cout << "Serialize RigidBody Failed!";
		SetRigidBody(0, 0.f, 0.f);
		return;
	}
	SetRigidBody(State, F, IM);
}
