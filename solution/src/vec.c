#include "vec.h"

vec* new_vec(void) {
    vec* res;
    res = (vec*)malloc(sizeof(vec));
    return res;
}

int  free_vec(vec *res) {
    free(res);
    return 1;
}

int vec_add_to(vec *res, const vec *b) {
	res->x += b->x;
	res->y += b->y;
	res->z += b->z;
	return 1;
}

int vec_add(vec *res, const vec *a, const vec *b) {
	res->x = a->x + b->x;
	res->y = a->y + b->y;
	res->z = a->z + b->z;
	return 1;
}