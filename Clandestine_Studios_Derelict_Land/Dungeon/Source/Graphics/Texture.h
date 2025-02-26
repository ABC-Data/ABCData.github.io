/******************************************************************************/
/*!
\file    Texture.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    November 2, 2022
\brief   Loading textures through STBI library

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Texture {
public:
	static void Init();
	/**************************************************************************/
	/*!
	  \brief
		Loads a .png/.jpeg file and returns a GPU binded texure id.

	  \param pathname
		Filename of .png/.jpeg file.

	  \param isRepeated
		Texture isRepeated flag.
	*/
	/**************************************************************************/
	static GLuint STBI_Load_Texture(char const* pathname, bool isRepeated = false);
	/**************************************************************************/
	/*!
	  \brief
		Loads a .png/.jpeg file and returns a GPU binded texure id.

	  \param pathname
		Filename of .png/.jpeg file.

	  \param width
		return width by reference.

	  \param height
		return height by reference.

	  \param isRepeated
		Texture isRepeated flag.
	*/
	/**************************************************************************/
	static GLuint STBI_Load_Texture_With_Size(char const* pathname, int& width, int& height, bool isRepeated = false);
};