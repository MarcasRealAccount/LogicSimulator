#pragma once

#include <stdint.h>
#include <vector>
#include <type_traits>
#include <mutex>

class Any {
public:
	template <typename T> Any(T* data);
	template <typename T> inline constexpr bool IsOfType();
	template <typename T> inline constexpr T* Cast();
	template <typename T> inline constexpr T* CastChecked();

private:
	const type_info& type;
	void* data;
};

class Entity {
public:
	virtual ~Entity();

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void PreRender();
	virtual void PostRender();
	virtual void OnDestroy();

	Entity* GetParent() const;

	// Entities:
	template <typename T> inline constexpr T* GetChild(uint32_t index);
	template <typename T> inline constexpr std::vector<T*> GetChildren();
	template <typename T> inline constexpr void GetChildren(std::vector<T*>& entities);
	template <typename T, typename... Args> inline constexpr T* AddChild(Args... args);
	template <typename T, typename... Args> inline constexpr T* InsertChild(uint32_t index, Args... args);
	template <typename T> inline constexpr void RemoveChild(T* child);
	template <typename T> inline constexpr void RemoveChild(uint32_t index);

	// Components:
	template <typename T> inline constexpr T* GetComponent();
	template <typename T> inline constexpr std::vector<T*> GetComponents();
	template <typename T> inline constexpr void GetComponents(std::vector<T*>& comps);
	template <typename T> inline constexpr std::vector<T*> GetAllComponents();
	template <typename T> inline constexpr void GetAllComponents(std::vector<T*>& comps);
	template <typename T, typename... Args> inline constexpr T* AddComponent(Args... args);
	template <typename T> inline constexpr void RemoveComponent(T* component);

private:
	// Entities:
	template <typename T> T* GetChildImplTrue(uint32_t index);
	template <typename T> std::vector<T*> GetChildrenImplTrue();
	template <typename T> void GetChildrenImplTrue(std::vector<T*>& entities);
	template <typename T, typename... Args> T* AddChildImplTrue(Args... args);
	template <typename T, typename... Args> T* InsertChildImplTrue(uint32_t index, Args... args);
	template <typename T> void RemoveChildImplTrue(T* child);
	template <typename T> void RemoveChildImplTrue(uint32_t index);

	template <bool B, typename T> inline constexpr T* GetChildImpl(uint32_t index);
	template <bool B, typename T> inline constexpr std::vector<T*> GetChildrenImpl();
	template <bool B, typename T> inline constexpr void GetChildrenImpl(std::vector<T*>& entities);
	template <bool B, typename T, typename... Args> inline constexpr T* AddChildImpl(Args... args);
	template <bool B, typename T, typename... Args> inline constexpr T* InsertChildImpl(uint32_t indx, Args... args);
	template <bool B, typename T> inline constexpr void RemoveChildImpl(T* child);
	template <bool B, typename T> inline constexpr void RemoveChildImpl(uint32_t index);

	// Components:
	template <typename T> T* GetComponentImplTrue();
	template <typename T> std::vector<T*> GetComponentsImplTrue();
	template <typename T> void GetComponentsImplTrue(std::vector<T*>& comps);
	template <typename T> std::vector<T*> GetAllComponentsImplTrue();
	template <typename T> void GetAllComponentsImplTrue(std::vector<T*>& comps);
	template <typename T, typename... Args> T* AddComponentImplTrue(Args... args);
	template <typename T> void RemoveComponentImplTrue(T* component);

	template <bool B, typename T> inline constexpr T* GetComponentImpl();
	template <bool B, typename T> inline constexpr std::vector<T*> GetComponentsImpl();
	template <bool B, typename T> inline constexpr void GetComponentsImpl(std::vector<T*>& comps);
	template <bool B, typename T> inline constexpr std::vector<T*> GetAllComponentsImpl();
	template <bool B, typename T> inline constexpr void GetAllComponentsImpl(std::vector<T*>& comps);
	template <bool B, typename T, typename... Args> inline constexpr T* AddComponentImpl(Args... args);
	template <bool B, typename T> inline constexpr void RemoveComponentImpl(T* component);

private:
	Entity* parent = nullptr;
	std::vector<Any> children;
	std::vector<Any> components;

	std::mutex childrenLock;
	std::mutex componentsLock;
};

#include "ECS/EntityImpl.h"