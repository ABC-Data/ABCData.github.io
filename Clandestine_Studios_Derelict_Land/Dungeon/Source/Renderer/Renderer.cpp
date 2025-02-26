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
#include "Renderer/Renderer.h"

/**************************************************************************/
	/*!
	\brief
		Default Constructor of Renderer component
	*/
/**************************************************************************/
Renderer::Renderer() :
	isVisible(true),
	isSpritesheet(false),
	no_of_sprites(0),
	milliseconds_per_sprite(0),
	TexHandle(""),
	Tex_UV_Min_COORD(0.f, 0.f),
	Tex_UV_Max_COORD(1.f, 1.f),
	RGB_0to1(0.f, 0.f, 0.f),
	Alpha_0to1(1.f)
{
	SetType(ComponentType::Renderer);
}
/**************************************************************************/
	/*!
	\brief
		Default Destructor of Renderer component
	*/
/**************************************************************************/
Renderer::~Renderer()
{

}

/**************************************************************************/
/*!
	\brief
		CopyData() virtual override function to copy data to another
		component
*/
/**************************************************************************/
void Renderer::CopyData(Component* Target)
{
	Renderer* t = dynamic_cast<Renderer*>(Target);
	if (t != nullptr)
	{
		this->isVisible = t->isVisible;
		this->isSpritesheet = t->isSpritesheet;
		this->no_of_sprites = t->no_of_sprites;
		this->milliseconds_per_sprite = t->milliseconds_per_sprite;
		this->TexHandle = t->TexHandle;
		this->Tex_UV_Min_COORD = t->Tex_UV_Min_COORD;
		this->Tex_UV_Max_COORD = t->Tex_UV_Max_COORD;
		this->RGB_0to1 = t->RGB_0to1;
		this->Alpha_0to1 = t->Alpha_0to1;
	}
}
/**************************************************************************/
	/*!
		\brief
			Serialize() virtual override function to serialise component to the
			txt file
	*/
/**************************************************************************/
void Renderer::Serialize(FILE* fp)
{
	fprintf(fp, "Renderer\n");
	fprintf(fp, "isVisible: %i\n", isVisible);
	fprintf(fp, "isSpritesheet: %i\n", isSpritesheet);
	fprintf(fp, "No. of Sprites: %i\n", no_of_sprites);
	fprintf(fp, "Milliseconds Per Sprite: %i\n", milliseconds_per_sprite);
	fprintf(fp, "Texture Handle: %s\n", TexHandle.c_str());
	fprintf(fp, "Texture UV MIN: %f, %f\n", Tex_UV_Min_COORD.x, Tex_UV_Min_COORD.y);
	fprintf(fp, "Texture UV MAX: %f, %f\n", Tex_UV_Max_COORD.x, Tex_UV_Max_COORD.y);
	fprintf(fp, "RGB_0to1: %f, %f, %f\n", RGB_0to1.x, RGB_0to1.y, RGB_0to1.z);
	fprintf(fp, "Alpha_0to1: %f\n", Alpha_0to1);
}
/**************************************************************************/
	/*!
	\brief
		Setter function for the entire Renderer Component
	\param the long list of params
		These parameters set the data members to these param values.
	*/
/**************************************************************************/

void Renderer::SetRenderer(bool isVisibile_param, bool isSprite,
	int no_sprites, int millisecs_per_sprite,
	std::string tex_hdl,
	Vec2 UV_min, Vec2 UV_max, Vec3 RGB_to_set, float alpha_param)
{
	this->isVisible = isVisibile_param;
	this->isSpritesheet = isSprite;
	this->no_of_sprites = no_sprites;
	this->milliseconds_per_sprite = millisecs_per_sprite;
	this->TexHandle = tex_hdl;
	this->Tex_UV_Min_COORD = UV_min;
	this->Tex_UV_Max_COORD = UV_max;
	this->RGB_0to1 = RGB_to_set;
	this->Alpha_0to1 = alpha_param;
}

/**************************************************************************/
	/*!
		\brief
			Deserialize() virtual override function to deserialize component
			from the txt file
	*/
