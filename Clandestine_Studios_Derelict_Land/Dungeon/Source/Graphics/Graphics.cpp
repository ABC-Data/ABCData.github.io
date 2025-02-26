/******************************************************************************/
/*!
\file    Graphics.cpp
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\co-author	Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    September 27, 2022
\brief   Graphics System Implementation

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Graphics.h"

//#define RENDER_TO_IMGUI_GAMEWINDOW
//#define ACCURATE_BUTTONS_BUT_SHIFTED_GAMEUI

uint64_t GW::Statics::texID;
bool GW::Statics::GameWindowOpen;

bool run_toggle;

DebugData debug_data;
bool show_debug_Data;

extern bool editor_open;

int gizmosXLength = 100, gizmosYLength = 100;

//Camera
extern bool keystateI, keystateK, keystateJ, keystateL, keystateF11;
Vec2 camera_drag_pos; // original position of camera when dragging
bool fullscreen;


/**************************************************************************/
/*!
  \brief
	Constructor for Graphics
*/
/**************************************************************************/
Graphics::Graphics() : WindowWidth{}, WindowHeight{}, CurrentWidth{}, CurrentHeight{} {
	WindowName = "GAME";
}
/**************************************************************************/
	/*!
	  \brief
		Destructor for Graphics.
	*/
	/**************************************************************************/
