/************************************************************************ /
/*!
\file    Fonts.cpp
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    September 30, 2022
\brief   Fonts Rendering Definitions

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "Fonts.h"


//Variables
GLint FontsRender::WindowWidth;
GLint FontsRender::WindowHeight;
unsigned int FontsRender::VAO;
unsigned int FontsRender::VBO;
GLSLShader FontsRender::text_shdrpgm;


/**************************************************************************/
/*!
	\brief
	Initializes Values used by this class for ortho projection
	\param
	Windowwidth, the width of the window
	\param
	Windowheight, the height of the window
*/
/**************************************************************************/
void FontsRender::Init(GLint windowWidth, GLint windowHeight)
{
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;

#ifdef TESTING_OF_FONTSRENDER_RENDER
	std::cout << "[FontsRender::Init] Initialized with Window Width,Height: " << WindowWidth << "," << WindowHeight<< std::endl;
	std::cout << "[FontsRender::Init] Values are used for ortho projection of Fonts" << std::endl;
#endif

}

/**************************************************************************/
/*!
	\brief
	This function compilers the shader program , for rendering fonts
	specifically.
*/
/**************************************************************************/
void FontsRender::ShaderProgram_Setup()
{
	std::string FR_vs_source =
		"#version 450 core                             \n"
		"                                              \n"
		"uniform mat4 Projection_Mat4;                      \n"
		"                                              \n"
		"layout (location=0) in vec4 vertex;		   \n"
		"out vec2 TexCoords;					           \n"
		"											   \n"
		"                                              \n"
		"void main()                               \n"
		"{                                             \n"
		"	gl_Position = Projection_Mat4 * vec4(vertex.x, vertex.y , 0.999999, 1.0);   \n"
		"	TexCoords = vec2(vertex.z,vertex.w);						\n"
		"}                                             \n";


	std::string FR_fs_source =
		"#version 450 core                              \n"
		"                                               \n"
		"in vec2 TexCoords;								\n"
		"out vec4 color;							    \n"
		"                                               \n"
		"uniform sampler2D text;						\n"
		"uniform vec4 textRGBA;							\n"
		"                                               \n"
		"void main()									\n"
		"{                                             \n"
		"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);   \n"
		"	color = vec4(textRGBA) * sampled;						\n"

		"}                                             \n";

	text_shdrpgm.CompileShaderFromString(GL_VERTEX_SHADER, FR_vs_source);
	text_shdrpgm.CompileShaderFromString(GL_FRAGMENT_SHADER, FR_fs_source);
	text_shdrpgm.Link();
	

	if (GL_FALSE == text_shdrpgm.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << text_shdrpgm.GetLog() << "\n";
		assert(text_shdrpgm.IsLinked());
		std::exit(EXIT_FAILURE);
	}
	

#ifdef TESTING_OF_FONTSRENDER_RENDER
	std::cout << "[FontsRender::ShaderProgram_Setup] " << text_shdrpgm.GetHandle() << std::endl;
#endif
}

/**************************************************************************/
/*!
	\brief
	This function initialise the shaderprogram's VAO and VBOs
*/
/**************************************************************************/

