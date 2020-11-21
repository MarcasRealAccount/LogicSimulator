#include "Scene/Entity.h"
#include "Scene/Scene.h"

Entity::~Entity() {
	if (this->scene) this->scene->DetachEntity(this);
}

Scene* Entity::GetScene() const {
	return this->scene;
}

Mesh* Entity::GetMesh() {
	return nullptr;
}

Material* Entity::GetMaterial() {
	return nullptr;
}

MeshedEntity::MeshedEntity(Mesh* mesh, Material* material)
	: mesh(mesh), material(material) {}

Mesh* MeshedEntity::GetMesh() {
	return this->mesh;
}

Material* MeshedEntity::GetMaterial() {
	return this->material;
}
