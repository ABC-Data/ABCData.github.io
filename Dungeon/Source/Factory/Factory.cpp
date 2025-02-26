/******************************************************************************/
/*!
\file    Factory.cpp
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function definitions to implement game object factory.
		 The functions include:
		 - Factory
		 Default constructor.
		 - ~Factory
		 Default constructor.
		 - Init
		 Initializes Factory system.
		 - AddGameObject
		 Add new game object with serialization.
		 - BuildAndSerialize
		 Get which components are to be in game object and build it.
		 - Shutdown
		 Shuts down Factory system by deleting all game objects.
		 - Destroy
		 To destroy game object.
		 - Update
		 To update game objects.
		 - Clone
		 Make a copy of a game object.
		 - FindObject
		 Finds object given its id.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Factory.h"
#define PARENT_CHILD_GO_ 0
extern bool fullscreen;
bool prev_fullscreen_bool;
int prev_w, prev_h;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
bool skip = false;

/*!*************************************************************************
****
\brief
	Default constructor.
****************************************************************************
***/
Factory::Factory()
{
	LastGameObjectId = 0;
}

/*!*************************************************************************
****
\brief
	Default destructor.
****************************************************************************
***/
Factory::~Factory()
{
	// Empty
}

/*!*************************************************************************
****
\brief
	Initializes Factory system.
****************************************************************************
***/
void Factory::Init()
{
	// Empty
	std::cout << "fullscreen bool:" << std::boolalpha << fullscreen << std::endl;
	std::cout << "window params.txt" << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
	std::cout << "glsetup" << GLSetup::width << "x" << GLSetup::height << std::endl;

	//if ((WINDOW_WIDTH != GLSetup::current_width) && (WINDOW_HEIGHT != GLSetup::current_height))
	//{
	float scaleX = (float)GLSetup::current_width / (float)GLSetup::width;
	float scaleY = (float)GLSetup::current_height / (float)GLSetup::height;
	for (int i = 0; i < this->GetLevel()->GetGameUILayer().size(); ++i)
	{
		if (this->GetLevel()->GetGameUILayer()[i]->layer != "UI")
			continue;
		GameUI* G_UI = dynamic_cast<GameUI*>(this->GetLevel()->GetGameUILayer()[i]->GetComponent(ComponentType::GameUI));
		Transform* trf = dynamic_cast<Transform*>(this->GetLevel()->GetGameUILayer()[i]->GetComponent(ComponentType::Transform));
		//TransScaleUIGO(trf, WINDOW_WIDTH, WINDOW_HEIGHT, GLSetup::current_width, GLSetup::current_height);
		//std::cout << trf->Position.x << "x" << trf->Position.y << std::endl;
		G_UI->SetAABB({ trf->Position.x * scaleX,  trf->Position.y * scaleY }, trf->Scale, { trf->Dimensions.x * scaleX, trf->Dimensions.y * scaleY });

	}
	//}

	//prev_w = WINDOW_WIDTH;prev_h = WINDOW_HEIGHT;
	prev_w = GLSetup::width; prev_h = GLSetup::height;
}

/*!*************************************************************************
****
\brief
	Reads file for component and property data to build game object
\param[in] filename
	File to open
\return
	Pointer to built game object
****************************************************************************
***/
GameObject* Factory::BuildAndSerialize(const std::string& filename)
{
	FILE* f = fopen(filename.c_str(), "r");
	if (!f)
	{
		printf("Error: Failed to open file %s\n", filename.c_str());
		return nullptr;
	}

	// Read file
	GameObject* go = BuildAndSerialize(f);
	fclose(f); return go;
}


