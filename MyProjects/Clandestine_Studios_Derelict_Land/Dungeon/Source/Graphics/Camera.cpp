/******************************************************************************/
/*!
\file    Camera.cpp
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    November 2, 2022
\brief   Camera functions to calculate and update OrthographicViewProjection Matrix to be used in graphics.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Graphics/Camera.h"

Camera::Camera(): projection{0}, view{0}, viewprojection{0}
{
}

Camera::~Camera()
{
}
/**************************************************************************/
/*!
  \brief
	Initializing Orthographic projection matrix.

  \param left
	xmin for use in orthographic matrix.

  \param right
	xmax for use in orthographic matrix.

  \param bottom
	ymin for use in orthographic matrix.

  \param top
	xmax for use in orthographic matrix.
*/
/**************************************************************************/
void Camera::Init(float left, float right, float bottom, float top)
{
	projection = glm::ortho(left, right, bottom, top, 0.f, 1.f);
	view = glm::mat4(1.f);
	viewprojection = projection * view;
}
/**************************************************************************/
/*!
  \brief
	Setting Current Camera Position.

  \param pos
	Position(x,y,z) of camera.
*/
/**************************************************************************/
void Camera::SetCameraPosition(const glm::vec3& pos) {
	cam_pos = pos;
}
/**************************************************************************/
/*!
  \brief
	Setting Camera panning speed.

  \param speed
	Camera panning speed.

  \param dt
	Delta time, default = 1.f.
*/
/**************************************************************************/
void Camera::SetCameraSpeed(const float& speed, const float& dt) {
	cam_spd = speed * dt;
}
/**************************************************************************/
/*!
  \brief
	Setting Camera rotation.

  \param rotation
	Degree to rotate camera.
*/
/**************************************************************************/
void Camera::SetCameraRotation(float rotation) {
	cam_rot = rotation;
}
/**************************************************************************/
/*!
  \brief
	Setting new Orthographic projection matrix.

  \param left
	xmin for use in orthographic matrix.

  \param right
	xmax for use in orthographic matrix.

  \param bottom
	ymin for use in orthographic matrix.

  \param top
	xmax for use in orthographic matrix.
*/
/**************************************************************************/
void Camera::SetProjection(float left, float right, float bottom, float top) {
	projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	viewprojection = projection * view;
}

/**************************************************************************/
/*!
  \brief
	Retrieve Current Camera Position.
*/
/**************************************************************************/
const glm::vec3& Camera::GetCameraPosition(){
	return cam_pos;
}

const float& Camera::GetCameraSpeed() {
	return cam_spd;
}
/**************************************************************************/
/*!
  \brief
	Retrieve Current Camera Rotation.
*/
/**************************************************************************/
const float& Camera::GetCameraRotation() {
	return cam_rot;
}
/**************************************************************************/
/*!
  \brief
	Retrieve Current View Matrix.
*/
/**************************************************************************/
const glm::mat4& Camera::GetViewMatrix() {
	return view;
}
/**************************************************************************/
/*!
  \brief
	Retrieve Current Projection Matrix.
*/
/**************************************************************************/
const glm::mat4& Camera::GetProjectionMatrix() {
	return projection;
}
/**************************************************************************/
/*!
  \brief
	Retrieve Current View Projection Matrix.
*/
/**************************************************************************/
const glm::mat4& Camera::GetViewProjectionMatrix() {
	return viewprojection;
}

/**************************************************************************/
/*!
  \brief
	Update View Projection Matrix.
*/
/**************************************************************************/
void Camera::UpdateViewMatrix() {
	glm::mat4 xform = glm::translate(glm::mat4(1.f), cam_pos) 
		* glm::rotate(glm::mat4(1.0f), glm::radians(cam_rot), glm::vec3(0.f, 0.f, 1.f));

	view = glm::inverse(xform);
	viewprojection = projection * view;
}


