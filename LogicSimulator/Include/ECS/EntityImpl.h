#pragma once

#include "ECS/Entity.h"
#include "ECS/Component.h"
#include "ECS/Components/TransformComponent.h"

template <typename T>
Any::Any(T* data)
	: type(typeid(T)), data(data) {}

template <typename T>
inline constexpr bool Any::IsOfType() {
	return type == typeid(T);
}

template <typename T>
inline constexpr T* Any::Cast() {
	return reinterpret_cast<T*>(this->data);
}

template <typename T>
inline constexpr T* Any::CastChecked() {
	if (IsOfType<T>()) return Cast<T>();
	return nullptr;
}

template <typename T>
inline constexpr T* Entity::GetChild(uint32_t index) {
	return GetChildImpl<std::is_base_of_v<Entity, T>, T>();
}

template <typename T>
inline constexpr std::vector<T*> Entity::GetChildren() {
	return GetChildrenImpl<std::is_base_of_v<Entity, T>, T>();
}

template <typename T>
inline constexpr void Entity::GetChildren(std::vector<T*>& entities) {
	GetChildrenImpl<std::is_base_of_v<Entity, T>, T>(entities);
}

template <typename T, typename... Args>
inline constexpr T* Entity::AddChild(Args... args) {
	return AddChildImpl<std::is_base_of_v<Entity, T>, T, Args...>(args...);
}

template <typename T, typename... Args>
inline constexpr T* Entity::InsertChild(uint32_t index, Args... args) {
	return InsertChildImpl<std::is_base_of_v<Entity, T>, Args...>(index, args...);
}

template <typename T>
inline constexpr void Entity::RemoveChild(T* child) {
	RemoveChildImpl<std::is_base_of_v<Entity, T>, T>(child);
}

template <typename T>
inline constexpr void Entity::RemoveChild(uint32_t index) {
	RemoveChildImpl<std::is_base_of_v<Entity, T>, T>(index);
}

template <typename T>
T* Entity::GetChildImplTrue(uint32_t index) {
	std::unique_lock<std::mutex> mlock(this->childrenLock);
	if (index < this->children.size()) {
		auto itr = this->children.begin() + index;
		if (itr->IsOfType<T>()) return itr->Cast<T>();
	}
	return nullptr;
}

template <typename T>
std::vector<T*> Entity::GetChildrenImplTrue() {
	std::unique_lock<std::mutex> mlock(this->childrenLock);
	std::vector<T*> entities;
	auto itr = this->children.begin();
	while (itr != this->children.end()) {
		if (itr->IsOfType<T>()) entities.push_back(itr->Cast<T>());
		itr++;
	}
	return entities;
}

template <typename T>
void Entity::GetChildrenImplTrue(std::vector<T*>& entities) {
	std::unique_lock<std::mutex> mlock(this->childrenLock);
	auto itr = this->children.begin();
	while (itr != this->children.end()) {
		if (itr->IsOfType<T>()) entities.push_back(itr->Cast<T>());
		itr++;
	}
}

template <typename T, typename... Args>
T* Entity::AddChildImplTrue(Args... args) {
	T* t = nullptr;
	{
		std::unique_lock<std::mutex> mlock(this->childrenLock);
		t = new T(args...);
		t->parent = this;
		t->AddComponent<TransformComponent>();
		this->children.push_back({ t });
	}
	if (t) t->OnCreate();
	return t;
}

template <typename T, typename... Args>
T* Entity::InsertChildImplTrue(uint32_t index, Args... args) {
	T* t = nullptr;
	{
		std::unique_lock<std::mutex> mlock(this->childrenLock);
		t = new T(args...);
		t->parent = this;
		t->AddComponent<TransformComponent>();
		std::vector<T*>::iterator itr;
		if (index < this->children.size()) itr = this->children.begin() + index;
		else itr = this->children.end();
		this->children.insert(itr, { t });
	}
	if (t) t->OnCreate();
	return t;
}

