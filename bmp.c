#include <stdio.h>
#include <math.h>
#include "bmp.h"

void write_bmp_file(bmp_file bmp, FILE *dest)
{
	fwrite(bmp.headers, sizeof(BYTE), 54, dest);
	fwrite(bmp.palette, sizeof(BYTE), bmp.palette_size_bytes, dest);
	fwrite(bmp.pixels, sizeof(BYTE), bmp.scan_size_bytes, dest);
	fflush(dest);
}

bmp_file * read_bmp_file(FILE *src, bmp_file * bmp)
{
	fread(bmp->headers, sizeof(BYTE), 54, src);

	if (0x42 != bmp->headers[0] || 0x4d != bmp->headers[1])
	{
		printf("Error");
		return NULL;
	}	
	bmp->file_size_bytes = *(unsigned int *)(bmp->headers + 2);
	bmp->px_offset = *(unsigned int *)(bmp->headers + 10);
	bmp->px_width = *(int *)(bmp->headers + 18);
	bmp->px_height = *(int *)(bmp->headers + 22);
	bmp->bpp = *(unsigned int *)(bmp->headers + 28);
	bmp->row_size_bytes = ceil(bmp->bpp * bmp->px_width / 32) * 4;
	bmp->scan_size_bytes = bmp->row_size_bytes * abs(bmp->px_height);
	bmp->num_of_colors = *(unsigned int *)(bmp->headers + 50);
	
	if (0 == bmp->num_of_colors)
	{
		bmp->num_of_colors = (int)pow(2, bmp->bpp);
	}
	bmp->palette_size_bytes = bmp->num_of_colors * 4;	
	
	bmp->palette = (BYTE *)malloc(bmp->palette_size_bytes);
	bmp->pixels = (BYTE *)malloc(bmp->scan_size_bytes);
	fread(bmp->palette, sizeof(BYTE), bmp->palette_size_bytes, src);
	fread(bmp->pixels, sizeof(BYTE), bmp->scan_size_bytes, src);
	
	return bmp;
}

void free_bmp_file(bmp_file f)
{
	free(f.palette);
	free(f.pixels);
}