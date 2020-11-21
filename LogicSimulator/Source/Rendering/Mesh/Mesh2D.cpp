#include "Rendering/Mesh/Mesh2D.h"
#include "Rendering/Apis/OpenGL/Mesh/OpenGLMesh2D.h"

Mesh2D::Mesh2D(bool editable, bool dynamic)
	: Mesh(editable, dynamic) {}

MeshData* Mesh2D::CreateCustomMeshData(RendererType type) {
	switch (type) {
	case RendererType::OPENGL:
		return new OpenGLMesh2DData(this);
	default:
		return nullptr;
	}
}