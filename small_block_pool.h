/**
 * @brief simple memory based on small block approach
 */

#ifndef SMALL_BLOCK_POOL_H_
#define SMALL_BLOCK_POOL_H_

/* stdint is a must, we need to know the bit size of a int word */
#include <stdint.h>

/* defines the structure which controls the memory pool */
typedef struct {
	uint32_t bitmap_h;
	uint32_t bitmap_l[32];
	uint32_t block_size;
	uint32_t numblocks;
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
#define SMALL_BLOCK_POOL_DECLARE(name, b_size, __numblocks)				\
	unsigned char pool_##name[b_size * __numblocks ] = {0};				\
	pool_info_t name = {												\
		.bitmap_h = 0xFFFFFFFF,											\
		.bitmap_l[0] = 0xFFFFFFFF,										\
		.bitmap_l[1] = 0xFFFFFFFF,										\
		.bitmap_l[2] = 0xFFFFFFFF,										\
		.bitmap_l[3] = 0xFFFFFFFF,										\
		.bitmap_l[4] = 0xFFFFFFFF,										\
		.bitmap_l[5] = 0xFFFFFFFF,										\
		.bitmap_l[6] = 0xFFFFFFFF,										\
		.bitmap_l[7] = 0xFFFFFFFF,										\
		.bitmap_l[8] = 0xFFFFFFFF,										\
		.bitmap_l[9] = 0xFFFFFFFF,										\
		.bitmap_l[10] = 0xFFFFFFFF,										\
		.bitmap_l[11] = 0xFFFFFFFF,										\
		.bitmap_l[12] = 0xFFFFFFFF,										\
		.bitmap_l[13] = 0xFFFFFFFF,										\
		.bitmap_l[14] = 0xFFFFFFFF,										\
		.bitmap_l[15] = 0xFFFFFFFF,										\
		.bitmap_l[16] = 0xFFFFFFFF,										\
		.bitmap_l[17] = 0xFFFFFFFF,										\
		.bitmap_l[18] = 0xFFFFFFFF,										\
		.bitmap_l[19] = 0xFFFFFFFF,										\
		.bitmap_l[20] = 0xFFFFFFFF,										\
		.bitmap_l[21] = 0xFFFFFFFF,										\
		.bitmap_l[22] = 0xFFFFFFFF,										\
		.bitmap_l[23] = 0xFFFFFFFF,										\
		.bitmap_l[24] = 0xFFFFFFFF,										\
		.bitmap_l[25] = 0xFFFFFFFF,										\
		.bitmap_l[26] = 0xFFFFFFFF,										\
		.bitmap_l[27] = 0xFFFFFFFF,										\
		.bitmap_l[28] = 0xFFFFFFFF,										\
		.bitmap_l[29] = 0xFFFFFFFF,										\
		.bitmap_l[30] = 0xFFFFFFFF,										\
		.bitmap_l[31] = 0xFFFFFFFF,										\		
		.block_size = b_size,											\
		.numblocks = __numblocks,										\
		.mem_pool = &pool_##name										\
	}

#endif /* SMALL_BLOCK_POOL_H_ */
