#include "Rendering/Apis/OpenGL/Mesh/OpenGLMesh2D.h"
#include "Rendering/Mesh/Mesh2D.h"

OpenGLMesh2DData::OpenGLMesh2DData(Mesh2D* mesh)
	: OpenGLMeshData(reinterpret_cast<Mesh*>(mesh)) {}

uint32_t OpenGLMesh2DData::GetCustomVBOCount() {
	return 1;
}

uint32_t OpenGLMesh2DData::GetCustomDataSize() {
	return static_cast<uint32_t>(GetMesh<Mesh2D>()->vertices.size());
}

void OpenGLMesh2DData::InitCustomGLData() {
	Mesh2D* mesh = GetMesh<Mesh2D>();
	BindNextVBO(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Mesh2DVertex), mesh->vertices.data(), GL_DYNAMIC_DRAW);
	SetVertexAttribPointer(static_cast<uint32_t>(VertexAttribIndex::POSITION), 2, GL_FLOAT, false, sizeof(Mesh2DVertex), offsetof(Mesh2DVertex, position));
	SetVertexAttribPointer(static_cast<uint32_t>(VertexAttribIndex::UV), 2, GL_FLOAT, false, sizeof(Mesh2DVertex), offsetof(Mesh2DVertex, uv));
	UnbindVBO(GL_ARRAY_BUFFER);
}

void OpenGLMesh2DData::ClearCustomData() {
	Mesh2D* mesh = GetMesh<Mesh2D>();
	mesh->vertices.clear();
}