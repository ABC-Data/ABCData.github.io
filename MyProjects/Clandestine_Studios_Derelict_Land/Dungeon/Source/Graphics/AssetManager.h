/******************************************************************************/
/*!
\file    AssetsManager.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    November 2, 2022
\brief   A class to hold all textures, fonts, shaders to be used in graphics, as
		 well as audio files to be used in-game.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include <map>
#include <string>
#include <sstream>

#include <Graphics/Texture.h>
#include <Graphics/glslshader.h>
#include <Graphics/Fonts.h>
#include "fmod.hpp"

class Assets {
public:
	struct Sprite
	{
		GLuint texid;
		std::vector<glm::vec2> texcoords;
		glm::vec2 texsize;
		int frames_per_row, frames_per_column, idx;
	};

	Assets();
	~Assets();

	/**************************************************************************/
	/*!
	  \brief
		Adds a texture with a string ID to be used for graphics.

	  \param name
		String ID of std::map.

	  \param filename
		.png/.jpeg file to be added to assets.

	  \param isRepeated
		Bool check to see if user wants texture to be repeated, default = false.
	*/
	/**************************************************************************/
	void AddTexture(std::string name, char const* filename, bool isRepeated = false);
	/**************************************************************************/
	/*!
	  \brief
		Adds a Spritesheet to be used for animation.

	  \param name
		String ID of std::map.

	  \param filename
		.png/.jpeg file to be added to assets.

	  \param frames_per_row
		Frames per row in the spritesheet.

	  \param frames_per_column
		Frames per column in the spritesheet.

	  \param isRepeated
		Bool check to see if user wants texture to be repeated, default = false.
	*/
	/**************************************************************************/
	void AddSpriteAnimation(std::string name, char const* filename, int frames_per_row, int frames_per_column = 1, bool isRepeated = false);
	/**************************************************************************/
	/*!
	  \brief
		Adds a shader through a string to be used in graphics.

	  \param name
		String ID of std::map.

	  \param vertex_shader
		Vertex shader to be compiled.

	  \param fragment_shader
		Fragment shader to be compiled.
	*/
	/**************************************************************************/
	void AddShader(std::string name, std::string vertex_shader, std::string fragment_shader);
	/**************************************************************************/
	/*!
	  \brief
		Adds a shader through a .vert/.frag file to be used in graphics.

	  \param name
		String ID of std::map.

	  \param vertex_shader_filename
		Vertex shader to be compiled.

	  \param fragment_shader_filename
		Fragment shader to be compiled.
	*/
	/**************************************************************************/
	void AddShaderFile(std::string name, char const* vertex_shader_filename, char const* fragment_shader_filename);
	/**************************************************************************/
	/*!
	  \brief
		loads a font into fonts_map

	  \param font_name
		key for the fonts map

	  \param ttf_filepath
		the file path to the font, .ttf file
	*/
	/**************************************************************************/
	void AddFont(std::string font_name,std::string ttf_filepath);
	/**************************************************************************/
	/*!
	  \brief
		Adds audio loaded by the audio system.

	  \param sound
		sound file.
	*/
	/**************************************************************************/
	void AddAudio(FMOD::Sound* sound);

	/**************************************************************************/
	/*!
		\brief
			Removes a texture from the assets' texture map.
		\param name
			String ID of the texture to be removed.
	*/
	/**************************************************************************/
	void RemoveTexture(std::string name);

	/**************************************************************************/
	/*!
		\brief
			Removes an audio file from the assets' audio map.
		\param name
			String ID of the audio file to be removed.
	*/
	/**************************************************************************/
	void RemoveAudio(std::string name);
	
	/**************************************************************************/
	/*!
	  \brief
		Retrieve Texture id with corresponding string ID.

	  \param name
		String ID of std::map.
	*/
	/**************************************************************************/
	Sprite GetTexture(std::string name);

	/**************************************************************************/
	/*!
		\brief
			Returns the map of textures, spritehseets.
	  \return
			Ordered map of the assets' textures map.
	*/
	/**************************************************************************/
	std::map<std::string, std::unique_ptr<Sprite>>& GetTextureMap();
	
	/**************************************************************************/
	/*!
	  \brief
		Retrieve Compiled Shader program with corresponding string ID.

	  \param name
		String ID of std::map.
	*/
	/**************************************************************************/
	GLSLShader& GetShader(std::string name);
	/**************************************************************************/
	/*!
	  \brief
		returns the inner map from the map of fonts

	  \param font_name
		the key to find the specific font set in the map
		
	*/
	/**************************************************************************/
	const std::map<char, Character>& GetFont(std::string font_name) const;
	/**************************************************************************/
	/*!
	  \brief
		returns the sprites used for animation

	  \param name
		the key to find the specific sprite set in the map

	*/
	/**************************************************************************/
	Sprite& GetSprite(std::string name);
	/**************************************************************************/
	/*!
	  \brief
		returns the container of the sprites used for animation
	*/
	/**************************************************************************/
	std::map<std::string, std::shared_ptr<Sprite>> GetSpriteContainer();
	/**************************************************************************/
	/*!
	  \brief
		Retrieve audio previously loaded.
	  \param audio_name
		String ID of the audio file.
	  \param audio_index
		Index of the audio file inside the audio map.
	*/
	/**************************************************************************/
	FMOD::Sound* GetAudio(int audio_index);
	FMOD::Sound* GetAudio(std::string_view audio_name, int* audio_index);
	
	/**************************************************************************/
	/*!
		\brief
			Returns the vector of unique audio file pointers.
		\return
			The vector of unique pointers to FMOD Sound pointers.
	*/
	/**************************************************************************/
	std::vector<std::unique_ptr<FMOD::Sound*>>& GetAudioMap();
	
	/**************************************************************************/
	/*!
	  \brief
		Check to see if Assets have been loaded for use.
	*/
	/**************************************************************************/
	bool IsInitialized();

private:
	std::map<std::string, std::unique_ptr<Sprite>> texture;
	std::map<std::string, std::unique_ptr<GLSLShader>> shader;
	std::map <std::string, std::unique_ptr<std::map<char, Character>>> fonts_map;
	std::map<std::string, std::shared_ptr<Sprite>> sprite;
	std::vector<std::unique_ptr<FMOD::Sound*>> audio;
};