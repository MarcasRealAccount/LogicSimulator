#pragma once

#include <stdint.h>

enum class UniformType : uint32_t {
	FLOAT,
	FLOAT_VEC2,
	FLOAT_VEC3,
	FLOAT_VEC4,
	INT,
	INT_VEC2,
	INT_VEC3,
	INT_VEC4,
	UINT,
	UINT_VEC2,
	UINT_VEC3,
	UINT_VEC4,
	FLOAT_MAT2,
	FLOAT_MAT3,
	FLOAT_MAT4
};

template <typename T>
struct Uniform {
public:
	Uniform(T defaultValue) : value(defaultValue) {}

public:
	T value;
};