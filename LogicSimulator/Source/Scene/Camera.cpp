#include "Scene/Camera.h"

#include <glm/gtx/projection.hpp>

OrthoCamera::OrthoCamera(float size, float near, float far)
	: size(size), near(near), far(far) {}

const glm::fmat4* OrthoCamera::GetProjectionMatrix() {
	if (this->size != this->pSize || this->near != this->pNear || this->far != this->pFar || this->width != this->pWidth || this->height != this->pHeight) {
		this->pSize = size;
		this->pNear = near;
		this->pFar = far;
		this->pWidth = width;
		this->pHeight = height;
		float right = this->size / 2.0f;
		float top = right / (static_cast<float>(this->width) / this->height);
		this->cachedProjectionMatrix = glm::ortho(-right, right, -top, top, this->near, this->far);
	}
	return &this->cachedProjectionMatrix;
}

const glm::fmat4* OrthoCamera::GetProjectionViewMatrix() {
	if (this->size != this->pSize || this->width != this->pWidth || this->height != this->pHeight || this->transform.ShouldUpdateTransform()) {
		this->cachedProjectionViewMatrix = *GetProjectionMatrix() * transform.GetTransformMatrix();
	}
	return &this->cachedProjectionViewMatrix;
}