/******************************************************************************/
/*!
\file    BatchRendering.cpp
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    November 2, 2022
\brief   Rendering of Colored/Textured quads and Colored lines in batches.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include <Graphics/BatchRendering.h>
#include <array>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

static const size_t MaxQuads = 10000;
static const size_t MaxVert = MaxQuads * 4;
static const size_t MaxIndex = MaxQuads * 6;
static const size_t MaxTextures = 32;
static const size_t MaxLights = 32;

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexIndex;
};

struct LineVertex {
	glm::vec3 Position;
	glm::vec4 Color;
};

struct Data
{
	GLuint QuadVAO = 0;
	GLuint QuadVBO = 0;
	GLuint QuadIBO = 0;

	GLuint LineVAO = 0;
	GLuint LineVBO = 0;

	GLuint LightVAO = 0;
	GLuint LightVBO = 0;
	GLuint LightIBO = 0;

	GLuint WhiteTexture = 0;
	GLuint WhiteTextureSlot = 0;

	GLuint QuadIndexCount = 0;
	QuadVertex* QuadBuffer = nullptr;
	QuadVertex* QuadBufferPtr = nullptr;

	GLuint LineIndexCount = 0;
	LineVertex* LineBuffer = nullptr;
	LineVertex* LineBufferPtr = nullptr;
	float LineWidth = 2.0f;

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	std::vector<GLfloat> lightIntensities;

	std::array<GLuint, MaxTextures> TextureSlots{};
	GLuint TextureSlotIndex = 1;
};

static Data Batch_Data;
GLSLShader Batch::quad_shader_program;
GLSLShader Batch::line_shader_program;
Camera Batch::camera;
/**************************************************************************/
/*!
  \brief
	Create and bind vertex, index and initializes white texture to be used for drawing quads.
*/
/**************************************************************************/
void Create_QuadBuffer() {
	Batch_Data.QuadBuffer = new QuadVertex[MaxVert];

	glCreateBuffers(1, &Batch_Data.QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, Batch_Data.QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxVert * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

	glCreateVertexArrays(1, &Batch_Data.QuadVAO);
	glBindVertexArray(Batch_Data.QuadVAO);

	glEnableVertexArrayAttrib(Batch_Data.QuadVAO, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Position));

	glEnableVertexArrayAttrib(Batch_Data.QuadVAO, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, Color));

	glEnableVertexArrayAttrib(Batch_Data.QuadVAO, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexCoords));

	glEnableVertexArrayAttrib(Batch_Data.QuadVAO, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexIndex));

	GLuint indices[MaxIndex];
	//GLuint* indices = new GLuint[MaxIndex];
	GLuint offset = 0;
	for (size_t i = 0; i < MaxIndex; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	glCreateBuffers(1, &Batch_Data.QuadIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Batch_Data.QuadIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glCreateTextures(GL_TEXTURE_2D, 1, &Batch_Data.WhiteTexture);
	glBindTexture(GL_TEXTURE_2D, Batch_Data.WhiteTexture);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GLuint clr = 0xffffffff;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &clr);

	memset(&Batch_Data.TextureSlots, 0, MaxTextures);
	Batch_Data.TextureSlots[0] = Batch_Data.WhiteTexture;
}
/**************************************************************************/
/*!
  \brief
	Create and bind vertex to be used for drawing lines.
*/
/**************************************************************************/
void Create_LineBuffer() {
	Batch_Data.LineBuffer = new LineVertex[MaxVert];

	glCreateBuffers(1, &Batch_Data.LineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, Batch_Data.LineVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxVert * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW);

	glCreateVertexArrays(1, &Batch_Data.LineVAO);
	glBindVertexArray(Batch_Data.LineVAO);

	glEnableVertexArrayAttrib(Batch_Data.LineVAO, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Position));

	glEnableVertexArrayAttrib(Batch_Data.LineVAO, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*)offsetof(LineVertex, Color));
}

