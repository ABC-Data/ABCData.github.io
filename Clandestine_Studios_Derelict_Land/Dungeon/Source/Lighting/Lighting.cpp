/******************************************************************************/
/*!
\file    Lighting.cpp
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    March 13, 2023
\brief   Lighting Component

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Lighting/Lighting.h"

Lighting::Lighting() : isVisible{ false }{
	SetType(ComponentType::Lighting);
	Light.Color = glm::vec3{ 0.f, 0.f, 0.f };
	Light.LightIntensity = 0.f;
}
Lighting::~Lighting(){

}

void Lighting::SetLighting(bool isVisible_param, glm::vec3 RGB_to_set, float LightIntensity) {
	isVisible = isVisible_param;
	Light.Color = RGB_to_set;
	Light.LightIntensity = LightIntensity;
}

void Lighting::SetLightingPos(glm::vec3 Pos) {
	Light.Pos = Pos;
}

bool Lighting::GetVisibility() {
	return isVisible;
}

void Lighting::CopyData(Component* Target) {
	Lighting* a = dynamic_cast<Lighting*>(Target);
	if (a != nullptr) {
		this->isVisible = a->isVisible;
		this->Light.Pos = a->Light.Pos;
		this->Light.Color = a->Light.Color;
		this->Light.LightIntensity = a->Light.LightIntensity;
	}
}

void Lighting::Serialize(FILE* fp) {
	fprintf(fp, "Lighting\n");
	fprintf(fp, "isVisible: %i\n", isVisible);
	fprintf(fp, "LightIntensity: %f\n", Light.LightIntensity);
	fprintf(fp, "RGB_0to1: %f, %f, %f\n", Light.Color.x, Light.Color.y, Light.Color.z);
}

void Lighting::Deserialize(FILE* fp) {
	int visibility;
	float red, green, blue;
	float ambient_strength;

	int success = 1;

	//bool
	success *= fscanf(fp, "isVisible: %i\n", &visibility);
	//float
	success *= fscanf(fp, "LightIntensity: %f\n", &ambient_strength);
	//vec3, 3 floats
	success *= fscanf(fp, "RGB_0to1: %f, %f, %f\n", &red, &green, &blue);

	if (!success) {
		std::cout << "Serialize Lighting Failed!";
		SetLighting(false, { 0.f, 0.f, 0.f }, 0.f);
		return;
	}


	SetLighting(static_cast<bool>(visibility), { red, green, blue }, ambient_strength);
}