template <typename T>
void Entity::RemoveChildImplTrue(T* child) {
	if (!child) return;
	std::unique_lock<std::mutex> mlock(this->childrenLock);
	auto itr = this->children.begin();
	while (itr != this->children.end()) {
		if (*itr == child) {
			child->OnDestroy();
			delete child;
			this->children.erase(itr);
			return;
		}
	}
}

template <typename T>
void Entity::RemoveChildImplTrue(uint32_t index) {
	std::unique_lock<std::mutex> mlock(this->childrenLock);
	if (index < this->children.size()) {
		auto itr = this->children.begin() + index;
		T* t = itr->CastChecked<T>();
		if (t) {
			t->OnDestroy();
			delete t;
		}
		this->children.erase(itr);
	}
}

template <bool B, typename T>
inline constexpr T* Entity::GetChildImpl(uint32_t index) {
	if constexpr (B) return GetChildImplTrue<T>(index);
	return nullptr;
}

template <bool B, typename T>
inline constexpr std::vector<T*> Entity::GetChildrenImpl() {
	if constexpr (B) return GetChildrenImplTrue<T>();
	return {};
}

template <bool B, typename T>
inline constexpr void Entity::GetChildrenImpl(std::vector<T*>& entities) {
	if constexpr (B) Entity::GetChildrenImplTrue<T>(entities);
}

template <bool B, typename T, typename... Args>
inline constexpr T* Entity::AddChildImpl(Args... args) {
	if constexpr (B) return AddChildImplTrue<T, Args...>(args...);
	return nullptr;
}

template <bool B, typename T, typename... Args>
inline constexpr T* Entity::InsertChildImpl(uint32_t indx, Args... args) {
	if constexpr (B) return InsertChildImplTrue<T, Args...>(index, args...);
	return nullptr;
}

template <bool B, typename T>
inline constexpr void Entity::RemoveChildImpl(T* child) {
	if constexpr (B) RemoveChildImplTrue<T>(child);
}

template <bool B, typename T>
inline constexpr void Entity::RemoveChildImpl(uint32_t index) {
	if constexpr (B) RemoveChildImplTrue<T>(index);
}

template <typename T>
inline constexpr T* Entity::GetComponent() {
	return GetComponentImpl<std::is_base_of_v<Component, T>, T>();
}

template <typename T>
inline constexpr std::vector<T*> Entity::GetComponents() {
	return GetComponentsImpl<std::is_base_of_v<Component, T>, T>();
}

template <typename T>
inline constexpr void Entity::GetComponents(std::vector<T*>& comps) {
	GetComponentsImpl<std::is_base_of_v<Component, T>, T>(comps);
}

template <typename T>
inline constexpr std::vector<T*> Entity::GetAllComponents() {
	return GetAllComponentsImpl<std::is_base_of_v<Component, T>, T>();
}

template <typename T>
inline constexpr void Entity::GetAllComponents(std::vector<T*>& comps) {
	GetAllComponentsImpl<std::is_base_of_v<Component, T>, T>(comps);
}

template <typename T, typename... Args>
inline constexpr T* Entity::AddComponent(Args... args) {
	return AddComponentImpl<std::is_base_of_v<Component, T>, T, Args...>(args...);
}

template <typename T>
inline constexpr void Entity::RemoveComponent(T* component) {
	if constexpr (std::is_same_v<TransformComponent, T>) return;
	return RemoveComponentImpl<std::is_base_of_v<Component, T>, T>(component);
}

template <typename T>
T* Entity::GetComponentImplTrue() {
	std::unique_lock<std::mutex> mlock(this->componentsLock);
	auto itr = this->components.begin();
	while (itr != this->components.end()) {
		if (itr->IsOfType<T>()) return itr->Cast<T>();
		itr++;
	}
	return nullptr;
}

