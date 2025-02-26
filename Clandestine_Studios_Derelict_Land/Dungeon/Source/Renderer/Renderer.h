#pragma once
/************************************************************************ /
/*!
\file    Renderer.h
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    January 5, 2023
\brief   Component for Rendering of GameObjects

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Essential.h"
#include "GameObject/Component.h"


class Renderer : public Component
{
public:
	bool isVisible;
	bool isSpritesheet;
	int no_of_sprites;
	int milliseconds_per_sprite;
	std::string TexHandle;
	Vec2 Tex_UV_Min_COORD;
	Vec2 Tex_UV_Max_COORD;
	Vec3 RGB_0to1;
	float Alpha_0to1;

	/**************************************************************************/
		/*!
		\brief
			Default Constructor of Renderer component
		*/
	/**************************************************************************/
	Renderer();
	/**************************************************************************/
		/*!
		\brief
			Default Destructor of Renderer component
		*/
	/**************************************************************************/
	~Renderer();
	/**************************************************************************/
		/*!
		\brief
			Setter function for the entire Renderer Component
		\param the long list of params
			These parameters set the data members to these param values.
		*/
	/**************************************************************************/
	void SetRenderer(bool isVisibile_param, bool isSprite,
		int no_of_sprites , int millisecs_per_sprite,
		std::string tex_hdl,
		Vec2 UV_min, Vec2 UV_max, Vec3 RGB_to_set, float alpha_param);

	/**************************************************************************/
		/*!
		\brief
			Getter function for the TexHandle of the Renderer Component
		\return 
			The tex handle value in the component
		*/
	/**************************************************************************/
	std::string GetTexHandle();
	/**************************************************************************/
		/*!
		\brief
			Setter function for the TexHandle of the Renderer Component
		\param texhandle_param
			The string to set as the tex handle for this instance of the 
			renderer component
		*/
	/**************************************************************************/
	void SetTexHandle(std::string texhandle_param);

	/**************************************************************************/
		/*!
		\brief
			Getter function for the visibility of the Renderer Component
		\return
			The bool value, true if the use case is to render the GO and false
			if no desire to render
		*/
	/**************************************************************************/
	bool GetVisibility();
	/**************************************************************************/
		/*!
		\brief
			Setter function for the visibility of the Renderer Component
		\param vis_param
			The bool to set as the visibility data member
		*/
	/**************************************************************************/
	void SetVisibility(bool vis_param);

	/**************************************************************************/
		/*!
		\brief
			Getter function for the UVMin coords of the texture, in the texture
			coordinates
		\return 
			the Vec2 data member UVMin
		*/
	/**************************************************************************/
	Vec2 GetUVMin();
	/**************************************************************************/
		/*!
		\brief
			Getter function for the UVMax coords of the texture, in the texture
			coordinates
		\return
			the Vec2 data member UVMax
		*/
	/**************************************************************************/
	Vec2 GetUVMax();

	/**************************************************************************/
		/*!
		\brief
			Setter function for the UVMin coords of the texture, in the texture
			coordinates
		\param uv_min_param
			the Vec2 data member UVMin will take the values of this param
		*/
	/**************************************************************************/
	void SetUVMin(Vec2 uv_min_param);
	/**************************************************************************/
		/*!
		\brief
			Setter function for the UVMax coords of the texture, in the texture
			coordinates
		\param uv_max_param
			the Vec2 data member UVMax will take the values of this param
		*/
	/**************************************************************************/
	void SetUVMax(Vec2 uv_max_param);

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
};