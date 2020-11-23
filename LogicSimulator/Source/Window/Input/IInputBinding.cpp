#include "Window/Input/IInputBinding.h"
#include "Window/Input/InputHandler.h"
#include "Window/Input/InputGroup.h"

IInputBinding::IInputBinding(InputGroup* inputGroup, const std::string& id, InputType type, uint32_t index, InputLocation location)
	: inputGroup(inputGroup), id(id), type(type), index(index), location(location) {}

IInputBinding::~IInputBinding() {
	// this->inputGroup->RemoveInputBindingImpl(this);
}

void IInputBinding::SetBinding(InputLocation location, uint32_t index) {
	this->location = location;
	this->index = index;
	InputHandler::SetBindingConfigs(this->inputGroup->GetId() + "." + this->id, GetType(), this->location, this->index);
}

void IInputBinding::SetInputLocation(InputLocation location) {
	this->location = location;
	InputHandler::SetBindingConfigs(this->inputGroup->GetId() + "." + this->id, GetType(), this->location, this->index);
}

void IInputBinding::SetIndex(uint32_t index) {
	this->index = index;
	InputHandler::SetBindingConfigs(this->inputGroup->GetId() + "." + this->id, GetType(), this->location, this->index);
}

InputGroup* IInputBinding::GetInputGroup() const {
	return this->inputGroup;
}

const std::string& IInputBinding::GetId() const {
	return this->id;
}

const InputType& IInputBinding::GetType() const {
	return this->type;
}

InputLocation IInputBinding::GetLocation() const {
	return this->location;
}

uint32_t IInputBinding::GetIndex() const {
	return this->index;
}