void FontsRender::ShaderProgram_Init()
{

	GLuint Text_VAO, Text_VBO;
	glGenVertexArrays(1, &Text_VAO);
	glGenBuffers(1, &Text_VBO);
	glBindVertexArray(Text_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Text_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	VAO = Text_VAO;
	VBO = Text_VBO;
	
#ifdef TESTING_OF_FONTSRENDER_RENDER
	std::cout << "[FontsRender::ShaderProgram_Init] SHDRPGM INIT" << std::endl;
	std::cout << "[FontsRender::ShaderProgram_Init] VAO: " << FontsRender::VAO << std::endl;
	std::cout << "[FontsRender::ShaderProgram_Init] VBO: " << FontsRender::VBO << std::endl;
#endif
}

/**************************************************************************/
	/*!
	  \brief
		This function initialise the shaderprogram's VAO and VBOs
	  \param font
		This is a map of the fonts loaded, and its texture gluint
	  \param text
		This is a string of text to render
	  \param x
		x position of text rendered on screen
	  \param y
		y position of text rendered on screen
	  \param scale
		scale factor of the text to be rendered
	  \param TextColor
		the color of text rendered

*/
/**************************************************************************/
void FontsRender::RenderText(std::map<char, Character> font, std::string text, float x, float y, float z_depth, float scale, glm::vec3 TextColor,float TextAlpha)
{
#ifdef TESTING_OF_FONTSRENDER_RENDER // found in Fonts.h
	std::cout << "[FontsRender::Render] WindowWidth " << WindowWidth << std::endl;
	std::cout << "[FontsRender::Render] WindowHeight " << WindowHeight << std::endl;
	std::cout << "[FontsRender::Render] VAO: " << FontsRender::VAO << std::endl;
	std::cout << "[FontsRender::Render] VBO: " << FontsRender::VBO << std::endl;
	std::cout << "[FontsRender::Render] text_shdrpgm handle " << text_shdrpgm.GetHandle() << std::endl;
	std::cout << "[FontsRender::Render] Rendering of Text Called" << std::endl;
	std::cout << "[FontsRender::Render] Param Font Map size " << font.size() << std::endl;
	std::cout << "[FontsRender::Render] Param Text" << text << std::endl;
	std::cout << "[FontsRender::Render] Param x" << x << std::endl;
	std::cout << "[FontsRender::Render] Param y" << y << std::endl;
	std::cout << "[FontsRender::Render] Param scale" << scale << std::endl;
	
	char test = 'T';
	std::cout << "[FontsRender::Render] Map::adv " << font[test].Advance << std::endl;
	std::cout << "[FontsRender::Render] Map::bearing.x " << font[test].Bearing.x << std::endl;
	std::cout << "[FontsRender::Render] Map::bearing.y " << font[test].Bearing.y << std::endl;
	std::cout << "[FontsRender::Render] Map::size.x " << font[test].Size.x << std::endl;
	std::cout << "[FontsRender::Render] Map::size.y " << font[test].Size.y << std::endl;
	std::cout << "[FontsRender::Render] Map::textureID " << font[test].TextureID << std::endl;
	

#endif

	std::string new_line = "\n";

	if (text.find(new_line) != std::string::npos) // no new line chars found
	{
		//std::cout << " new line chars found\n";
		std::cout << text;
	}


	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)WindowWidth, 0.0f, (GLfloat)WindowHeight);


	// activate corresponding render state	
	text_shdrpgm.Use();

	// UNIFORM VARIABLES
	//text_shdrpgm.SetUniform("textAlpha", TextAlpha);
	//text_shdrpgm.SetUniform("textColor", TextColor);
	glm::vec4 RGBA = { TextColor.x,TextColor.y ,TextColor.z, TextAlpha };
	text_shdrpgm.SetUniform("textRGBA", RGBA);
	text_shdrpgm.SetUniform("Projection_Mat4", projection);
	text_shdrpgm.SetUniform("float_depth", z_depth);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(FontsRender::VAO);

	glm::vec4 test_vertice;
	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = font[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		if (c == text.begin())
		{
			test_vertice.x = vertices[1][0];
			test_vertice.y = vertices[1][1];
			test_vertice.z = vertices[1][2];
			test_vertice.w = vertices[1][3];
		}


		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, FontsRender::VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	text_shdrpgm.UnUse();

}

/**************************************************************************/
	/*!
	  \brief
		This function initialise the shaderprogram's VAO and VBOs, ideally 
		for the render of gameui font text that will contain 
		rendering of multiple lines
	  \param GO
		This param contains all the data required for rendering of the font,
		include, components used are the transfrom and gameui components.
*/
/**************************************************************************/
void FontsRender::RenderText(std::map<char, Character> font, GameObject* GO)
{
#ifdef TESTING_OF_FONTSRENDER_RENDER // found in Fonts.h
	std::cout << "[FontsRender::Render] WindowWidth " << WindowWidth << std::endl;
	std::cout << "[FontsRender::Render] WindowHeight " << WindowHeight << std::endl;
	std::cout << "[FontsRender::Render] VAO: " << FontsRender::VAO << std::endl;
	std::cout << "[FontsRender::Render] VBO: " << FontsRender::VBO << std::endl;
	std::cout << "[FontsRender::Render] text_shdrpgm handle " << text_shdrpgm.GetHandle() << std::endl;
	std::cout << "[FontsRender::Render] Rendering of Text Called" << std::endl;
	std::cout << "[FontsRender::Render] Param Font Map size " << font.size() << std::endl;
	std::cout << "[FontsRender::Render] Param Text" << text << std::endl;
	std::cout << "[FontsRender::Render] Param x" << x << std::endl;
	std::cout << "[FontsRender::Render] Param y" << y << std::endl;
	std::cout << "[FontsRender::Render] Param scale" << scale << std::endl;

	char test = 'T';
	std::cout << "[FontsRender::Render] Map::adv " << font[test].Advance << std::endl;
	std::cout << "[FontsRender::Render] Map::bearing.x " << font[test].Bearing.x << std::endl;
	std::cout << "[FontsRender::Render] Map::bearing.y " << font[test].Bearing.y << std::endl;
	std::cout << "[FontsRender::Render] Map::size.x " << font[test].Size.x << std::endl;
	std::cout << "[FontsRender::Render] Map::size.y " << font[test].Size.y << std::endl;
	std::cout << "[FontsRender::Render] Map::textureID " << font[test].TextureID << std::endl;


#endif

	//get the transform and the gameui components to get position and font data
	Transform* trf = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
	GameUI* gui = (dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI)));

	// vector to store the lines
	std::vector<std::string> lines;
	std::string temp = std::regex_replace(gui->GetText(), std::regex(R"(\\n)"), "\n");
	lines = splitString(temp, "\n");


	//create projection matrix
	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)WindowWidth, 0.0f, (GLfloat)WindowHeight);


	// activate corresponding render state	
	text_shdrpgm.Use();

	// UNIFORM VARIABLES
	//text_shdrpgm.SetUniform("textAlpha", TextAlpha);
	//text_shdrpgm.SetUniform("textColor", TextColor);
	glm::vec4 RGBA = { gui->GetFontColor_vec3().x, gui->GetFontColor_vec3().y,gui->GetFontColor_vec3().z,gui->Alpha };
	text_shdrpgm.SetUniform("textRGBA", RGBA);
	text_shdrpgm.SetUniform("Projection_Mat4", projection);
	text_shdrpgm.SetUniform("float_depth", gui->GetFontDepth());

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(FontsRender::VAO);
	

	for (int i = 0; i < lines.size(); i++)
	{

		Vec2 pos = { trf->Position.x + gui->x_offset ,
				trf->Position.y + gui->y_offset };

		pos.y -= static_cast<float>(i * gui->GetLineSpacing());


		// iterate through all characters
		std::string::const_iterator c;
		for (c = lines[i].begin(); c != lines[i].end(); c++)
		{
			Character ch = font[*c];

			float xpos = pos.x + ch.Bearing.x * gui->GetSize();
			float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * gui->GetSize();

			float w = ch.Size.x * gui->GetSize();
			float h = ch.Size.y * gui->GetSize();
			// update VBO for each character

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};


			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, FontsRender::VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			pos.x += (ch.Advance >> 6) * gui->GetSize(); // bitshift by 6 to get value in pixels (2^6 = 64)
		}

	}
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	text_shdrpgm.UnUse();
	

}

