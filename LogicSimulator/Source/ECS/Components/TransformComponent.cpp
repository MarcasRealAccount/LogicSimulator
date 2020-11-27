#include "ECS/Components/TransformComponent.h"

#include <glm/gtx/transform.hpp>

bool TransformComponent::ShouldUpdateTransform() const {
	return this->position != this->pPosition || this->rotation != this->pRotation || this->scale != this->pScale || this->negativeTranslation != this->pNegativeTranslation;
}

const glm::fmat4& TransformComponent::GetTransformMatrix() {
	if (ShouldUpdateTransform()) {
		ResetPreviousValues();
		this->cachedTransformationMatrix = glm::translate(negativeTranslation ? -this->position : this->position);
		this->cachedTransformationMatrix *= static_cast<glm::fmat4>(this->rotation);
		this->cachedTransformationMatrix = glm::scale(this->cachedTransformationMatrix, this->scale);
	}
	return this->cachedTransformationMatrix;
}

const glm::fvec3& TransformComponent::GetPreviousPosition() const {
	return this->pPosition;
}

const glm::fquat& TransformComponent::GetPreviousRotation() const {
	return this->pRotation;
}

const glm::fvec3& TransformComponent::GetPreviousScale() const {
	return this->pScale;
}

void TransformComponent::ResetPreviousValues() {
	this->pPosition = this->position;
	this->pRotation = this->rotation;
	this->pScale = this->scale;
	this->pNegativeTranslation = this->negativeTranslation;
}