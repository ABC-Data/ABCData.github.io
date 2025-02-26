/******************************************************************************/
/*!
\file    AssetsManager.cpp
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    November 2, 2022
\brief   A class to hold all textures, fonts, shaders to be used in graphics.
		(Audio files to be included in M3)

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <Graphics/AssetManager.h>

Assets::Assets() {Texture::Init();}

Assets::~Assets() {}

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
void Assets::AddTexture(std::string name, char const* filename, bool isRepeated) {
	int width{}, height{};
	GLuint texid = Texture::STBI_Load_Texture_With_Size(filename, width, height, isRepeated);

	Sprite s = Sprite(texid, std::vector<glm::vec2>(0), { width, height }, 1, 1);
	auto tex = std::make_unique<Sprite>(s);
	texture[name] = std::move(tex);
}
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
void Assets::AddSpriteAnimation(std::string name, char const* filename, int frames_per_row, int frames_per_column, bool isRepeated) {
	int width{}, height{};
	GLuint texid = Texture::STBI_Load_Texture_With_Size(filename, width, height, isRepeated);

	int width_of_frame = width/frames_per_row, height_of_frame = height/frames_per_column;

	float width_in_texcoords = static_cast<float>( width_of_frame) / static_cast<float>(width), height_in_texcoords = static_cast<float>(height_of_frame) / static_cast<float>(height);
	float TexX{}, TexY{};
	std::vector<glm::vec2> TexCoords;
	glm::vec2 TexSize{ width_in_texcoords , height_in_texcoords };

	for (int i = 0; i < frames_per_column; ++i)
	{
		for (int j = 0; j < frames_per_row; ++j)
		{
			TexCoords.push_back(glm::vec2(TexX, TexY));
			TexX += width_in_texcoords;
		}
		TexX = 0.f;
		TexY += height_in_texcoords;
	}
	
	auto spritedata = std::make_shared<Sprite>(Sprite(texid, TexCoords, TexSize, frames_per_row, frames_per_column, 0));
	sprite[name] = std::move(spritedata);
}

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
void Assets::AddShader(std::string name, std::string vertex_shader, std::string fragment_shader) {
	GLSLShader shader_program;
	shader_program.CompileShaderFromString(GL_VERTEX_SHADER, vertex_shader);
	shader_program.CompileShaderFromString(GL_FRAGMENT_SHADER, fragment_shader);
	shader_program.Link();
	shader_program.Validate();

	if (GL_FALSE == shader_program.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shader_program.GetLog() << "\n";
		assert(shader_program.IsLinked());
		std::exit(EXIT_FAILURE);
	}

	auto shd = std::make_unique<GLSLShader>(shader_program);

	shader[name] = std::move(shd);
}
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
void Assets::AddShaderFile(std::string name, char const* vertex_shader_filename, char const* fragment_shader_filename) {
	std::vector<std::pair<GLenum, std::string>> shader_file;
	shader_file.emplace_back(std::make_pair(GL_VERTEX_SHADER, vertex_shader_filename));
	shader_file.emplace_back(std::make_pair(GL_FRAGMENT_SHADER, fragment_shader_filename));

	GLSLShader shader_program;
	shader_program.CompileLinkValidate(shader_file);
	if (GL_FALSE == shader_program.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shader_program.GetLog() << "\n";
		assert(shader_program.IsLinked());
		std::exit(EXIT_FAILURE);
	}

	auto shd = std::make_unique<GLSLShader>(shader_program);

	shader[name] = std::move(shd);
}

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
void Assets::AddFont(std::string font_name, std::string ttf_filepath)
{
	std::map<char, Character> Characters_MAP;

	FT_Library FT{};

	// loading the Freetype Helper Variables to help load the fonts
	if (FT_Init_FreeType(&FT))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		assert(FT != nullptr);
		exit(EXIT_FAILURE);
	}

	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		assert(!font_name.empty());
		exit(EXIT_FAILURE);
	}


	FT_Face face{};
	if (FT_New_Face(FT, ttf_filepath.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		assert(face != nullptr);
		exit(EXIT_FAILURE);//return 0;

	}
	else
	{
#ifdef TESTING_OF_FONTSRENDER_RENDER
		std::cout << "[Assets::AddFont] Freetype 2.12.1 " << font_name << " successfully loaded from " << ttf_filepath << std::endl;
#endif

		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);
		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction		

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph for " << c << std::endl;
				continue;
			}
			// generate texture
			GLuint tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character =
			{
				tex,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters_MAP.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(0, 0);



#ifdef TESTING_OF_FONTSRENDER_RENDER
		char test = 'T';
		std::cout << "[Assets::AddFonts] Map::adv " << Characters_MAP[test].Advance << std::endl;
		std::cout << "[Assets::AddFonts] Map::bearing.x " << Characters_MAP[test].Bearing.x << std::endl;
		std::cout << "[Assets::AddFonts] Map::bearing.y " << Characters_MAP[test].Bearing.y << std::endl;
		std::cout << "[Assets::AddFonts] Map::size.x " << Characters_MAP[test].Size.x << std::endl;
		std::cout << "[Assets::AddFonts] Map::size.y " << Characters_MAP[test].Size.y << std::endl;
		std::cout << "[Assets::AddFonts] Map::textureID " << Characters_MAP[test].TextureID << std::endl;
#endif
	}

	// finish using the Face and FT from Freetype Library
	FT_Done_Face(face);
	FT_Done_FreeType(FT);

	auto char_map = std::make_unique<std::map<char, Character>>(Characters_MAP);
	fonts_map[font_name] = std::move(char_map);

}
/**************************************************************************/
	/*!
	  \brief
		Adds audio loaded by the audio system.

	  \param sound
		sound file.
	*/
	/**************************************************************************/
