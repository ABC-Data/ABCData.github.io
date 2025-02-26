/******************************************************************************/
/*!
\file		Vector4D.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 19, 2022
\brief		Implementation of the 4D vector library

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "Vector4D.h"
#include <math.h>

/**************************************************************************/
/*!
	\brief
	Default constructor for Vec4
*/
/**************************************************************************/
Vec4::Vec4() {
	x = 0.f;
	y = 0.f;
	z = 0.f;
	w = 0.f;
}

/**************************************************************************/
/*!
  \brief
	Constructor for Vec4

  \param _x
	X to set

  \param _y
	Y to set

  \param _z
	Z to set

  \param _w
	W to set
*/
/**************************************************************************/
Vec4::Vec4(float _x, float _y, float _z, float _w) {
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

/**************************************************************************/
/*!
  \brief
	Vec4 += Vec4

  \param rhs
	Vec4 to add
*/
/**************************************************************************/
Vec4& Vec4::operator += (const Vec4& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	this->w += rhs.w;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec4 -= Vec4

  \param rhs
	Vec4 to subtract
*/
/**************************************************************************/
Vec4& Vec4::operator -= (const Vec4& rhs) {
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	this->w -= rhs.w;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec4 *= scalar

  \param rhs
	Amount to multiply Vec4 by
*/
/**************************************************************************/
Vec4& Vec4::operator *= (float rhs) {
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;
	this->w *= rhs;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Vec4 /= scalar

  \param rhs
	Amount to divide Vec4 by
*/
/**************************************************************************/
Vec4& Vec4::operator /= (float rhs) {
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;
	this->w /= rhs;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	-Vec4 (Negate Vec4)
*/
/**************************************************************************/
Vec4 Vec4::operator -() const {
	return { - this->x, - this->y , - this->z , - this->w};
}

/**************************************************************************/
/*!
  \brief
	Vec4 + Vec4

  \param lhs, rhs
	Vec4

  \return
	Result
*/
/**************************************************************************/
Vec4 operator + (const Vec4& lhs, const Vec4& rhs) {
	Vec4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec4 - Vec4

  \param lhs
	Vec4

  \return
	Result
*/
/**************************************************************************/
Vec4 operator - (const Vec4& lhs, const Vec4& rhs) {
	Vec4 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec4 * scalar

  \param lhs
	Vec4

  \param rhs
	Scalar to multiply Vec4 by

  \return
	Result Vec4
*/
/**************************************************************************/
Vec4 operator * (const Vec4& lhs, float rhs) {
	Vec4 result;
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	result.z = lhs.z * rhs;
	result.w = lhs.w * rhs;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Scalar * Vec4

  \param lhs
	Scalar to multiply Vec4 by

  \param rhs
	Vec4

  \return
	Result Vec4
*/
/**************************************************************************/
Vec4 operator * (float lhs, const Vec4& rhs) {
	Vec4 result;
	result.x = lhs * rhs.x;
	result.y = lhs * rhs.y;
	result.z = lhs * rhs.z;
	result.w = lhs * rhs.w;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Vec4 / scalar

  \param lhs
	Vec4

  \param rhs
	Scalar to divide by

  \return
	Result Vec4
*/
/**************************************************************************/
Vec4 operator / (const Vec4& lhs, float rhs) {
	Vec4 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	result.z = lhs.z / rhs;
	result.w = lhs.w / rhs;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Set a Vec4 to a specified value

  \param pResult
	Vec4 to set

  \param _x
	X to set

  \param _y
	Y to set

  \param _z
	Z to set

  \param _w
	W to set
*/
/**************************************************************************/
void 	Vec4Set(Vec4 &pResult, float _x, float _y, float _z, float _w){
	pResult.x = _x;
	pResult.y = _y;
	pResult.z = _z;
	pResult.w = _w;
}

/**************************************************************************/
/*!
  \brief
	Normalises a Vec4

  \param pResult
	Vec4 to store result in

  \param pVec0
	Original Vec4
*/
/**************************************************************************/
void	Vec4Normalize(Vec4& pResult, const Vec4& pVec0) {
	float vecLength = Vec4Length(pVec0);
	pResult.x = pVec0.x / vecLength;
	pResult.y = pVec0.y / vecLength;
	pResult.z = pVec0.z / vecLength;
	pResult.w = pVec0.w / vecLength;
}

/**************************************************************************/
/*!
  \brief
	Gets length of a Vec4

  \param pVec0
	Vec4 to get length

  \return
	Length of Vec4
*/
/**************************************************************************/
float	Vec4Length(const Vec4& pVec0) {
	return sqrtf(pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z + pVec0.w * pVec0.w);
}

/**************************************************************************/
/*!
  \brief
	Gets squared length of a Vec4

  \param pVec0
	Vec4 to get length

  \return
	Squared length of Vec4
*/
/**************************************************************************/
float	Vec4SquareLength(const Vec4& pVec0) {
	return pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z + pVec0.w * pVec0.w;
}

/**************************************************************************/
/*!
  \brief
	Gets distance between 2 Vec4 s

  \param pVec0
	First Vec4

  \param pVec1
	Second Vec4

  \return
	Distance
*/
/**************************************************************************/
float	Vec4Distance(const Vec4& pVec0, const Vec4& pVec1) {
	return Vec4Length(pVec1 - pVec0);
}

/**************************************************************************/
/*!
  \brief
	Gets squared distance between 2 Vec4 s

  \param pVec0
	First Vec4

  \param pVec1
	Second Vec4

  \return
	Squared distance
*/
/**************************************************************************/
float	Vec4SquareDistance(const Vec4& pVec0, const Vec4& pVec1) {
	return Vec4SquareLength(pVec1 - pVec0);
}

/**************************************************************************/
/*!
  \brief
	Vec4 dot Vec4

  \param pVec0
	First Vec4

  \param pVec1
	Second Vec4

  \return
	Vec4 dot Vec4
*/
/**************************************************************************/
float	Vec4DotProduct(const Vec4& pVec0, const Vec4& pVec1) {
	return (pVec0.x * pVec1.x + pVec0.y * pVec1.y + pVec0.z * pVec1.z + pVec0.w * pVec1.w);
}

/**************************************************************************/
/*!
	\brief
	Default constructor for Color
*/
/**************************************************************************/
Color::Color() {
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

/**************************************************************************/
/*!
  \brief
	Constructor for Color

  \param _r
	R to set

  \param _g
	G to set

  \param _b
	B to set

  \param _a
	A to set
*/
/**************************************************************************/
Color::Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

/**************************************************************************/
/*!
  \brief
	Color += Color

  \param rhs
	Color to add
*/
/**************************************************************************/
Color& Color::operator += (const Color &rhs){
	this->r += rhs.r;
	this->g += rhs.g;
	this->b += rhs.b;
	this->a += rhs.a;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Color -= Color

  \param rhs
	Color to subtract
*/
/**************************************************************************/
Color& Color::operator -= (const Color &rhs){
	this->r -= rhs.r;
	this->g -= rhs.g;
	this->b -= rhs.b;
	this->a -= rhs.a;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Color *= Scalar

  \param rhs
	Scalar to multiply
*/
/**************************************************************************/
Color& Color::operator *= (unsigned char rhs){
	this->r *= rhs;
	this->g *= rhs;
	this->b *= rhs;
	this->a *= rhs;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Color + Color

  \param lhs, rhs
	Color

  \return
	Result
*/
/**************************************************************************/
Color operator + (const Color &lhs, const Color &rhs){
	Color result;
	result.r = lhs.r + rhs.r;
	result.g = lhs.g + rhs.g;
	result.b = lhs.b + rhs.b;
	result.a = lhs.a + rhs.a;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Color - Color

  \param lhs, rhs
	Color

  \return
	Result
*/
/**************************************************************************/
Color operator - (const Color &lhs, const Color &rhs){
	Color result;
	result.r = lhs.r - rhs.r;
	result.g = lhs.g - rhs.g;
	result.b = lhs.b - rhs.b;
	result.a = lhs.a - rhs.a;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Color * Scalar

  \param lhs
	Color

  \param rhs
	Scalar

  \return
	Result
*/
/**************************************************************************/
Color operator * (const Color &lhs, unsigned char rhs){
	Color result;
	result.r = lhs.r * rhs;
	result.g = lhs.g * rhs;
	result.b = lhs.b * rhs;
	result.a = lhs.a * rhs;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Scalar * Color

  \param lhs
	Scalar

  \param rhs
	Color

  \return
	Result
*/
/**************************************************************************/
Color operator * (unsigned char lhs, const Color &rhs){
	Color result;
	result.r = lhs * rhs.r;
	result.g = lhs * rhs.g;
	result.b = lhs * rhs.b;
	result.a = lhs * rhs.a;
	return result;
}

/**************************************************************************/
/*!
  \brief
	Adds an amount to a component of Color

  \param pTarget
	Color to change

  \param addAmount
	Amount to add

  \param compIndex
	Index of component to add
*/
/**************************************************************************/
void	ColorAddComponent(Color& pTarget, unsigned char addAmount, int compIndex){
	if (compIndex < 0 || compIndex > 3) return;
	pTarget.c[compIndex] += addAmount;
}

/**************************************************************************/
/*!
  \brief
	Removes an amount to a component of Color

  \param pTarget
	Color to change

  \param removeAmount
	Amount to remove

  \param compIndex
	Index of component to remove
*/
/**************************************************************************/
void	ColorRemoveComponent(Color& pTarget, unsigned char removeAmount, int compIndex){
	if (compIndex < 0 || compIndex > 3) return;
	pTarget.c[compIndex] -= removeAmount;
}

/**************************************************************************/
/*!
  \brief
	Removes all color for an index in Color

  \param pTarget
	Color to change

  \param compIndex
	Index of component to clear
*/
/**************************************************************************/
void	ColorRemoveComponent(Color& pTarget, int compIndex){
	if (compIndex < 0 || compIndex > 3) return;
	pTarget.c[compIndex] = 0;
}

/**************************************************************************/
/*!
  \brief
	Normalizes the range of Color values from 0-255 to 0-1

  \param pTarget
	Color to normalize

  \return
	Vec4 of normalized colors
*/
/**************************************************************************/
Vec4	ColorGetNormalized(Color& pTarget){
	Vec4 result;
	result.x = static_cast<float>(pTarget.r / 255.f);
	result.y = static_cast<float>(pTarget.g / 255.f);
	result.z = static_cast<float>(pTarget.b / 255.f);
	result.w = static_cast<float>(pTarget.a / 255.f);
	return result;
}

/**************************************************************************/
/*!
  \brief
	Normalizes the range of Color values from 0-255 to 0-1

  \param source
	Source Color

  \param dest
	Destination Vec4
*/
/**************************************************************************/
void	ColorGetNormalized(const Color& source, Vec4& dest){
	dest.x = static_cast<float>(source.r / 255.f);
	dest.y = static_cast<float>(source.g / 255.f);
	dest.z = static_cast<float>(source.b / 255.f);
	dest.w = static_cast<float>(source.a / 255.f);
}

/**************************************************************************/
/*!
  \brief
	Scales up a normalized Color (Vec4) from 0-1 to 0-255

  \param pTarget
	Normalized color to scale

  \return
	Color with scaled up values
*/
/**************************************************************************/
Color	ColorFromNormalized(Vec4& pTarget){
	Color result;
	result.r = static_cast<unsigned char>(pTarget.x * 255.f);
	result.g = static_cast<unsigned char>(pTarget.y * 255.f);
	result.b = static_cast<unsigned char>(pTarget.z * 255.f);
	result.a = static_cast<unsigned char>(pTarget.w * 255.f);
	return result;
}

/**************************************************************************/
/*!
  \brief
	Scales up a normalized Color (Vec4) from 0-1 to 0-255

  \param source
	Source Vec4

  \param dest
	Destination Color
*/
/**************************************************************************/
void	ColorFromNormalized(const Vec4& source, Color& dest){
	dest.r = static_cast<unsigned char>(source.x * 255.f);
	dest.g = static_cast<unsigned char>(source.y * 255.f);
	dest.b = static_cast<unsigned char>(source.z * 255.f);
	dest.a = static_cast<unsigned char>(source.w * 255.f);
}