/*!*************************************************************************
****
\brief
	Get which components are to be in game object and build it.
	Reads in a game object's properties & then its components' data.
\param[in] fp
	File to open
\return
	Pointer to built game object
****************************************************************************
***/
GameObject* Factory::BuildAndSerialize(FILE* fp)
{
	GameObject* GameObj = new GameObject();	char ObjNameCstr[100]{ 0 };
	if (!fscanf(fp, "\nName: %[^\n]\n", &ObjNameCstr)) { std::cout << "Failed to read GameObject Name\n"; }
	GameObj->SetName(std::string(ObjNameCstr)); std::cout << "Name: " << GameObj->GetName() << std::endl;

	// clear ObjNameCstr
	strcpy_s(ObjNameCstr, 1, "\0");

	if (!fscanf(fp, "\nTag: %[^\n]\n", &ObjNameCstr)) { std::cout << "Failed to read GameObject Tag\n"; }
	GameObj->tag = std::string(ObjNameCstr); std::cout << "Tag: " << GameObj->tag << std::endl;

	// clear ObjNameCstr
	strcpy_s(ObjNameCstr, 1, "\0");

	if (!fscanf(fp, "\nLayer: %[^\n]\n", &ObjNameCstr)) { std::cout << "Failed to read GameObject Layer\n"; }
	GameObj->layer = std::string(ObjNameCstr); std::cout << "Layer: " << GameObj->layer << std::endl;

	// clear ObjNameCstr
	strcpy_s(ObjNameCstr, 1, "\0");

	if (!fscanf(fp, "\nActive: %[^\n]\n", &ObjNameCstr)) { std::cout << "Failed to read GameObject Active\n"; }
	std::cout << std::noboolalpha; GameObj->SetActive(std::stoi(ObjNameCstr)); std::cout << "Active: " << GameObj->GetActive() << std::endl;

	// clear ObjNameCstr
	strcpy_s(ObjNameCstr, 1, "\0");

#if PARENT_CHILD_GO_
	if (!fscanf(fp, "\ID: %[^\n]\n", &ObjNameCstr)) { std::cout << "Failed to read GameObject ID\n"; }
	GameObj->SetObjectId(std::stoi(ObjNameCstr)); std::cout << "ID: " << GameObj->GetObjectId() << std::endl;
	strcpy_s(ObjNameCstr, 1, "\0");
	
	// get parent game object id - -1 if no parent
	if (!fscanf(fp, "\nParent: %[^\n]\n", &ObjNameCstr)) { std::cout << "Failed to read GameObject Parent ID\n"; }
	if (std::stoi(ObjNameCstr) != -1)
	{
		int parent_id = std::stoi(ObjNameCstr); std::cout << "Parent: " << parent_id << std::endl;
		GameObj->parentID = parent_id;
		// std::cout << "Parent: " << GameObj->parent->GetObjectId() << std::endl;
	}
	else {std::cout << "Parent: " << "-1" << std::endl;}
	
	strcpy_s(ObjNameCstr, 1, "\0");
#endif 

	for (int i = 0; i < (int)ComponentType::ComponentTypeSize; ++i) {
		char ComponentName[64]{ 0 };
		(void)fscanf(fp, "{\n");
		//Get a component
		if (fscanf(fp, "Component: %s\n", &ComponentName) == 1) {
			//Match it up to a name
			for (int j = i; j < (int)ComponentType::ComponentTypeSize; ++j) {
				if (strncmp(ComponentName, ComponentTypeStrings[j].c_str(), strlen(ComponentTypeStrings[j].c_str())) == 0)
				{
					//Match found
					i = j;
					break;
				}
			}
			switch (static_cast<ComponentType>(i)) {
			case ComponentType::Transform:
			{
				Transform* t = new Transform;
				GameObj->AddComponent(ComponentType::Transform, t);
				FilePtr::SerializeTransform(fp, t);
				std::cout << "Position: " << t->Position.x << ", " << t->Position.y << ", " << t->Depth << "\n";
				std::cout << "Rotation Angle: " << t->RotationAngle << "\n";
				std::cout << "Rotation Speed: " << t->RotationSpeed << "\n";
				std::cout << "Scale: " << t->Scale.x << ", " << t->Scale.y << "\n";
				break;
			}
			case ComponentType::RigidBody:
			{
				GameObj->AddComponent(ComponentType::RigidBody, new RigidBody);
				RigidBody*&& r = GameObj->GetComponent<RigidBody>();
				FilePtr::SerializeRigidBody(fp, r);
				std::cout << "Solid: " << r->Solid << "\n";
				std::cout << "Inverse Mass: " << r->InverseMass << "\n";
				std::cout << "Force: " << r->Force << "\n";
				break;
			}
			case ComponentType::Collision:
			{
				GameObj->AddComponent(ComponentType::Collision, new Collision);
				break;
			}
			case ComponentType::Logic:
			{
				LogicComponent* lc = new LogicComponent;
				GameObj->AddComponent(ComponentType::Logic, lc);
				FilePtr::SerializeLogicComponent(fp, lc);
				std::cout << GameObj->GetName() << " scripts: ";
				for (size_t k = 0; k < lc->GetLogicIDs().size(); ++k) {
					std::cout << lc->GetLogicIDs()[k].first << " ";
				}
				std::cout << "\n";
				break;
			}
			case ComponentType::GameUI:
			{
				GameUI* G_UI = new GameUI;
				GameObj->AddComponent(ComponentType::GameUI, G_UI);
				FilePtr::SerializeGameUI(fp, G_UI);
				//set the clicking AABB once its done deserializing
				Transform* trf = dynamic_cast<Transform*>(GameObj->GetComponent(ComponentType::Transform));
				G_UI->SetAABB(trf->Position, trf->Scale, trf->Dimensions);
				std::cout << "UI Click AA: " << G_UI->AA.x << ", " << G_UI->AA.y << std::endl;
				std::cout << "UI Click BB: " << G_UI->BB.x << ", " << G_UI->BB.y << std::endl;
				std::cout << "Font Family: " << G_UI->FontFamily << std::endl;
				std::cout << "Font Text: " << G_UI->Text << std::endl;
				std::cout << "Font Size: " << G_UI->size << std::endl;
				std::cout << "Font x offset: " << G_UI->x_offset << std::endl;
				std::cout << "Font y offset: " << G_UI->y_offset << std::endl;
				std::cout << "Font RGB: " << G_UI->FontColor.x << ", " << G_UI->FontColor.y << ", " << G_UI->FontColor.z << std::endl;
				std::cout << "Font Alpha: " << G_UI->Alpha << std::endl;
				std::cout << "Font Visibility: " << G_UI->Visibility << std::endl;
				std::cout << "Font Depth: " << G_UI->depth << std::endl;
				std::cout << "Font Line Spacing: " << G_UI->Line_spacing << std::endl;
				break;
			}
			case ComponentType::Renderer:
			{
				Renderer* RDR = new Renderer;
				GameObj->AddComponent(ComponentType::Renderer, RDR);
				RDR->Deserialize(fp);
				std::cout << "isVisible: " << RDR->isVisible << std::endl;
				std::cout << "isSpritesheet: " << RDR->isSpritesheet << std::endl;
				std::cout << "No. of Sprites: " << RDR->no_of_sprites << std::endl;
				std::cout << "Milliseconds Per Sprite: " << RDR->milliseconds_per_sprite << std::endl;
				std::cout << "Texture Handle: " << RDR->TexHandle << std::endl;
				std::cout << "Texture UV MIN: " << RDR->Tex_UV_Min_COORD.x << ", " << RDR->Tex_UV_Min_COORD.y << std::endl;
				std::cout << "Texture UV MAX: " << RDR->Tex_UV_Max_COORD.x << ", " << RDR->Tex_UV_Max_COORD.y << std::endl;
				std::cout << "RGB_0to1: " << RDR->RGB_0to1.x << ", " << RDR->RGB_0to1.y << ", " << RDR->RGB_0to1.z << std::endl;
				std::cout << "Alpha_0to1: " << RDR->Alpha_0to1 << std::endl;
				break;
			}
			case ComponentType::Controller:
			{
				Controller* CT = new Controller;
				GameObj->AddComponent(ComponentType::Controller, CT);
				CT->Deserialize(fp);
				std::cout << "Loaded Controller\n";
				break;
			}
			case ComponentType::Animate:
			{
				Animate* Anim = new Animate;
				GameObj->AddComponent(ComponentType::Animate, Anim);
				Anim->Deserialize(fp);
				std::cout << "isVisible: " << Anim->isVisible << std::endl;
				std::cout << "isPlaying: " << Anim->isPlaying << std::endl;
				std::cout << "Milliseconds Per Sprite: " << Anim->milliseconds_per_sprite << std::endl;
				std::cout << "Texture Handle: " << Anim->TexHandle << std::endl;
				std::cout << "RGB_0to1: " << Anim->RGB_0to1.x << ", " << Anim->RGB_0to1.y << ", " << Anim->RGB_0to1.z << std::endl;
				std::cout << "Alpha_0to1: " << Anim->Alpha_0to1 << std::endl;
				break;
			}
			case ComponentType::Lighting:
			{
				Lighting* Light = new Lighting;
				GameObj->AddComponent(ComponentType::Lighting, Light);
				Light->Deserialize(fp);
				std::cout << "isVisible: " << Light->isVisible << std::endl;
				std::cout << "LightIntensity: " << Light->Light.LightIntensity << std::endl;
				std::cout << "RGB_0to1: " << Light->Light.Color.x << ", " << Light->Light.Color.y << ", " << Light->Light.Color.z << std::endl;
				break;
			}
			case ComponentType::LogicTargets:
			{
				LogicTargets* LT = new LogicTargets;
				GameObj->AddComponent(ComponentType::LogicTargets, LT);
				FilePtr::SerializeLogicTargets(fp, LT);
				std::cout << "Loaded LogicTargets\n";
				break;
			}
			case ComponentType::Inventory:
			{
				Inventory* IC = new Inventory;
				GameObj->AddComponent(ComponentType::Inventory, IC);
				IC->Deserialize(fp);
				std::cout << "Loaded Inventory\n";
				break;
			}
			case ComponentType::PlayerCombat:
			{
				PlayerCombat* CB = new PlayerCombat;
				GameObj->AddComponent(ComponentType::PlayerCombat, CB);
				CB->Deserialize(fp);
				std::cout << "Loaded PlayerCombat\n";
				break;
			}
			case ComponentType::EnemyCombat:
			{
				EnemyCombat* ECB = new EnemyCombat;
				GameObj->AddComponent(ComponentType::EnemyCombat, ECB);
				ECB->Deserialize(fp);
				std::cout << "Loaded EnemyCombat\n";
				break;
			}
			case ComponentType::Lifetime:
			{
				Lifetime* L = new Lifetime;
				GameObj->AddComponent(ComponentType::Lifetime, L);
				L->Deserialize(fp);
				std::cout << "Loaded Lifetime\n";
				break;
			}
			}

		}
		(void)fscanf(fp, "}\n");
	}
	(void)fscanf(fp, "###\n");
	std::cout << "----------------------------------------------------------------------\n";

	return GameObj;
}