/**************************************************************************/
/*!
  \brief
	Initialize all things required to run BatchRendering.
*/
/**************************************************************************/
void Batch::Init() {
	Create_QuadBuffer();
	Create_LineBuffer();

	glUseProgram(Batch::quad_shader_program.GetHandle());
	GLuint loc = glGetUniformLocation(Batch::quad_shader_program.GetHandle(), "uTex2d");
	int samplers[32];
	for (size_t i = 0; i < 32; ++i)
		samplers[i] = (int)i;
	glUniform1iv(loc, 32, samplers);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.f, 1.f);
}
/**************************************************************************/
/*!
  \brief
	Free-ing of memory used in BatchRendering
*/
/**************************************************************************/
void Batch::Cleanup() {
	glDeleteVertexArrays(1, &Batch_Data.QuadVAO);
	glDeleteBuffers(1, &Batch_Data.QuadVBO);
	glDeleteBuffers(1, &Batch_Data.QuadIBO);
	glDeleteTextures(1, &Batch_Data.WhiteTexture);

	glDeleteVertexArrays(1, &Batch_Data.LineVAO);
	glDeleteBuffers(1, &Batch_Data.LineVBO);

	delete[] Batch_Data.QuadBuffer;
	delete[] Batch_Data.LineBuffer;
}
/**************************************************************************/
/*!
  \brief
	Setting all pointers to start of the batch.
*/
/**************************************************************************/
void Batch::StartBatch() {
	Batch_Data.QuadIndexCount = 0;
	Batch_Data.QuadBufferPtr = Batch_Data.QuadBuffer;

	Batch_Data.LineIndexCount = 0;
	Batch_Data.LineBufferPtr = Batch_Data.LineBuffer;
}
/**************************************************************************/
/*!
  \brief
	Binding all required buffers to send to GPU for drawing.
*/
/**************************************************************************/
void Batch::EndBatch() {
	if (Batch_Data.QuadIndexCount) {
		GLsizeiptr size = (uint8_t*)Batch_Data.QuadBufferPtr - (uint8_t*)Batch_Data.QuadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, Batch_Data.QuadVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, Batch_Data.QuadBuffer);
	}
	
	if (Batch_Data.LineIndexCount) {
		GLsizeiptr size = (uint8_t*)Batch_Data.LineBufferPtr - (uint8_t*)Batch_Data.LineBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, Batch_Data.LineVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, Batch_Data.LineBuffer);
	}
}
/**************************************************************************/
/*!
  \brief
	Draw all current Quads and/or Lines called using draw calls.
*/
/**************************************************************************/
void Batch::Flush() {
	glm::mat4 view = glm::ortho(0.f + (camera.GetCameraPosition().x / GLSetup::width), 1.f + (camera.GetCameraPosition().x / GLSetup::width)
		, 0.f + (camera.GetCameraPosition().y / GLSetup::height), 1.f + (camera.GetCameraPosition().y / GLSetup::height));

	if (Batch_Data.QuadIndexCount) {
		quad_shader_program.Use();
		for (GLuint i = 0; i < Batch_Data.TextureSlotIndex; ++i)
			glBindTextureUnit(i, Batch_Data.TextureSlots[i]);

		glBindVertexArray(Batch_Data.QuadVAO);
		glDrawElements(GL_TRIANGLES, Batch_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);

		Batch_Data.QuadIndexCount = 0;
		Batch_Data.TextureSlotIndex = 1;	

		GLuint loc2 = glGetUniformLocation(Batch::quad_shader_program.GetHandle(), "uViewProj");
		if (loc2 >= 0)
			glUniformMatrix4fv(loc2, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 device(1.f / GLSetup::width, 0.f, 0.f, 0.f,
			0.f, 1.f / GLSetup::height, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);

		GLuint loc = glGetUniformLocation(quad_shader_program.GetHandle(), "uTransform");
		if (loc >= 0)
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(device));

		GLint lightSize = glGetUniformLocation(quad_shader_program.GetHandle(), "light_size");
		glUniform1i(lightSize, (GLint)Batch_Data.lightIntensities.size());

		if (Batch_Data.lightPositions.size() > 0 && Batch_Data.lightColors.size() > 0 && Batch_Data.lightIntensities.size() > 0) {
			GLint lightPositionsLoc = glGetUniformLocation(quad_shader_program.GetHandle(), "light_pos");
			GLint lightColorsLoc = glGetUniformLocation(quad_shader_program.GetHandle(), "light_color");
			GLint lightIntensitiesLoc = glGetUniformLocation(quad_shader_program.GetHandle(), "light_intensity");

			glUniform3fv(lightPositionsLoc, (GLsizei)Batch_Data.lightPositions.size(), glm::value_ptr(Batch_Data.lightPositions[0]));
			glUniform3fv(lightColorsLoc, (GLsizei)Batch_Data.lightColors.size(), glm::value_ptr(Batch_Data.lightColors[0]));
			glUniform1fv(lightIntensitiesLoc, (GLsizei)Batch_Data.lightIntensities.size(), &Batch_Data.lightIntensities[0]);


			Batch_Data.lightPositions.clear();
			Batch_Data.lightColors.clear();
			Batch_Data.lightIntensities.clear();
		}
	}
	
	if (Batch_Data.LineIndexCount) {
		line_shader_program.Use();
		glBindVertexArray(Batch_Data.LineVAO);
		glLineWidth(Batch_Data.LineWidth);
		glDrawArrays(GL_LINES, 0, Batch_Data.LineIndexCount);

		Batch_Data.LineIndexCount = 0;

		GLuint loc2 = glGetUniformLocation(Batch::line_shader_program.GetHandle(), "uViewProj");
		if (loc2 >= 0)
			glUniformMatrix4fv(loc2, 1, GL_FALSE, glm::value_ptr(view));
	}
}
/**************************************************************************/
/*!
  \brief
	Draw a Colored Quad.

  \param pos
	Position(x,y,z) in screen space of quad to draw.

  \param size
	Size(x,y) of quad to draw.

  \param scale
	Scale(x,y) of quad to draw.

  \param clr
	Color(r,g,b,a) of quad to draw.
*/
/**************************************************************************/
void Batch::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, const glm::vec4& clr, const float& angle_in_degree) {
	if (Batch_Data.QuadIndexCount >= MaxIndex) {
		EndBatch();
		Flush();
		StartBatch();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos))
		* glm::rotate(glm::mat4(1.f), angle_in_degree, glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(size * scale / 100.f, 1.f));

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { 0.f, 0.f };
	Batch_Data.QuadBufferPtr->TexIndex = 0.f;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { 1.f, 0.f };
	Batch_Data.QuadBufferPtr->TexIndex = 0.f;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { 1.f, 1.f };
	Batch_Data.QuadBufferPtr->TexIndex = 0.f;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { 0.f, 1.f };
	Batch_Data.QuadBufferPtr->TexIndex = 0.f;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadIndexCount += 6;
}
/**************************************************************************/
/*!
  \brief
	Draw a Textured Quad.

  \param pos
	Position(x,y,z) in screen space of quad to draw.

  \param size
	Size(x,y) of quad to draw.

  \param scale
	Scale(x,y) of quad to draw.

  \param texid
	Texture of quad to draw.
*/
/**************************************************************************/
void Batch::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, GLuint texid, const float& angle_in_degree) {
	if (Batch_Data.QuadIndexCount >= MaxIndex || Batch_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		StartBatch();
	}

	glm::vec4 clr = { 1.f, 1.f, 1.f, 1.f };

	float TexIndex = 0.f;
	for (GLuint i = 0; i < Batch_Data.TextureSlotIndex; ++i)
	{
		if (Batch_Data.TextureSlots[i] == texid) {
			TexIndex = (float)i;
			break;
		}
	}

	if (TexIndex == 0.f && Batch_Data.TextureSlotIndex < 32) {
		TexIndex = (float)Batch_Data.TextureSlotIndex;
		Batch_Data.TextureSlots[Batch_Data.TextureSlotIndex] = texid;
		++Batch_Data.TextureSlotIndex;
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos))
		* glm::rotate(glm::mat4(1.f), angle_in_degree, glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(size * scale/100.f, 1.f));

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { 0.f, 0.f };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { 1.f, 0.f };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { 1.f, 1.f };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { 0.f, 1.f };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadIndexCount += 6;
}

