/******************************************************************************/
/*!
\file		ControllerComponent.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Jan 25, 2023
\brief		Contains Controller Component for Level Controllers

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "ControllerComponent.h"

Controller::Controller() : LevelTag{ "NULL" }, LevelFlags{}, LevelFlagsSize{ 0 }
{
	SetType(ComponentType::Controller);
}

Controller::Controller(std::string tag, int size) : LevelTag{ tag }, LevelFlags{}, LevelFlagsSize{size}
{
	SetType(ComponentType::Controller);
}

void Controller::Serialize(FILE* fp)
{
	fprintf(fp, "Controller\n");
	fprintf(fp, "Tag: %s\n", LevelTag.c_str());
	fprintf(fp, "Number of flags: %d\n", LevelFlagsSize);
	fprintf(fp, "Flags: ");
	for (int i = 0; i < LevelFlagsSize; ++i) {
		if (LevelFlags[i]) {
			fprintf(fp, "1");
		}
		else {
			fprintf(fp, "0");
		}
		if (i == LevelFlagsSize - 1) {
			fprintf(fp, "\n");
		}
		else {
			fprintf(fp, " ");
		}
	}
	if (LevelFlagsSize == 0) fprintf(fp, "\n");
}

void Controller::Deserialize(FILE* fp)
{
	char leveltag[32]{ 0 };
	int discard = 0;
	int success = fscanf(fp, "Tag: %[^\n]\n", &leveltag);
	LevelTag = leveltag;
	success *= fscanf(fp, "Number of flags: %d\n", &LevelFlagsSize);
	discard = fscanf(fp, "Flags: ");
	for (int i = 0; i < LevelFlagsSize; ++i) {
		int flag = 0;
		success *= fscanf(fp, "%d", &flag);
		if (!success) {
			std::cout << "Failed to load Controller flags!\n";
			return;
		}
		LevelFlags.push_back(flag ? true : false);
	}
	discard = fscanf(fp, "\n");
	if (!success) {
		std::cout << "Failed to load Controller Component!\n";
	}
}

void Controller::CopyData(Component* Target)
{
	Controller* CT = dynamic_cast<Controller*>(Target);

	if (CT != nullptr) {
		this->LevelTag = CT->LevelTag;
		this->LevelFlags = CT->LevelFlags;
		this->LevelFlagsSize = CT->LevelFlagsSize;
	}
}

