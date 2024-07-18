#pragma once

#include "Vector3.h"

struct AABB {
public:
	Vector3 min;
	Vector3 max;
};

bool IsCollision(const AABB& aabb1, const AABB& aabb2);