/*!*************************************************************************
****
\brief
	To add new game object
\param[in] filename
	File to open
\return
	Pointer to added game object
****************************************************************************
***/
GameObject* Factory::AddGameObject(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "r");
	if (!fp) { printf("File not found\n"); return nullptr; }
	GameObject* goc = AddGameObject(fp);
	fclose(fp); return goc;
}

/*!*************************************************************************
****
\brief
	To add a new game object.
\param[in] filename
	File to open.
\return
	Pointer to added game object.
****************************************************************************
***/
GameObject* Factory::AddGameObject(FILE* filename)
{
	GameObject* goc = BuildAndSerialize(filename);
	if (goc) goc->Init(++LastGameObjectId);
	return goc;
}

/*!*************************************************************************
****
\brief
	To destroy game object
\param[in] gameObject
	Game object to destroy
****************************************************************************
***/
void Factory::Destroy(GameObject* gameObject)
{
	gameObject->Destroy(); // Call GameObject class Destroy to destruct components
	delete gameObject;
}

/*!*************************************************************************
****
\brief
	To update game objects.
\param[in] dt
	delta time
****************************************************************************
***/
void Factory::Update(float dt)
{
	dt;
	//if full screen changes to window or vice versa

	//if (/*fullscreen != prev_fullscreen_bool || */prev_w != GLSetup::current_width)
	//{
	float scaleX = (float)GLSetup::current_width / (float)GLSetup::width;
	float scaleY = (float)GLSetup::current_height / (float)GLSetup::height;
	for (int i = 0; i < this->GetLevel()->GetGameUILayer().size(); ++i)
	{
		if (this->GetLevel()->GetGameUILayer()[i]->layer != "UI")
			continue;

		GameUI* G_UI = dynamic_cast<GameUI*>(this->GetLevel()->GetGameUILayer()[i]->GetComponent(ComponentType::GameUI));
		Transform* trf = dynamic_cast<Transform*>(this->GetLevel()->GetGameUILayer()[i]->GetComponent(ComponentType::Transform));
		//TransScaleUIGO(trf, prev_w, prev_h, GLSetup::width, GLSetup::height);
		G_UI->SetAABB({ trf->Position.x * scaleX,  trf->Position.y * scaleY }, trf->Scale, { trf->Dimensions.x * scaleX, trf->Dimensions.y * scaleY });
	}
	for (GameObject* GO : DeleteQueue) {
		for (auto it = this->GetLevel()->GetGameObjectArray().begin(); it != this->GetLevel()->GetGameObjectArray().end(); ++it) {
			if ((*it)->GetName() == GO->GetName()) {
				this->GetLevel()->GetGameObjectArray().erase(it);
				break;
			}
		}
		this->GetLevel()->RemoveGOsPtrsFromAllLayers(GO);
		Destroy(GO);
		GO = nullptr;
	}
	for (GameObject* GO : AddQueue) {
		GetLevel()->AddGameObject(GO);
	}
	DeleteQueue.clear();
	AddQueue.clear();
	prev_fullscreen_bool = fullscreen;
}

