#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
public:
	bool ShouldUpdateTransform() const;
	const glm::fmat4& GetTransformMatrix(bool negativeTranslation = false);

	const glm::fvec3& GetPreviousPosition() const;
	const glm::fquat& GetPreviousRotation() const;
	const glm::fvec3& GetPreviousScale() const;

public:
	glm::fvec3 position{ 0.0f, 0.0f, 0.0f };
	glm::fquat rotation{ 0.0f, 0.0f, 0.0f, 0.0f };
	glm::fvec3 scale{ 1.0f, 1.0f, 1.0f };

private:
	glm::fvec3 pPosition{ 0.0f, 0.0f, 0.0f };
	glm::fquat pRotation{ 0.0f, 0.0f, 0.0f, 0.0f };
	glm::fvec3 pScale{ 1.0f, 1.0f, 1.0f };

	glm::fmat4 cachedTransformationMatrix{ 1.0f };
};