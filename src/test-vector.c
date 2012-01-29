#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <mtbl.h>

#include "vector_types.h"

#define NAME	"test-vector"

const uint32_t u32_array[] = { 23, 17, 1, 2, 3, 4, 5, 4, 3, 2, 1, 17, 23 };

const char str_array[] = "`1234567890[]',.PYFGCRL/=AOEUIDHTNS-;QJKXBMWVZ";

static int
test5(void)
{
	int ret = 0;
	uint32_vec *v;

	v = uint32_vec_init(0);
	for (unsigned i = 0; i < 16; i++)
		uint32_vec_append(v, u32_array, sizeof(u32_array));
	//for (unsigned i = 0; i < v->n; i++)
		//fprintf(stderr, "v->v[%d] = %u\n", i, v->v[i]);
	uint32_vec_reset(v);

	for (uint32_t i = 0; i < 1024; i++)
		uint32_vec_add(v, i);
	for (uint32_t i = 0; i < 1024; i++) {
		//fprintf(stderr, "v->v[%d] = %u\n", i, v->v[i]);
		if (v->v[i] != i)
			ret |= 1;
	}

	uint32_vec_destroy(&v);

	return (ret);
}

static int
test4(void)
{
	int ret = 0;
	ubuf *v;

	v = ubuf_init(0);
	for (unsigned i = 0; i < 1024; i++)
		ubuf_append(v, (uint8_t *) str_array, sizeof(str_array));

	for (unsigned i = 0 ; i < v->n; i++) {
		unsigned j = i % sizeof(str_array);
		/*
		fprintf(stderr, "v->v[%d] = '%c', str_array[%d] = '%c'\n",
			i, v->v[i], j, str_array[j]);
		*/
		if (v->v[i] != str_array[j])
			ret |= 1;
	}

	ubuf_destroy(&v);

	return (ret);
}

static int
test3(void)
{
	int ret = 0;
	ubuf *v;

	v = ubuf_init(16);
	ubuf_append(v, (uint8_t *) str_array, sizeof(str_array));
	for (unsigned i = 0; i < v->n; i++) {
		//fprintf(stderr, "v->v[%d] = '%c'\n", i, v->v[i]);
		if (v->v[i] != str_array[i])
			ret |= 1;
	}
	ubuf_destroy(&v);

	return (ret);
}

static int
test2(void)
{
	int ret = 0;
	uint32_vec *v;

	v = uint32_vec_init(0);
	uint32_vec_append(v, u32_array, sizeof(u32_array));
	for (uint32_t i = 0; i < v->n; i++) {
		if (v->v[i] != u32_array[i])
			ret |= 1;
		//fprintf(stderr, "v->v[%d] = %u\n", i, v->v[i]);
	}
	uint32_vec_destroy(&v);

	return (ret);
}

static int
test1(void)
{
	int ret = 0;
	uint32_vec *v;

	v = uint32_vec_init(0);
	for (uint32_t i = 0; i < 1024; i++)
		uint32_vec_add(v, i);
	if (uint32_vec_bytes(v) != v->n * sizeof(uint32_t)) {
		fprintf(stderr, "uint32_vec_bytes (%zd) != v->n * sizeof(uint32_t) (%zd)\n",
			uint32_vec_bytes(v), v->n * sizeof(uint32_t));
		ret |= 1;
	}
	for (uint32_t i = 0; i < 1024; i++) {
		//fprintf(stderr, "v->v[%d] = %u\n", i, v->v[i]);
		if (v->v[i] != i)
			ret |= 1;
	}
	uint32_vec_destroy(&v);

	return (ret);
}

static int
check(int ret, const char *s)
{
	if (ret == 0)
		fprintf(stderr, NAME ": PASS: %s\n", s);
	else
		fprintf(stderr, NAME ": FAIL: %s\n", s);
	return (ret);
}

int
main(int argc, char **argv)
{
	int ret = 0;

	ret |= check(test1(), "test1");
	ret |= check(test2(), "test2");
	ret |= check(test3(), "test3");
	ret |= check(test4(), "test4");
	ret |= check(test5(), "test5");

	if (ret)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}