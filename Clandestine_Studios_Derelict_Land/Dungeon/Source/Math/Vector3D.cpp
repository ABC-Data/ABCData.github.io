/******************************************************************************/
/*!
\file		Vector3D.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 19, 2022
\brief		Implementation of the 3D vector library

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "Vector3D.h"
#include <math.h>

/**************************************************************************/
/*!
	\brief
	Default constructor for Vec3
*/
/**************************************************************************/
Vec3::Vec3() {
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

/**************************************************************************/
/*!
  \brief
	Constructor for Vec3

  \param _x
	X to set

  \param _y
	Y to set

  \param _z
	Z to set
*/
/**************************************************************************/
Vec3::Vec3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

/**************************************************************************/
/*!
  \brief
	Vec3 += Vec3

  \param rhs
	Vec3 to add
*/
/**************************************************************************/
Vec3& Vec3::operator += (const Vec3& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec3 -= Vec3

  \param rhs
	Vec3 to subtract
*/
/**************************************************************************/
Vec3& Vec3::operator -= (const Vec3& rhs) {
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec3 *= scalar

  \param rhs
	Amount to multiply Vec3 by
*/
/**************************************************************************/
Vec3& Vec3::operator *= (float rhs) {
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec3 /= scalar

  \param rhs
	Amount to divide Vec3 by
*/
/**************************************************************************/
Vec3& Vec3::operator /= (float rhs) {
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	-Vec3 (Negate Vec3)
*/
/**************************************************************************/
Vec3 Vec3::operator -() const {
	return { - this->x, - this->y , - this->z };
}

/**************************************************************************/
/*!
  \brief
	Vec3 + Vec3

  \param lhs, rhs
	Vec3

  \return
	Result
*/
/**************************************************************************/
Vec3 operator + (const Vec3& lhs, const Vec3& rhs) {
	Vec3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec3 - Vec3

  \param lhs
	Vec3

  \return
	Result
*/
/**************************************************************************/
Vec3 operator - (const Vec3& lhs, const Vec3& rhs) {
	Vec3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec3 * scalar

  \param lhs
	Vec3

  \param rhs
	Scalar to multiply Vec3 by

  \return
	Result Vec3
*/
/**************************************************************************/
Vec3 operator * (const Vec3& lhs, float rhs) {
	Vec3 result;
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	result.z = lhs.z * rhs;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Scalar * Vec3

  \param lhs
	Scalar to multiply Vec3 by

  \param rhs
	Vec3

  \return
	Result Vec3
*/
/**************************************************************************/
Vec3 operator * (float lhs, const Vec3& rhs) {
	Vec3 result;
	result.x = lhs * rhs.x;
	result.y = lhs * rhs.y;
	result.z = lhs * rhs.z;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec3 / scalar

  \param lhs
	Vec3

  \param rhs
	Scalar to divide by

  \return
	Result Vec3
*/
/**************************************************************************/
Vec3 operator / (const Vec3& lhs, float rhs) {
	Vec3 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	result.z = lhs.z / rhs;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Set a Vec3 to a specified value

  \param pResult
	Vec3 to set

  \param _x
	X to set

  \param _y
	Y to set

  \param _z
	Z to set
*/
/**************************************************************************/
void 	Vec3Set(Vec3 &pResult, float _x, float _y, float _z){
	pResult.x = _x;
	pResult.y = _y;
	pResult.z = _z;
}

/**************************************************************************/
/*!
  \brief
	Sets a Vec3 as a point

  \param pResult
	Vec3 to store result in

  \param _x
	X to set

  \param _y
	Y to set
*/
/**************************************************************************/
void	Vec3SetPoint(Vec3 &pResult, float _x, float _y){
	pResult.x = _x;
	pResult.y = _y;
	pResult.z = 1.f;
}

/**************************************************************************/
/*!
  \brief
	Sets a Vec3 as a vector

  \param pResult
	Vec3 to store result in

  \param _x
	X to set

  \param _y
	Y to set
*/
/**************************************************************************/
void	Vec3SetVector(Vec3 &pResult, float _x, float _y){
	pResult.x = _x;
	pResult.y = _y;
	pResult.z = 0.f;
}

/**************************************************************************/
/*!
  \brief
	Normalises a Vec3

  \param pResult
	Vec3 to store result in

  \param pVec0
	Original Vec3
*/
/**************************************************************************/
void	Vec3Normalize(Vec3& pResult, const Vec3& pVec0) {
	float vecLength = Vec3Length(pVec0);
	pResult.x = pVec0.x / vecLength;
	pResult.y = pVec0.y / vecLength;
	pResult.z = pVec0.z / vecLength;
}

/**************************************************************************/
/*!
  \brief
	Gets length of a Vec3

  \param pVec0
	Vec3 to get length

  \return
	Length of Vec3
*/
/**************************************************************************/
float	Vec3Length(const Vec3& pVec0) {
	return sqrtf(pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z);
}

/**************************************************************************/
/*!
  \brief
	Gets squared length of a Vec3

  \param pVec0
	Vec3 to get length

  \return
	Squared length of Vec3
*/
/**************************************************************************/
float	Vec3SquareLength(const Vec3& pVec0) {
	return pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z;
}

/**************************************************************************/
/*!
  \brief
	Gets distance between 2 Vec3 s

  \param pVec0
	First Vec3

  \param pVec1
	Second Vec3

  \return
	Distance
*/
/**************************************************************************/
float	Vec3Distance(const Vec3& pVec0, const Vec3& pVec1) {
	return Vec3Length(pVec1 - pVec0);
}

/**************************************************************************/
/*!
  \brief
	Gets squared distance between 2 Vec3 s

  \param pVec0
	First Vec3

  \param pVec1
	Second Vec3

  \return
	Squared distance
*/
/**************************************************************************/
float	Vec3SquareDistance(const Vec3& pVec0, const Vec3& pVec1) {
	return Vec3SquareLength(pVec1 - pVec0);
}

/**************************************************************************/
/*!
  \brief
	Vec3 dot Vec3

  \param pVec0
	First Vec3

  \param pVec1
	Second Vec3

  \return
	Vec3 dot Vec3
*/
/**************************************************************************/
float	Vec3DotProduct(const Vec3& pVec0, const Vec3& pVec1) {
	return (pVec0.x * pVec1.x + pVec0.y * pVec1.y + pVec0.z * pVec1.z);
}

/**************************************************************************/
/*!
  \brief
	Gets cross product of 2 Vec3

  \param pVec0
	First Vec3

  \param pVec1
	Second Vec3

  \return
	Cross Product
*/
/**************************************************************************/
Vec3	Vec3CrossProduct(const Vec3& pVec0, const Vec3& pVec1) {
	Vec3 result;
	result.x = (pVec0.y * pVec1.z - pVec0.z * pVec1.y);
	result.y = (pVec0.z * pVec1.x - pVec0.x * pVec1.z);
	result.z = (pVec0.x * pVec1.y - pVec0.y * pVec1.x);
	return result;
}

/**************************************************************************/
/*!
  \brief
	Validates if a Vec3 is a homogeneous coordinate

  \param pVec0
	Vec3

  \return
	True if homogeneous
*/
/**************************************************************************/
bool	Vec3ValidateHomogenous(const Vec3& pVec0){
	return (pVec0.z == 0.f || pVec0.z == 1.f);
}