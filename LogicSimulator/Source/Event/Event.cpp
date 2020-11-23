#include "Event/Event.h"

void Event::SetHandled() {
	this->handled = true;
}

bool Event::HasBeenHandled() const {
	return this->handled;
}