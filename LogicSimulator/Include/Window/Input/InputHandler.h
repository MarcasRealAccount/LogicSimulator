#pragma once

#include "Event/EventHandler.h"
#include "Window/Input/IInputBinding.h"

#include <unordered_map>

struct Event;
class InputGroup;
class Window;

class InputEventHandler : public EventHandler {
public:
	InputEventHandler();

	virtual void HandleEvent(Event* event) override;

	void SetWindow(Window* window);

	void EnableInputGroup(InputGroup* inputGroup);
	void DisableInputGroup(InputGroup* inputGroup);
	void SetInputGroupOverride(InputGroup* inputGroup);

	InputGroup* GetInputGroup(const std::string& id);
	InputGroup* GetOrCreateInputGroup(const std::string& id, bool enabled = true);

	void SetBindingConfigs(const std::string& id, InputType inputType, InputLocation location, uint32_t index);
	void GetBindingConfigs(const std::string& id, InputType inputType, InputLocation* location, uint32_t* index);

private:
	std::unordered_map<std::string, InputGroup*> inputGroups;

	InputGroup* inputGroupOverride = nullptr;
	Window* window = nullptr;
};

class InputHandler {
public:
	static inline void Init() { EventHandler::RegisterEventHandler(&InputHandler::EventHandler); }

	static inline void SetWindow(Window* window) { InputHandler::EventHandler.SetWindow(window); }

	static inline void EnableInputGroup(const std::string& id) { InputHandler::EventHandler.EnableInputGroup(GetInputGroup(id)); }
	static inline void EnableInputGroup(InputGroup* inputGroup) { InputHandler::EventHandler.EnableInputGroup(inputGroup); }
	static inline void DisableInputGroup(const std::string& id) { InputHandler::EventHandler.DisableInputGroup(GetInputGroup(id)); }
	static inline void DisableInputGroup(InputGroup* inputGroup) { InputHandler::EventHandler.DisableInputGroup(inputGroup); }

	static inline void SetInputGroupOverride(const std::string& id) { InputHandler::EventHandler.SetInputGroupOverride(GetInputGroup(id)); }
	static inline void SetInputGroupOverride(InputGroup* inputGroup) { InputHandler::EventHandler.SetInputGroupOverride(inputGroup); }

	static inline InputGroup* GetInputGroup(const std::string& id) { return InputHandler::EventHandler.GetInputGroup(id); }
	static inline InputGroup* GetOrCreateInputGroup(const std::string& id, bool enabled = true) { return InputHandler::EventHandler.GetOrCreateInputGroup(id, enabled); }

	static inline void SetBindingConfigs(const std::string& id, InputType inputType, InputLocation location, uint32_t index) { InputHandler::EventHandler.SetBindingConfigs(id, inputType, location, index); }
	static inline void GetBindingConfigs(const std::string& id, InputType inputType, InputLocation* location, uint32_t* index) { InputHandler::EventHandler.GetBindingConfigs(id, inputType, location, index); }

private:
	static InputEventHandler EventHandler;
};