/**************************************************************************/
/*!
  \brief
	This function renders the debug info of the engine
  \param font
	This is a map of the fonts loaded, and its texture gluint
  \param text
	This is a string of text to render
  \param data
	Struct of fps, frame count and delta time
  \param x
	x position of text rendered on screen
  \param y
	y position of text rendered on screen
  \param scale
	scale factor of the text to be rendered
  \param TextColor
	the color of text rendered
*/
/**************************************************************************/
void FontsRender::RenderDEBUG_INFO(std::map<char, Character> font, DebugData data, float x, float y, float scale, glm::vec3 TextColor)
{
	

	std::stringstream debug_data_ss;
	debug_data_ss << " MouseCoord: " << data.Mouse_x_screencoord << "," << data.Mouse_y_screencoord 
		<< "Frame: " << data.Frame_cnt
		<< " GameObjects: " << data.GO_cnt
		<< " FPS: " << std::setw(8) << std::setprecision(5) << static_cast<double>(1 / data.dt)
		<< " Delta Time: " << data.dt;


	FontsRender::RenderText(font, debug_data_ss.str(), x, y, 0.9999f, scale, TextColor,1.f);
}

/**************************************************************************/
	/*!
	  \brief
		This function renders the total percentage time of a frame that each
		system took to finish its update function, within that frame
	  \param font
		This is a map of the fonts loaded, and its texture gluint
	  \param da
		Struct of fps, frame count and delta time
	  \param font
		This is a map of the fonts loaded, and its texture gluint
	  \param text
		This is a string of text to render
	  \param x
		x position of text rendered on screen
	  \param y
		y position of text rendered on screen
	  \param scale
		scale factor of the text to be rendered
	  \param TextColor
		the color of text rendered

*/
/**************************************************************************/
void FontsRender::RenderDEBUG_Performance_Data(std::map<char, Character> font, float y_increment, float x, float y, float scale, glm::vec3 TextColor)
{
	FontsRender::RenderText(font, "Performance Stats", x, y, 0.999f, scale*1.25f, TextColor,1.f);
	//float curr_y = y;
	for (auto const& sys : FontsRender::Sys_To_Percentage_MAP)
	{
		std::string system_name = sys.first;
		int system_percentage = static_cast<int>(sys.second);

		std::stringstream ss;
		ss << std::setw(15) << system_name
			<< ": "
			<< "~"
			<< std::setw(2) << system_percentage
			<< "%";
		//std::cout << ss.str() << std::endl;
		y -= y_increment;
		FontsRender::RenderText(font, ss.str(), x, y, 0.999f ,scale , TextColor,1.f);
	}
}

/**************************************************************************/
	/*!
	  \brief
		This function aids in the splitting a string,by its delimiter.
	  \param longtext_str
		This function
	  \param str
		Struct of fps, frame count and delta time
	  \return 
		Vector of strings that have been split by the new line character.
	  */
/**************************************************************************/
std::vector<std::string> FontsRender::splitString(const std::string longtext_str, std::string delimiter)
{
	
	std::vector<std::string> tokens;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = longtext_str.find(delimiter, prev)) != std::string::npos) {
		tokens.push_back(longtext_str.substr(prev, pos - prev));
		prev = pos + 1;
	}
	tokens.push_back(longtext_str.substr(prev));

	return tokens;
		
}