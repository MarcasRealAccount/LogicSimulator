#include "Window/Input/InputGroup.h"
#include "Window/Input/InputHandler.h"
#include "Window/Input/AxisInputBinding.h"
#include "Window/Input/ButtonInputBinding.h"
#include "Event/Event.h"

InputGroup::InputGroup(const std::string& id, bool enabled)
	: id(id), enabled(enabled) {}

InputGroup::~InputGroup() {
	for (auto& binding : this->inputBindings) delete binding.second;
}

void InputGroup::Enable() {
	this->enabled = true;
}

void InputGroup::Disable() {
	this->enabled = false;
}

bool InputGroup::IsEnabled() const {
	return this->enabled;
}

void InputGroup::SetCaptureMouse(bool captureMouse) {
	this->captureMouse = captureMouse;
}

bool InputGroup::DoesCaptureMouse() const {
	return this->captureMouse;
}

void InputGroup::HandleEvent(Event* event) {
	for (auto& binding : this->inputBindings) {
		binding.second->HandleEvent(event);
		if (event->HasBeenHandled()) break;
	}
}

IInputBinding* InputGroup::GetInputBinding(const std::string& id) {
	auto itr = this->inputBindings.find(id);
	if (itr != this->inputBindings.end()) return itr->second;
	return nullptr;
}

void InputGroup::RemoveInputBinding(IInputBinding* binding) {
	delete binding;
}

ButtonInputBinding* InputGroup::CreateButtonInputBinding(const std::string& id, uint32_t button, InputLocation location, ButtonInputType inputType, ButtonCallback callback) {
	if (this->inputBindings.find(id) != this->inputBindings.end()) return nullptr;

	InputHandler::GetBindingConfigs(this->id + "." + id, InputType::BUTTON, &location, &button);
	ButtonInputBinding* binding = new ButtonInputBinding(this, id, button, inputType, location, callback);
	this->inputBindings.insert({ id, binding });
	return binding;
}

AxisInputBinding* InputGroup::CreateAxisInputBinding(const std::string& id, uint32_t axis, InputLocation location, AxisCallback callback) {
	if (this->inputBindings.find(id) != this->inputBindings.end()) return nullptr;

	InputHandler::GetBindingConfigs(this->id + "." + id, InputType::AXIS, &location, &axis);
	AxisInputBinding* binding = new AxisInputBinding(this, id, axis, location, callback);
	this->inputBindings.insert({ id, binding });
	return binding;
}

const std::string& InputGroup::GetId() const {
	return this->id;
}

void InputGroup::RemoveInputBindingImpl(IInputBinding* binding) {
	auto itr = this->inputBindings.find(binding->GetId());
	if (itr != this->inputBindings.end()) this->inputBindings.erase(itr);
}