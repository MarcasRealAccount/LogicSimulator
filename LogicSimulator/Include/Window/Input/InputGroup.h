#pragma once

#include "Window/Input/ButtonInputBinding.h"
#include "Window/Input/AxisInputBinding.h"

#include <string>
#include <unordered_map>

struct Event;

class InputGroup {
public:
	InputGroup(const std::string& id, bool enabled = true);
	~InputGroup();

	void Enable();
	void Disable();
	bool IsEnabled() const;

	void SetCaptureMouse(bool captureMouse);
	bool DoesCaptureMouse() const;

	void HandleEvent(Event* event);

	IInputBinding* GetInputBinding(const std::string& id);

	void RemoveInputBinding(IInputBinding* binding);

	ButtonInputBinding* CreateButtonInputBinding(const std::string& id, uint32_t button, InputLocation location = InputLocation::KEYBOARD, ButtonInputType inputType = ButtonInputType::PRESS, ButtonCallback callback = nullptr);
	AxisInputBinding* CreateAxisInputBinding(const std::string& id, uint32_t axis, InputLocation location = InputLocation::MOUSE, AxisCallback callback = nullptr);

	const std::string& GetId() const;

protected:
	void RemoveInputBindingImpl(IInputBinding* binding);

private:
	std::string id;
	std::unordered_map<std::string, IInputBinding*> inputBindings;
	bool captureMouse = false;
	bool enabled;
};