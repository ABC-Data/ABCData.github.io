/******************************************************************************/
/*!
\file		Vector3D.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 19, 2022
\brief		Header of the 3D vector library

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
/**************************************************************************/
/*!
Vec3 Struct
*/
/**************************************************************************/
typedef union Vec3
{
	struct
	{
		float x, y, z;
	};

	float m[3];

	/**************************************************************************/
	/*!
		\brief
		Default constructor for Vec3
	*/
	/**************************************************************************/
	Vec3();

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
	Vec3(float _x, float _y, float _z);

	/**************************************************************************/
	/*!
	  \brief
		Vec3 += Vec3

	  \param rhs
		Vec3 to add
	*/
	/**************************************************************************/
	Vec3& operator += (const Vec3 &rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec3 -= Vec3

	  \param rhs
		Vec3 to subtract
	*/
	/**************************************************************************/
	Vec3& operator -= (const Vec3 &rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec3 *= scalar

	  \param rhs
		Amount to multiply Vec3 by
	*/
	/**************************************************************************/
	Vec3& operator *= (float rhs);

	/**************************************************************************/
	/*!
	  \brief
		Vec3 /= scalar

	  \param rhs
		Amount to divide Vec3 by
	*/
	/**************************************************************************/
	Vec3& operator /= (float rhs);

	/**************************************************************************/
	/*!
	  \brief
		-Vec3 (Negate Vec3)
	*/
	/**************************************************************************/
	Vec3 operator -() const;

} Vec3, Vector3D, Vector3;

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
Vec3 operator + (const Vec3 &lhs, const Vec3 &rhs);

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
Vec3 operator - (const Vec3 &lhs, const Vec3 &rhs);

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
Vec3 operator * (const Vec3 &lhs, float rhs);

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
Vec3 operator * (float lhs, const Vec3 &rhs);

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
Vec3 operator / (const Vec3 &lhs, float rhs);

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
void 	Vec3Set(Vec3 &pResult, float _x, float _y, float _z);

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
void	Vec3SetPoint(Vec3 &pResult, float _x, float _y);

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
void	Vec3SetVector(Vec3 &pResult, float _x, float _y);

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
void	Vec3Normalize(Vec3 &pResult, const Vec3 &pVec0);

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
float	Vec3Length(const Vec3 &pVec0);

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
float	Vec3SquareLength(const Vec3 &pVec0);

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
float	Vec3Distance(const Vec3 &pVec0, const Vec3 &pVec1);

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
float	Vec3SquareDistance(const Vec3 &pVec0, const Vec3 &pVec1);

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
float	Vec3DotProduct(const Vec3 &pVec0, const Vec3 &pVec1);

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
Vec3	Vec3CrossProduct(const Vec3& pVec0, const Vec3& pVec1);

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
bool	Vec3ValidateHomogenous(const Vec3& pVec0);


#endif