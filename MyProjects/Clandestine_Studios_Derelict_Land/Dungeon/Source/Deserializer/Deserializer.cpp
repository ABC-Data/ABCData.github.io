/******************************************************************************/
/*!
\file    Deserializer.cpp
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function definitions to implement data driven features of game object
		 properties or components. The functions include:
		 - FilePtr
		 Copy Constructor.
		 - FilePtr
		 Delegating Constructor.
		 - ~FilePtr
		 Default destructor.
		 - ReadInt
		 Read ints from file.
		- SerializeTransform
		Serializes transform properties from file.
		- SerializeRigidBody
		Serializes RigidBody properties from file.
		- SerializeWindowParams
		Serializes window properties from file.
		- ReadString
		Read strings from file.
		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Deserializer.h"

#define PARENT_CHILD_GO_ 0

int WINDOW_HEIGHT; // global
int WINDOW_WIDTH; // global

/*!*************************************************************************
****
\brief
	Copy Constructor.
\param[in] n
	Filename
\param[in] a
	Mode of opening file
****************************************************************************
***/
FilePtr::FilePtr(char const* n, char const* a) : p(fopen(n, a)) { // open file n in mode a
	if (!p) throw std::runtime_error("Can't open file");
}

/*!*************************************************************************
****
\brief
	Delegating Constructor.
\param[in] n
	Filename
\param[in] a
	Mode of opening file
****************************************************************************
***/
FilePtr::FilePtr(std::string const& n, char const* a) : FilePtr(n.c_str(), a) { }// delegating ctor

/*!*************************************************************************
****
\brief
	Default destructor.
****************************************************************************
***/
FilePtr::~FilePtr()
{
	fclose(p);
} // dtor

/*!*************************************************************************
****
\brief
	Read ints from file.
\param[in] p
	File
****************************************************************************
***/
int* FilePtr::ReadInt(FILE* ptr) {
	int i = 0;
	int* Data = new int[32]{ 0 }; // free in Serializer
	while (fscanf(ptr, "%d", &Data[i]) > 0) {
		++i;
	}
	return Data;
}

/*!*************************************************************************
****
\brief
	Serializes transform properties from file.
\param[in] ptr
	File to open.
\param[in] tInst
	Pointer to transform component to serialize.
****************************************************************************
***/
void FilePtr::SerializeTransform(FILE* ptr, Transform* tInst)
{
	float x, y, z, ra, rs, sx, sy, dx, dy;
	int success = 1;
	success *= fscanf(ptr, "Position: %f, %f, %f\n", &x, &y, &z);
	success *= fscanf(ptr, "Rotation Angle: %f\n", &ra);
	success *= fscanf(ptr, "Rotation Speed: %f\n", &rs);
	success *= fscanf(ptr, "Scale: %f, %f\n", &sx, &sy);
	success *= fscanf(ptr, "Dimensions: %f, %f\n", &dx, &dy);
	if (!success) {
		std::cout << "Serialize Transform Failed!";
	}
	tInst->SetTransform({ x,y }, ra, rs, { sx,sy }, { dx, dy }, z);
}

/*!*************************************************************************
****
\brief
	Serializes rigidbody properties from file.
\param[in] ptr
	File to open.
\param[in] rbInst
	Pointer to rigidbody component to serialize.
****************************************************************************
***/
void FilePtr::SerializeRigidBody(FILE* ptr, RigidBody* rbInst)
{
	int State;
	float F, IM;
	int success = 1;
	success *= fscanf(ptr, "Solid: %i\n", &State);
	success *= fscanf(ptr, "Inverse Mass: %f\n", &IM);
	success *= fscanf(ptr, "Force: %f\n", &F);

	if (!success) {
		std::cout << "Serialize RigidBody Failed!";
	}
	rbInst->SetRigidBody(State, F, IM);
}

/*!*************************************************************************
****
\brief
	Serializes window properties from file.
\param[in] Filename
	File to open.
\param[in] gInst
	Pointer to graphics component to serialize.
****************************************************************************
***/
bool FilePtr::SerializeWindowParams(std::string Filename, Graphics* gInst)
{
	GLint w, h;
	std::string wname;
	std::ifstream wfs(Filename);
	if (!wfs) {
		std::cout << "Invalid file!\n";
		return true;
	}
	wfs >> w >> h;
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT= h;
	std::getline(wfs, wname);
	wname = wname.substr(1);
	wfs.close();
	//std::cout << "w: " << w << " h: " << h << " name: " << wname << std::endl;
	gInst->SetWindowParams(w, h, wname);
	return false;
}

