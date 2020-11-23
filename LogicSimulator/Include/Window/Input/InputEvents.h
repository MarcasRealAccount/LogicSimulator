#pragma once

#include "Event/Event.h"
#include "Window/Input/ButtonInputBinding.h"
#include "Window/Input/AxisInputBinding.h"

#include <stdint.h>

struct InputEvent : public Event {
	InputEvent(InputLocation location, uint32_t index);

	virtual EventType GetType() const = 0;

	InputLocation GetLocation() const;
	uint32_t GetIndex() const;

private:
	InputLocation location;
	uint32_t index;
};

struct ButtonEvent : public InputEvent {
public:
	ButtonEvent(InputLocation location, uint32_t button, ButtonInputType inputType);

	virtual EventType GetType() const override;
	constexpr inline static EventType GetTypeS() { return EventType::BUTTON_EVENT; }

	ButtonInputType GetInputType() const;

private:
	ButtonInputType inputType;
};

struct AxisEvent : public InputEvent {
public:
	AxisEvent(InputLocation location, uint32_t axis, double value);

	virtual EventType GetType() const override;
	constexpr inline static EventType GetTypeS() { return EventType::AXIS_EVENT; }

	double GetValue() const;

private:
	double value;
};