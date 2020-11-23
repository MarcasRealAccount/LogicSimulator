#pragma once

#include <stdint.h>
#include <string>

class InputGroup;
struct Event;

enum class InputLocation : uint32_t {
	KEYBOARD,
	MOUSE,
	LAST = MOUSE,
	UNKNOWN
};

enum class InputType : uint32_t {
	BUTTON,
	AXIS
};

struct IInputBinding {
public:
	IInputBinding(InputGroup* inputGroup, const std::string& id, InputType type, uint32_t index, InputLocation location);
	virtual ~IInputBinding();

	virtual void HandleEvent(Event* event) = 0;

	void SetBinding(InputLocation location, uint32_t index);
	void SetInputLocation(InputLocation location);
	void SetIndex(uint32_t index);

	InputGroup* GetInputGroup() const;

	const std::string& GetId() const;
	const InputType& GetType() const;
	InputLocation GetLocation() const;
	uint32_t GetIndex() const;

private:
	InputGroup* inputGroup;

	std::string id;
	InputType type;
	InputLocation location;
	uint32_t index;
};