/*!*************************************************************************
****
\brief
	Read strings from file.
\param[in] p
	File
****************************************************************************
***/
std::string* ReadString(std::string Filename)
{
	std::ifstream PathStream(Filename);
	int i = 0;
	std::string* Data = new std::string[32]; // free in Serializer
	while (std::getline(PathStream, Data[i])) {
		++i;
	}
	return Data;
}

/*!*************************************************************************
****
\brief
	Serializes an entire level and its game objects. Data is read by way of
	its name, and then its game objects' data through the factory.
\param[in] Filename
	File to open.
\return
	Properly built level object containing all game objects and their data.
****************************************************************************
***/
Level* FilePtr::SerializeLevel(std::string Filename) {
	FilePtr levelfile(Filename, "r"); char levelname[32];
	float levelmin{ 0.f };
	float levelmax{ 0.f };
	factory->ResetLastGOID();
	// fscanf returns 1 (succesful input value read) if working
	if (!fscanf(levelfile.p, "Level: %[^\n]\n", &levelname)) { return nullptr; }
	if (!fscanf(levelfile.p, "Bounds: %f, %f\n", &levelmin, &levelmax)) { return nullptr; }
	Level* slevel = new Level(levelname);
	slevel->SetLevelBounds(levelmin, levelmax);
	int c;
	while ((c = fgetc(levelfile.p)) != EOF) {
		ungetc(c, levelfile.p);
		slevel->AddGameObject(factory->AddGameObject(levelfile.p));
	}
	
#if PARENT_CHILD_GO_
	factory->BindGameObjectParents(slevel);
#endif
	return slevel;
}

/*!*************************************************************************
****
\brief
	Saves a level to file.
\param[in] level
	Level to save.
****************************************************************************
***/
void FilePtr::SaveLevel(Level* level)
{
	std::stringstream levelpath;
	levelpath << "./Scene/" << level->GetId() << ".scn";
	FILE* sceneFile = fopen(levelpath.str().c_str(), "w");
	if (!sceneFile) {throw std::runtime_error("Can't open file");}

	fprintf(sceneFile, "Level: %s\n", level->GetId().c_str());
	fprintf(sceneFile, "Bounds: %f, %f\n", level->GetLevelMin(), level->GetLevelMax());
	// save all game objects
	// first: the component numbers for each game object
	for (GameObject*& go : level->GetGameObjectArray())
	{
		fprintf(sceneFile, "Name: %s\n", go->GetName().c_str());
		fprintf(sceneFile, "Tag: %s\n", go->tag.c_str());
		fprintf(sceneFile, "Layer: %s\n", go->layer.c_str());
		fprintf(sceneFile, "Active: %i\n", go->GetActive());
	
#if PARENT_CHILD_GO_
		fprintf(sceneFile, "ID: %i\n", go->GetObjectId());
		if (go->parent) { fprintf(sceneFile, "Parent: %d\n", go->parent->GetObjectId()); }
		else { fprintf(sceneFile, "Parent: -1\n"); }
#endif
		
		// each component's data for each game object
		for (Component*& comp : go->GetComponentList())
		{
			fprintf_s(sceneFile, "{\n");
			fprintf(sceneFile, "Component: ");
			comp->Serialize(sceneFile);
			fprintf_s(sceneFile, "}\n");
		}
		fprintf_s(sceneFile, "###\n");
	}
	fclose(sceneFile);
}

/*!*************************************************************************
****
\brief
	Reads tags from file
\param[in] Filename
	Filepath
\return
	True if serialized, false if could not
****************************************************************************
***/
bool FilePtr::SerializeTags(std::string Filename)
{
	std::ifstream tagfile(Filename);
	if (!tagfile) { std::cerr << "Invalid file!\n"; return false; }
	
	// The tag text file is formatted as follows:
	// tags:
	//  - tag1
	//  - tag2
	//  - ...
	
	// Skip the first line
	std::string line; std::getline(tagfile, line);

	// Read the tags
	while (std::getline(tagfile, line)) {
		// Skip the first two characters
		line = line.substr(line.find_first_of('-') + 2);
		// Add the tag to the list
		GameObject::AddTag(line);
	}
	
	return true;
}

