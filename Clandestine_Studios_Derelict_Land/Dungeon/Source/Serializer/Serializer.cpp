/******************************************************************************/
/*!
\file		Serializer.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 1, 2022
\brief		Serialization system

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Serializer/Serializer.h"

/**************************************************************************/
/*!
	\brief
	Serializes the current Level to a file (.scn)
	\param CurrentLevel
	The level to serialize
*/
/**************************************************************************/
void FileSerializer::SeralizeLevelToSceneFile(Level* CurrentLevel)
{
	std::stringstream levelpath;
	levelpath << "./Scene/" << CurrentLevel->GetId() << ".scn";
	FILE* scenefile = fopen(levelpath.str().c_str(), "w");
	if (!scenefile) {
		throw std::runtime_error("Can't open file");
	}

	fprintf(scenefile, "Level: %s\n", CurrentLevel->GetId().c_str());

	for (size_t i = 0; i < CurrentLevel->GetGOASize(); ++i) {
		//Write the indices of the components of the object first
		/*
		for (size_t j = 0; j < CurrentLevel->GetGameObjectArray()[i]->GetComponentList().size(); ++j) {
			fprintf(scenefile, "%d", (int)CurrentLevel->GetGameObjectArray()[i]->GetComponentList()[j]->GetType());
			if (j < CurrentLevel->GetGameObjectArray()[i]->GetComponentList().size() - 1) {
				fprintf(scenefile, " ");
			}
			else {
				fprintf(scenefile, "\n");
			}
		}
		*/

		fprintf(scenefile, "Name: %s\n", CurrentLevel->GetGameObjectArray()[i]->GetName().c_str());
		fprintf(scenefile, "Tag: %s\n", CurrentLevel->GetGameObjectArray()[i]->tag.c_str());
		fprintf(scenefile, "Layer: %s\n", CurrentLevel->GetGameObjectArray()[i]->layer.c_str());

		//Write the actual data of each component
		for (size_t j = 0; j < CurrentLevel->GetGameObjectArray()[i]->GetComponentList().size(); ++j) {
			CurrentLevel->GetGameObjectArray()[i]->GetComponentList()[j]->Serialize(scenefile);
		}
	}
	fclose(scenefile);
}