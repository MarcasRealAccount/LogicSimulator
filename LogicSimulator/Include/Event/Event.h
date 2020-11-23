#pragma once

enum class EventType {
	BUTTON_EVENT,
	AXIS_EVENT
};

struct Event {
public:
	virtual EventType GetType() const = 0;

	void SetHandled();
	bool HasBeenHandled() const;

private:
	bool handled = false;
};