#pragma once

class Entity;

class Component {
public:
	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void PreRender();
	virtual void PostRender();
	virtual void OnDestroy();

	Entity* GetParent() const;

	friend Entity;

private:
	void SetParentEntity(Entity* entity);

private:
	Entity* parent = nullptr;
};