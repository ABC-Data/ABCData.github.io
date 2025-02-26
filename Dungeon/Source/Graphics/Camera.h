/******************************************************************************/
/*!
\file    Camera.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    November 2, 2022
\brief   Camera functions to calculate and update OrthographicViewProjection Matrix to be used in graphics.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Graphics/GLFWsetup.h>

class Camera
{
public:
	Camera();
	~Camera();

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
	void Init(float left, float right, float bottom, float top);

	/**************************************************************************/
	/*!
	  \brief
		Setting Current Camera Position.

	  \param pos
		Position(x,y,z) of camera.
	*/
	/**************************************************************************/
	void SetCameraPosition(const glm::vec3& pos);
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
	void SetCameraSpeed(const float& speed, const float& dt = 1.f);
	/**************************************************************************/
	/*!
	  \brief
		Setting Camera rotation.

	  \param rotation
		Degree to rotate camera.
	*/
	/**************************************************************************/
	void SetCameraRotation(float rotation);
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
	void SetProjection(float left, float right, float bottom, float top);

	/**************************************************************************/
	/*!
	  \brief
		Retrieve Current Camera Position.
	*/
	/**************************************************************************/
	const glm::vec3& GetCameraPosition();
	/**************************************************************************/
	/*!
	  \brief
		Retrieve Current Camera Speed.
	*/
	/**************************************************************************/
	const float& GetCameraSpeed();
	/**************************************************************************/
	/*!
	  \brief
		Retrieve Current Camera Rotation.
	*/
	/**************************************************************************/
	const float& GetCameraRotation();
	/**************************************************************************/
	/*!
	  \brief
		Retrieve Current View Matrix.
	*/
	/**************************************************************************/
	const glm::mat4& GetViewMatrix();
	/**************************************************************************/
	/*!
	  \brief
		Retrieve Current Projection Matrix.
	*/
	/**************************************************************************/
	const glm::mat4& GetProjectionMatrix();
	/**************************************************************************/
	/*!
	  \brief
		Retrieve Current View Projection Matrix.
	*/
	/**************************************************************************/
	const glm::mat4& GetViewProjectionMatrix();

	/**************************************************************************/
	/*!
	  \brief
		Update View Projection Matrix.
	*/
	/**************************************************************************/
	void UpdateViewMatrix();
private:
	glm::vec3 cam_pos = {0.f, 0.f, 0.f};
	float cam_rot = 0.f;
	float cam_spd = 0.f;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewprojection;
};


