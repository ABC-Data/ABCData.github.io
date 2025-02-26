/************************************************************************ /
/*!
\file    GameUI.h
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
#pragma once
//#include "System/System.h"
#include "Essential.h"
#include "GameObject/Component.h"

class GameUI : public Component
{
public:
	Vec2 AA, BB;

	std::string FontFamily;
	std::string Text;
	float size;
	float x_offset;
	float y_offset;
	Vec3 FontColor;
	float Alpha;
	bool Visibility;
	float depth;
	int Line_spacing;
	//std::string TextureFilepath;
	//bool is_active;
	void(*func)();
	/**************************************************************************/
		/*!
		\brief
			Default Constructor of GameUI
		*/
	/**************************************************************************/
	GameUI();
	/**************************************************************************/
		/*!
		\brief
			This function set all the variables in the GameUI component
		*/
	/**************************************************************************/
	void SetGameUI(std::string font_fam, std::string text_param,
		float size_param, float x_off_param, float y_off_param,
		Vec3 color_param, float alpha_param, bool vis_param, float depth_param, int line_spacing_param);
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
	void SetAABB(Vec2 PositionInst, Vec2 ScaleInst, Vec2 DimensionInst);
	/**************************************************************************/
	/*!
		\brief
			Serialize() virtual override function to serialise component to the
			txt file
	*/
	/**************************************************************************/
	virtual void Serialize(FILE* fp) override;
	/**************************************************************************/
	/*!
		\brief
			Deserialize() virtual override function to deserialize component
			from the txt file
	*/
	/**************************************************************************/
	virtual void Deserialize(FILE* fp) override;
	/**************************************************************************/
	/*!
		\brief
			CopyData() virtual override function to copy data to another
			component
	*/
	/**************************************************************************/
	virtual void CopyData(Component* Target) override;

	/**************************************************************************/
		/*!
			\brief
				This is a setter function that sets the font family of the 
				GameUI component data member
			\param FontFam
				This is a string which is the handle to a map of fonts
		*/
	/**************************************************************************/
	void SetTextFontFamily(std::string FontFam);
	/**************************************************************************/
		/*!
			\brief
				This is a getter function that returns the handle of the font 
				family being used in this component
			\return
				string var which is the font family being loaded into the 
				asset manager
		*/
	/**************************************************************************/
	std::string GetTextFontFamily();
	/**************************************************************************/
		/*!
			\brief
				This is a getter function that gets the text rendered in the 
				GameUI component
			\param text_param
				The text value is going to override the text data member
				in the component
		*/
	/**************************************************************************/
	void SetText(std::string text_param);
	/**************************************************************************/
		/*!
			\brief
				This is a getter function that gets the text rendered in the 
				GameUI component
			\return
				The text data member in the GameUI Component
		*/
	/**************************************************************************/
	std::string GetText();
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
	void SetSize(float size_param);
	/**************************************************************************/
		/*!
			\brief
				This function gets the size of the font rendered
			\return
				The size of the font.
		*/
	/**************************************************************************/
	float GetSize();
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
	void SetFontColor(float r_param, float g_param, float b_param);
	/**************************************************************************/
		/*!
			\brief
				This function gets the Vec3 colour of the font.
			\return
				The value in the RGB data member
		*/
	/**************************************************************************/
	Vec3 GetFontColor_vec3();
	/**************************************************************************/
		/*!
			\brief
				This function return true or false if a GameUI obj has been
				hovered over.
		*/
	/**************************************************************************/
	bool MouseHoverClick(int MouseX, int MouseY);

	/**************************************************************************/
		/*!
			\brief
				This function sets the visibility of the font in the visibility
		*/
	/**************************************************************************/
	void SetFontVisibility(bool vis_param); 
	/**************************************************************************/
		/*!
			\brief
				This function return true or false if the GameUI's font are 
				visible or not
		*/
	/**************************************************************************/
	bool GetFontVisibility();
	/**************************************************************************/
		/*!
			\brief
				This function sets the depth of the font rendered
		*/
	/**************************************************************************/
	void SetFontDepth(bool Depth_param);
	/**************************************************************************/
		/*!
			\brief
				This function returns the depth value of the font on the GameUI
				object
		*/
	/**************************************************************************/
	float GetFontDepth();

	/**************************************************************************/
		/*!
			\brief
				This function sets the line spacing of the text font rendered
		*/
	/**************************************************************************/
	void SetLineSpacing(int line_spacing);
	/**************************************************************************/
		/*!
			\brief
				This function gets the line spacing value text font rendered
		*/
	/**************************************************************************/
	int GetLineSpacing();
};
