#ifndef KERNELS_LIGHTS_BSDF_H
#define KERNELS_LIGHTS_BSDF_H

#include "kernels/wrappers.h"

// Quad-shaped light source
struct QuadLight {
	float3 emission;
	float3 position;
	float3 normal;
	// x and y vectors spanning the quad, with
	// the half-width and height in the w component
	float3 v_x;
	float width;
	float3 v_y;
	float height;
};

__device__ float3 sample_quad_light_position(IN(QuadLight, light), float2 samples) {
	return samples.x * light.v_x * light.width
		+ samples.y * light.v_y * light.height + light.position;
}

/* Compute the PDF of sampling the sampled point p light with the ray specified by orig and dir,
 * assuming the light is not occluded
 */
__device__ float quad_light_pdf(IN(QuadLight, light), IN(float3, p), IN(float3, orig), IN(float3, dir)) {
	float surface_area = light.width * light.height;
	float3 to_pt = p - dir;
	float dist_sqr = dot(to_pt, to_pt);
	float n_dot_w = dot(light.normal, neg(dir));
	if (n_dot_w < EPSILON) {
		return 0.f;
	}
	return dist_sqr / (n_dot_w * surface_area);
}

__device__ bool quad_intersect(IN(QuadLight, light), IN(float3, orig), IN(float3, dir),
	OUT(float, t), OUT(float3, light_pos))
{
	float denom = dot(dir, light.normal);
	if (denom >= EPSILON) {
		t = dot(light.position - orig, light.normal) / denom;
		if (t < 0.f) {
			return false;
		}

		// It's a finite plane so now see if the hit point is actually inside the plane
		light_pos = orig + dir * t;
		float3 hit_v = light_pos - light.position;
		if (abs(dot(hit_v, light.v_x)) < light.width && abs(dot(hit_v, light.v_y)) < light.height) {
			return true;
		}
	}
	return false;
}

#endif

