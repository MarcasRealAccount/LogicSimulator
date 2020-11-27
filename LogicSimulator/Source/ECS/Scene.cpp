#include "ECS/Scene.h"

#include "ECS/Entity.h"
#include "ECS/Components/CameraComponent.h"

Scene* Scene::mainScene = nullptr;

void Scene::SetMainCamera(Entity* entity) {
	if (!entity) SetMainCamera(entity);
	else SetMainCamera(entity->GetComponent<CameraComponent>());
}

void Scene::SetMainCamera(CameraComponent* mainCamera) {
	if (mainCamera) this->mainCamera = mainCamera->GetParent();
	else this->mainCamera = nullptr;
}

Entity* Scene::GetMainCamera() const {
	return this->mainCamera;
}

void Scene::SetMainScene(Scene* scene) {
	Scene::mainScene = scene;
}

Scene* Scene::GetMainScene() {
	return Scene::mainScene;
}