#include "Window/Input/AxisInputBinding.h"
#include "Window/Input/InputEvents.h"

AxisInputBinding::AxisInputBinding(InputGroup* inputGroup, const std::string& id, uint32_t index, InputLocation location, AxisCallback callback)
	: IInputBinding(inputGroup, id, InputType::AXIS, index, location), callback(callback) {}

void AxisInputBinding::HandleEvent(Event* event) {
	if (!this->callback) return;

	EventType eventType = event->GetType();
	switch (eventType) {
	case AxisEvent::GetTypeS():
	{
		AxisEvent* axisEvent = reinterpret_cast<AxisEvent*>(event);

		if (GetLocation() != axisEvent->GetLocation()) return;
		if (GetIndex() != axisEvent->GetIndex()) return;

		this->callback({ GetLocation(), GetIndex(), axisEvent->GetValue(), GetId() });
		event->SetHandled();
		break;
	}
	}
}

void AxisInputBinding::BindCallback(AxisCallback callback) {
	this->callback = callback;
}