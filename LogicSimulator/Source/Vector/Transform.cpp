#include "Vector/Transform.h"

#include <glm/gtx/transform.hpp>

bool Transform::ShouldUpdateTransform() const {
	return this->position != this->pPosition || this->rotation != this->pRotation || this->scale != this->pScale;
}

const glm::fmat4& Transform::GetTransformMatrix(bool negativeTranslation) {
	if (ShouldUpdateTransform()) {
		this->pPosition = this->position;
		this->pRotation = this->rotation;
		this->pScale = this->scale;
		this->cachedTransformationMatrix = glm::translate(negativeTranslation ? -this->position : this->position);
		this->cachedTransformationMatrix *= static_cast<glm::fmat4>(this->rotation);
		this->cachedTransformationMatrix = glm::scale(this->cachedTransformationMatrix, this->scale);
	}
	return this->cachedTransformationMatrix;
}

const glm::fvec3& Transform::GetPreviousPosition() const {
	return this->pPosition;
}

const glm::fquat& Transform::GetPreviousRotation() const {
	return this->pRotation;
}

const glm::fvec3& Transform::GetPreviousScale() const {
	return this->pScale;
}