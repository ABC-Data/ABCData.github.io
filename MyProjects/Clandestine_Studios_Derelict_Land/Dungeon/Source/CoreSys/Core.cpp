/******************************************************************************/
/*!
\file		Core.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Sep 14, 2022
\brief		Core System implementation

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Core.h"
#include "Graphics/GLFWsetup.h"



/**************************************************************************/
/*!
  \brief
	Constructor for Core
*/
/**************************************************************************/
Core::Core()
{
	GameRunning = true;
	LastTick = std::chrono::high_resolution_clock::now();
	CurrentLevel = nullptr;
	NextLevel = nullptr;
	PV = nullptr;
	AppTimeElapsed = 0;
}

/**************************************************************************/
/*!
  \brief
	Destructor for Core. Doesn't do anything yet
*/
/**************************************************************************/
Core::~Core()
{
}

/**************************************************************************/
/*!
  \brief
	Initialises Core. Use after setting up
*/
/**************************************************************************/
void Core::Init()
{
	//Only to be called after all systems added in
	for (System*& sys : Systems)
	{
		sys->SetLevel(CurrentLevel);
		sys->Init();
	}

	//PV = new PerformanceViewer;
	PV = std::make_shared<PerformanceViewer>();

	AppTimeElapsed = 0;
}

/**************************************************************************/
/*!
  \brief
	Shuts down Core. Kind of like a destructor
*/
/**************************************************************************/
void Core::Shutdown()
{
	//Remove all systems
	for (int i = 0; i < Systems.size(); ++i)
	{
		Systems[Systems.size() - i - 1]->Shutdown();
		delete Systems[Systems.size() - i - 1];
	}
	delete CurrentLevel;

	// Destroy the PV obj
	PV->Shutdown();
	// Free-ing PV's alloced mem
	PV.reset(new PerformanceViewer);//delete PV;
}

const float fixedDeltaTime = 1.0f / 60.0f;
float accumulatedTime = 0.0f;//one time definition
int currentNumberOfSteps = 0;

/**************************************************************************/
/*!
  \brief
	Update loop for Core, aka the main game loop
*/
/**************************************************************************/
void Core::Update()
{
	LastTick = std::chrono::high_resolution_clock::now();

	//Main game loop
	while (GameRunning) {
		if (glfwWindowShouldClose(GLSetup::ptr_window)) {
			GameRunning = false;
			break;
		}

		ClockPoint CurrentTick = std::chrono::high_resolution_clock::now();

		currentNumberOfSteps = 0;//reset
		float dt = std::chrono::duration<float>(CurrentTick - LastTick).count();
		//deltaTime = timeEnd – timeStart;//compute the actual game loop time
		accumulatedTime += dt;//adding the actual game loop time
		while (accumulatedTime >= fixedDeltaTime)
		{
			accumulatedTime -= fixedDeltaTime;//this will save the exact accumulated time differences, among all game loops
			currentNumberOfSteps++;
		}

		LastTick = CurrentTick;

		std::vector<std::string> SysNames{ "Graphics", "Animation", "Input", "Logic", "Physics", "GO Factory", "IMGUI Editor", "Audio"};
		
		for (int i = 0; i < Systems.size(); ++i) {
			ClockPoint sys_start = std::chrono::high_resolution_clock::now();

			Systems[i]->Update(dt);
				
			ClockPoint sys_end = std::chrono::high_resolution_clock::now();

			PV->Update(SysNames[i], std::chrono::duration<float>(sys_end - sys_start).count());
		}		
		for (int i = 0; i < Systems.size(); ++i) {
			Systems[i]->AdditionalUpdate(dt);
		}
		PV->GenerateTotalTime();
		PV->Percentage_Conversion();
		//PV->PrintStats();
		if (NextLevel != nullptr) {
			SceneManager::LoadLevel(NextLevel);
			NextLevel = nullptr;
		}
		//sClockPoint Aft_systems = std::chrono::high_resolution_clock::now();
		AppTimeElapsed += dt;
	}
}

/**************************************************************************/
/*!
  \brief
	Adds a system to the core, do only before initialising

  \param s
	System to add
*/
/**************************************************************************/
void Core::AddSystem(System* s)
{
	Systems.push_back(s);
}

