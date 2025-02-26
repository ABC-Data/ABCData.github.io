/******************************************************************************/
/*!
\file		Vector2D.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 19, 2022
\brief		Implementation of the 2D vector library

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "Vector2D.h"
#include <math.h>

/**************************************************************************/
/*!
  \brief
	Default constructor for Vec2
*/
/**************************************************************************/
Vec2::Vec2() {
	x = 0.f;
	y = 0.f;
}


/**************************************************************************/
/*!
  \brief
	Constructor for Vec2

  \param _x
	X to set

  \param _y
	Y to set
*/
/**************************************************************************/
Vec2::Vec2(float _x, float _y) {
	x = _x;
	y = _y;
}

/**************************************************************************/
/*!
  \brief
	Vec2 += Vec2

  \param rhs
	Vec2 to add
*/
/**************************************************************************/
Vec2& Vec2::operator += (const Vec2& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec2 -= Vec2

  \param rhs
	Vec2 to subtract
*/
/**************************************************************************/
Vec2& Vec2::operator -= (const Vec2& rhs) {
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec2 *= scalar

  \param rhs
	Amount to multiply Vec2 by
*/
/**************************************************************************/
Vec2& Vec2::operator *= (float rhs) {
	this->x *= rhs;
	this->y *= rhs;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec2 /= scalar

  \param rhs
	Amount to divide Vec2 by
*/
/**************************************************************************/
Vec2& Vec2::operator /= (float rhs) {
	this->x /= rhs;
	this->y /= rhs;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	-Vec2 (Negate Vec2)
*/
/**************************************************************************/
Vec2 Vec2::operator -() const {
	return { - this->x, - this->y };
}

/**************************************************************************/
/*!
  \brief
	Vec2 + Vec2

  \param lhs, rhs
	Vec2

  \return
	Result
*/
/**************************************************************************/
Vec2 operator + (const Vec2& lhs, const Vec2& rhs) {
	Vec2 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec2 - Vec2

  \param lhs
	Vec2

  \return
	Result
*/
/**************************************************************************/
Vec2 operator - (const Vec2& lhs, const Vec2& rhs) {
	Vec2 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec2 * scalar

  \param lhs
	Vec2

  \param rhs
	Scalar to multiply Vec2 by

  \return
	Result Vec2
*/
/**************************************************************************/
Vec2 operator * (const Vec2& lhs, float rhs) {
	Vec2 result;
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Scalar * Vec2

  \param lhs
	Scalar to multiply Vec2 by

  \param rhs
	Vec2

  \return
	Result Vec2
*/
/**************************************************************************/
Vec2 operator * (float lhs, const Vec2& rhs) {
	Vec2 result;
	result.x = lhs * rhs.x;
	result.y = lhs * rhs.y;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec2 / scalar

  \param lhs
	Vec2

  \param rhs
	Scalar to divide by

  \return
	Result Vec2
*/
/**************************************************************************/
Vec2 operator / (const Vec2& lhs, float rhs) {
	Vec2 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Set a Vec2 to a specified value

  \param pResult
	Vec2 to set

  \param _x
	X to set

  \param _y
	Y to set
*/
/**************************************************************************/
void 	Vec2Set(Vec2 &pResult, float _x, float _y){
	pResult.x = _x;
	pResult.y = _y;
}

/**************************************************************************/
/*!
  \brief
	Normalises a Vec2

  \param pResult
	Vec2 to store result in

  \param pVec0
	Original Vec2
*/
/**************************************************************************/
void	Vec2Normalize(Vec2& pResult, const Vec2& pVec0) {
	float vecLength = Vec2Length(pVec0);
	pResult.x = pVec0.x / vecLength;
	pResult.y = pVec0.y / vecLength;
}

/**************************************************************************/
/*!
  \brief
	Gets length of a Vec2

  \param pVec0
	Vec2 to get length

  \return
	Length of Vec2
*/
/**************************************************************************/
float	Vec2Length(const Vec2& pVec0) {
	return sqrtf(pVec0.x * pVec0.x + pVec0.y * pVec0.y);
}

/**************************************************************************/
/*!
  \brief
	Gets squared length of a Vec2

  \param pVec0
	Vec2 to get length

  \return
	Squared length of Vec2
*/
/**************************************************************************/
float	Vec2SquareLength(const Vec2& pVec0) {
	return pVec0.x * pVec0.x + pVec0.y * pVec0.y;
}

/**************************************************************************/
/*!
  \brief
	Gets distance between 2 Vec2 s

  \param pVec0
	First Vec2

  \param pVec1
	Second Vec2

  \return
	Distance
*/
/**************************************************************************/
float	Vec2Distance(const Vec2& pVec0, const Vec2& pVec1) {
	return Vec2Length(pVec1 - pVec0);
}

/**************************************************************************/
/*!
  \brief
	Gets squared distance between 2 Vec2 s

  \param pVec0
	First Vec2

  \param pVec1
	Second Vec2

  \return
	Squared distance
*/
/**************************************************************************/
float	Vec2SquareDistance(const Vec2& pVec0, const Vec2& pVec1) {
	return Vec2SquareLength(pVec1 - pVec0);
}

/**************************************************************************/
/*!
  \brief
	Vec2 dot Vec2

  \param pVec0
	First Vec2

  \param pVec1
	Second Vec2

  \return
	Vec2 dot Vec2
*/
/**************************************************************************/
float	Vec2DotProduct(const Vec2& pVec0, const Vec2& pVec1) {
	return (pVec0.x * pVec1.x + pVec0.y * pVec1.y);
}

/**************************************************************************/
/*!
  \brief
	Gets cross product magnitude of 2 Vec2

  \param pVec0
	First Vec2

  \param pVec1
	Second Vec2

  \return
	Cross Product Magnitude
*/
/**************************************************************************/
float	Vec2CrossProductMag(const Vec2& pVec0, const Vec2& pVec1) {
	//Let all z coords be 0, theres only really the z coord left, which is all we need
	return (pVec0.x * pVec1.y - pVec0.y * pVec1.x);
}