/*!*************************************************************************
****
\brief
	Shuts down Factory system by deleting all game objects.
****************************************************************************
***/
void Factory::Shutdown()
{
	//FileSerializer::SeralizeLevelToSceneFile(this->GetLevel());
	//FilePtr::SaveLevel(this->GetLevel());
	for (int i = 0; i < this->GetLevel()->GetGOASize(); ++i) {
		this->GetLevel()->GetGameObjectArray()[this->GetLevel()->GetGOASize() - i - 1]->Destroy();
		delete this->GetLevel()->GetGameObjectArray()[this->GetLevel()->GetGOASize() - i - 1];
	}

}

/*!*************************************************************************
****
\brief
	Make a copy of a game object. (Todo: use copy constructor)
\param[in] GameObj
	Game object to be copied
\return
	Reference to clone
****************************************************************************
***/
GameObject* Factory::Clone(GameObject* GameObj) {
	GameObject* clone = new GameObject;	clone->SetName(GameObj->GetName() + "(Clone)");
	clone->tag = GameObj->tag; clone->layer = GameObj->layer; clone->SetActive(GameObj->GetActive());
	clone->SetLayerMask(GameObj->GetLayerMask());
	for (int i = 0; i < GameObj->GetComponentList().size(); ++i) {

		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::Transform) {
			Transform* t = new Transform;
			t->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::Transform, t);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::Collision) {
			Collision* c = new Collision;
			c->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::Collision, c);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::RigidBody) {
			RigidBody* r = new RigidBody;
			r->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::RigidBody, r);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::Logic) {
			LogicComponent* LC = new LogicComponent;
			LC->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::Logic, LC);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::LogicTargets) {
			LogicTargets* LT = new LogicTargets;
			LT->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::LogicTargets, LT);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::GameUI) {
			GameUI* G = new GameUI;
			G->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::GameUI, G);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::Renderer) {
			Renderer* RDR = new Renderer;
			RDR->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::Renderer, RDR);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::Inventory) {
			Inventory* I = new Inventory;
			I->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::Inventory, I);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::PlayerCombat) {
			PlayerCombat* CB = new PlayerCombat;
			CB->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::PlayerCombat, CB);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::EnemyCombat) {
			EnemyCombat* ECB = new EnemyCombat;
			ECB->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::EnemyCombat, ECB);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::Animate) {
			Animate* Anim = new Animate;
			Anim->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::Animate, Anim);
		}
		if (GameObj->GetComponentList()[i]->GetType() == ComponentType::Lighting) {
			Lighting* Light = new Lighting;
			Light->CopyData(GameObj->GetComponentList()[i]);
			clone->AddComponent(ComponentType::Lighting, Light);
		}
	}
	clone->Init(++LastGameObjectId);
	return clone;
}

