#pragma once

#include "ECS/Entity.h"

class Renderer;
class CameraComponent;
class Entity;

class Scene : public Entity {
public:
	void SetMainCamera(Entity* entity);
	void SetMainCamera(CameraComponent* mainCamera);
	Entity* GetMainCamera() const;

	friend Renderer;

public:
	static void SetMainScene(Scene* scene);
	static Scene* GetMainScene();

private:
	Entity* mainCamera = nullptr;

private:
	static Scene* mainScene;
};