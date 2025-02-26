/******************************************************************************/
/*!
\file		Vector4D.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 19, 2022
\brief		Header of the 4D vector library

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef VECTOR4D_H_
#define VECTOR4D_H_

#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
/**************************************************************************/
/*!
Vec4 Struct
*/
/**************************************************************************/
typedef union Vec4
{
	struct
	{
		float x, y, z, w;
	};

	float m[3];

	/**************************************************************************/
	/*!
		\brief
		Default constructor for Vec4
	*/
	/**************************************************************************/
	Vec4();

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
	Vec4(float _x, float _y, float _z, float _w);

	/**************************************************************************/
	/*!
	  \brief
		Vec4 += Vec4

	  \param rhs
		Vec4 to add
	*/
	/**************************************************************************/
	Vec4& operator += (const Vec4 &rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec4 -= Vec4

	  \param rhs
		Vec4 to subtract
	*/
	/**************************************************************************/
	Vec4& operator -= (const Vec4 &rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec4 *= scalar

	  \param rhs
		Amount to multiply Vec4 by
	*/
	/**************************************************************************/
	Vec4& operator *= (float rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec4 /= scalar

	  \param rhs
		Amount to divide Vec4 by
	*/
	/**************************************************************************/
	Vec4& operator /= (float rhs);

	/**************************************************************************/
	/*!
	  \brief
		-Vec4 (Negate Vec4)
	*/
	/**************************************************************************/
	Vec4 operator -() const;

} Vec4, Vector4D, Vector4;

/**************************************************************************/
/*!
Color Struct
*/
/**************************************************************************/
typedef union Color
{
	struct
	{
		unsigned char r, g, b, a;
	};

	unsigned char c[4];
	
	/**************************************************************************/
	/*!
		\brief
		Default constructor for Color
	*/
	/**************************************************************************/
	Color();

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
	Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a);

	/**************************************************************************/
	/*!
	  \brief
		Color += Color

	  \param rhs
		Color to add
	*/
	/**************************************************************************/
	Color& operator += (const Color &rhs);

	/**************************************************************************/
	/*!
	  \brief
		Color -= Color

	  \param rhs
		Color to subtract
	*/
	/**************************************************************************/
	Color& operator -= (const Color &rhs);

	/**************************************************************************/
	/*!
	  \brief
		Color *= Scalar

	  \param rhs
		Scalar to multiply
	*/
	/**************************************************************************/
	Color& operator *= (unsigned char rhs);
	
} Color, RGBA;

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
Vec4 operator + (const Vec4 &lhs, const Vec4 &rhs);

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
Vec4 operator - (const Vec4 &lhs, const Vec4 &rhs);

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
Vec4 operator * (const Vec4 &lhs, float rhs);

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
Vec4 operator * (float lhs, const Vec4 &rhs);

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
Vec4 operator / (const Vec4 &lhs, float rhs);

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
void 	Vec4Set(Vec4 &pResult, float _x, float _y, float _z, float _w);

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
void	Vec4Normalize(Vec4 &pResult, const Vec4 &pVec0);

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
float	Vec4Length(const Vec4 &pVec0);

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
float	Vec4SquareLength(const Vec4 &pVec0);

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
float	Vec4Distance(const Vec4 &pVec0, const Vec4 &pVec1);

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
float	Vec4SquareDistance(const Vec4 &pVec0, const Vec4 &pVec1);

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
float	Vec4DotProduct(const Vec4 &pVec0, const Vec4 &pVec1);

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
Color operator + (const Color &lhs, const Color &rhs);

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
Color operator - (const Color &lhs, const Color &rhs);

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
Color operator * (const Color &lhs, unsigned char rhs);

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
Color operator * (unsigned char lhs, const Color &rhs);

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
void	ColorAddComponent(Color& pTarget, unsigned char addAmount, int compIndex);

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
void	ColorRemoveComponent(Color& pTarget, unsigned char removeAmount, int compIndex);

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
void	ColorRemoveComponent(Color& pTarget, int compIndex);

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
Vec4	ColorGetNormalized(Color& pTarget);

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
void	ColorGetNormalized(const Color& source, Vec4& dest);

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
Color	ColorFromNormalized(Vec4& pTarget);

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
void	ColorFromNormalized(const Vec4& source, Color& dest);

#endif