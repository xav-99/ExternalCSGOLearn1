#pragma once

struct Vector4
{
	float x{}, y{}, z{}, w{};
};

struct Vector3
{
	float x{}, y{}, z{};
};

struct Vector2
{
	float x{}, y{};
};

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};


struct Colour
{
	uint8_t r{ }, g{ }, b{ };
};