/**************************************************************************/
/*!
  \brief
	Sets the current level to another level

  \param lvl
	Level to change to
*/
/**************************************************************************/
void Core::SetLevel(Level* lvl)
{
	if (CurrentLevel) 
	{
		/*
			A level object has in itself, the allocated memory of the data of
			all its game objects. Each game object has, in turn, allocated
			memory for each of its components. To ensure a leak-free changing
			of levels, these memory blocks must be cleared beforehand.
		*/

		for (GameObject*& go : CurrentLevel->GetGameObjectArray()) 
		{
			factory->Destroy(go);
		}
		delete CurrentLevel;
	} 
	CurrentLevel = lvl;
	for (System*& sys : Systems) {sys->SetLevel(CurrentLevel);}
}

/**************************************************************************/
/*!
  \brief
	Gets the App Time Elapsed

  \return
	App Time Elapsed
*/
/**************************************************************************/
long double Core::GetAppTimeElapsed()
{
	return Core::AppTimeElapsed;
}

/**************************************************************************/
/*!
  \brief
	Gets the current level

  \return
	CurrentLevel
*/
/**************************************************************************/
Level* Core::GetLevel() {
	return CurrentLevel;
}

void Core::SetNextLevel(Level* lvl)
{
	NextLevel = lvl;
}

Level* Core::GetNextLevel()
{
	return NextLevel;
}

/**************************************************************************/
/*!
  \brief
	Gets game object by ID.

  \param id
	ID of object to search

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetGameObject(int id)
{
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (GO->GetObjectId() == id && GO->GetActive()) {
			return GO;
		}
	}
	return nullptr;
}

/**************************************************************************/
/*!
  \brief
	Gets game object by id and component.

  \param name
	ID of object to search

  \param component
	Object has this component?

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetGameObject(int id, ComponentType component)
{
	GameObject* found = GetGameObject(id);
	if (found != nullptr) {
		if (found->HasComponent(component)) return found;
	}
	return nullptr;
}

/**************************************************************************/
/*!
  \brief
	Gets game object by name. Returns first with matching name

  \param name
	Name of object to search

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetGameObject(std::string name)
{
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (GO->GetName() == name && GO->GetActive()) {
			return GO;
		}
	}
	return nullptr;
}

/**************************************************************************/
/*!
  \brief
	Gets game object by name and component.

  \param name
	Name of object to search

  \param component
	Object has this component?

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetGameObject(std::string name, ComponentType component)
{
	GameObject* found = GetGameObject(name);
	if (found != nullptr) {
		if (found->HasComponent(component)) return found;
	}
	return nullptr;
}

/**************************************************************************/
/*!
  \brief
	Gets game object closest to a point.

  \param source
	Point of origin

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetClosestGameObject(Vec2 source, ComponentType component)
{
	GameObject* found = nullptr;
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (!GO->GetActive() || !GO->HasComponent(ComponentType::Transform)) continue;
		if (component != ComponentType::ComponentTypeSize && !GO->HasComponent(component)) continue;
		if (found == nullptr) {
			found = GO;
			continue;
		}
		Transform* T = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
		Transform* T2 = dynamic_cast<Transform*>(found->GetComponent(ComponentType::Transform));
		if (Vec2SquareLength(T->Position - source) < Vec2SquareLength(T2->Position - source)) {
			found = GO;
		}
	}
	return found;
}

/**************************************************************************/
/*!
  \brief
	Gets game object closest to another game object that is not itself.

  \param source
	Game object of origin

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetClosestGameObject(GameObject* source, ComponentType component)
{
	if (!source->HasComponent(ComponentType::Transform)) return nullptr;
	if (CurrentLevel->GetGOASize() == 1) return nullptr;
	Transform* T = dynamic_cast<Transform*>(source->GetComponent(ComponentType::Transform));
	GameObject* found = nullptr;
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (!GO->GetActive() || !GO->HasComponent(ComponentType::Transform) || GO == source) continue;
		if (component != ComponentType::ComponentTypeSize && !GO->HasComponent(component)) continue;
		if (found == nullptr) {
			found = GO;
			continue;
		}
		Transform* T2 = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
		Transform* T3 = dynamic_cast<Transform*>(found->GetComponent(ComponentType::Transform));
		if (Vec2SquareLength(T2->Position - T->Position) < Vec2SquareLength(T3->Position - T->Position)) {
			found = GO;
		}
	}
	return found;
}


/**************************************************************************/
/*!
  \brief
	Gets game object whose name starts with a certain prefix

  \param prefix
	Name prefix

  \param component
	Component to search for

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetObjectByPrefix(std::string prefix, ComponentType component)
{
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (GO->GetName().starts_with(prefix) && (component == ComponentType::ComponentTypeSize || GO->HasComponent(component))) {
			return GO;
		}
	}
	return nullptr;
}

/**************************************************************************/
/*!
  \brief
	Gets game object whose name ends with a certain suffix

  \param suffix
	Name suffix

  \param component
	Component to search for

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetObjectBySuffix(std::string suffix, ComponentType component)
{
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (GO->GetName().ends_with(suffix) && (component == ComponentType::ComponentTypeSize || GO->HasComponent(component))) {
			return GO;
		}
	}
	return nullptr;
}

/**************************************************************************/
/*!
  \brief
	Gets game object whose name starts with a certain prefix and ends with a certain suffix

  \param prefix
	Name prefix

  \param suffix
	Name suffix

  \param component
	Component to search for

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetObjectByPrefixSuffix(std::string prefix, std::string suffix, ComponentType component)
{
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (GO->GetName().starts_with(prefix) && GO->GetName().ends_with(suffix) && (component == ComponentType::ComponentTypeSize || GO->HasComponent(component))) {
			return GO;
		}
	}
	return nullptr;
}

/**************************************************************************/
/*!
  \brief
	Gets game object whose name starts with a certain prefix and is within a certain proximity to a source

  \param source
	Vec2 source

  \param prefix
	Name prefix

  \param proximity
	Distance between source and GO

  \param component
	Component to search for

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetObjectInProximityByPrefix(Vec2 source, std::string prefix, float proximity, ComponentType component)
{
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (!GO->HasComponent(ComponentType::Transform)) continue;
		if (component != ComponentType::ComponentTypeSize && !GO->HasComponent(component)) continue;
		Transform* T = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
		if (GO->GetName().starts_with(prefix) && fabsf(Vec2Distance(source, T->Position)) <= proximity) {
			return GO;
		}
	}
	return nullptr;
}

/**************************************************************************/
/*!
  \brief
	Gets all game objects whose name starts with a certain prefix

  \param GOA
	Vector container to store results in

  \param prefix
	Name prefix to search for

  \param component
	Component to search for
*/
/**************************************************************************/
void Core::GetAllObjectsByPrefix(std::vector<GameObject*>& GOA, std::string prefix, ComponentType component)
{
	for (GameObject* GO : CurrentLevel->GetGameObjectArray()) {
		if (GO->GetName().starts_with(prefix) && (component == ComponentType::ComponentTypeSize || GO->HasComponent(component))) {
			GOA.emplace_back(GO);
		}
	}
}

