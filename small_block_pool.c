/**
 * @brief simple memory based on small block approach
 */

#include "small_block_pool.h"
#include <string.h> /* for NULL definition */

/* define block status and lead zeros limit */
#define SMALL_BLOCK_FREE 		0x00
#define SMALL_BLOCK_USED 		0x01
#define SMALL_BLOCK_INVALID		0x02
#define SMALL_BLOCK_BMP_LIMIT	31



static int small_block_count_lead_zeros(uint32_t bmp) {
	int ret = SMALL_BLOCK_BMP_LIMIT + 1;

    const char debruijn_32[32] = {
        0, 31, 9, 30, 3, 8, 13, 29, 2, 5, 7, 21, 12, 24, 28, 19,
        1, 10, 4, 14, 6, 22, 25, 20, 11, 15, 23, 26, 16, 27, 17, 18
    };


	if(bmp != 0) {
		/* extract the zeros of bitmap */
	    bmp |= bmp>>1;
	    bmp |= bmp>>2;
	    bmp |= bmp>>4;
	    bmp |= bmp>>8;
	    bmp |= bmp>>16;
	    bmp++;

	    /* use a hacker debrujin table + magic to extract the leading zeros
	     * quantity
	     */
	    ret = debruijn_32[bmp * 0x076be629 >> 27];
	}

	return(ret);
}



void *small_block_alloc(pool_info_t *mem){
	void *ret = NULL;

	/* check memory pool pointer */
	if(mem != NULL) {
		int delta = SMALL_BLOCK_BMP_LIMIT -
					small_block_count_lead_zeros(mem->bitmap);

		/* at least 1 block is free */
		if(delta >= 0){

			/* obtains its address */
			int block_address = mem->block_size * delta;

			/* mark as used block */
			mem->bitmap &= ~(1 << delta);
			ret = &mem->mem_pool[block_address];
		}
	}
	/* return the block address */
	return(ret);
}

void small_block_free(pool_info_t *mem, void *p){

	/* check pointer */
	if((p != NULL) && (mem != NULL)) {

		uint32_t pbase = (uint32_t)mem->mem_pool;
		uint32_t palloc = (uint32_t)p;
		int bit_position = ((palloc - pbase)/mem->block_size);

		/* bit position out of range, does not accept the block */
		if(bit_position >= 0 && bit_position <= 31) {
			/* mark the newly returned block as free */
			mem->bitmap |= (1 << bit_position);
		}

	}

	p = NULL;
}


