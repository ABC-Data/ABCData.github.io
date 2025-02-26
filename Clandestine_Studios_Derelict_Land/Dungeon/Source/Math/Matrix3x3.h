/******************************************************************************/
/*!
\file		Matrix3x3.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 19, 2022
\brief		Header file of the 3x3 Matrix library

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef MATRIX3X3_H_
#define MATRIX3X3_H_

#include "Vector2D.h"
#include "Vector3D.h"

#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
/**************************************************************************/
/*!
Mat3 struct
	*/
/**************************************************************************/
typedef union Mat3
{
	struct 
	{
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	};

	float m[9];
	float m2[3][3];

	Mat3();
	Mat3(const float *pArr);
	Mat3(float _00, float _01, float _02,
				float _10, float _11, float _12,
				float _20, float _21, float _22);
	Mat3& operator=(const Mat3 &rhs);

	// Assignment operators
	Mat3& operator *= (const Mat3 &rhs);

} Mat3, Mtx33, M33, Matrix33;

/**************************************************************************/
/*!
  \brief
	Mat3 * Mat3

  \param lhs, rhs
	Mat3
*/
/**************************************************************************/
Mat3 operator * (const Mat3 &lhs, const Mat3 &rhs);

/**************************************************************************/
/*!
  \brief
	Mat3 * Vec2, Assumes last row of Vec2 is 1

  \param pMtx
	Mat3 to multiply

  \param rhs
	Vec2 to multiply

  \return
	Vec2 result
*/
/**************************************************************************/
Vec2  operator * (const Mat3 &pMtx, const Vec2 &rhs);

/**************************************************************************/
/*!
  \brief
	Mat3 * Vec3

  \param pMtx
	Mat3 to multiply

  \param rhs
	Vec3 to multiply

  \return
	Vec3 result
*/
/**************************************************************************/
Vec3  operator * (const Mat3 &pMtx, const Vec3 &rhs);

/**************************************************************************/
/*!
  \brief
	Creates an identity matrix

  \param pResult
	Matrix to modify
*/
/**************************************************************************/
void Mat3Identity(Mat3 &pResult);

/**************************************************************************/
/*!
  \brief
	Creates a Mat3 with translation

  \param pResult
	Matrix to modify

  \param x
	X coord to translate by

  \param y
	Y coord to translate by
*/
/**************************************************************************/
void Mat3Translate(Mat3 &pResult, float x, float y);

/**************************************************************************/
/*!
  \brief
	Creates a Mat3 with scaling

  \param pResult
	Matrix to modify

  \param x
	X amount to scale by

  \param y
	Y amount to scale by
*/
/**************************************************************************/
void Mat3Scale(Mat3 &pResult, float x, float y);

/**************************************************************************/
/*!
  \brief
	Creates a Mat3 with rotation

  \param pResult
	Matrix to modify

  \param angle
	Angle in radians
*/
/**************************************************************************/
void Mat3RotRad(Mat3 &pResult, float angle);

/**************************************************************************/
/*!
  \brief
	Creates a Mat3 with rotation

  \param pResult
	Matrix to modify

  \param angle
	Angle in degrees
*/
/**************************************************************************/
void Mat3RotDeg(Mat3 &pResult, float angle);

/**************************************************************************/
/*!
  \brief
	Transposes a Mat3

  \param pResult
	Matrix to store transposed matrix

  \param pMtx
	Original matrix
*/
/**************************************************************************/
void Mat3Transpose(Mat3 &pResult, const Mat3 &pMtx);

/**************************************************************************/
/*!
  \brief
	Creates an inverse Mat3

  \param pResult
	Matrix to store result

  \param determinant
	Determinant of the matrix

  \param pMtx
	Original Matrix to be inverted
*/
/**************************************************************************/
void Mat3Inverse(Mat3 &pResult, float *determinant, const Mat3 &pMtx);

/**************************************************************************/
/*!
  \brief
	Applies scaling, rotating, and translating in the right order

  \param pResult
	Matrix to store result

  \param scale
	Amount to scale by

  \param rotateDeg
	Amount to rotate by in degrees

  \param trans
	Amount to translate by
*/
/**************************************************************************/
void Mat3SRT(Mat3 &pResult, const Vec2 &scale, float rotateDeg, const Vec2 &trans);

#endif