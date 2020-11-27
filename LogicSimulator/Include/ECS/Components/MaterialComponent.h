#pragma once

#include "ECS/Component.h"

class Material;

class MaterialComponent : public Component {
public:
	MaterialComponent(Material* material = nullptr);

public:
	Material* material;
};