void Factory::QueueDelete(GameObject* GO)
{
	DeleteQueue.emplace_back(GO);
}

void Factory::QueueAdd(GameObject* GO)
{
	AddQueue.emplace_back(GO);
}

/*!*************************************************************************
****
\brief
	Needed for GO creation outside of main.
\return
	Created Factory.
****************************************************************************
***/
Factory* Factory::Instance() { if (!factory) { factory = new Factory(); } return factory; }

/*!*************************************************************************
****
\brief
	Delete static factory.
****************************************************************************
***/
void Factory::DeleteStaticFactory() { if (factory) { delete factory; factory = nullptr; } }
/**************************************************************************/
/*!
* \brief
*	This function is called in factory::update when the GLFW window has
*	changed sizes, and it scales up if the window is resized from
*	windowed to fullscreen. It will do the opposite for the vice-versa
*	scenario.
*
* \param GO_xform_component
*	Transform component of the selected UI GameObject that is going to be
*	scaled to the repsective screen resolution
* \param prev_width
*	the previous width of the screen to scale from
* \param prev_heighth
*	the previous height of the screen to scale from
* \param current_width
*	the current width of the screen to scale to.
* \param current_height
*	the current height of the screen to scale to.
*/
/**************************************************************************/
void Factory::TransScaleUIGO(Transform* GO_xform_component, int prev_width, int prev_height, int current_width, int current_height)
{
	//std::cout << "prev_width: " << prev_width << std::endl;
	//std::cout << "prev_height: " << prev_height << std::endl;
	//std::cout << "current_width: " << current_width << std::endl;
	//std::cout << "current_height: " << current_height << std::endl;
	//std::cout << "Factory::TransScaleUIGO called " << std::endl;

	//Vec2 pos_translate = { static_cast<float>(current_width - qprev_width)/2.f , static_cast<float>(current_height - prev_height)/2.f };
	//std::cout << "pos BEFORE : " << GO_xform_component->Position.x << " , " << GO_xform_component->Position.y << std::endl;
	Vec2 pos_scale = { static_cast<float>((float)current_width / (float)prev_width), static_cast<float>((float)current_height / (float)prev_height) };
	//std::cout << "Pos Translate : {" << pos_scale.x << " , " << pos_scale.y << std::endl;
	GO_xform_component->SetPosition({ GO_xform_component->Position.x * pos_scale.x, GO_xform_component->Position.y * pos_scale.y });
	//std::cout << "pos After : " << GO_xform_component->Position.x << " , " << GO_xform_component->Position.y << std::endl;
	//std::cout << "Pos Translate : {" << pos_translate.x << " , " << pos_translate.y << std::endl;

	float x_scale = (float)current_width / (float)prev_width;
	float y_scale = (float)current_height / (float)prev_height;
	GO_xform_component->Dimensions.x *= x_scale;
	GO_xform_component->Dimensions.y *= y_scale;
	/*
	if ((prev_width < current_width) && (prev_height < current_height))
	{
		// translate back down
		//std::cout << "Scale UP" << std::endl;
	}
	else
	{
		// translate up
		//std::cout << "Scale DOWN" << std::endl;
	}
	*/
}
/**************************************************************************/
	/*!
	* \brief
	*	This function is used to read a txt file and add multiple gameobjects
	*	in runtime of the game. (Purpose of creating this : Louis needed to add
	*	Options menu game objects during runtime and not have them in the scene
	*	file)
	*
	* \param filename
	*	The file containing the various game objects that are needed to
	*	for the specific purpose.
	*/
	/**************************************************************************/
