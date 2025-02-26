/*!
@file    Main.cpp
@author  a.warrenjunxuan@digipen.edu
@author  louismineo.l@digipen.edu
@date    2/9/2022

This file uses functionality defined in types "file names" to
initialize an OpenGL context and implement a game loop.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Essential.h"
#include "CoreSys/Core.h"
#include "CoreSys/MasterObjects.h"
#include "Physics/Physics.h"
#include "Input/Input.h"
#include "Graphics/Graphics.h"
#include "Editor/UI/Editor.h"
#include "Audio/Audio.h"
#include "Factory/Factory.h"
#include "Scripts/AllScripts.h"
#include "Animation/Animation.h"

#include <filesystem>

bool is_paused = false; // if game is paused

int main(){
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	SetConsoleTitle(L"Ohmega Engine"); // rename debug console
	
	const std::string projectDir = std::filesystem::current_path().string();
	Graphics* graphics = new Graphics();
	Animation* animator = new Animation();
	FilePtr* fileptr = new FilePtr("./Data/WindowParams.txt", "r");

	MasterObjectList->Init();

	Level* Level1 = FilePtr::SerializeLevel("./Scene/MainMenu.scn");
	if (fileptr->SerializeWindowParams("./Data/WindowParams.txt", graphics)) { return 1; }
	delete fileptr;

	
	if (!FilePtr::SerializeTags(projectDir + "/Data/Tags.txt")) { return 1; }
	if (!FilePtr::SerializeLayers(projectDir + "/Data/Layers.txt")) { return 1; }

	Engine->AddSystem(graphics);
	Engine->AddSystem(animator);
	Engine->AddSystem(new Input());
	Engine->AddSystem(LoadLogic());
	Engine->AddSystem(new Physics());
	Engine->AddSystem(factory);
	Engine->AddSystem(editor);
	Engine->AddSystem(audio);
	

	Engine->SetLevel(Level1);
	Engine->Init();
	Engine->Update();

	// Update layers and tags
	FilePtr::DeserializerLayers(projectDir + "/Data/Layers.txt");
	// If there were any deleted tags, this removes them for real.
	FilePtr::DeserializeTags(projectDir + "/Data/Tags.txt");

	Engine->Shutdown();

	delete MasterObjectList;
	delete Engine;

	return 0;
}
