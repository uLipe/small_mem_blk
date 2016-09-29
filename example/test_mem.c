#include <stdio.h>
#include <string.h>
#include "../small_block_pool.h"

/* creates several different pools  */
SMALL_BLOCK_POOL_DECLARE(test1, 32);


int main (int argc, char **argv)
{
	/* allocate  32 bytes string */
	char *str  = (char *)small_block_alloc(&test1);
	char *str2 = (char *)small_block_alloc(&test1);
	char *str3 = (char *)small_block_alloc(&test1);

	/* fills strings */
	strcpy(str, "allocated_memory 1 \0");
	strcpy(str2, "allocated_memory 2 \0");
	strcpy(str3, "allocated_memory 3 \0");

	/* prints the contents */
	printf("%s \n\r", str);
	printf("%s \n\r", str2);
	printf("%s \n\r", str3);


	small_block_free(&test1,str);

	int *ptr = small_block_alloc(&test1);
	*ptr = 123456789;

	printf("%d \n\r", *ptr);
	printf("%s \n\r", str2);
	printf("%s \n\r", str3);



	small_block_free(&test1,ptr);
	small_block_free(&test1,str2);
	small_block_free(&test1,str3);



	return 0;
}
