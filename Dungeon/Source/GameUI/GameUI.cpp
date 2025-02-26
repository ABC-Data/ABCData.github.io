/************************************************************************ /
/*!
\file    GameUI.cpp
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    November 1, 2022
\brief   Component for mangling of in game UI elements and rendering fonts
		 on these UI elements

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"

//extern MouseInput MyMouse;

/**************************************************************************/
	/*!
	\brief
		Default Constructor of GameUI
	*/
	/**************************************************************************/
GameUI::GameUI() :
	AA{ 0.f,0.f },
	BB{ 0.f, 0.f },
	FontFamily{ "" },
	Text{ "" },
	size{ 0.f },
	x_offset{ 0.f },
	y_offset{ 0.f },
	FontColor{ 0.f,0.f,0.f },
	Alpha{ 0.f },
	Visibility{false},
	depth{0.f},
	Line_spacing{0},
	func(nullptr)
{
	SetType(ComponentType::GameUI);
}

/**************************************************************************/
/*!
	\brief
		SetTextureHandle() setter function to change the Font Family String
	\param handle
		The string of the Font Family String
*/
/**************************************************************************/

void GameUI::SetTextFontFamily(std::string FontFam)
{
	this->FontFamily = FontFam;
}

/**************************************************************************/
/*!
	\brief
		GetTextFontFamily() getter function to get the Font Family String
*/
/**************************************************************************/

std::string GameUI::GetTextFontFamily()
{
	return this->FontFamily;
}

/**************************************************************************/
/*!
	\brief
		SetTextureFilepath() getter function to get the texture filepath
*/
/**************************************************************************/

void GameUI::SetText(std::string text_param)
{
	this->Text = text_param;
}

/**************************************************************************/
/*!
	\brief
		GetTextureFilepath() getter function to get the texture filepath
*/
/**************************************************************************/

std::string GameUI::GetText()
{
	return this->Text;
}
/**************************************************************************/
	/*!
	\brief
		This function set all the variables in the GameUI component
	*/
/**************************************************************************/	
void GameUI::SetGameUI(std::string font_fam, std::string text_param,
	float size_param, float x_off_param, float y_off_param,
	Vec3 color_param, float alpha_param, bool vis_param, float depth_param, int line_spacing_param)
{
	this->FontFamily = font_fam;
	this->Text = text_param;
	this->size = size_param;
	this->x_offset = x_off_param;
	this->y_offset = y_off_param;
	this->FontColor = color_param;
	this->Alpha = alpha_param;
	this->Visibility = vis_param;
	this->depth = depth_param;
	this->Line_spacing = line_spacing_param;
}

/**************************************************************************/
/*!
	\brief
		Serialize() virtual override function to serialise component to the
		txt file
*/
/**************************************************************************/
void GameUI::Serialize(FILE* fp)
{
	fprintf(fp, "GameUI\n");
	fprintf(fp, "FontFamily: %s\n", FontFamily.c_str());
	fprintf(fp, "Text: %s\n", Text.c_str());
	fprintf(fp, "Font Size: %f\n", size);
	fprintf(fp, "x_offset: %f\n", x_offset);
	fprintf(fp, "y_offset: %f\n", y_offset);
	fprintf(fp, "FontColor: %f, %f, %f\n", FontColor.x, FontColor.y, FontColor.z);
	fprintf(fp, "Alpha: %f\n", Alpha);
	fprintf(fp, "Visibility: %i\n", Visibility);
	fprintf(fp, "Depth: %f\n", depth);
	fprintf(fp, "Line Spacing: %d\n", Line_spacing);
}
/**************************************************************************/
/*!
	\brief
		Deserialize() virtual override function to deserialize componenet
		from the txt file
*/
/**************************************************************************/
void GameUI::Deserialize(FILE* fp)
{
	char FF_str[100]{ 0 };
	char Text_str[200]{ 0 };
	float temp_size, temp_x_off, temp_y_off,
		temp_r, temp_g, temp_b, temp_alpha, temp_depth;
	int temp_vis, temp_linespacing;

	int success = 1;

	//font family string
	int discard = fscanf(fp, "FontFamily: ");
	success = fscanf(fp, "%[^,\n]\n", &FF_str);

	//Text string
	discard = fscanf(fp, "Text: ");
	success *= fscanf(fp, "%[^,\n]\n", &Text_str);

	//size float
	success *= fscanf(fp, "Font Size: %f\n", &temp_size);

	//x_offset float
	success *= fscanf(fp, "x_offset: %f\n", &temp_x_off);

	//y_offset float
	success *= fscanf(fp, "y_offset: %f\n", &temp_y_off);

	//Font Color Vec3
	success *= fscanf(fp, "FontColor: %f, %f, %f\n", &temp_r, &temp_g, &temp_b);

	//Alpha float
	success *= fscanf(fp, "Alpha: %f\n", &temp_alpha);

	//Visibility bool
	success *= fscanf(fp, "Visibility: %i\n", &temp_vis);

	//Depth float
	success *= fscanf(fp, "Depth: %f\n", &temp_depth);

	//Depth float
	success *= fscanf(fp, "Line Spacing: %i\n", &temp_linespacing);

	if (!success)
	{
		std::cout << "FAILED TO LOAD GAMEUI OBJ PROPERLY" << std::endl;
		SetGameUI("", "",
			1.f, 0.f, 0.f,
			{ 0.f,0.f,0.f }, 1.f, false,0.0f, 0);
		return;
	}
	else
	{
		SetGameUI(FF_str, Text_str,
			temp_size, temp_x_off, temp_y_off,
			{ temp_r,temp_g,temp_b }, temp_alpha, (bool)temp_vis, temp_depth, temp_linespacing);
	}


	discard = fscanf(fp, "\n");
	(void)discard;

}
/**************************************************************************/
/*!
	\brief
		CopyData() virtual override function to copy data to another
		component
*/
/**************************************************************************/
void GameUI::CopyData(Component* Target)
{
	GameUI* G = dynamic_cast<GameUI*>(Target);

	if (G != nullptr) {
		this->AA = G->AA;
		this->BB = G->BB;
		//this->TextureHandle = G->TextureHandle;
		//this->TextureFilepath = G->TextureFilepath;
		//this->is_active = G->is_active;
		this->func = G->func;
	}
}
/**************************************************************************/
/*!
	\brief
		SetAABB() setter function to set the AABB bounding box of the
		GameUI. So that the AABB changes every update loop if applied
		Translation and Scaling
	\param PositionInst
		The position Vec2 of the GO
	\param ScaleInst
		The scale Vec2 of the GO
	\param DimensionInst
		The dimension Vec2 of the GO
*/
/**************************************************************************/
void GameUI::SetAABB(Vec2 PositionInst, Vec2 ScaleInst, Vec2 DimensionInst)
{
	AA.x = -0.5f * DimensionInst.x * (ScaleInst.x / 100.f) + PositionInst.x;
	AA.y = -0.5f * DimensionInst.y * (ScaleInst.y / 100.f) + PositionInst.y;
	BB.x = 0.5f * DimensionInst.x * (ScaleInst.x / 100.f) + PositionInst.x;
	BB.y = 0.5f * DimensionInst.y * (ScaleInst.y / 100.f) + PositionInst.y;
}
/**************************************************************************/
	/*!
		\brief
			This function sets the size of the font that is going to be
			rendered
		\param size_param
			This is the value that is going to be size of the fonts
			rendered
	*/
