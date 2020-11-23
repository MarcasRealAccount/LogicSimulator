#include "Window/Input/InputEvents.h"

InputEvent::InputEvent(InputLocation location, uint32_t index)
	: location(location), index(index) {}

InputLocation InputEvent::GetLocation() const {
	return this->location;
}

uint32_t InputEvent::GetIndex() const {
	return this->index;
}

ButtonEvent::ButtonEvent(InputLocation location, uint32_t button, ButtonInputType inputType)
	: InputEvent(location, button), inputType(inputType) {}

EventType ButtonEvent::GetType() const {
	return ButtonEvent::GetTypeS();
}

ButtonInputType ButtonEvent::GetInputType() const {
	return this->inputType;
}

AxisEvent::AxisEvent(InputLocation location, uint32_t axis, double value)
	: InputEvent(location, axis), value(value) {}

EventType AxisEvent::GetType() const {
	return AxisEvent::GetTypeS();
}

double AxisEvent::GetValue() const {
	return this->value;
}