/**************************************************************************/
/*!
  \brief
	Gets all game objects that overlap a certain location

  \param GOA
	Vector container to store results in

  \param position
	Position to check

  \param component
	Component to search for
*/
/**************************************************************************/
void Core::GetAllObjectsAtLocation(std::vector<GameObject*>& GOA, Vec2 position, ComponentType component)
{
	for (GameObject* GO : CurrentLevel->GetGameObjectArray())
	{
		if (!GO->GetActive() || !GO->HasComponent(ComponentType::Transform)
			|| !(GO->HasComponent(ComponentType::Renderer) || GO->HasComponent(ComponentType::Animate))) continue;
		if (GO->HasComponent(ComponentType::Renderer)) {
			if (!GO->GetComponent<Renderer>()->GetVisibility()) continue;
		}
		if (GO->HasComponent(ComponentType::Animate)) {
			if (!GO->GetComponent<Animate>()->GetVisibility()) continue;
		}
		//if (GO->HasComponent(ComponentType::Lighting)) {
		//	if (!GO->GetComponent<Lighting>()->GetVisibility()) continue;
		//}
		Transform* T = GO->GetComponent<Transform>();
		Vec2 minbound;
		Vec2 maxbound;
		if (GO->layer == "UI") {
			minbound = { (Batch::camera.GetCameraPosition().x + T->Position.x) - (T->Dimensions.x / 2.f * (T->Scale.x / 100.f)), (Batch::camera.GetCameraPosition().y + T->Position.y) - (T->Dimensions.y / 2.f * (T->Scale.y / 100.f)) };
			maxbound = { (Batch::camera.GetCameraPosition().x + T->Position.x) + (T->Dimensions.x / 2.f * (T->Scale.x / 100.f)), (Batch::camera.GetCameraPosition().y + T->Position.y) + (T->Dimensions.y / 2.f * (T->Scale.y / 100.f)) };
		}
		else {
			minbound = { T->Position.x - (T->Dimensions.x / 2.f * (T->Scale.x / 100.f)) , T->Position.y - (T->Dimensions.y / 2.f * (T->Scale.y / 100.f)) };
			maxbound = { T->Position.x + (T->Dimensions.x / 2.f * (T->Scale.x / 100.f)) , T->Position.y + (T->Dimensions.y / 2.f * (T->Scale.y / 100.f)) };
		}
		if (position.x >= minbound.x && position.x <= maxbound.x
			&& position.y >= minbound.y && position.y <= maxbound.y
			&& (component == ComponentType::ComponentTypeSize || GO->HasComponent(component))) {
			GOA.emplace_back(GO);
		}
	}
}

