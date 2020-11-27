#include "ECS/Entity.h"

Entity::~Entity() {
	{
		std::unique_lock<std::mutex> mlock(this->childrenLock);
		auto itr = this->children.begin();
		while (itr != this->children.end()) {
			Entity* e = itr->Cast<Entity>();
			if (e) delete e;
			itr++;
		}
		this->children.clear();
	}
	{
		std::unique_lock<std::mutex> mlock(this->componentsLock);
		auto itr = this->components.begin();
		while (itr != this->components.end()) {
			Component* c = itr->Cast<Component>();
			if (c) delete c;
			itr++;
		}
		this->components.clear();
	}
}

void Entity::OnCreate() {}
void Entity::OnUpdate() {}
void Entity::PreRender() {}
void Entity::PostRender() {}
void Entity::OnDestroy() {}

Entity* Entity::GetParent() const {
	return this->parent;
}