/******************************************************************************/
/*!
\file    Animate.cpp
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    February 2, 2023
\brief   Animate Component

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Animation/Animate.h"

Animate::Animate() : isVisible{ true }, isPlaying{ true }, milliseconds_per_sprite{ 0 }, TexHandle{ "" }, RGB_0to1{ 1.f,1.f,1.f }, Alpha_0to1{ 1.f }, last_frame{ false }, total_frame{ 0 }, curr_frame{ 0 } {
	SetType(ComponentType::Animate);
}
Animate::~Animate() {

}

void Animate::SetAnimate(bool isVisibile_param, bool Playing, int millisecs_per_sprite,
	std::string tex_hdl, Vec3 RGB_to_set, float alpha_param) {
	this->isVisible = isVisibile_param;
	this->isPlaying = Playing;
	this->milliseconds_per_sprite = millisecs_per_sprite;
	this->TexHandle = tex_hdl;
	this->RGB_0to1 = RGB_to_set;
	this->Alpha_0to1 = alpha_param;
}

std::string Animate::GetTexHandle() {
	return this->TexHandle;
}
void Animate::SetTexHandle(std::string texhandle_param) {
	this->TexHandle = texhandle_param;
}
bool Animate::GetVisibility() {
	return this->isVisible;
}
void Animate::SetVisibility(bool vis_param) {
	this->isVisible = vis_param;
}

bool Animate::GetIsPlaying() {
	return this->isPlaying;
}
void Animate::SetIsPlaying(bool ply_param) {
	this->isPlaying = ply_param;
}

int Animate::GetTotalFrame() {
	return total_frame;
}
void Animate::SetTotalFrame(int frames) {
	total_frame = frames;
}

int Animate::GetCurrentFrame() {
	return curr_frame;
}
void Animate::SetCurrentFrame(int frames) {
	curr_frame = frames;
}

bool Animate::CheckLastFrame() {
	return last_frame;
}
void Animate::SetCheckLastFrame(bool state) {
	last_frame = state;
}

int Animate::GetSpriteSpeed() {
	return milliseconds_per_sprite;
}
void Animate::SetSpriteSpeed(int millisec) {
	milliseconds_per_sprite = millisec;
}

void Animate::CopyData(Component* Target) {
	Animate* a = dynamic_cast<Animate*>(Target);
	if (a != nullptr) {
		this->isVisible = a->isVisible;
		this->isPlaying = a->isPlaying;
		this->milliseconds_per_sprite = a->milliseconds_per_sprite;
		this->TexHandle = a->TexHandle;
		this->RGB_0to1 = a->RGB_0to1;
		this->Alpha_0to1 = a->Alpha_0to1;		
	}
}
void Animate::Serialize(FILE* fp) {
	fprintf(fp, "Animate\n");
	fprintf(fp, "isVisible: %i\n", isVisible);
	fprintf(fp, "isPlaying: %i\n", isPlaying);
	fprintf(fp, "Milliseconds Per Sprite: %i\n", milliseconds_per_sprite);
	fprintf(fp, "Texture Handle: %s\n", TexHandle.c_str());
	fprintf(fp, "RGB_0to1: %f, %f, %f\n", RGB_0to1.x, RGB_0to1.y, RGB_0to1.z);
	fprintf(fp, "Alpha_0to1: %f\n", Alpha_0to1);
}
void Animate::Deserialize(FILE* fp) {
	int playing, millisecs_per_sprite, visibility;
	char texhdl[100]{ 0 };
	float red, green, blue;
	float alpha_f;

	int success = 1;
	int discard = 0;
	//bool
	success *= fscanf(fp, "isVisible: %i\n", &visibility);
	//bool
	success *= fscanf(fp, "isPlaying: %i\n", &playing);
	//int
	success *= fscanf(fp, "Milliseconds Per Sprite: %i\n", &millisecs_per_sprite);
	//std::string
	discard = fscanf(fp, "Texture Handle: ");
	success *= fscanf(fp, "%[^,\n]\n", &texhdl);

	//vec3, 3 floats
	success *= fscanf(fp, "RGB_0to1: %f, %f, %f\n", &red, &green, &blue);

	//float
	success *= fscanf(fp, "Alpha_0to1: %f\n", &alpha_f);


	if (!success) {
		std::cout << "Serialize Animate Failed!";
		SetAnimate(false, false, 0, "\0", { 0.f, 0.f, 0.f }, 1.f);
		return;
	}


	SetAnimate(static_cast<bool>(visibility), static_cast<bool>(playing),
		millisecs_per_sprite, texhdl, { red, green, blue }, alpha_f);
}
