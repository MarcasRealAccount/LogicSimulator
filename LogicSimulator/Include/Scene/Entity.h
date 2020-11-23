#pragma once

#include "Utilities/Vector/Transform.h"

struct Scene;
struct Mesh;
class Material;

class Entity {
public:
	virtual ~Entity();

	Scene* GetScene() const;

	virtual Mesh* GetMesh();
	virtual Material* GetMaterial();

	friend Scene;

public:
	Transform transform;

protected:
	Scene* scene = nullptr;
};

class MeshedEntity : public Entity {
public:
	MeshedEntity(Mesh* mesh = nullptr, Material* material = nullptr);

	virtual Mesh* GetMesh() override;
	virtual Material* GetMaterial() override;

protected:
	Mesh* mesh;
	Material* material;
};