#include "ECS/Component.h"

void Component::OnCreate() {}
void Component::OnUpdate() {}
void Component::PreRender() {}
void Component::PostRender() {}
void Component::OnDestroy() {}

Entity* Component::GetParent() const {
	return this->parent;
}

void Component::SetParentEntity(Entity* entity) {
	this->parent = entity;
}