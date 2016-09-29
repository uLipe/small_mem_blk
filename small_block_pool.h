/**
 * @brief simple memory based on small block approach
 */

#ifndef SMALL_BLOCK_POOL_H_
#define SMALL_BLOCK_POOL_H_

/* stdint is a must, we need to know the bit size of a int word */
#include <stdint.h>

/* defines the structure which controls the memory pool */
typedef struct {
	uint32_t bitmap;
	int block_size;
	void *mem_pool;
}pool_info_t;


/**
 * @brief allocate memory of fixed size
 */
void *small_block_alloc(pool_info_t *mem);

/**
 * @brief free a previous allocated block
 */
void small_block_free(pool_info_t *mem, void *p);


/* declares a fully initialized memory pool structure */
#define SMALL_BLOCK_POOL_DECLARE(name, b_size)							\
	unsigned char pool_##name[(b_size + sizeof(uint32_t)) * 32 ] = {0};	\
	pool_info_t name = {												\
		.bitmap = 0xFFFFFFFF,											\
		.block_size = b_size,											\
		.mem_pool = &pool_##name										\
	}

#endif /* SMALL_BLOCK_POOL_H_ */
