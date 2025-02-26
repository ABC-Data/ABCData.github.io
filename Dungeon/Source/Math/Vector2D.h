/******************************************************************************/
/*!
\file		Vector2D.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 19, 2022
\brief		Header of the 2D vector library

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
/**************************************************************************/
/*!
Vec2 struct
*/
/**************************************************************************/
typedef union Vec2
{
	struct
	{
		float x, y;
	};

	float m[2];

	/**************************************************************************/
	/*!
	  \brief
		Default constructor for Vec2
	*/
	/**************************************************************************/
	Vec2();

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
	Vec2(float _x, float _y);


	/**************************************************************************/
	/*!
	  \brief
		Vec2 += Vec2

	  \param rhs
		Vec2 to add
	*/
	/**************************************************************************/
	Vec2& operator += (const Vec2 &rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec2 -= Vec2

	  \param rhs
		Vec2 to subtract
	*/
	/**************************************************************************/
	Vec2& operator -= (const Vec2 &rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec2 *= scalar

	  \param rhs
		Amount to multiply Vec2 by
	*/
	/**************************************************************************/
	Vec2& operator *= (float rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec2 /= scalar

	  \param rhs
		Amount to divide Vec2 by
	*/
	/**************************************************************************/
	Vec2& operator /= (float rhs);

	/**************************************************************************/
	/*!
	  \brief
		-Vec2 (Negate Vec2)
	*/
	/**************************************************************************/
	Vec2 operator -() const;

} Vec2, Vector2D, Vector2;

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
Vec2 operator + (const Vec2 &lhs, const Vec2 &rhs);

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
Vec2 operator - (const Vec2 &lhs, const Vec2 &rhs);

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
Vec2 operator * (const Vec2 &lhs, float rhs);

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
Vec2 operator * (float lhs, const Vec2 &rhs);

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
Vec2 operator / (const Vec2 &lhs, float rhs);

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
void 	Vec2Set(Vec2 &pResult, float _x, float _y);

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
void	Vec2Normalize(Vec2 &pResult, const Vec2 &pVec0);

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
float	Vec2Length(const Vec2 &pVec0);

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
float	Vec2SquareLength(const Vec2 &pVec0);

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
float	Vec2Distance(const Vec2 &pVec0, const Vec2 &pVec1);

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
float	Vec2SquareDistance(const Vec2 &pVec0, const Vec2 &pVec1);

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
float	Vec2DotProduct(const Vec2 &pVec0, const Vec2 &pVec1);

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
float	Vec2CrossProductMag(const Vec2 &pVec0, const Vec2 &pVec1);


#endif