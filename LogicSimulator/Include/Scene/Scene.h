#pragma once

#include <vector>
#include <mutex>

class Entity;
class Camera;

class Renderer;

struct Scene {
public:
	void AttachEntity(Entity* entity);
	void DetachEntity(Entity* entity);

	void SetMainCamera(Camera* camera);
	Camera* GetMainCamera() const;

	const std::vector<Entity*>& GetEntities() const;

	friend Renderer;

public:
	static void SetMainScene(Scene* scene);
	static Scene* GetMainScene();

private:
	std::mutex lock;

	std::vector<Entity*> entities;
	Camera* mainCamera = nullptr;

	static Scene* mainScene;
};