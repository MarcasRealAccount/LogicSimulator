#pragma once

#include "ECS/Component.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class TransformComponent : public Component {
public:
	virtual bool ShouldUpdateTransform() const;
	virtual const glm::fmat4& GetTransformMatrix();

	const glm::fvec3& GetPreviousPosition() const;
	const glm::fquat& GetPreviousRotation() const;
	const glm::fvec3& GetPreviousScale() const;

protected:
	virtual void ResetPreviousValues();

public:
	glm::fvec3 position{ 0.0f, 0.0f, 0.0f };
	glm::fquat rotation{ 0.0f, 0.0f, 0.0f, 0.0f };
	glm::fvec3 scale{ 1.0f, 1.0f, 1.0f };
	bool negativeTranslation = false;

private:
	glm::fvec3 pPosition{ 0.0f, 0.0f, 0.0f };
	glm::fquat pRotation{ 0.0f, 0.0f, 0.0f, 0.0f };
	glm::fvec3 pScale{ 1.0f, 1.0f, 1.0f };
	bool pNegativeTranslation = false;

	glm::fmat4 cachedTransformationMatrix{ 1.0f };
};