std::map<std::string, GameObject*> Factory::AddMultipleGameObjects(const std::string& filename)
{
	FilePtr add_GO_file(filename, "r");

	std::map<std::string, GameObject*> added_GOs;

	int c;
	while ((c = fgetc(add_GO_file.p)) != EOF)
	{
		ungetc(c, add_GO_file.p);
		GameObject* curr_GO = BuildAndSerialize(add_GO_file.p);
		if (curr_GO) curr_GO->Init(++LastGameObjectId);
		added_GOs[curr_GO->GetName()] = curr_GO;
	}

	std::cout << "End of adding GameObjects from " << filename << std::endl;

	return added_GOs;
}

/*!*************************************************************************
****
\brief
	Using the parent ID set in the GOs, sets pointers of the children GO vector
	to the parents, and a pointer to the parent in each of the child GO(s).
\param level
	Level w/ GOs to have parents / children GO's set.
****************************************************************************
***/
void Factory::BindGameObjectParents(Level* level)
{
	// for each game object in the GOA, check if it has a parent
	// if so, then add it to the parent's children list, and keep a ptr to it in the child

	for (auto& GO : level->GetGameObjectArray())
	{
		if (GO->parentID > 0)
		{
			std::cout << "GO parent id: " << GO->parentID << std::endl;
			GO->parent = level->GetGameObjectArray()[GO->parentID - 1];
			GO->parent->GetChildren().push_back(GO);
		}
	}
}