/*!*************************************************************************
****
\brief
	Writes back to the tag file. Any tags marked for deletion will be removed
	by way of not being written back.
\param[in] Filename
	Name of the file to write to.
\return
	True if successful, false otherwise.
****************************************************************************
***/
bool FilePtr::DeserializeTags(std::string Filename)
{
	std::ofstream tagfile(Filename);
	if (!tagfile) {std::cout << "Invalid file!\n"; return false;}
	// The tag text file is formatted as follows:
	// tags:
	//  - tag1
	//  - tag2
	//  - ...
	// Write the first line
	tagfile << "Tags:" << std::endl;
	// Write the tags
	for (std::pair<const unsigned int, std::string> tag : GameObject::GetTagMap()) {
		if (tag.second == "(Removed)") continue;
		tagfile << "  - " << tag.second << std::endl;
	}
	return true;
}

/*!*************************************************************************
****
\brief
	Reads the layers & layer mask for the layer manager.
\param[in] Filename
	Name of the layer file to open.
\return
	True if successful, false otherwise.
****************************************************************************
***/
bool FilePtr::SerializeLayers(std::string Filename)
{
	std::ifstream layerfile(Filename);
	if (!layerfile) { std::cerr << "Invalid file!\n"; return false; }
	
	// The layer text file is formatted as follows:
	// layers:
	//  - layer1
	//  - layer2
	//  - ...

	// Skip the first line
	std::string line; std::getline(layerfile, line);
	
	short layerNum = 0;
	// Read the layers
	while (std::getline(layerfile, line)) {
		line = line.substr(line.find_first_of('-') + 2);
		GameObject::AddLayer(line, layerNum); ++layerNum;
	}

	// if there are < 32 layers used, initialize the remainder with empty strings
	for (int i = layerNum; i < 32; ++i) {GameObject::AddLayer("", i);}
	
	// after the layers is a 32 bit of 1's & 0's representing the layer mask
	// the first bit is the first layer, the second bit is the second layer, etc.
	std::getline(layerfile, line); // skip masking header
	
	std::getline(layerfile, line);
	GameObject::GetLayerMask() = std::bitset<32>(line);
	
	return true;
}

/*!*************************************************************************
****
\brief
	Writes back the layers & layer mask to the layer file.
\param[in] Filename
	Name of the layer file to open.
\return
	True if deserialized successfully, false otherwise.
****************************************************************************
***/
bool FilePtr::DeserializerLayers(std::string Filename)
{
	std::ofstream layerFile(Filename);
	if (!layerFile) { std::cout << "Invalid file!\n"; return false; }
	
	layerFile << "Layers:" << std::endl;
	for (std::string layer : GameObject::GetLayers()) {
		layerFile << "  - " << layer << std::endl;
	}

	// Write the layer mask
	layerFile << "Layer Mask:" << std::endl;
	
	// layer mask is to be written back in the reverse manner(!)
	for (int i = 31; i >= 0; --i) {layerFile << GameObject::GetLayerMask()[i];}

	GameObject::GetLayers().clear();
	
	return true;
}

/*!*************************************************************************
****
\brief
	Reads Logic component data
\param[in] Filename
	Filepath
\param[in] lInst
	Logic Component to store data to
****************************************************************************
***/
void FilePtr::SerializeLogicComponent(FILE* ptr, LogicComponent* lInst) {
	int discard = fscanf(ptr, "Scripts: ");
	while (true) {
		char buffer[100]{ 0 };
		int success = fscanf(ptr, "%[^,\n]", &buffer);
		if (!success) break;
		lInst->AddScriptKey(buffer);
		success = fscanf(ptr, ",");
	}
	discard = fscanf(ptr, "\n");
}

