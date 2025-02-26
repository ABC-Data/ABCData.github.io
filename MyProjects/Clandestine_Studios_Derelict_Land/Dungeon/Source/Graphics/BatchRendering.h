/******************************************************************************/
/*!
\file    BatchRendering.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    November 2, 2022
\brief   Rendering of Colored/Textured quads and Colored lines in batches.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#ifndef BATCHRENDERING_H
#define BATCHRENDERING_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Graphics/glslshader.h"
#include <string>
#include <Graphics/GLFWsetup.h>
#include <Graphics/Camera.h>
#include <Lighting/Lighting.h>

class Batch {
public:
	/**************************************************************************/
	/*!
	  \brief
		Initialize all things required to run BatchRendering.
	*/
	/**************************************************************************/
	static void Init();
	/**************************************************************************/
	/*!
	  \brief
		Free-ing of memory used in BatchRendering
	*/
	/**************************************************************************/
	static void Cleanup();

	/**************************************************************************/
	/*!
	  \brief
		Setting all pointers to start of the batch.
	*/
	/**************************************************************************/
	static void StartBatch();
	/**************************************************************************/
	/*!
	  \brief
		Binding all required buffers to send to GPU for drawing.
	*/
	/**************************************************************************/
	static void EndBatch();
	/**************************************************************************/
	/*!
	  \brief
		Draw all current Quads and/or Lines called using draw calls.
	*/
	/**************************************************************************/
	static void Flush();

	/**************************************************************************/
	/*!
	  \brief
		Draw a Colored Quad.

	  \param pos
		Position(x,y,z) in screen space of quad to draw.

	  \param size
		Size(x,y) of quad to draw.

	  \param scale
		Scale(x,y) of quad to draw.

	  \param clr
		Color(r,g,b,a) of quad to draw.

	  \param angle_in_degree
		rotation of angle in degree of texture.
	*/
	/**************************************************************************/
	static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, const glm::vec4& clr, const float& angle_in_degree = 0);
	/**************************************************************************/
	/*!
	  \brief
		Draw a Textured Quad.

	  \param pos
		Position(x,y,z) in screen space of quad to draw.

	  \param size
		Size(x,y) of quad to draw.

	  \param scale
		Scale(x,y) of quad to draw.

	  \param texid
		Texture of quad to draw.

	  \param angle_in_degree
		rotation of angle in degree of texture.
	*/
	/**************************************************************************/
	static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, GLuint texid, const float& angle_in_degree = 0);
	/**************************************************************************/
	/*!
	  \brief
		Draw a Textured Quad with NDC Coords.

	  \param pos
		Position(x,y,z) in screen space of quad to draw.

	  \param size
		Size(x,y) of quad to draw.

	  \param scale
		Scale(x,y) of quad to draw.

	  \param texid
		Texture of quad to draw.

	  \param uv_start
		UV min of tecture.

	  \param uv_end
		UV max of tecture.

	  \param angle_in_degree
		rotation of angle in degree of texture.
	*/
	/**************************************************************************/
	static void DrawQuadWithUV(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, GLuint texid, const glm::vec2& uv_start, const glm::vec2& uv_end, const float& angle_in_degree = 0);
	/**************************************************************************/
	/*!
	  \brief
		Draw a Textured Quad with Pixel Coords.

	  \param pos
		Position(x,y,z) in screen space of quad to draw.

	  \param size
		Size(x,y) of quad to draw.

	  \param scale
		Scale(x,y) of quad to draw.

	  \param texid
		Texture of quad to draw.

	  \param uv_start
		UV min of tecture.

	  \param uv_end
		UV max of tecture.

	  \param texsize
		Size of Texture.

	  \param clr
		Tint color of tecture.

	  \param angle_in_degree
		rotation of angle in degree of texture.
	*/
	/**************************************************************************/
	static void DrawQuadWithPixelCoords(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, GLuint texid, const glm::vec2& uv_start, const glm::vec2& uv_end,
		const glm::vec2 texsize, const glm::vec4 clr = {1,1,1,1}, const float& angle_in_degree = 0);
	/**************************************************************************/
	/*!
	  \brief
		Draw a Colored Line.

	  \param p0
		Start position(x,y,z) of line to draw.

	  \param p1
		End position(x,y,z) of line to draw.

	  \param clr
		Color(r,g,b,a) of line to draw.
	*/
	/**************************************************************************/
	static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& clr);
	/**************************************************************************/
	/*!
	  \brief
		Draw a Colored Line Box.

	  \param pos
		Position(x,y,z) of box to draw.

	  \param size
		Size(x,y) of box to draw.

	  \param scale
		Scale(x,y) of box to draw.

	  \param clr
		Color(r,g,b,a) of box to draw.
	*/
	/**************************************************************************/
	static void DrawBox(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, const glm::vec4& clr);
	/**************************************************************************/
	/*!
	  \brief
		Draw a Textured Quad with Pixel Coords.

	  \param pos
		Position(x,y,z) in screen space of quad to draw.

	  \param size
		Size(x,y) of quad to draw.

	  \param scale
		Scale(x,y) of quad to draw.

	  \param texid
		Texture of quad to draw.

	  \param uv_start
		UV min of tecture.

	  \param uv_end
		UV max of tecture.

	  \param angle_in_degree
		rotation of angle in degree of texture.
	*/
	/**************************************************************************/
	static void DrawSprite(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, GLuint texid, const glm::vec2& texcoords, glm::vec2& texsize, const glm::vec4 clr = {1.f, 1.f, 1.f, 1.f}, const float& angle_in_degree = 0);
	/**************************************************************************/
	/*!
	  \brief
		Set the width of line drawn.

	  \param width
		Width of the line.
	*/
	/**************************************************************************/
	static void SetLineWidth(float width);
	/**************************************************************************/
	/*!
	  \brief
		Set lights position, color and intensity.

	  \param pointlights
		Lights to set.
	*/
	/**************************************************************************/
	static void SetLight(std::vector<Lighting::PointLight> pointlights);

	//Variable
	static GLSLShader quad_shader_program;
	static GLSLShader line_shader_program;
	static Camera camera;
};

#endif
