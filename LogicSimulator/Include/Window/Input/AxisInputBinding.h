#pragma once

#include "Window/Input/IInputBinding.h"

#include <functional>

struct AxisCallbackData {
	InputLocation location;
	uint32_t axis;
	double value;
	const std::string id;
};

using AxisCallback = std::function<void(AxisCallbackData data)>;

struct AxisInputBinding : public IInputBinding {
public:
	AxisInputBinding(InputGroup* inputGroup, const std::string& id, uint32_t index, InputLocation location, AxisCallback callback);

	virtual void HandleEvent(Event* event) override;

	void BindCallback(AxisCallback callback);

private:
	AxisCallback callback;
};

namespace axises {
	constexpr uint32_t mouseX = 0;
	constexpr uint32_t mouseY = 1;
	constexpr uint32_t mouseWheelX = 2;
	constexpr uint32_t mouseWheelY = 3;
	constexpr uint32_t mouseLast = mouseWheelY;
}