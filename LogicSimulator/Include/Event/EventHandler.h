#pragma once

#include "Utilities/Logger.h"

#include <mutex>
#include <vector>

struct Event;

class EventHandler {
public:
	EventHandler(const char* name, bool getHandledEvents = false);

	virtual void HandleEvent(Event* event) = 0;

protected:
	Logger& GetLogger();

public:
	static void PushEvent(Event* event);
	static void HandleEvents();

	static void RegisterEventHandler(EventHandler* handler);
	static void UnregisterEventHandler(EventHandler* handler);

private:
	bool getHandledEvents;

	const char* name;
	Logger logger;

private:
	static std::vector<Event*> EventQueue;
	static std::mutex EventQueueLock;

	static std::vector<EventHandler*> EventHandlers;
	static std::mutex EventHandlersLock;

	static Logger Logger;
};