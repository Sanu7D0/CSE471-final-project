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

	const auto bmin = b.min + b.center;
	const auto bmax = b.max + b.center;

	float t1 = (bmin.x - r.origin.x) * dirfrac.x;
	float t2 = (bmax.x - r.origin.x) * dirfrac.x;
	float t3 = (bmin.y - r.origin.y) * dirfrac.y;
	float t4 = (bmax.y - r.origin.y) * dirfrac.y;
	float t5 = (bmin.z - r.origin.z) * dirfrac.z;
	float t6 = (bmax.z - r.origin.z) * dirfrac.z;

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