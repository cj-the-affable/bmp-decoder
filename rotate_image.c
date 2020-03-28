#include <stdio.h>
#include <math.h>

typedef unsigned char BYTE;

typedef struct {
	BYTE headers[54];
	unsigned int file_size_bytes;
	unsigned int px_offset;
	int px_width;
	int px_height;
	unsigned int bpp;
	int row_size_bytes;
	int scan_size_bytes;
	int num_of_colors;
	int palette_size_bytes;
	BYTE *palette;
	BYTE *pixels;
} bmp_file;


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

int main()
{
	int i, j;
	FILE *fIn, *fOut;	
	bmp_file bmp;
	BYTE *newPixels;
	
	fIn = fopen("./img/lena512.bmp", "rb");
	fOut = fopen("./img/rotated.bmp", "wb+");	
	bmp = *read_bmp_file(fIn, &bmp);	
	
	
	newPixels = (BYTE *)malloc(bmp.scan_size_bytes);
	for (i = 0; i < bmp.px_height; i++)
	{		
		for (j = 0; j < bmp.px_width; j++)
		{	
			int destRowOffset =  (bmp.px_height - 1 - j) * bmp.row_size_bytes;
			unsigned char *destAddress = newPixels + destRowOffset + i;
			unsigned char *srcAddress = bmp.pixels + (bmp.row_size_bytes * i) + j;
			memcpy(destAddress, srcAddress, sizeof(BYTE));
		}
	}
	
	bmp.pixels = newPixels;
	write_bmp_file(bmp, fOut);
	free_bmp_file(bmp);
	free(newPixels);
	fclose(fIn);
	fclose(fOut);
	return 0;
}