Graphics::~Graphics() {
}
/**************************************************************************/
/*!
  \brief
	Initialises GLFW window and Graphics.

  \param width
	Sets width of window size.(default set to 1600).

  \param height
	Sets height of window size.(default set to 900).

  \param name
	Displays Game Name on window bar.(default set to "Game Name").
*/
/**************************************************************************/
void Graphics::Init() {
	if (!GLSetup::init(WindowWidth, WindowHeight, WindowName)) {
		std::cout << "Unable to create OpenGL context" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//SetVSync(!editor_open);

	run_toggle = false;

#ifndef _DEBUG
	fullscreen = true;
#else
	fullscreen = false;
#endif

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
#ifdef ACCURATE_BUTTONS_BUT_SHIFTED_GAMEUI
	GLSetup::width = GLSetup::current_width = CurrentWidth = fullscreen ? glfwGetVideoMode(monitor)->width : WindowWidth;
	GLSetup::height = GLSetup::current_height = CurrentHeight = fullscreen ? glfwGetVideoMode(monitor)->height : WindowHeight;
	
#else
	GLSetup::current_width = CurrentWidth = fullscreen ? glfwGetVideoMode(monitor)->width : WindowWidth;
	GLSetup::current_height = CurrentHeight = fullscreen ? glfwGetVideoMode(monitor)->height : WindowHeight;
#endif	

	mFrameBuffer = std::make_shared<FrameBuffer>(CurrentWidth, CurrentHeight);

	FilePtr::DeserializeTextures("./Data/SpriteSheets.txt", MasterObjectList->MasterAssets);
	FilePtr::DeserializeShaders("./Data/Shaders.txt", MasterObjectList->MasterAssets);
	FilePtr::DeserializeFonts("./Data/Fonts.txt", MasterObjectList->MasterAssets);
	FilePtr::DeserializeSpriteSheet("./Data/SpriteAnimations.txt", MasterObjectList->MasterAssets);

	this->GetLevel()->GetAssets() = &MasterObjectList->MasterAssets;

	Batch::line_shader_program = MasterObjectList->MasterAssets.GetShader("Line");
	Batch::quad_shader_program = MasterObjectList->MasterAssets.GetShader("Quad");
	FontsRender::text_shdrpgm = MasterObjectList->MasterAssets.GetShader("Font");

	Batch::Init();
	FontsRender::Init(GetWindowWidth(), GetWindowHeight());
	FontsRender::ShaderProgram_Init();

	Batch::camera.SetCameraSpeed(5.f);
	show_debug_Data = !fullscreen;
}

/**************************************************************************/
/*!
  \brief
	Draws the gizmos representing the transform mode of the selected object.
	Any selected object with the GameUI component will have its gizmos position
	with the main camera's position factored in.
  \param mode
	Mode of the gizmos to be drawn. Translation, Rotation, or Scaling.
  \param isUI
	Whether the gizmos is for an object that contains the GameUI component.
*/
/**************************************************************************/
void DrawGizmos(const TransformMode mode, const bool isUI)
{
	Transform* go = dynamic_cast<Transform*>(editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent(ComponentType::Transform));
	if (go == nullptr) return;
	// If the selected object has the GameUI component, then the gizmos will be drawn with the main camera's position temporarily added in.
	// AKA, the GO itself is temporarily moved.
	if (isUI) { go->Position += Vec2{ Batch::camera.GetCameraPosition().x, Batch::camera.GetCameraPosition().y }; }

	if (mode == TransformMode::Translate)
	{
		Batch::DrawLine({ go->Position.x, go->Position.y, 1.0f }, { go->Position.x + gizmosXLength, go->Position.y, 1.0f }, { 1,0,0,1 });
		Batch::DrawLine({ go->Position.x, go->Position.y, 1.0f }, { go->Position.x, go->Position.y + gizmosYLength, 1.0f }, { 0,1,0,1 });
		// ... and a box at the corner of the gizmos. (0.9f is the depth of the 'gizmos')
		Batch::DrawQuad({ go->Position.x + gizmosXLength * 0.15f, go->Position.y + gizmosYLength * 0.15f, 1.0f },
			{ 30.0f, 30.0f }, { 100,100 }, { 0,0,1,0.5f }, 0.0f);
	}
	else if (mode == TransformMode::Rotate)
	{
		float x1, x2, y1, y2; const float radius = 50.0f;
		// draw a (rather expensive) circle of 360 lines
		for (int i = 0; i < 360; ++i)
		{
			x1 = go->Position.x + cos(glm::radians(float(i))) * radius;
			y1 = go->Position.y + sin(glm::radians(float(i))) * radius;
			x2 = go->Position.x + cos(glm::radians(i + 1.0f)) * radius;
			y2 = go->Position.y + sin(glm::radians(i + 1.0f)) * radius;
			Batch::DrawLine({ x1, y1, 1.0f }, { x2, y2, 1.0f }, { 1, 0, 0, 1 });
		}
	}
	else if (mode == TransformMode::Scale)
	{
		Batch::DrawLine({ go->Position.x, go->Position.y, 0.99f }, { go->Position.x + gizmosXLength, go->Position.y, 1.0f }, { 1,0,0,1 });
		Batch::DrawLine({ go->Position.x, go->Position.y, 0.99f }, { go->Position.x, go->Position.y + gizmosYLength, 1.0f }, { 0,1,0,1 });
		// draw small quads at the ends of the two lines
		Batch::DrawQuad({ go->Position.x + gizmosXLength, go->Position.y, 1.0f }, { 15.0f, 15.0f }, { 100,100 }, { 1,0,0,1 });
		Batch::DrawQuad({ go->Position.x, go->Position.y + gizmosYLength, 1.0f }, { 15.0f, 15.0f }, { 100,100 }, { 0,1,0,1 });
		// and another at the center for general scaling
		Batch::DrawQuad({ go->Position.x, go->Position.y, 1.0f }, { 30.0f, 30.0f }, { 100,100 }, { 1,1,1,1 });
	}

	// Once the gizmos for any UI object is drawn, the GO's position is then returned to its original value.
	if (isUI) { go->Position -= Vec2{ Batch::camera.GetCameraPosition().x, Batch::camera.GetCameraPosition().y }; }
}

/**************************************************************************/
/*!
  \brief
	Graphics updates and draw functions.
  \param dt
	Delta time.
*/
/**************************************************************************/
void Graphics::Update(float dt) {
	if (this->GetLevel()->GetAssets() == nullptr) {
		this->GetLevel()->GetAssets() = &MasterObjectList->MasterAssets;
	}
	//SetVSync(!editor_open);
	if (keystateF11) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		fullscreen = !fullscreen;
#ifdef ACCURATE_BUTTONS_BUT_SHIFTED_GAMEUI
		GLSetup::width = GLSetup::current_width = CurrentWidth = fullscreen ? glfwGetVideoMode(monitor)->width : WindowWidth;
		GLSetup::height = GLSetup::current_height = CurrentHeight = fullscreen ? glfwGetVideoMode(monitor)->height : WindowHeight;
#else
		GLSetup::current_width = CurrentWidth = fullscreen ? glfwGetVideoMode(monitor)->width : WindowWidth;
		GLSetup::current_height = CurrentHeight = fullscreen ? glfwGetVideoMode(monitor)->height : WindowHeight;
#endif	
		mFrameBuffer = std::make_shared<FrameBuffer>(CurrentWidth, CurrentHeight);
		glfwSetWindowMonitor(GLSetup::ptr_window, fullscreen ? glfwGetPrimaryMonitor() : NULL, 500, 200, CurrentWidth, CurrentHeight, glfwGetVideoMode(monitor)->refreshRate);
		keystateF11 = false;
	}

	//if(GW::Statics::GameWindowOpen)mFrameBuffer->BindFrameBuffer();
	if (editor_open) {
		//#ifdef RENDER_TO_IMGUI_GAMEWINDOW
		mFrameBuffer->BindFrameBuffer();
		//#endif
	}
	// Emulate functions
	if (Input::mouseObj.triggered) {
		Input::mouseObj.triggered = false;
	}
	if (Input::keyObj.triggered) {
		Input::keyObj.triggered = false;
	}
	glfwPollEvents();
	GLSetup::fps_counter();

	debug_data.dt = dt;
	debug_data.Frame_cnt = static_cast<int>(GLSetup::frame_no);
	debug_data.GO_cnt = static_cast<int>(this->GetLevel()->GetGameObjectArray().size());
	debug_data.Mouse_x_screencoord = Input::mouseObj.x;
	debug_data.Mouse_y_screencoord = GLSetup::current_height - Input::mouseObj.y;


	// Camera
	//Transform* cam = (this->GetLevel()->GetGameObjectArray())[0]->GetComponent<Transform>();
	Transform* cam = (GameObject::FindWithTag("Camera", this->GetLevel()->GetGameObjectArray())->GetComponent<Transform>());
	//Batch::camera.SetCameraPosition(glm::vec3(cam->Position.x - GLSetup::width * 0.5f, cam->Position.y - GLSetup::height * 0.5f, 0.f));
	if (editor_open && editor->GetState() != EditorState::EditorPlaying)
	{
		// Mark positon of the camera to move from
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
		{
			if (ImGui::IsWindowNavFocusable(ImGui::FindWindowByName("Game Window"))) { camera_drag_pos = cam->Position; }
		}
		else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) { camera_drag_pos = { 0.0f, 0.0f }; }

		ImVec2 m_ImGuiMouseDragDelta; // Dragging to move camera
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) || ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			m_ImGuiMouseDragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

			cam->Position.x = camera_drag_pos.x - m_ImGuiMouseDragDelta.x;
			cam->Position.y = camera_drag_pos.y + m_ImGuiMouseDragDelta.y;

			//	cam->Position.x -= m_ImGuiMouseDragDelta.x * dt;
			//	cam->Position.y += m_ImGuiMouseDragDelta.y * dt;
		}
		else
		{
			cam->Position.x = keystateJ ? cam->Position.x - Batch::camera.GetCameraSpeed() : cam->Position.x;
			cam->Position.x = keystateL ? cam->Position.x + Batch::camera.GetCameraSpeed() : cam->Position.x;
			cam->Position.y = keystateI ? cam->Position.y + Batch::camera.GetCameraSpeed() : cam->Position.y;
			cam->Position.y = keystateK ? cam->Position.y - Batch::camera.GetCameraSpeed() : cam->Position.y;
		}
	}
	Batch::camera.SetCameraPosition(glm::vec3(cam->Position.x - GLSetup::width * 0.5f, cam->Position.y - GLSetup::height * 0.5f, 0.f));

	// Draw functions
	glfwSwapBuffers(GLSetup::ptr_window);
	// clear color buffer | depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	//glClearDepth(1.f);
	Batch::SetLineWidth(5.f);
	Batch::StartBatch();


	if (editor_open) {
		// Lights
		std::vector<Lighting::PointLight> pointlights{};

		for (size_t i = 1; i < this->GetLevel()->GetGameObjectArray().size(); ++i)
		{
			// inactive gameobjects will not be drawn (colliders are still active though)
			if (this->GetLevel()->GetGameObjectArray()[i]->GetActive() == false) { continue; }

			std::string_view goLayer = this->GetLevel()->GetGameObjectArray()[i]->layer;
			// check if the bitmask of the layer is set to 1 (visible)
			if (!GameObject::GetLayerMask().test(std::distance(GameObject::GetLayers().begin(), std::find(GameObject::GetLayers().begin(), GameObject::GetLayers().end(), goLayer)))) { continue; }

			Transform* go = dynamic_cast<Transform*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Transform));
			Collision* col = dynamic_cast<Collision*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Collision));
			GameUI* GAMEUI = dynamic_cast<GameUI*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::GameUI));
			Renderer* RDR = dynamic_cast<Renderer*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Renderer));
			Animate* Anim = dynamic_cast<Animate*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Animate));
			Lighting* Light = dynamic_cast<Lighting*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Lighting));
			

			if (GAMEUI != nullptr)
			{	
				//for simple culling
				glm::vec2 Pos{ (go->Position.x + go->Dimensions.x) * go->Scale.x , (go->Position.y + go->Dimensions.y) * go->Scale.y };
				if (Pos.x > cam->Position.x + GLSetup::current_width / 2
					&& Pos.x < cam->Position.x - GLSetup::current_width / 2
					&& Pos.y > cam->Position.y + GLSetup::current_height / 2
					&& Pos.y < cam->Position.y - GLSetup::current_height / 2) continue;
				if (Anim != nullptr && Anim->GetVisibility()) {
					if (this->GetLevel()->GetGameObjectArray()[i]->layer == "UI") {
						Batch::DrawSprite({ Batch::camera.GetCameraPosition().x + go->Position.x, Batch::camera.GetCameraPosition().y + go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texid,
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texcoords[this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx],
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texsize, { Anim->RGB_0to1.x, Anim->RGB_0to1.y, Anim->RGB_0to1.z, Anim->Alpha_0to1 }, go->RotationAngle);
					}
					else {
						Batch::DrawSprite({ go->Position.x, go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texid,
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texcoords[this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx],
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texsize, { Anim->RGB_0to1.x, Anim->RGB_0to1.y, Anim->RGB_0to1.z, Anim->Alpha_0to1 }, go->RotationAngle);
					}
				}
				else if (RDR != nullptr && RDR->GetVisibility()) // if active 
				{
					if (this->GetLevel()->GetGameObjectArray()[i]->layer == "UI")
					{
						if (RDR->GetTexHandle() == "" || RDR->GetTexHandle() == "-")
							Batch::DrawQuad({ Batch::camera.GetCameraPosition().x + go->Position.x, Batch::camera.GetCameraPosition().y + go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
								{ RDR->RGB_0to1.x, RDR->RGB_0to1.y, RDR->RGB_0to1.z, RDR->Alpha_0to1 }, go->RotationAngle);

						else {
							Batch::DrawQuadWithPixelCoords({ Batch::camera.GetCameraPosition().x + go->Position.x,Batch::camera.GetCameraPosition().y + go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
								this->GetLevel()->GetAssets()->GetTexture(RDR->GetTexHandle()).texid, { RDR->GetUVMin().x,  RDR->GetUVMin().y }, { RDR->GetUVMax().x,  RDR->GetUVMax().y },
								this->GetLevel()->GetAssets()->GetTexture(RDR->GetTexHandle()).texsize, { RDR->RGB_0to1.x, RDR->RGB_0to1.y, RDR->RGB_0to1.z, RDR->Alpha_0to1 });
						}
					}
					else
					{
						if (RDR->GetTexHandle() == "" || RDR->GetTexHandle() == "-") // if texture handle is " or "-"
						{
							Batch::DrawQuad({ go->Position.x, go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
								{ RDR->RGB_0to1.x, RDR->RGB_0to1.y, RDR->RGB_0to1.z, RDR->Alpha_0to1 }, go->RotationAngle);
						}
						else {
							Batch::DrawQuadWithPixelCoords({ go->Position.x, go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
								this->GetLevel()->GetAssets()->GetTexture(RDR->GetTexHandle()).texid, { RDR->GetUVMin().x,  RDR->GetUVMin().y }, { RDR->GetUVMax().x,  RDR->GetUVMax().y },
								this->GetLevel()->GetAssets()->GetTexture(RDR->GetTexHandle()).texsize, { RDR->RGB_0to1.x, RDR->RGB_0to1.y, RDR->RGB_0to1.z, RDR->Alpha_0to1 });
						}
					}
				}
			}
			if (Light != nullptr) {
				glm::vec3 pos = glm::translate(glm::mat4(1.0f), glm::vec3(go->Position.x, go->Position.y, go->Depth)) * glm::vec4{ 0.5f, 0.5f, 0.f, 1.f };
				if (Light->GetVisibility())
					pointlights.push_back(Lighting::PointLight(pos, Light->Light.Color * 255.f, Light->Light.LightIntensity));
				else
					pointlights.push_back(Lighting::PointLight(pos, Light->Light.Color * 255.f, 0.f));
			}

			if (col != nullptr)
			{
				if (col->showBox)
				{

					if (col->go_is_colliding == true)
						Batch::DrawBox({ go->Position.x, go->Position.y, 1.0f }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y }, { 1,0,0,1 });
					else
						Batch::DrawBox({ go->Position.x, go->Position.y, 1.0f }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y }, { 0,1,0,1 });
				}
			}
			//std::cout << this->GetLevel()->GetGameObjectArray()[i]->GetName() << std::endl;
		}

		// If there is a game object selected, draw a simple 'gizmos' depending on the transform mode selected in the GameWindow
		if (editor->GetPropertyEditor().GetSelectedGameObject() != nullptr) { DrawGizmos(editor->GetGameWindow().currentTransformMode, editor->GetPropertyEditor().GetSelectedGameObject()->layer == "UI"); }

		//GAME UI
		//GameUI::RenderButtonWImage({ 70,825,0.9999 }, { 25,25 }, { 200.f, 200.f }, AssetsManager.GetTexture("ExitBtn"));
		if (pointlights.size() > 0)
			Batch::SetLight(pointlights);
		Batch::EndBatch();
		Batch::Flush();

		// THIS SPACE RESERVED FOR FONT 
		for (int i = 0; i < this->GetLevel()->GetGameUILayer().size(); ++i)
		{

			Transform* trf = dynamic_cast<Transform*>((this->GetLevel()->GetGameUILayer())[i]->GetComponent(ComponentType::Transform));
			GameUI* gui = dynamic_cast<GameUI*>((this->GetLevel()->GetGameUILayer())[i]->GetComponent(ComponentType::GameUI));
			if (gui != nullptr && gui->GetText() != "-" && gui->GetText() != "" && gui->Visibility != false && gui->GetComponentOwner()->GetActive())
			{
				//for UI LAYER OBJS
				if (this->GetLevel()->GetGameUILayer()[i]->layer == "UI")
				{
					FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont(gui->GetTextFontFamily()), (this->GetLevel()->GetGameUILayer())[i]);
				}
				if (this->GetLevel()->GetGameUILayer()[i]->layer == "Default")
				{
					//world to vp
					float pos_x_vp = trf->Position.x - Batch::camera.GetCameraPosition().x;
					float pos_y_vp = trf->Position.y - Batch::camera.GetCameraPosition().y;


					//if text is outside of the bounds, dont render text, skip
					if (pos_x_vp < 0.f || pos_x_vp > GLSetup::current_width ||
						pos_y_vp < 0.f || pos_y_vp > GLSetup::current_height)
						continue;


					FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont(gui->GetTextFontFamily().c_str()),
						gui->GetText(),
						pos_x_vp + gui->x_offset,
						pos_y_vp + gui->y_offset,
						gui->GetFontDepth(),
						gui->GetSize(),
						glm::vec3(gui->GetFontColor_vec3().x, gui->GetFontColor_vec3().y, gui->GetFontColor_vec3().z),
						gui->Alpha);

				}
			}
		}

		if (show_debug_Data)
		{
			FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont("Roboto"), "Ohmega Engine", 0.1f * WindowWidth, 0.90f * WindowHeight, 1.f, 0.9999f , glm::vec3(1.0f, 1.0f, 0.0f),1.0f);
			FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont("Custom"), "By Clandestine Studios", 0.1f * WindowWidth, 0.85f * WindowHeight, 1.f, 0.9999f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
			FontsRender::RenderDEBUG_INFO(this->GetLevel()->GetAssets()->GetFont("Roboto"), debug_data, 0.1f * WindowWidth, 0.8f * WindowHeight, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
			FontsRender::RenderDEBUG_Performance_Data(this->GetLevel()->GetAssets()->GetFont("Roboto"), 0.025f * WindowHeight, 0.1f * WindowWidth, 0.75f * WindowHeight, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
			
		} // DEBUG
	}
	else {
		// Rendering game objects in the current scene
		for (size_t i = 1; i < this->GetLevel()->GetGameObjectArray().size(); ++i)
		{
			// inactive gameobjects will not be drawn (colliders are still active though)
			if (this->GetLevel()->GetGameObjectArray()[i]->GetActive() == false) { continue; }

			std::string_view goLayer = this->GetLevel()->GetGameObjectArray()[i]->layer;
			// check if the bitmask of the layer is set to 1 (visible)
			if (!GameObject::GetLayerMask().test(std::distance(GameObject::GetLayers().begin(), std::find(GameObject::GetLayers().begin(), GameObject::GetLayers().end(), goLayer)))) { continue; }

			Transform* go = dynamic_cast<Transform*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Transform));
			Collision* col = dynamic_cast<Collision*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Collision));
			GameUI* GAMEUI = dynamic_cast<GameUI*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::GameUI));
			Renderer* RDR = dynamic_cast<Renderer*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Renderer));
			Animate* Anim = dynamic_cast<Animate*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Animate));
			//Lighting* Light = dynamic_cast<Lighting*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Lighting));
			
			

			if (GAMEUI != nullptr)
			{
				//for simple culling
				glm::vec2 Pos{ (go->Position.x + go->Dimensions.x) * go->Scale.x , (go->Position.y + go->Dimensions.y) * go->Scale.y };
				if (Pos.x > cam->Position.x + GLSetup::current_width / 2
					&& Pos.x < cam->Position.x - GLSetup::current_width / 2
					&& Pos.y > cam->Position.y + GLSetup::current_height / 2
					&& Pos.y < cam->Position.y - GLSetup::current_height / 2) continue;

				if (this->GetLevel()->GetGameObjectArray()[i]->layer == "UI")
					continue;

				if (Anim != nullptr && Anim->GetVisibility()) {
					if (this->GetLevel()->GetGameObjectArray()[i]->layer != "UI") {
						Batch::DrawSprite({ go->Position.x, go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texid,
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texcoords[this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx],
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texsize, { Anim->RGB_0to1.x, Anim->RGB_0to1.y, Anim->RGB_0to1.z, Anim->Alpha_0to1 }, go->RotationAngle);
					}
				}
				else if (RDR != nullptr && RDR->GetVisibility()) // if active 
				{
					if (this->GetLevel()->GetGameObjectArray()[i]->layer != "UI") {
						if (RDR->GetTexHandle() == "" || RDR->GetTexHandle() == "-") // if texture handle is " or "-"
						{
							Batch::DrawQuad({ go->Position.x, go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
								{ RDR->RGB_0to1.x, RDR->RGB_0to1.y, RDR->RGB_0to1.z, RDR->Alpha_0to1 }, go->RotationAngle);
						}
						else {
							Batch::DrawQuadWithPixelCoords({ go->Position.x, go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
								this->GetLevel()->GetAssets()->GetTexture(RDR->GetTexHandle()).texid, { RDR->GetUVMin().x,  RDR->GetUVMin().y }, { RDR->GetUVMax().x,  RDR->GetUVMax().y },
								this->GetLevel()->GetAssets()->GetTexture(RDR->GetTexHandle()).texsize, { RDR->RGB_0to1.x, RDR->RGB_0to1.y, RDR->RGB_0to1.z, RDR->Alpha_0to1 });
						}
					}
				}
			}

			if (col != nullptr)
			{
				if (col->showBox)
				{

					if (col->go_is_colliding == true)
						Batch::DrawBox({ go->Position.x, go->Position.y, 1.0f }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y }, { 1,0,0,1 });
					else
						Batch::DrawBox({ go->Position.x, go->Position.y, 1.0f }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y }, { 0,1,0,1 });
				}
			}
			//std::cout << this->GetLevel()->GetGameObjectArray()[i]->GetName() << std::endl;
		}

		// If there is a game object selected, draw a simple 'gizmos' depending on the transform mode selected in the GameWindow
		if (editor->GetPropertyEditor().GetSelectedGameObject() != nullptr) { DrawGizmos(editor->GetGameWindow().currentTransformMode, editor->GetPropertyEditor().GetSelectedGameObject()->layer == "UI"); }

		//GAME UI
		//GameUI::RenderButtonWImage({ 70,825,0.9999 }, { 25,25 }, { 200.f, 200.f }, AssetsManager.GetTexture("ExitBtn"));
		Batch::EndBatch();
		Batch::Flush();

			// THIS SPACE RESERVED FOR FONT 
		for (int i = 0; i < this->GetLevel()->GetGameUILayer().size(); ++i)
		{

			Transform* trf = dynamic_cast<Transform*>((this->GetLevel()->GetGameUILayer())[i]->GetComponent(ComponentType::Transform));
			GameUI* gui = dynamic_cast<GameUI*>((this->GetLevel()->GetGameUILayer())[i]->GetComponent(ComponentType::GameUI));
			if (gui != nullptr && gui->GetText() != "-" && gui->GetText() != "" && gui->Visibility != false && gui->GetComponentOwner()->GetActive())
			{
				//for UI LAYER OBJS
				if (this->GetLevel()->GetGameUILayer()[i]->layer == "UI")
				{
					continue;
					//FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont(gui->GetTextFontFamily()), (this->GetLevel()->GetGameUILayer())[i]);
				}
				if (this->GetLevel()->GetGameUILayer()[i]->layer == "Default")
				{
					//world to vp
					float pos_x_vp = trf->Position.x - Batch::camera.GetCameraPosition().x;
					float pos_y_vp = trf->Position.y - Batch::camera.GetCameraPosition().y;


					//if text is outside of the bounds, dont render text, skip
					if (pos_x_vp < 0.f || pos_x_vp > GLSetup::current_width ||
						pos_y_vp < 0.f || pos_y_vp > GLSetup::current_height)
						continue;


					FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont(gui->GetTextFontFamily().c_str()),
						gui->GetText(),
						pos_x_vp + gui->x_offset,
						pos_y_vp + gui->y_offset,
						gui->GetFontDepth(),
						gui->GetSize(),
						glm::vec3(gui->GetFontColor_vec3().x, gui->GetFontColor_vec3().y, gui->GetFontColor_vec3().z),
						gui->Alpha);

				}
			}
		}

		//if (show_debug_Data)
		//{
		//	FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont("Roboto"), "Ohmega Engine", 0.1f * WindowWidth, 0.90f * WindowHeight, 1.f, 0.9999f , glm::vec3(1.0f, 1.0f, 0.0f),1.0f);
		//	FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont("Custom"), "By Clandestine Studios", 0.1f * WindowWidth, 0.85f * WindowHeight, 1.f, 0.9999f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		//	FontsRender::RenderDEBUG_INFO(this->GetLevel()->GetAssets()->GetFont("Roboto"), debug_data, 0.1f * WindowWidth, 0.8f * WindowHeight, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		//	FontsRender::RenderDEBUG_Performance_Data(this->GetLevel()->GetAssets()->GetFont("Roboto"), 0.025f * WindowHeight, 0.1f * WindowWidth, 0.75f * WindowHeight, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
		//	
		//} // DEBUG
	}

	GLSetup::frame_no++;
	//if (GW::Statics::GameWindowOpen)
	//mFrameBuffer->UnbindFrameBuffer();
	//GW::Statics::texID = mFrameBuffer->GetTextureID();

	if (editor_open)
	{
		//#ifdef RENDER_TO_IMGUI_GAMEWINDOW
		mFrameBuffer->UnbindFrameBuffer();
		GW::Statics::texID = mFrameBuffer->GetTextureID();
		//#endif
	}

	//std::cout << GLSetup::width << " X " << GLSetup::height << std::endl;
}