/**************************************************************************/
void GameUI::SetSize(float size_param)
{
	this->size = size_param;
}
/**************************************************************************/
	/*!
		\brief
			This function gets the size of the font rendered
		\return
			The size of the font.
	*/
/**************************************************************************/
float GameUI::GetSize()
{
	return this->size;
}
/**************************************************************************/
	/*!
		\brief
			This function sets the color of the font, typically via the
			ImGUI colour picker
		\param r_param
			the scale of 0 to 1 for the red value
		\param g_param
			the scale of 0 to 1 for the green value
		\param b_param
			the scale of 0 to 1 for the blue value
	*/
/**************************************************************************/

void GameUI::SetFontColor(float r_param, float g_param, float b_param)
{
	this->FontColor.x = r_param;
	this->FontColor.y = g_param;
	this->FontColor.z = b_param;
}
/**************************************************************************/
	/*!
		\brief
			This function gets the Vec3 colour of the font.
		\return
			The value in the RGB data member
	*/
/**************************************************************************/

Vec3 GameUI::GetFontColor_vec3()
{
	return this->FontColor;
}
/**************************************************************************/
	/*!
		\brief
			This function return true or false if a GameUI obj has been
			hovered over.
	*/
/**************************************************************************/

bool GameUI::MouseHoverClick(int MouseX, int MouseY) {
	if (MouseX >= AA.x && MouseY >= AA.y && MouseX <= BB.x && MouseY <= BB.y) {
		return true;
	}
	else {
		return false;
	}
}

/**************************************************************************/
		/*!
			\brief
				This function sets the visibility of the font in the visibility
		*/
		/**************************************************************************/
void GameUI::SetFontVisibility(bool vis_param)
{
	this->Visibility = vis_param;
}
/**************************************************************************/
	/*!
		\brief
			This function return true or false if the GameUI's font are
			visible or not
	*/
	/**************************************************************************/
bool GameUI::GetFontVisibility()
{
	return Visibility;
}
/**************************************************************************/
	/*!
		\brief
			This function sets the depth of the font rendered
	*/
	/**************************************************************************/
void GameUI::SetFontDepth(bool Depth_param)
{
	this->depth = Depth_param;
}
/**************************************************************************/
	/*!
		\brief
			This function returns the depth value of the font on the GameUI
			object
	*/
	/**************************************************************************/
float GameUI::GetFontDepth()
{
	return this->depth;
}

/**************************************************************************/
		/*!
			\brief
				This function sets the line spacing of the text font rendered
		*/
		/**************************************************************************/
void GameUI::SetLineSpacing(int line_spacing)
{
	this->Line_spacing = line_spacing;
}
/**************************************************************************/
	/*!
		\brief
			This function gets the line spacing value text font rendered
	*/
	/**************************************************************************/
int GameUI::GetLineSpacing()
{
	return this->Line_spacing;
}