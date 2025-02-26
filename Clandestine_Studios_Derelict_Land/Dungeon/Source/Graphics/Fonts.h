/************************************************************************ /
/*!
\file    Fonts.h
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    October 30, 2022
\brief   Fonts Rendering Declarations

        Copyright (C) 2022 DigiPen Institute of Technology.
        Reproduction or disclosure of this file or its contents without the
        prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include <glm/glm.hpp>
#include <ft2build.h>
#include <map>
#include "Graphics/glslshader.h"
#include "Graphics/GLFWsetup.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject/GameObject.h"
#include "Transform/Transform.h"
#include "GameUI/GameUI.h"
#include <regex>

#include FT_FREETYPE_H

//#define TESTING_OF_FONTSRENDER_RENDER

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

struct DebugData
{
    int Frame_cnt;
    int GO_cnt;
    float dt;
    int Mouse_x_screencoord;
    int Mouse_y_screencoord;
};

class FontsRender
{
public:
    //Loading is handled by Assets Manager
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
    static void Init(GLint windowWidth, GLint windowHeight);

    /**************************************************************************/
    /*!
      \brief
        This function compilers the shader program , for rendering fonts 
        specifically.
    */
    /**************************************************************************/
    static void ShaderProgram_Setup();

    /**************************************************************************/
    /*!
      \brief
        This function initialise the shaderprogram's VAO and VBOs
    */
    /**************************************************************************/
    static void ShaderProgram_Init();

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
    static void RenderText(std::map<char, Character> font, std::string text,  float x, float y, float z_depth ,  float scale, glm::vec3 TextColor, float TextAlpha);


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
    static void RenderText(std::map<char, Character> font,GameObject* GO);

    /**************************************************************************/
    /*!
      \brief
        This function initialise the shaderprogram's VAO and VBOss
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
    static void RenderDEBUG_INFO(std::map<char, Character> font, DebugData data, float x, float y, float scale, glm::vec3 TextColor);
   
    /**************************************************************************/
    /*!
      \brief
        This function initialise the shaderprogram's VAO and VBOss
      \param font
        This is a map of the fonts loaded, and its texture gluint
      \param y_increment
        y coord translation when printing the perfomance info
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
    static void RenderDEBUG_Performance_Data(std::map<char, Character> font, float y_increment, float x, float y, float scale, glm::vec3 TextColor);


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
   static std::vector<std::string> splitString(const std::string longtext_str, std::string delimiter);


    //Variables
    static GLint WindowWidth;
    static GLint WindowHeight;
    static unsigned int VAO;
    static unsigned int VBO;
    static GLSLShader text_shdrpgm;

    static std::map<std::string, double> Sys_To_Percentage_MAP;
    
};