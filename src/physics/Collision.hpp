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

	float tmin = max(
		max(min(t1, t2), min(t3, t4)),
		min(t5, t6));
	float tmax = min(
		min(max(t1, t2), max(t3, t4)),
		max(t5, t6));

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
static bool aabb(const BoxCollider& a, const BoxCollider& b) {
	const auto amin = a.min + a.center;
	const auto amax = a.max + a.center;
	const auto bmin = b.min + b.center;
	const auto bmax = b.max + b.center;
	if (bmin.x <= amax.x && bmax.x >= amin.x && bmin.z <= amax.z && bmax.z >= amin.z) {
		return true;
	}
	return false;
	//if (bmin.x >= amax.x) return true;
	//if (bmin.x <= amax.x) return false;
	
	float t1 = (amax.x >= bmin.x);
	float t2 = (amax.y >= bmin.y);
	float t3 = (amax.z >= bmin.z);
	float t4 = (amax.x <= bmax.x);
	float t5 = (amax.y <= bmax.y);
	float t6 = (amax.z <= bmax.y);


	float t7 = (amin.x <= bmax.x);
	float t8 = (amin.y >= bmax.y);
	float t9 = (amin.z >= bmax.z);
	float t10 = (amin.x >= bmin.x);
	float t11 = (amin.y >= bmin.y);
	float t12 = (amin.z >= bmin.z);


	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
	if (tmax < 0.0f)
	{
		return true;
	}
	if ((t1 && t2 && t3) && (t4 && t5 && t6)) {
		return true;
	}
	if ((t7 && t8 && t9) && (t10 && t11 && t12)) {
		return true;
	}
	return false;
}