#include "Window/Input/ButtonInputBinding.h"
#include "Window/Input/InputEvents.h"

ButtonInputBinding::ButtonInputBinding(InputGroup* inputGroup, const std::string& id, uint32_t button, ButtonInputType inputType, InputLocation location, ButtonCallback callback)
	: IInputBinding(inputGroup, id, InputType::BUTTON, button, location), inputType(inputType), callback(callback) {}

void ButtonInputBinding::HandleEvent(Event* event) {
	if (!this->callback) return;

	EventType eventType = event->GetType();
	switch (eventType) {
	case ButtonEvent::GetTypeS():
	{
		ButtonEvent* buttonEvent = reinterpret_cast<ButtonEvent*>(event);

		if (GetLocation() != buttonEvent->GetLocation()) return;
		if (GetIndex() != buttonEvent->GetIndex()) return;
		if (GetInputType() != buttonEvent->GetInputType()) return;

		this->callback({ GetLocation(), GetInputType(), GetIndex(), GetId() });
		event->SetHandled();
		break;
	}
	}
}

void ButtonInputBinding::BindCallback(ButtonCallback callback) {
	this->callback = callback;
}

ButtonInputType ButtonInputBinding::GetInputType() const {
	return this->inputType;
}
