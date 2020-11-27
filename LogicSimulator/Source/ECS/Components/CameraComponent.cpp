#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"

#include <glm/gtx/projection.hpp>

CameraComponent::CameraComponent(float size, bool ortho, float nearPlane, float farPlane)
	: size(size), fov(size), ortho(ortho), nearPlane(nearPlane), farPlane(farPlane) {}

bool CameraComponent::ShouldUpdateProjectionMatrix() const {
	return this->width != this->pWidth || this->height != this->pHeight || this->nearPlane != this->pNearPlane || this->farPlane != this->pFarPlane || this->ortho != this->pOrtho || (this->ortho && this->size != this->pSize) || (!this->ortho && this->fov != this->pFov);
}

bool CameraComponent::ShouldUpdateProjectionViewMatrix() const {
	TransformComponent* transformComp = this->GetParent()->GetComponent<TransformComponent>();
	if (!transformComp) return ShouldUpdateProjectionMatrix();
	return ShouldUpdateProjectionMatrix() && transformComp->ShouldUpdateTransform();
}

const glm::fmat4& CameraComponent::GetProjectionMatrix() {
	if (ShouldUpdateProjectionMatrix()) {
		ResetPreviousValues();
		if (ortho) {
			float right = this->size * 0.5f;
			float top = right / (static_cast<float>(this->width) / this->height);
			this->cachedProjectionMatrix = glm::ortho(-right, right, -top, top, this->nearPlane, this->farPlane);
		} else {
			this->cachedProjectionMatrix = glm::perspectiveFov(this->fov, static_cast<float>(this->width), static_cast<float>(this->height), this->nearPlane, this->farPlane);
		}
	}
	return this->cachedProjectionMatrix;
}

const glm::fmat4& CameraComponent::GetProjectionViewMatrix() {
	if (ShouldUpdateProjectionViewMatrix()) {
		TransformComponent* transformComp = this->GetParent()->GetComponent<TransformComponent>();
		if (!transformComp) this->cachedProjectionViewMatrix = GetProjectionMatrix();
		else {
			transformComp->negativeTranslation = true;
			this->cachedProjectionViewMatrix = GetProjectionMatrix() * transformComp->GetTransformMatrix();
		}
	}
	return this->cachedProjectionViewMatrix;
}

void CameraComponent::ResetPreviousValues() {
	this->pWidth = this->width;
	this->pHeight = this->height;
	this->pNearPlane = this->nearPlane;
	this->pFarPlane = this->farPlane;
	// Ortho
	this->pSize = this->size;
	// Perspective
	this->pFov = this->fov;
}

uint32_t CameraComponent::GetPreviousWidth() const {
	return pWidth;
}

uint32_t CameraComponent::GetPreviousHeight() const {
	return pHeight;
}

float CameraComponent::GetPreviousNearPlane() const {
	return this->pNearPlane;
}

float CameraComponent::GetPreviousFarPlane() const {
	return this->pFarPlane;
}

float CameraComponent::GetPreviousSize() const {
	return this->pSize;
}

float CameraComponent::GetPreviousFOV() const {
	return this->fov;
}
