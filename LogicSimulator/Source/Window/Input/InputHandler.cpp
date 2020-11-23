#include "Window/Input/InputHandler.h"
#include "Window/Input/InputGroup.h"
#include "Utilities/Config/ConfigManager.h"
#include "Utilities/Config/ConfigFile.h"
#include "Window/Window.h"
#include "Event/Event.h"

extern EnumVector<std::pair<InputType, std::pair<InputLocation, uint32_t>>> InputNames;

InputEventHandler InputHandler::EventHandler;

InputEventHandler::InputEventHandler()
	: EventHandler("Input Event Handler", true) {}

void InputEventHandler::HandleEvent(Event* event) {
	if (this->inputGroupOverride) {
		if (this->window) {
			if (this->window->GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_NORMAL && this->inputGroupOverride->DoesCaptureMouse())
				this->window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else if (this->window->GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_DISABLED && !this->inputGroupOverride->DoesCaptureMouse())
				this->window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		this->inputGroupOverride->HandleEvent(event);
	} else {
		if (this->window) {
			bool disabled = false;
			for (auto& group : this->inputGroups) {
				if (group.second->IsEnabled()) {
					if (group.second->DoesCaptureMouse()) {
						disabled = true;
						break;
					}
				}
			}

			if (this->window->GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_NORMAL && disabled) this->window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else if (this->window->GetInputMode(GLFW_CURSOR) == GLFW_CURSOR_DISABLED && !disabled) this->window->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		for (auto& group : this->inputGroups) {
			if (group.second->IsEnabled()) {
				group.second->HandleEvent(event);
			}
		}
	}
}

void InputEventHandler::SetWindow(Window* window) {
	this->window = window;
}

void InputEventHandler::EnableInputGroup(InputGroup* inputGroup) {
	inputGroup->Enable();
}

void InputEventHandler::DisableInputGroup(InputGroup* inputGroup) {
	inputGroup->Disable();
}

void InputEventHandler::SetInputGroupOverride(InputGroup* inputGroup) {
	this->inputGroupOverride = inputGroup;
}

InputGroup* InputEventHandler::GetInputGroup(const std::string& id) {
	auto itr = this->inputGroups.find(id);
	if (itr != this->inputGroups.end()) return itr->second;
	return nullptr;
}

InputGroup* InputEventHandler::GetOrCreateInputGroup(const std::string& id, bool enabled) {
	InputGroup* group = GetInputGroup(id);
	if (group) return group;
	group = new InputGroup(id, enabled);
	this->inputGroups.insert({ id, group });
	return group;
}

void InputEventHandler::SetBindingConfigs(const std::string& id, InputType inputType, InputLocation location, uint32_t index) {
	ConfigFile* inputBindings = ConfigManager::GetConfigFile("InputBindings");
	inputBindings->SetConfigEnum(id, { inputType, { location, index } }, InputNames);
}

void InputEventHandler::GetBindingConfigs(const std::string& id, InputType inputType, InputLocation* location, uint32_t* index) {
	ConfigFile* inputBindings = ConfigManager::GetConfigFile("InputBindings");
	auto input = inputBindings->GetConfigEnum(id, { inputType, { *location, *index } }, InputNames);
	if (input.first != inputType) return;
	*location = input.second.first;
	*index = input.second.second;
}

// PLEASE GOD NOOOO:
EnumVector<std::pair<InputType, std::pair<InputLocation, uint32_t>>>  InputNames = {
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keySpace } }, "Key.Space" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyApostrophe } }, "Key.Apostrophe" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyComma } }, "Key.Comma" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyMinus } }, "Key.Minus" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyPeriod } }, "Key.Period" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keySlash } }, "Key.Slash" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key0 } }, "Key.0" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key1 } }, "Key.1" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key2 } }, "Key.2" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key3 } }, "Key.3" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key4 } }, "Key.4" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key5 } }, "Key.5" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key6 } }, "Key.6" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key7 } }, "Key.7" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key8 } }, "Key.8" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::key9 } }, "Key.9" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keySemicolon } }, "Key.Semicolon" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyEqual } }, "Key.Equal" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyA } }, "Key.A" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyB } }, "Key.B" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyC } }, "Key.C" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyD } }, "Key.D" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyE } }, "Key.E" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF } }, "Key.F" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyG } }, "Key.G" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyH } }, "Key.H" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyI } }, "Key.I" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyJ } }, "Key.J" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyK } }, "Key.K" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyL } }, "Key.L" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyM } }, "Key.M" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyN } }, "Key.N" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyO } }, "Key.O" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyP } }, "Key.P" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyQ } }, "Key.Q" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyR } }, "Key.R" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyS } }, "Key.S" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyT } }, "Key.T" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyU } }, "Key.U" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyV } }, "Key.V" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyW } }, "Key.W" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyX } }, "Key.X" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyY } }, "Key.Y" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyZ } }, "Key.Z" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyLeftBracket } }, "Key.LeftBracket" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyBackslash } }, "Key.Backslash" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyRightBracket } }, "Key.RightBracket" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyGraveAccent } }, "Key.GraveAccent" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyWorld1 } }, "Key.World1" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyWorld2 } }, "Key.World2" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyEscape } }, "Key.Escape" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyEnter } }, "Key.Enter" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyTab } }, "Key.Tab" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyBackspace } }, "Key.Backspace" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyInsert } }, "Key.Insert" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyDelete } }, "Key.Delete" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyRight } }, "Key.Right" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyLeft } }, "Key.Left" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyDown } }, "Key.Down" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyUp } }, "Key.Up" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyPageUp } }, "Key.PageUp" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyPageDown } }, "Key.PageDown" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyHome } }, "Key.Home" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyEnd } }, "Key.End" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyCapsLock } }, "Key.CapsLock" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyScrollLock } }, "Key.ScrollLock" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyNumLock } }, "Key.NumLock" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyPrintScreen } }, "Key.PrintScreen" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyPause } }, "Key.Pause" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF1 } }, "Key.F1" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF2 } }, "Key.F2" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF3 } }, "Key.F3" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF4 } }, "Key.F4" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF5 } }, "Key.F5" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF6 } }, "Key.F6" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF7 } }, "Key.F7" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF8 } }, "Key.F8" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF9 } }, "Key.F9" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF10 } }, "Key.F10" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF11 } }, "Key.F11" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF12 } }, "Key.F12" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF13 } }, "Key.F13" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF14 } }, "Key.F14" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF15 } }, "Key.F15" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF16 } }, "Key.F16" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF17 } }, "Key.F17" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF18 } }, "Key.F18" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF19 } }, "Key.F19" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF20 } }, "Key.F20" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF21 } }, "Key.F21" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF22 } }, "Key.F22" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF23 } }, "Key.F23" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF24 } }, "Key.F24" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyF25 } }, "Key.F25" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp0 } }, "Key.Kp0" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp1 } }, "Key.Kp1" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp2 } }, "Key.Kp2" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp3 } }, "Key.Kp3" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp4 } }, "Key.Kp4" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp5 } }, "Key.Kp5" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp6 } }, "Key.Kp6" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp7 } }, "Key.Kp7" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp8 } }, "Key.Kp8" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKp9 } }, "Key.Kp9" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKpDecimal } }, "Key.KpDecimal" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKpDivide } }, "Key.KpDivide" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKpMultiply } }, "Key.KpMultiply" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKpSubtract } }, "Key.KpSubtract" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKpAdd } }, "Key.KpAdd" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKpEnter } }, "Key.KpEnter" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyKpEqual } }, "Key.KpEqual" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyLeftShift } }, "Key.LeftShift" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyLeftControl } }, "Key.LeftControl" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyLeftAlt } }, "Key.LeftAlt" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyLeftSuper } }, "Key.LeftSuper" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyRightShift } }, "Key.RightShift" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyRightControl } }, "Key.RightControl" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyRightAlt } }, "Key.RightAlt" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyRightSuper } }, "Key.RightSuper" },
	{ { InputType::BUTTON, { InputLocation::KEYBOARD, buttons::keyMenu } }, "Key.Menu" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouseLeft } }, "Mouse.Left" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouseRight } }, "Mouse.Right" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouseMiddle } }, "Mouse.Middle" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouse1 } }, "Mouse.1" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouse2 } }, "Mouse.2" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouse3 } }, "Mouse.3" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouse4 } }, "Mouse.4" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouse5 } }, "Mouse.5" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouse6 } }, "Mouse.6" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouse7 } }, "Mouse.7" },
	{ { InputType::BUTTON, { InputLocation::MOUSE, buttons::mouse8 } }, "Mouse.8" },
	{ { InputType::AXIS, { InputLocation::MOUSE, axises::mouseX } }, "Mouse.X" },
	{ { InputType::AXIS, { InputLocation::MOUSE, axises::mouseY } }, "Mouse.Y" },
	{ { InputType::AXIS, { InputLocation::MOUSE, axises::mouseWheelX } }, "Mouse.WheelX" },
	{ { InputType::AXIS, { InputLocation::MOUSE, axises::mouseWheelY } }, "Mouse.WheelY" }
};