template <typename T>
std::vector<T*> Entity::GetComponentsImplTrue() {
	std::unique_lock<std::mutex> mlock(this->componentsLock);
	std::vector<T*> comps;
	auto itr = this->components.begin();
	while (itr != this->components.end()) {
		if (itr->IsOfType<T>()) comps.push_back(itr->Cast<T>());
		itr++;
	}
	return comps;
}

template <typename T>
void Entity::GetComponentsImplTrue(std::vector<T*>& comps) {
	std::unique_lock<std::mutex> mlock(this->componentsLock);
	auto itr = this->components.begin();
	while (itr != this->components.end()) {
		if (itr->IsOfType<T>()) comps.push_back(itr->Cast<T>());
		itr++;
	}
}

template <typename T>
std::vector<T*> Entity::GetAllComponentsImplTrue() {
	std::vector<T*> comps;
	std::unique_lock<std::mutex> mChildrenLock(this->childrenLock);
	auto childItr = this->children.begin();
	while (childItr != this->children.end()) {
		childItr->Cast<Entity>()->GetAllComponents<T>(comps);
		childItr++;
	}
	auto compItr = this->components.begin();
	while (compItr != this->components.end()) {
		if (compItr->IsOfType<T>()) comps.push_back(compItr->Cast<T>());
		compItr++;
	}
	return comps;
}

template <typename T>
void Entity::GetAllComponentsImplTrue(std::vector<T*>& comps) {
	std::unique_lock<std::mutex> mChildrenLock(this->childrenLock);
	auto childItr = this->children.begin();
	while (childItr != this->children.end()) {
		childItr->Cast<Entity>()->GetAllComponents<T>(comps);
		childItr++;
	}
	auto compItr = this->components.begin();
	while (compItr != this->components.end()) {
		if (compItr->IsOfType<T>()) comps.push_back(compItr->Cast<T>());
		compItr++;
	}
}

template <typename T, typename... Args>
T* Entity::AddComponentImplTrue(Args... args) {
	T* t = nullptr;
	{
		std::unique_lock<std::mutex> mlock(this->componentsLock);
		t = new T(args...);
		t->SetParentEntity(this);
		this->components.push_back(t);
	}
	if (t) t->OnCreate();
	return t;
}

template <typename T>
void Entity::RemoveComponentImplTrue(T* component) {
	if (!component) return;
	std::unique_lock<std::mutex> mlock(this->componentsLock);
	auto itr = this->components.begin();
	while (itr != this->components.end()) {
		if (*itr == component) {
			component->OnDestroy();
			delete component;
			this->components.erase(itr);
			return;
		}
		itr++;
	}
}

template <bool B, typename T>
inline constexpr T* Entity::GetComponentImpl() {
	if constexpr (B) return GetComponentImplTrue<T>();
	return nullptr;
}

template <bool B, typename T>
inline constexpr std::vector<T*> Entity::GetComponentsImpl() {
	if constexpr (B) return GetComponentsImplTrue<T>();
	return {};
}

template <bool B, typename T>
inline constexpr void Entity::GetComponentsImpl(std::vector<T*>& comps) {
	if constexpr (B) GetComponentsImplTrue<T>(comps);
}

template <bool B, typename T>
inline constexpr std::vector<T*> Entity::GetAllComponentsImpl() {
	if constexpr (B) return GetAllComponentsImplTrue<T>();
	return {};
}

template <bool B, typename T>
inline constexpr void Entity::GetAllComponentsImpl(std::vector<T*>& comps) {
	if constexpr (B) GetAllComponentsImplTrue<T>(comps);
}

template <bool B, typename T, typename... Args>
inline constexpr T* Entity::AddComponentImpl(Args... args) {
	if constexpr (B) return AddComponentImplTrue<T, Args...>(args...);
	return nullptr;
}

template <bool B, typename T>
inline constexpr void Entity::RemoveComponentImpl(T* component) {
	if constexpr (B) RemoveComponentImplTrue<T>();
}