/*!*************************************************************************
****
\brief
	Reads GameUI component data
\param[in] Filename
	Filepath
\param[in] G_UI_Inst
	GameUI component to store data to
****************************************************************************
***/
void FilePtr::SerializeGameUI(FILE* ptr, GameUI* G_UI_Inst)
{
	// no need for AABB yet because we will use the transform compononent's
	// dimensions of the GO
	//int success = 1;
	//success *= fscanf(ptr, "\nTexHandle: %s\n", &tex_filepath);
	//tex_filepath = "../images/GameUI/ExitBtn1.png";
	
	//DATA VARIABLES
	char FF_str[100]{ 0 };
	char Text_str[200]{ 0 };
	float temp_size, temp_x_off, temp_y_off,
		temp_r, temp_g, temp_b, temp_alpha, temp_depth;
	int  temp_vis, temp_linespacing;

	int success = 1;

	//font family string
	int discard = fscanf(ptr, "FontFamily: ");
	success *= fscanf(ptr, "%[^,\n]\n", &FF_str);

	//Text string
	discard = fscanf(ptr, "Text: ");
	success *= fscanf(ptr, "%[^,\n]\n", &Text_str);

	//size float
	success *= fscanf(ptr, "Font Size: %f\n", &temp_size);

	//x_offset float
	success *= fscanf(ptr, "x_offset: %f\n", &temp_x_off);

	//y_offset float
	success *= fscanf(ptr, "y_offset: %f\n", &temp_y_off);

	//Font Color Vec3
	success *= fscanf(ptr, "FontColor: %f, %f, %f\n", &temp_r, &temp_g, &temp_b);

	//Alpha float
	success *= fscanf(ptr, "Alpha: %f\n", &temp_alpha);

	//Visibility bool
	success *= fscanf(ptr, "Visibility: %i\n", &temp_vis);

	//Depth float
	success *= fscanf(ptr, "Depth: %f\n", &temp_depth);

	//Line Spacing float
	success *= fscanf(ptr, "Line Spacing: %d\n", &temp_linespacing);


	if (!success)
	{
		std::cout << "FAILED TO LOAD TEXTURE OF GAMEUI OBJ" << std::endl;
		G_UI_Inst->SetGameUI("", "",
			1.f, 0.f, 0.f,
			{ 0.f,0.f,0.f }, 1.f, false, 0.0f,0);
		return;
	}
	else
	{
		G_UI_Inst->SetGameUI(FF_str, Text_str,
			temp_size, temp_x_off, temp_y_off,
			{ temp_r,temp_g,temp_b }, temp_alpha, (bool)temp_vis , temp_depth, temp_linespacing);
	}


	discard = fscanf(ptr, "\n");
	
}

/*!*************************************************************************
****
\brief
	Reads Logic Target component data
\param[in] Filename
	Filepath
\param[in] LT
	Logic Target Component to store data to
****************************************************************************
***/
void FilePtr::SerializeLogicTargets(FILE* ptr, LogicTargets* LT) {
	int discard = fscanf(ptr, "Logic Target IDs: ");
	while (true) {
		unsigned int ID{};
		int success = fscanf(ptr, "%u", &ID);
		if (!success) break;
		LT->GOIDs.push_back((size_t)ID);
	}
	discard = fscanf(ptr, "|\n");
}
/*!*************************************************************************
	****
	\brief
		Reads Renderer	component data
	\param[in] Filename
		Filepath
	\param[in] LT
		Logic Target Component to store data to
	****************************************************************************
	***/
void FilePtr::SerializeRenderer(FILE* ptr, Renderer* Renderer_Inst)
{
	//std::cout << "[FilePtr::SerializeRenderer] called\n";

	int isSprite, sprite_cnt, millisecs_per_sprite, visibility;
	char texhdl[100]{ 0 };
	float uv_min_x, uv_min_y, uv_max_x, uv_max_y;
	float red, green, blue;
	float alpha;

	int success = 1;
	int discard = 0;

	//bool
	success *= fscanf(ptr, "isVisible: %i\n", &visibility);
	//bool
	success *= fscanf(ptr, "isSpritesheet: %i\n", &isSprite);
	//int
	success *= fscanf(ptr, "No. of Sprites: %i\n", &sprite_cnt);
	//int
	success *= fscanf(ptr, "Milliseconds Per Sprite: %i\n", &millisecs_per_sprite);
	//std::string
	discard = fscanf(ptr, "Texture Handle: ");
	success *= fscanf(ptr, "%[^,\n]\n", &texhdl);

	//vec2 uv_min
	success *= fscanf(ptr, "Texture UV MIN: %f, %f\n", &uv_min_x, &uv_min_y);

	//vec2 uv_min
	success *= fscanf(ptr, "Texture UV MAX: %f, %f\n", &uv_max_x, &uv_max_y);

	//vec3, 3 floats
	success *= fscanf(ptr, "RGB_0to1: %f, %f, %f\n", &red, &green, &blue);

	//float for alpha
	success *= fscanf(ptr, "Alpha_0to1: %f\n", &alpha);

	if (!success) {
		std::cout << "Serialize Renderer Failed!";
		Renderer_Inst->SetRenderer(false, false,
			1, 0, "",
			{ 0.f, 0.f }, { 1.f,1.f }, { 0.f, 0.f, 0.f }, 1.f);
		return;
	}


	Renderer_Inst->SetRenderer(static_cast<bool>(visibility), static_cast<bool>(isSprite),
		sprite_cnt, millisecs_per_sprite,
		texhdl,
		{ uv_min_x , uv_min_y }, { uv_max_x , uv_max_y }, { red, green, blue }, alpha);
}

