#include "Scene/Scene.h"
#include "Scene/Entity.h"

Scene* Scene::mainScene;

void Scene::AttachEntity(Entity* entity) {
	if (entity->scene) entity->scene->DetachEntity(entity);
	this->entities.push_back(entity);
	entity->scene = this;
}

void Scene::DetachEntity(Entity* entity) {
	if (entity->scene != this) return;

	auto itr = this->entities.begin();
	while (itr != this->entities.end()) {
		if (*itr == entity) {
			itr = this->entities.erase(itr);
			entity->scene = nullptr;
			break;
		}
		itr++;
	}
}

void Scene::SetMainCamera(Camera* camera) {
	if (reinterpret_cast<Entity*>(camera)->scene == this) {
		this->mainCamera = camera;
	}
}

Camera* Scene::GetMainCamera() const {
	return this->mainCamera;
}

const std::vector<Entity*>& Scene::GetEntities() const {
	return this->entities;
}

void Scene::SetMainScene(Scene* scene) {
	Scene::mainScene = scene;
}

Scene* Scene::GetMainScene() {
	return Scene::mainScene;
}