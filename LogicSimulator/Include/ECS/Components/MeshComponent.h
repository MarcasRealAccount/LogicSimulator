#pragma once

#include "ECS/Component.h"

struct Mesh;

class MeshComponent : public Component {
public:
	MeshComponent(Mesh* mesh = nullptr);

public:
	Mesh* mesh;
};