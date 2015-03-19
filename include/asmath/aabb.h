#ifndef ASMATH_INCLUDE_AABB_H
#define ASMATH_INCLUDE_AABB_H

#include <stdint.h>
#include <asmath/asmath_platform.h>

struct AABB_i32 {
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
};

ASMATH_EXT_LANG char aabb_intersects_i32(struct AABB_i32 *a, struct AABB_i32 *b);
ASMATH_EXT_LANG char aabb_intersects_conventional_i32(struct AABB_i32 *a, struct AABB_i32 *b);

#endif