void Batch::DrawQuadWithUV(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, GLuint texid, const glm::vec2& uv_start, const glm::vec2& uv_end, const float& angle_in_degree) {
	if (Batch_Data.QuadIndexCount >= MaxIndex || Batch_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		StartBatch();
	}

	glm::vec4 clr = { 1.f, 1.f, 1.f, 1.f };

	float TexIndex = 0.f;
	for (GLuint i = 0; i < Batch_Data.TextureSlotIndex; ++i)
	{
		if (Batch_Data.TextureSlots[i] == texid) {
			TexIndex = (float)i;
			break;
		}
	}

	if (TexIndex == 0.f && Batch_Data.TextureSlotIndex < 32) {
		TexIndex = (float)Batch_Data.TextureSlotIndex;
		Batch_Data.TextureSlots[Batch_Data.TextureSlotIndex] = texid;
		++Batch_Data.TextureSlotIndex;
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos))
		* glm::rotate(glm::mat4(1.f), angle_in_degree, glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(size * scale / 100.f, 1.f));

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { uv_start.x, uv_start.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { uv_end.x, uv_start.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { uv_end.x, uv_end.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { uv_start.x, uv_end.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadIndexCount += 6;
}

void Batch::DrawQuadWithPixelCoords(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, GLuint texid, const glm::vec2& uv_start, const glm::vec2& uv_end, const glm::vec2 texsize, const glm::vec4 clr, const float& angle_in_degree) {
	if (Batch_Data.QuadIndexCount >= MaxIndex || Batch_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		StartBatch();
	}

	//glm::vec4 clr = { 1.f, 1.f, 1.f, 1.f };

	float TexIndex = 0.f;
	for (GLuint i = 0; i < Batch_Data.TextureSlotIndex; ++i)
	{
		if (Batch_Data.TextureSlots[i] == texid) {
			TexIndex = (float)i;
			break;
		}
	}

	if (TexIndex == 0.f && Batch_Data.TextureSlotIndex < 32) {
		TexIndex = (float)Batch_Data.TextureSlotIndex;
		Batch_Data.TextureSlots[Batch_Data.TextureSlotIndex] = texid;
		++Batch_Data.TextureSlotIndex;
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos))
		* glm::rotate(glm::mat4(1.f), angle_in_degree, glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(size * scale / 100.f, 1.f));

	glm::vec2 start = { uv_start.x / texsize.x, uv_start.y / texsize.y };
	glm::vec2 end = { uv_end.x / texsize.x, uv_end.y / texsize.y };

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { start.x, start.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { end.x, start.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { end.x, end.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { start.x, end.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadIndexCount += 6;
}
/**************************************************************************/
/*!
  \brief
	Draw a Colored Line.

  \param p0
	Start position(x,y,z) of line to draw.

  \param p1
	End position(x,y,z) of line to draw.

  \param clr
	Color(r,g,b,a) of line to draw.
*/
/**************************************************************************/
void Batch::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& clr) {
	if (Batch_Data.LineIndexCount >= MaxIndex) {
		EndBatch();
		Flush();
		StartBatch();
	}

	Batch_Data.LineBufferPtr->Position = p0;
	Batch_Data.LineBufferPtr->Color = clr;
	++Batch_Data.LineBufferPtr;

	Batch_Data.LineBufferPtr->Position = p1;
	Batch_Data.LineBufferPtr->Color = clr;
	++Batch_Data.LineBufferPtr;

	Batch_Data.LineIndexCount += 2;

	glm::mat4 s = glm::scale(glm::mat4(1.f), { 1.f,1.f,1.f });
	glm::mat4 device(1.f / GLSetup::width, 0.f, 0.f, 0.f,
		0.f, 1.f / GLSetup::height, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);
	glm::mat4 transform = device * s;

	line_shader_program.Use();
	GLuint loc = glGetUniformLocation(line_shader_program.GetHandle(), "uTransform");
	if (loc >= 0)
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(transform));
}
/**************************************************************************/
/*!
  \brief
	Draw a Colored Line Box.

  \param pos
	Position(x,y,z) of box to draw.

  \param size
	Size(x,y) of box to draw.

  \param scale
	Scale(x,y) of box to draw.

  \param clr
	Color(r,g,b,a) of box to draw.
*/
/**************************************************************************/
void Batch::DrawBox(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, const glm::vec4& clr) {
	glm::vec2 half_size = { size.x * 0.5f * (scale.x * 0.01f) , size.y * 0.5f * (scale.y * 0.01f) };

	glm::vec3 p0 = glm::vec3(pos.x - half_size.x, pos.y - half_size.y, pos.z);
	glm::vec3 p1 = glm::vec3(pos.x + half_size.x, pos.y - half_size.y, pos.z);
	glm::vec3 p2 = glm::vec3(pos.x + half_size.x, pos.y + half_size.y, pos.z);
	glm::vec3 p3 = glm::vec3(pos.x - half_size.x, pos.y + half_size.y, pos.z);

	DrawLine(p0, p1, clr);
	DrawLine(p1, p2, clr);
	DrawLine(p2, p3, clr);
	DrawLine(p3, p0, clr);
}

void Batch::DrawSprite(const glm::vec3& pos, const glm::vec2& size, const glm::vec2& scale, GLuint texid, const glm::vec2& texcoords, glm::vec2& texsize, const glm::vec4 clr, const float& angle_in_degree) {
	if (Batch_Data.QuadIndexCount >= MaxIndex || Batch_Data.TextureSlotIndex > 31) {
		EndBatch();
		Flush();
		StartBatch();
	}

	float TexIndex = 0.f;
	for (GLuint i = 0; i < Batch_Data.TextureSlotIndex; ++i)
	{
		if (Batch_Data.TextureSlots[i] == texid) {
			TexIndex = (float)i;
			break;
		}
	}

	if (TexIndex == 0.f && Batch_Data.TextureSlotIndex < 32) {
		TexIndex = (float)Batch_Data.TextureSlotIndex;
		Batch_Data.TextureSlots[Batch_Data.TextureSlotIndex] = texid;
		++Batch_Data.TextureSlotIndex;
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos))
		* glm::rotate(glm::mat4(1.f), angle_in_degree, glm::vec3(0.f, 0.f, 1.f))
		* glm::scale(glm::mat4(1.f), glm::vec3(size * scale / 100.f, 1.f));

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { texcoords.x, texcoords.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f, -0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { texcoords.x + texsize.x, texcoords.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ 0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { texcoords.x + texsize.x, texcoords.y + texsize.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadBufferPtr->Position = transform * glm::vec4{ -0.5f,  0.5f, 0.0f, 1.f };
	Batch_Data.QuadBufferPtr->Color = clr;
	Batch_Data.QuadBufferPtr->TexCoords = { texcoords.x, texcoords.y + texsize.y };
	Batch_Data.QuadBufferPtr->TexIndex = TexIndex;
	++Batch_Data.QuadBufferPtr;

	Batch_Data.QuadIndexCount += 6;
}
/**************************************************************************/
/*!
  \brief
	Set the width of line drawn.

  \param width
	Width of the line.
*/
/**************************************************************************/
void Batch::SetLineWidth(float width) {
	Batch_Data.LineWidth = width;
}

void Batch::SetLight(std::vector<Lighting::PointLight> pointlights) {
	if (Batch_Data.lightPositions.size() > 31 || Batch_Data.lightColors.size() > 31 || Batch_Data.lightIntensities.size() > 31) {
		EndBatch();
		Flush();
		StartBatch();
	}	

	for (const auto& pointlight : pointlights)
	{
		if (Batch_Data.lightPositions.size() > 31 || Batch_Data.lightColors.size() > 31 || Batch_Data.lightIntensities.size() > 31) {
			EndBatch();
			Flush();
			StartBatch();
		}

		Batch_Data.lightPositions.push_back(pointlight.Pos);
		Batch_Data.lightColors.push_back(pointlight.Color);
		Batch_Data.lightIntensities.push_back(pointlight.LightIntensity);
	}
}

