#include <stdio.h>
#include "bmp.h"

int main()
{
	int i, j;
	FILE *fIn, *fOut;	
	bmp_file bmp;
	BYTE *newPixels;
	
	fIn = fopen("./img/lena512.bmp", "rb");
	bmp = *read_bmp_file(fIn, &bmp);	
	fclose(fIn);
	
	
	newPixels = (BYTE *)malloc(bmp.scan_size_bytes);
	for (i = 0; i < bmp.scan_size_bytes; i++)
	{		
		int curPixel = *(bmp.pixels + i);
		*(newPixels + i) = bmp.num_of_colors - 1 - curPixel;
	}	
	bmp.pixels = newPixels;	
	
	fOut = fopen("./img/negative.bmp", "wb+");	
	write_bmp_file(bmp, fOut);
	fclose(fOut);
	
	free_bmp_file(bmp);
	free(newPixels);
	
	return 0;
}