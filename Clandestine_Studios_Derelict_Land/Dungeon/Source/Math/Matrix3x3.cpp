/******************************************************************************/
/*!
\file		Matrix3x3.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 19, 2022
\brief		Implementation of the 3x3 Matrix library

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "Matrix3x3.h"
#include <cmath>
#define _USE_MATH_DEFINES
#define EPSILON		0.0001f
# define M_PI           3.14159265358979323846f


/**************************************************************************/
/*!
  \brief
	Default constructor for Mat3
*/
/**************************************************************************/
Mat3::Mat3() {
	for (int i = 0; i < 9; i++) {
		m[i] = 0.f;
	}
}

/**************************************************************************/
/*!
  \brief
	Copy constructor for Mat3

  \param pArr
	Array of 9 floats
*/
/**************************************************************************/
Mat3::Mat3(const float* pArr) {
	for (int i = 0; i < 9; i++) {
		m[i] = *(pArr + i);
	}
}

/**************************************************************************/
/*!
  \brief
	Conversion constructor for Mat3

  \param _00 - _22
	Each individual element of the matrix
*/
/**************************************************************************/
Mat3::Mat3(float _00, float _01, float _02,
	float _10, float _11, float _12,
	float _20, float _21, float _22) {
	m00 = _00;
	m01 = _01;
	m02 = _02;
	m10 = _10;
	m11 = _11;
	m12 = _12;
	m20 = _20;
	m21 = _21;
	m22 = _22;
}

/**************************************************************************/
/*!
  \brief
	Copy assignment operator for Mat3

  \param rhs
	Mat3 to copy
*/
/**************************************************************************/
Mat3& Mat3::operator=(const Mat3& rhs) {
	for (int i = 0; i < 9; i++) {
		this->m[i] = rhs.m[i];
	}
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Mat3 *= Mat3

  \param rhs
	Mat3
*/
/**************************************************************************/
Mat3& Mat3::operator *= (const Mat3& rhs) {
	*this = *this * rhs;
	return *this;
}

/**************************************************************************/
/*!
  \brief
	Mat3 * Mat3

  \param lhs, rhs
	Mat3
*/
/**************************************************************************/
Mat3 operator * (const Mat3& lhs, const Mat3& rhs) {
	Mat3 resultMatrix;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			resultMatrix.m[x + (3 * y)] = (lhs.m[y * 3] * rhs.m[x]) + (lhs.m[y * 3 + 1] * rhs.m[x + 3]) + (lhs.m[y * 3 + 2] * rhs.m[x + 6]);
		}
	}
	return resultMatrix;
}

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
Vec2  operator * (const Mat3& pMtx, const Vec2& rhs) {
	//Assumes 3rd row and col are unused
	Vec2 resultVec;
	resultVec.x = (pMtx.m[0] * rhs.x) + (pMtx.m[1] * rhs.y) + (pMtx.m[2] * 1.f);
	resultVec.y = (pMtx.m[3] * rhs.x) + (pMtx.m[4] * rhs.y) + (pMtx.m[5] * 1.f);
	return resultVec;
}

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
Vec3  operator * (const Mat3& pMtx, const Vec3& rhs) {
	Vec3 resultVec;
	resultVec.x = (pMtx.m[0] * rhs.x) + (pMtx.m[1] * rhs.y) + (pMtx.m[2] * rhs.z);
	resultVec.y = (pMtx.m[3] * rhs.x) + (pMtx.m[4] * rhs.y) + (pMtx.m[5] * rhs.z);
	resultVec.z = (pMtx.m[6] * rhs.x) + (pMtx.m[7] * rhs.y) + (pMtx.m[8] * rhs.z);
	return resultVec;
}