void Graphics::AdditionalUpdate(float dt) {
	(void)dt;
	if (editor_open) {
		//#ifdef RENDER_TO_IMGUI_GAMEWINDOW
		mFrameBuffer->BindFrameBuffer();
		//#endif
	}

	if (!editor_open) {
		Transform* cam = (GameObject::FindWithTag("Camera", this->GetLevel()->GetGameObjectArray())->GetComponent<Transform>());
		// Lights
		std::vector<Lighting::PointLight> pointlights{};

		Batch::StartBatch();
		for (size_t i = 1; i < this->GetLevel()->GetGameObjectArray().size(); ++i)
		{
			// inactive gameobjects will not be drawn (colliders are still active though)
			if (this->GetLevel()->GetGameObjectArray()[i]->GetActive() == false) { continue; }

			std::string_view goLayer = this->GetLevel()->GetGameObjectArray()[i]->layer;
			// check if the bitmask of the layer is set to 1 (visible)
			if (!GameObject::GetLayerMask().test(std::distance(GameObject::GetLayers().begin(), std::find(GameObject::GetLayers().begin(), GameObject::GetLayers().end(), goLayer)))) { continue; }

			Transform* go = dynamic_cast<Transform*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Transform));
			GameUI* GAMEUI = dynamic_cast<GameUI*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::GameUI));
			Renderer* RDR = dynamic_cast<Renderer*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Renderer));
			Animate* Anim = dynamic_cast<Animate*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Animate));
			Lighting* Light = dynamic_cast<Lighting*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Lighting));

			

			if (GAMEUI != nullptr)
			{
				//for simple culling
				glm::vec2 Pos{ (go->Position.x + go->Dimensions.x) * go->Scale.x , (go->Position.y + go->Dimensions.y) * go->Scale.y };
				if (Pos.x > cam->Position.x + GLSetup::current_width / 2
					&& Pos.x < cam->Position.x - GLSetup::current_width / 2
					&& Pos.y > cam->Position.y + GLSetup::current_height / 2
					&& Pos.y < cam->Position.y - GLSetup::current_height / 2) continue;

				if (this->GetLevel()->GetGameObjectArray()[i]->layer != "UI")
					continue;

				if (Anim != nullptr && Anim->GetVisibility()) {
					if (this->GetLevel()->GetGameObjectArray()[i]->layer == "UI") {
						Batch::DrawSprite({ Batch::camera.GetCameraPosition().x + go->Position.x, Batch::camera.GetCameraPosition().y + go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texid,
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texcoords[this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx],
							this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).texsize, { Anim->RGB_0to1.x, Anim->RGB_0to1.y, Anim->RGB_0to1.z, Anim->Alpha_0to1 }, go->RotationAngle);
					}
				}
				else if (RDR != nullptr && RDR->GetVisibility()) // if active 
				{
					if (this->GetLevel()->GetGameObjectArray()[i]->layer == "UI")
					{
						if (RDR->GetTexHandle() == "" || RDR->GetTexHandle() == "-")
							Batch::DrawQuad({ Batch::camera.GetCameraPosition().x + go->Position.x, Batch::camera.GetCameraPosition().y + go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
								{ RDR->RGB_0to1.x, RDR->RGB_0to1.y, RDR->RGB_0to1.z, RDR->Alpha_0to1 }, go->RotationAngle);

						else {
							Batch::DrawQuadWithPixelCoords({ Batch::camera.GetCameraPosition().x + go->Position.x,Batch::camera.GetCameraPosition().y + go->Position.y, go->Depth }, { go->Dimensions.x, go->Dimensions.y }, { go->Scale.x, go->Scale.y },
								this->GetLevel()->GetAssets()->GetTexture(RDR->GetTexHandle()).texid, { RDR->GetUVMin().x,  RDR->GetUVMin().y }, { RDR->GetUVMax().x,  RDR->GetUVMax().y },
								this->GetLevel()->GetAssets()->GetTexture(RDR->GetTexHandle()).texsize, { RDR->RGB_0to1.x, RDR->RGB_0to1.y, RDR->RGB_0to1.z, RDR->Alpha_0to1 });
						}
					}
				}
			}
			if (Light != nullptr) {
				glm::vec3 pos = glm::translate(glm::mat4(1.0f), glm::vec3(go->Position.x, go->Position.y, go->Depth)) * glm::vec4{ 0.5f, 0.5f, 0.f, 1.f };
				if (Light->GetVisibility())
					pointlights.push_back(Lighting::PointLight(pos, Light->Light.Color * 255.f, Light->Light.LightIntensity));
				else
					pointlights.push_back(Lighting::PointLight(pos, Light->Light.Color * 255.f, 0.f));
			}
		}
		if (pointlights.size() > 0)
			Batch::SetLight(pointlights);
		Batch::EndBatch();
		Batch::Flush();

		// THIS SPACE RESERVED FOR FONT 
		for (int i = 0; i < this->GetLevel()->GetGameUILayer().size(); ++i)
		{
			//Transform* trf = dynamic_cast<Transform*>((this->GetLevel()->GetGameUILayer())[i]->GetComponent(ComponentType::Transform));
			GameUI* gui = dynamic_cast<GameUI*>((this->GetLevel()->GetGameUILayer())[i]->GetComponent(ComponentType::GameUI));
			if (gui != nullptr && gui->GetText() != "-" && gui->GetText() != "" && gui->Visibility != false && gui->GetComponentOwner()->GetActive())
			{
				//for UI LAYER OBJS
				if (this->GetLevel()->GetGameUILayer()[i]->layer == "UI")
					FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont(gui->GetTextFontFamily()), (this->GetLevel()->GetGameUILayer())[i]);
				else
					continue;
			}
		}

		if (show_debug_Data)
		{
			FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont("Roboto"), "Ohmega Engine", 0.1f * WindowWidth, 0.90f * WindowHeight, 1.f, 0.9999f, glm::vec3(1.0f, 1.0f, 0.0f), 1.0f);
			FontsRender::RenderText(this->GetLevel()->GetAssets()->GetFont("Custom"), "By Clandestine Studios", 0.1f * WindowWidth, 0.85f * WindowHeight, 1.f, 0.9999f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
			FontsRender::RenderDEBUG_INFO(this->GetLevel()->GetAssets()->GetFont("Roboto"), debug_data, 0.1f * WindowWidth, 0.8f * WindowHeight, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
			FontsRender::RenderDEBUG_Performance_Data(this->GetLevel()->GetAssets()->GetFont("Roboto"), 0.025f * WindowHeight, 0.1f * WindowWidth, 0.75f * WindowHeight, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

		} // DEBUG
	}
	

	if (editor_open)
	{
		//#ifdef RENDER_TO_IMGUI_GAMEWINDOW
		mFrameBuffer->UnbindFrameBuffer();
		GW::Statics::texID = mFrameBuffer->GetTextureID();
		//#endif
	}
}

/**************************************************************************/
	/*!
	  \brief
		Cleanup function for Graphics.
	*/
	/**************************************************************************/
void Graphics::Shutdown() {
	GLSetup::cleanup();
	//Render::Object::cleanup();
	Batch::Cleanup();
}

/**************************************************************************/
/*!
	\brief
	Sets the Window parameters

	\param width
	Sets width of window size.

	\param height
	Sets height of window size.

	\param name
	Set Window bar title
*/
/**************************************************************************/
void Graphics::SetWindowParams(GLint width, GLint height, std::string name) {
	WindowWidth = width;
	WindowHeight = height;
	WindowName = name;
	CurrentWidth = width;
	CurrentHeight = height;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the Window's width
	*/
	/**************************************************************************/
GLint Graphics::GetWindowWidth()
{
	return this->WindowWidth;
}

/**************************************************************************/
	/*!
	  \brief
		Gets the Window's height
	*/
	/**************************************************************************/
GLint Graphics::GetWindowHeight()
{
	return this->WindowHeight;
}

void Graphics::SetVSync(const bool bool_param)
{
	glfwSwapInterval(bool_param ? 1 : 0);
}