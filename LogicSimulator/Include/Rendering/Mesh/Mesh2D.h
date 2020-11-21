#pragma once

#include "Rendering/Mesh/Mesh.h"

#include <glm/glm.hpp>

struct Mesh2DVertex {
	glm::fvec2 position;
	glm::fvec2 uv;
};

struct Mesh2D : public Mesh {
public:
	Mesh2D(bool editable = true, bool dynamic = false);

private:
	virtual MeshData* CreateCustomMeshData(RendererType type) override;

public:
	std::vector<Mesh2DVertex> vertices;
};