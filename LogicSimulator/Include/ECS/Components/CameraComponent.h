#pragma once

#include "ECS/Component.h"

#include <stdint.h>
#include <glm/glm.hpp>

class CameraComponent : public Component {
public:
	CameraComponent(float size = 5.0f, bool ortho = true, float nearPlane = 0.0f, float farPlane = 1000.0f);

	bool ShouldUpdateProjectionMatrix() const;
	bool ShouldUpdateProjectionViewMatrix() const;

	const glm::fmat4& GetProjectionMatrix();
	const glm::fmat4& GetProjectionViewMatrix();

	uint32_t GetPreviousWidth() const;
	uint32_t GetPreviousHeight() const;
	float GetPreviousNearPlane() const;
	float GetPreviousFarPlane() const;

public: // Ortho
	float GetPreviousSize() const;

public: // Perspective
	float GetPreviousFOV() const;

protected:
	virtual void ResetPreviousValues();

public:
	uint32_t width = 0, height = 0;
	glm::fvec4 clearColor{ 0.2f, 0.2f, 0.2f, 1.0f };

	float nearPlane;
	float farPlane;

	bool ortho;

public: // Ortho
	float size;

public: // Perspective
	float fov;

protected:
	uint32_t pWidth = 0, pHeight = 0;

	float pNearPlane = 0.0f;
	float pFarPlane = 0.0f;

	bool pOrtho = true;

	glm::fmat4 cachedProjectionMatrix{ 1.0f }, cachedProjectionViewMatrix{ 1.0f };

protected: // Ortho
	float pSize = 0.0f;

protected: // Perspective
	float pFov = 0.0f;
};