#include <stdio.h>
#include "bmp.h"


int main()
{
	int i, j;
	FILE *fIn;	
	bmp_file bmp;
	BYTE *newPixels;
	unsigned char *hist;
	
	
	fIn = fopen("./img/lena512.bmp", "rb");
	bmp = *read_bmp_file(fIn, &bmp);	
	fclose(fIn);
	
	
	hist = (unsigned char *)calloc(bmp.num_of_colors, sizeof(unsigned char));
	
	for( i = 0; i < bmp.scan_size_bytes; i++)
	{
		unsigned char val = *(bmp.pixels + i);
		(*(hist + val))++;
	}
	
	for (j = 0; j < bmp.num_of_colors; j++)
	{
		printf("Key: %d; Value: %u\n", j, *(hist + j));
	}
	
	free_bmp_file(bmp);
	
	return 0;
}