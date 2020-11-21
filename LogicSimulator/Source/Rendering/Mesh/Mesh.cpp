#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Renderer.h"

Mesh::Mesh(bool editable, bool dynamic)
	: editable(editable), dynamic(dynamic) {}

Mesh::~Mesh() {
	if (this->meshData) {
		this->meshData->CleanUp();
		delete this->meshData;
	}
}

void Mesh::MarkDirty() {
	if (this->editable) this->dirty = true;
}

void Mesh::ClearDirty() {
	this->dirty = false;
}

bool Mesh::IsDirty() {
	return this->dirty;
}

bool Mesh::IsEditable() {
	return this->editable;
}

bool Mesh::IsDynamic() {
	return this->dynamic;
}

MeshData* Mesh::GetMeshData(Renderer* renderer) {
	if (!this->meshData || !renderer->IsMeshDataUsable(this->meshData)) {
		if (this->meshData) delete this->meshData;
		this->meshData = CreateCustomMeshData(renderer->GetRendererType());
	}
	return this->meshData;
}

MeshData::MeshData(Mesh* mesh)
	: mesh(mesh) {}

MeshData::~MeshData() {
	if (this->mesh) this->mesh->meshData = nullptr;
}