/**************************************************************************/
/*!
  \brief
	Creates an identity matrix

  \param pResult
	Matrix to modify
*/
/**************************************************************************/
void Mat3Identity(Mat3& pResult) {
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			pResult.m[x + (3 * y)] = (x == y) ? 1.f : 0.f;
		}
	}
}

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
void Mat3Translate(Mat3& pResult, float x, float y) {
	Mat3Identity(pResult);
	pResult.m[2] = x;
	pResult.m[5] = y;
}

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
void Mat3Scale(Mat3& pResult, float x, float y) {
	Mat3Identity(pResult);
	pResult.m[0] = x;
	pResult.m[4] = y;
}

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
void Mat3RotRad(Mat3& pResult, float angle) {
	Mat3Identity(pResult);
	pResult.m[0] = cosf(angle);
	pResult.m[1] = -sinf(angle);
	pResult.m[3] = sinf(angle);
	pResult.m[4] = cosf(angle);
}

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
void Mat3RotDeg(Mat3& pResult, float angle) {
	float rAngle = M_PI / 180.f * angle;
	Mat3Identity(pResult);
	//SPECIAL ANGLES HANDLING TO AVOID FLOATING POINT ERRORS:
	if (angle == 90.f){
		pResult.m[0] = 0.f;
		pResult.m[1] = -1.f;
		pResult.m[3] = 1.f;
		pResult.m[4] = 0.f;
	}
	else if (angle == 180.f){
		pResult.m[0] = -1.f;
		pResult.m[1] = 0.f;
		pResult.m[3] = 0.f;
		pResult.m[4] = -1.f;
	}
	else if (angle == 270.f){
		pResult.m[0] = 0.f;
		pResult.m[1] = 1.f;
		pResult.m[3] = -1.f;
		pResult.m[4] = 0.f;
	}
	else{
		pResult.m[0] = cosf(rAngle);
		pResult.m[1] = -sinf(rAngle);
		pResult.m[3] = sinf(rAngle);
		pResult.m[4] = cosf(rAngle);
	}
}

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
void Mat3Transpose(Mat3& pResult, const Mat3& pMtx) {
	Mat3 copyMatrix = pMtx;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			pResult.m[x + (3 * y)] = copyMatrix.m[y + (3 * x)];
		}
	}
}

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
void Mat3Inverse(Mat3& pResult, float* determinant, const Mat3& pMtx) {
	//first determine 3x3 determinant
	*determinant = (pMtx.m[0] * (pMtx.m[4] * pMtx.m[8] - pMtx.m[5] * pMtx.m[7]) -
		pMtx.m[1] * (pMtx.m[3] * pMtx.m[8] - pMtx.m[5] * pMtx.m[6]) +
		pMtx.m[2] * (pMtx.m[3] * pMtx.m[7] - pMtx.m[4] * pMtx.m[6]));
	if (*determinant <= EPSILON && *determinant >= -EPSILON) pResult = NULL; // check if absolute det <= EPSILON
	else {
		//Use A^-1 = adj(A) * 1/det(A) to determine inverse matrix
		Mat3 adjoint;
		for (int i = 0; i < 9; i++) { // counter for adjoint signs
			Mat3 temp;
			int j = 0;
			while (j < 4) {
				for (int y = 0; y < 3; y++) {
					for (int x = 0; x < 3; x++) {
						if (x == (i % 3) || y == (i / 3)) {
							continue;
						}
						else {
							temp.m[j++] = pMtx.m[x + 3 * y];
						}
					}
				}
			}
			adjoint.m[i] = (temp.m[0] * temp.m[3]) - (temp.m[1] * temp.m[2]);
			if (i % 2 == 0) adjoint.m[i] = -adjoint.m[i]; // if even, negate
		}
		Mat3Transpose(adjoint, adjoint);
		for (int i = 0; i < 9; i++) {
			pResult.m[i] = adjoint.m[i] / *(determinant);
		}
	}
	if (*determinant < 0.f) *determinant = -*determinant; //since driver does not abs det itself, absolute it here so the true det can be used above
}

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
void Mat3SRT(Mat3 &pResult, const Vec2 &scale, float rotateDeg, const Vec2 &trans){
	Mat3Identity(pResult);
	Mat3 compMtx = pResult;
	Mat3Scale(compMtx, scale.x, scale.y);
	pResult *= compMtx;
	Mat3RotDeg(compMtx, rotateDeg);
	pResult *= compMtx;
	Mat3Translate(compMtx, trans.x, trans.y);
	pResult *= compMtx;
}