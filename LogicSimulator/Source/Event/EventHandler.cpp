#include "Event/EventHandler.h"
#include "Event/Event.h"

std::vector<Event*> EventHandler::EventQueue;
std::mutex EventHandler::EventQueueLock;

std::vector<EventHandler*> EventHandler::EventHandlers;
std::mutex EventHandler::EventHandlersLock;

Logger EventHandler::Logger("Event Handler");

EventHandler::EventHandler(const char* name, bool getHandledEvents)
	: name(name), logger(name), getHandledEvents(getHandledEvents) {}

Logger& EventHandler::GetLogger() {
	return this->logger;
}

void EventHandler::PushEvent(Event* event) {
	if (!event) {
		EventHandler::Logger.LogWarning("EventHandler::PushEvent got a nullptr!");
		return;
	}

	std::unique_lock<std::mutex> mlock(EventHandler::EventQueueLock);
	EventHandler::EventQueue.push_back(event);
}

void EventHandler::HandleEvents() {
	std::vector<Event*> eventsToHandle;
	{
		std::unique_lock<std::mutex> mlock(EventHandler::EventQueueLock);
		eventsToHandle = EventHandler::EventQueue;
		EventHandler::EventQueue.clear();
	}

	std::unique_lock<std::mutex> mlock(EventHandler::EventHandlersLock);
	for (auto& event : eventsToHandle) {
		for (auto& handler : EventHandler::EventHandlers) {
			if (event->HasBeenHandled() && !handler->getHandledEvents) continue;
			try {
				handler->HandleEvent(event);
			} catch (std::exception e) {
				handler->logger.LogError("Exception was thrown from HandleEvent.\n%s", e.what());
			}
		}
		delete event;
	}
}

void EventHandler::RegisterEventHandler(EventHandler* handler) {
	if (!handler) {
		EventHandler::Logger.LogWarning("EventHandler::RegisterEventHandler got a nullptr!");
		return;
	}

	std::unique_lock<std::mutex> mlock(EventHandler::EventHandlersLock);
	auto itr = EventHandler::EventHandlers.begin();
	while (itr != EventHandler::EventHandlers.end()) {
		if (*itr == handler) return;
		itr++;
	}

	EventHandler::EventHandlers.push_back(handler);
	EventHandler::Logger.LogInfo("Registered event handler '%s'", handler->name);
}

void EventHandler::UnregisterEventHandler(EventHandler* handler) {
	if (!handler) {
		EventHandler::Logger.LogWarning("EventHandler::UnregisterEventHandler got a nullptr!");
		return;
	}

	std::unique_lock<std::mutex> mlock(EventHandler::EventHandlersLock);
	auto itr = EventHandler::EventHandlers.begin();
	while (itr != EventHandler::EventHandlers.end()) {
		if (*itr == handler) {
			EventHandler::EventHandlers.erase(itr);
			break;
		}
		itr++;
	}
	EventHandler::Logger.LogInfo("Unregistered event handler '%'s", handler->name);
}