void FilePtr::SerializeTextures(FILE* ptr, Assets& assets)
{
	(void)ptr; (void)assets;
}


/**************************************************************************/
/*!
  \brief
	Deserializes textures read in Texture.txt.

  \param Filename
	File path of Texture.txt.

  \param AssetsManager
	AssetsManager to keep textures.
*/
/**************************************************************************/
void FilePtr::DeserializeTextures(std::string Filename, Assets& AssetsManager) {
	std::ifstream inFile;
	inFile.open(Filename);
	if (inFile.is_open()) {
		std::string id, texfile;
		while (inFile >> id >> texfile)
			AssetsManager.AddTexture(id, texfile.c_str());
	}
	else {
		std::cout << "Failed to open file.\n";
	}
	inFile.close();
}
/**************************************************************************/
/*!
  \brief
	Deserializes shaders read in Shaders.txt.

  \param Filename
	File path of Shaders.txt.

  \param AssetsManager
	AssetsManager to keep shaders.
*/
/**************************************************************************/
void FilePtr::DeserializeShaders(std::string Filename, Assets& AssetsManager) {
	std::ifstream inFile;
	inFile.open(Filename);
	if (inFile.is_open()) {
		std::string id, vert, frag;
		while (inFile >> id >> vert >> frag)
			AssetsManager.AddShaderFile(id, vert.c_str(), frag.c_str());
	}
	else {
		std::cout << "Failed to open file.\n";
	}
	inFile.close();
}
/**************************************************************************/
/*!
  \brief
	Deserializes sprites read in Sprites.txt.

  \param Filename
	File path of Sprites.txt.

  \param AssetsManager
	AssetsManager to keep sprites.
*/
/**************************************************************************/
void FilePtr::DeserializeSprites(std::string Filename, Assets& AssetsManager) {
	(void)AssetsManager;
	std::ifstream inFile;
	inFile.open(Filename);
	if (inFile.is_open()) {

		std::string id, spritefile[20];
		std::vector<const char*> sprites;
		while (std::getline(inFile, id)) {
			int i = 0;
			while (std::getline(inFile, spritefile[i])) {
				if (!strcmp(spritefile[i].c_str(), ""))
					break;
				++i;
			}
			for (int j = 0; j < i; ++j)
			{
				sprites.push_back(spritefile[j].c_str());
			}
			//AssetsManager.AddSpriteSet(id, sprites);
		}
	}
	else {
		std::cout << "Failed to open file.\n";
	}
	inFile.close();
}

void FilePtr::DeserializeSpriteSheet(std::string Filename, Assets& AssetsManager) {
	std::ifstream inFile;
	inFile.open(Filename);
	if (inFile.is_open()) {
		std::string id, spritesheet;
		int frames_per_row, frames_per_column;
		while (inFile >> id >> spritesheet >> frames_per_row >> frames_per_column)
			AssetsManager.AddSpriteAnimation(id, spritesheet.c_str(), frames_per_row, frames_per_column);
	}
	else {
		std::cout << "Failed to open file.\n";
	}
	inFile.close();
}
/**************************************************************************/
/*!
  \brief
	Deserializes fonts read in Fonts.txt.

  \param Filename
	File path of Fonts.txt.

  \param AssetsManager
	AssetsManager to keep fonts.
*/
/**************************************************************************/
void FilePtr::DeserializeFonts(std::string Filename, Assets& AssetsManager) {
	std::ifstream inFile;
	inFile.open(Filename);
	if (inFile.is_open()) {
		std::string id, fontfile;
		while (inFile >> id >> fontfile)
			AssetsManager.AddFont(id, fontfile.c_str());
	}
	else {
		std::cout << "Failed to open file.\n";
	}
	inFile.close();
}