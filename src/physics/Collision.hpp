#pragma once
#include <glm/glm.hpp>

#include <algorithm>

#include "Collider.hpp"
#include "Ray.hpp"

using std::min;
using std::max;

static bool intersect(const BoxCollider& b, Ray r)
{
	float t; // length of ray untill intersection
	const auto dirfrac = glm::vec3(1.0f) / r.dir;

	float t1 = (b.min.x - r.origin.x) * dirfrac.x;
	float t2 = (b.max.x - r.origin.x) * dirfrac.x;
	float t3 = (b.min.y - r.origin.y) * dirfrac.y;
	float t4 = (b.max.y - r.origin.y) * dirfrac.y;
	float t5 = (b.min.z - r.origin.z) * dirfrac.z;
	float t6 = (b.max.z - r.origin.z) * dirfrac.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	if (tmax < 0.0f)
	{
		t = tmax;
		return false;
	}

	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;
	return true;
}