void Assets::AddAudio(FMOD::Sound* sound) {
	auto aud_ptr = std::make_unique<FMOD::Sound*>(sound);
	audio.push_back(std::move(aud_ptr));
}

void Assets::RemoveTexture(std::string name)
{
	if (GetTexture(name).texid != 0) { texture.erase(name); }
	else { std::cout << "[Assets::RemoveTexture] Texture of ID " << name << " cannot be found." << std::endl; }
}

void Assets::RemoveAudio(std::string name)
{
	int i = 0;
	if (GetAudio(name, &i) != nullptr) { audio.erase(audio.begin() + i); }
	else { std::cout << "[Assets::RemoveAudio] Audio of ID " << name << " cannot be found.\n"; }
}

/**************************************************************************/
/*!
  \brief
	Retrieve Texture id with corresponding string ID.

  \param name
	String ID of std::map.
*/
/**************************************************************************/
Assets::Sprite Assets::GetTexture(std::string name) {
	if (texture.find(name) == texture.end()) {
		Sprite nothing{ 0,std::vector<glm::vec2>(), {0,0},0,0 };
		return nothing;
	}		

	return *(texture.at(name).get());
}

/**************************************************************************/
/*!
  \brief
	Retrieves the texture map of the Assets class.
  \return
	Reference to the texture asset map.
*/
/**************************************************************************/
std::map<std::string, std::unique_ptr<Assets::Sprite>>& Assets::GetTextureMap() { return texture; }

/**************************************************************************/
/*!
  \brief
	Retrieve Compiled Shader program with corresponding string ID.

  \param name
	String ID of std::map.
*/
/**************************************************************************/
GLSLShader& Assets::GetShader(std::string name) {
	return *(shader.at(name).get());
}

/**************************************************************************/
	/*!
	  \brief
		returns the inner map from the map of fonts

	  \param font_name
		the key to find the specific font set in the map

	*/
	/**************************************************************************/
const std::map<char, Character>& Assets::GetFont(std::string font_name) const
{
	return *(fonts_map.at(font_name).get());
}
/**************************************************************************/
/*!
  \brief
	returns the sprites used for animation

  \param name
	the key to find the specific sprite set in the map

*/
/**************************************************************************/
Assets::Sprite& Assets::GetSprite(std::string name) {
	return *(sprite.at(name).get());
}
/**************************************************************************/
/*!
  \brief
	returns the container of the sprites used for animation
*/
/**************************************************************************/
std::map<std::string, std::shared_ptr<Assets::Sprite>> Assets::GetSpriteContainer() {
	return sprite;
}
/**************************************************************************/
/*!
  \brief
	Retrieve audio previously loaded .

  \param audio_index
	sound file.
*/
/**************************************************************************/
FMOD::Sound* Assets::GetAudio(int audio_index) {
	return *(audio.at(audio_index).get());
}
/**************************************************************************/
/*!
  \brief
	Retrieves audio previously loaded. The audio to get is nominally checked
	against its file name for .wav files. However, in its current iteration,
	(it seems), if the audio is not a .wav file, it may not use only the filename.
	
	E.g.
	In .mp3 files, the check is done using the TITLE of the audio file. 
	(RMB on file->Properties->Details->Title) against the filename. 
	Care must be taken to ensure that the title and filename of the file match.
  \param audio_name
	name of the audio file desired
  \param audio_index
	pointer to keep track of the index of the audio file
  \return
	the audio file if found, else nullptr
*/
/**************************************************************************/
FMOD::Sound* Assets::GetAudio(std::string_view audio_name, int* audio_index)
{
	char name[256]; *audio_index = 0;
	for (auto& upsfx : audio)
	{
		(*upsfx.get())->getName(name, sizeof(name));
		if (std::strcmp(audio_name.data(),name) == 0) 
		{ 
			*name = '\0';
			return *upsfx;
		}
		++(*audio_index);
	}
	return nullptr;
}
std::vector<std::unique_ptr<FMOD::Sound*>>& Assets::GetAudioMap() { return audio; }

/**************************************************************************/
/*!
  \brief
	Check to see if Assets have been loaded for use.
*/
/**************************************************************************/
bool Assets::IsInitialized() {
	if (!texture.size() || !shader.size() || !fonts_map.size() || !sprite.size())
		return false;
	return true;
}

