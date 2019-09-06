#version 460

#include "util.glsl"

layout(location = PRIMARY_RAY) rayPayloadInNV RayPayload payload;

void main() {
    payload.dist = -1;

	vec3 dir = gl_WorldRayDirectionNV;
	float u = (1.f + atan(-dir.z, dir.x) * M_1_PI) * 0.5f;
	float v = acos(dir.y) * M_1_PI;

	int check_x = int(u * 10.f);
	int check_y = int(v * 10.f);

	if (dir.y > -0.1 && (check_x + check_y) % 2 == 0) {
		payload.normal.rgb = vec3(0.5f);
	} else {
		payload.normal.rgb = vec3(0.1f);
	}
}