/**************************************************************************/
/*!
  \brief
	Advanced Game Object Searching function. Covers basically all supported cases.
	!!!PUT COMPONENT FIRST IF SEARCHING WITH COMPONENTS!!!

  \param pattern
	Search pattern to search for objects. Supply these params for these patterns:
	ID: Object ID (int)
	NAME: Object Name (std::string)
	PREFIX: Name prefix (std::string)
	SUFFIX: Name suffix (std::string)
	PREFIX_AND_SUFFIX: Name prefix (std::string), Name suffix (std::string)
	VEC2: Point (Vec2)
	CLOSEST_FROM_OBJECT: Source Game Object (GameObject*)

  \param comppattern
	Search pattern for components within objects. Supply these params for these patterns:
	NO_COMPONENT: None - Do not search for components
	WITH_COMPONENT: Component type to search for (ComponentType)

  \return
	Found game object
*/
/**************************************************************************/
GameObject* Core::GetGameObjectAdvanced(GOSearchPattern pattern, GOSearchComponent comppattern, ...)
{
	va_list argp;
	va_start(argp, comppattern);
	GameObject* found = nullptr;
	bool withcomp = false;
	ComponentType searchcomp = ComponentType::ComponentTypeSize;
	if (comppattern == GOSearchComponent::WITH_COMPONENT) {
		withcomp = true;
		searchcomp = va_arg(argp, ComponentType);
	}
	switch (pattern) {
	case(GOSearchPattern::ID):
	{
		int id = va_arg(argp, int);
		found = (withcomp ? GetGameObject(id, searchcomp) : GetGameObject(id));
		break;
	}
	case(GOSearchPattern::NAME):
	{
		std::string name = va_arg(argp, std::string);
		found = (withcomp ? GetGameObject(name, searchcomp) : GetGameObject(name));
		break;
	}
	case(GOSearchPattern::PREFIX):
	{
		std::string prefix = va_arg(argp, std::string);
		found = (withcomp ? GetObjectByPrefix(prefix, searchcomp) : GetObjectByPrefix(prefix));
		break;
	}
	case(GOSearchPattern::SUFFIX):
	{
		std::string suffix = va_arg(argp, std::string);
		found = (withcomp ? GetObjectBySuffix(suffix, searchcomp) : GetObjectBySuffix(suffix));
		break;
	}
	case(GOSearchPattern::PREFIX_AND_SUFFIX):
	{
		std::string prefix = va_arg(argp, std::string);
		std::string suffix = va_arg(argp, std::string);
		found = (withcomp ? GetObjectByPrefixSuffix(prefix, suffix, searchcomp) : GetObjectByPrefixSuffix(prefix, suffix));
		break;
	}
	case(GOSearchPattern::VEC2):
	{
		Vec2 pos = va_arg(argp, Vec2);
		found = (withcomp ? GetClosestGameObject(pos, searchcomp) : GetClosestGameObject(pos));
		break;
	}
	case(GOSearchPattern::CLOSEST_FROM_OBJECT):
	{
		GameObject* source = va_arg(argp, GameObject*);
		found = (withcomp ? GetClosestGameObject(source, searchcomp) : GetClosestGameObject(source));
		break;
	}
	case(GOSearchPattern::PROXIMITY_PREFIX):
	{
		Vec2 source = va_arg(argp, Vec2);
		std::string prefix = va_arg(argp, std::string);
		float proximity = va_arg(argp, float);
		found = (withcomp ? GetObjectInProximityByPrefix(source, prefix, proximity, searchcomp) : GetObjectInProximityByPrefix(source, prefix, proximity));
		break;
	}
	}
	va_end(argp);
	return found;
}

std::vector<System*>& Core::GetSystem() {
	return Systems;
}

/*!*************************************************************************
****
\brief
	Instance of Core
\return
	Created Core.
****************************************************************************
***/
Core* Core::Instance() { if (!Engine) { Engine = new Core(); } return Engine; }

/*!*************************************************************************
****
\brief
	Delete static Core above.
****************************************************************************
***/
void Core::DeleteStaticCore() { if (Engine) { delete Engine; Engine = nullptr; } }