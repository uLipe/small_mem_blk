/**
 * @brief simple memory based on small block approach
 */

#include "small_block_pool.h"
#include <string.h> /* for NULL definition */
#include <stdint.h>
#include <stdio.h>

/* define block status and lead zeros limit */
#define SMALL_BLOCK_FREE 		0x00
#define SMALL_BLOCK_USED 		0x01
#define SMALL_BLOCK_INVALID		0x02
#define SMALL_BLOCK_BMP_LIMIT	31





static uint8_t small_block_count_lead_zeros(uint32_t x) {


	static uint8_t const clz_lkup[] = {
		32, 31, 30, 30, 29, 29, 29, 29,
		28, 28, 28, 28, 28, 28, 28, 28
	};

    uint32_t n;
	
    /*
     * Scan if bit is in top word
     */
    if (x >= (1 << 16)) {
		if (x >= (1 << 24)) {
			if (x >= (1 << 28)) {
				n = 28;
			}
			else {
				n = 24;
			}
		}
		else {
			if (x >= (1 << 20)) {
				n = 20;
			}
			else {
				n = 16;
			}
		}
    }
    else {
        /* now scan if the bit is on rightmost byte */
		if (x >= (1 << 8)) {
			if (x >= (1 << 12)) {
				n = 12;
			}
			else {
				n = 8;
			}
		}
        else {
            if (x >= (1 << 4)) {
                n = 4;
            }
            else {
                n = 0;
            }
        }
    }

	uint8_t ret = (uint8_t)(clz_lkup[x >> n] - n);

#ifdef POOL_DEBUG
	printf("%s: output: %u \n\r", __func__, ret );
#endif
	return (ret);
}



void *small_block_alloc(pool_info_t *mem){
	void *ret = NULL;

	/* check memory pool pointer */
	if(mem != NULL) {
		uint32_t x;
		uint32_t y;

		if(mem->bitmap_h) {
			x = SMALL_BLOCK_BMP_LIMIT - small_block_count_lead_zeros(mem->bitmap_h);
			y = SMALL_BLOCK_BMP_LIMIT - small_block_count_lead_zeros(mem->bitmap_l[x]);			
		}

		uint32_t block_pos = (x << 5) | y;


#ifdef POOL_DEBUG
		printf("%s: h: 0x%X\n\r",__func__,mem->bitmap_h);
		printf("%s: l: 0x%X\n\r",__func__,mem->bitmap_l[x]);
		printf("%s: x: 0x%X\n\r",__func__,x);
		printf("%s: y: 0x%X\n\r",__func__,y);
		printf("%s: block_position: 0x%X\n\r\n\r\n\r",__func__,block_pos);
#endif


		/* at least 1 block is free */
		if(mem->numblocks){

			/* obtains its address */
			int block_address = mem->block_size * block_pos;

			/* mark as used block */
			mem->bitmap_l[x] &= ~(1 << y);
			if(!mem->bitmap_l[x])
				mem->bitmap_h &= ~(1 << x);
				
			mem->numblocks--;	
			ret = &mem->mem_pool[block_address];

		}
	}
	/* return the block address */
	return(ret);
}



void small_block_free(pool_info_t *mem, void *p){

	/* check pointer */
	if((p != NULL) && (mem != NULL)) {
		uint8_t x = 0;
		uint8_t y = 0;

		uint32_t pbase = (uint32_t)mem->mem_pool;
		uint32_t palloc = (uint32_t)p;
		uint32_t block_position = ((palloc - pbase)/mem->block_size);


		/* bit position out of range, does not accept the block */
		if(block_position < 1024) {
			x = (uint8_t)((block_position >> 5 )& 31);
			y = (uint8_t)(block_position & 31);

			mem->bitmap_h |= (1 << x);
			mem->bitmap_l[x] |= 1 << y;
			mem->numblocks++;
		}
	}

	p = NULL;
}