/**************************************************************************/
void Renderer::Deserialize(FILE* fp)
{
	 
	int isSprite, sprite_cnt, millisecs_per_sprite, visibility;
	char texhdl[100]{ 0 };
	float uv_min_x, uv_min_y, uv_max_x, uv_max_y;
	float red, green, blue;
	float alpha_f;

	int discard = 0;
	int success = 1;
	//bool
	success *= fscanf(fp, "isVisible: %i\n", &visibility);
	//bool
	success *= fscanf(fp, "isSpritesheet: %i\n", &isSprite);
	//int
	success *= fscanf(fp, "No. of Sprites: %i\n", &sprite_cnt);
	//int
	success *= fscanf(fp, "Milliseconds Per Sprite: %i\n", &millisecs_per_sprite);
	//std::string
	discard = fscanf(fp, "Texture Handle: ");
	success *= fscanf(fp, "%[^,\n]\n", &texhdl);

	//vec2 uv_min
	success *= fscanf(fp, "Texture UV MIN: %f, %f\n", &uv_min_x, &uv_min_y);

	//vec2 uv_min
	success *= fscanf(fp, "Texture UV MAX: %f, %f\n", &uv_max_x, &uv_max_y);

	//vec3, 3 floats
	success *= fscanf(fp, "RGB_0to1: %f, %f, %f\n", &red, &green, &blue);

	//float
	success *= fscanf(fp, "Alpha_0to1: %f\n", &alpha_f);


	if (!success) {
		std::cout << "Serialize Renderer Failed!";
		SetRenderer(false, false, 1, 0, "\0", {0.f, 0.f}, {1.f,1.f}, {0.f, 0.f, 0.f}, 1.f);
		return;
	}


	SetRenderer(static_cast<bool>(visibility), static_cast<bool>(isSprite),
		sprite_cnt, millisecs_per_sprite,
		texhdl,
		{uv_min_x , uv_min_y}, {uv_max_x , uv_max_y}, {red, green, blue}, alpha_f);
}
/**************************************************************************/
	/*!
	\brief
		Getter function for the TexHandle of the Renderer Component
	\return
		The tex handle value in the component
	*/
/**************************************************************************/
std::string Renderer::GetTexHandle()
{
	return this->TexHandle;
}
/**************************************************************************/
	/*!
	\brief
		Setter function for the TexHandle of the Renderer Component
	\param texhandle_param
		The string to set as the tex handle for this instance of the
		renderer component
	*/
/**************************************************************************/

void Renderer::SetTexHandle(std::string texhandle_param)
{
	this->TexHandle = texhandle_param;
}
/**************************************************************************/
	/*!
	\brief
		Getter function for the visibility of the Renderer Component
	\return
		The bool value, true if the use case is to render the GO and false
		if no desire to render
	*/
/**************************************************************************/
bool Renderer::GetVisibility()
{
	return this->isVisible;
}
/**************************************************************************/
	/*!
	\brief
		Setter function for the visibility of the Renderer Component
	\param vis_param
		The bool to set as the visibility data member
	*/
/**************************************************************************/
void Renderer::SetVisibility(bool vis_param)
{
	this->isVisible = vis_param;
}
/**************************************************************************/
	/*!
	\brief
		Getter function for the UVMin coords of the texture, in the texture
		coordinates
	\return
		the Vec2 data member UVMin
	*/
/**************************************************************************/
Vec2 Renderer::GetUVMin() {
	return Tex_UV_Min_COORD;
}
/**************************************************************************/
	/*!
	\brief
		Getter function for the UVMax coords of the texture, in the texture
		coordinates
	\return
		the Vec2 data member UVMax
	*/
/**************************************************************************/
Vec2 Renderer::GetUVMax() {
	return Tex_UV_Max_COORD;
}
/**************************************************************************/
	/*!
	\brief
		Setter function for the UVMin coords of the texture, in the texture
		coordinates
	\param uv_min_param
		the Vec2 data member UVMin will take the values of this param
	*/
/**************************************************************************/
void Renderer::SetUVMin(Vec2 uv_min_param)
{
	Tex_UV_Min_COORD = uv_min_param;
}
/**************************************************************************/
	/*!
	\brief
		Setter function for the UVMax coords of the texture, in the texture
		coordinates
	\param uv_max_param
		the Vec2 data member UVMax will take the values of this param
	*/
/**************************************************************************/
void Renderer::SetUVMax(Vec2 uv_max_param)
{
	Tex_UV_Max_COORD = uv_max_param;
}