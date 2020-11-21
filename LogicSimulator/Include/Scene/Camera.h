#pragma once

#include "Entity.h"

#include <stdint.h>

class Camera : public Entity {
public:
	virtual const glm::fmat4* GetProjectionMatrix() = 0;
	virtual const glm::fmat4* GetProjectionViewMatrix() = 0;

public:
	uint32_t width = 0, height = 0;
	glm::fvec4 clearColor{ 0.2f, 0.2f, 0.2f, 1.0f };

protected:
	uint32_t pWidth = 0, pHeight = 0;
};

class OrthoCamera : public Camera {
public:
	OrthoCamera(float size, float near = 0.0f, float far = 1000.0f);

	virtual const glm::fmat4* GetProjectionMatrix() override;
	virtual const glm::fmat4* GetProjectionViewMatrix() override;

public:
	float size;
	float near;
	float far;

protected:
	float pSize = 0.0f;
	float pNear = 0.0f;
	float pFar = 0.0f;

	glm::fmat4 cachedProjectionMatrix{ 1.0f };
	glm::fmat4 cachedProjectionViewMatrix{ 1.0f };
};