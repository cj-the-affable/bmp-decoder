#include <stdio.h>
#include <math.h>


int main()
{
	FILE *fIn, *fOut;	
	fIn = fopen("./img/lena512.bmp", "rb");
	fOut = fopen("./img/rotated.bmp", "wb+");
	
	
	int i, j,srcRowOffset, colOffset;
	unsigned char bitmapHeader[14];
	fread(bitmapHeader, sizeof(unsigned char), 14, fIn);
	fwrite(bitmapHeader, sizeof(unsigned char), 14, fOut);		
	printf("Header Field: 0x%X%X\n", bitmapHeader[0], bitmapHeader[1]);
	printf("File Size in Bytes: %u\n", *(unsigned int *)&bitmapHeader[2]);
	printf("Starting Address: %u\n", *(unsigned int *)&bitmapHeader[10]);
	
	
	unsigned char dibHeader[40];
	for (i = 0; i < 40; i++)
	{
		dibHeader[i] = fgetc(fIn);
	}
	fwrite(dibHeader, 40, 1, fOut);	
	
	int imageWidth = *(int *)&dibHeader[4];
	int imageHeight = *(int *)&dibHeader[8];
	unsigned int bpp = *(unsigned int *)&dibHeader[14];
	unsigned int rowSizeBytes = ceil(bpp * imageWidth / 32) * 4;
	
	printf("BPP: %u\n", bpp);
	printf("Width in Pixels: %u (%u bytes per row)\n", imageWidth, rowSizeBytes);
	printf("Height in Pixels: %u\n", imageHeight);
	
	
	int totalImgSizeInBytes = rowSizeBytes * abs(imageHeight);
	printf("Size of Pixel Array (bytes): %u\n", totalImgSizeInBytes);
	
	unsigned int numOfColors = *(unsigned int *)&dibHeader[36];	
	
	if (0 == numOfColors )
	{
	  numOfColors = (int)pow(2, bpp);
	}
	
	unsigned int paletteSizeBytes = numOfColors * 4;
	printf("Palette Size: %u (%u bytes)\n", numOfColors, paletteSizeBytes);
	unsigned char *colorTable = (unsigned char *)malloc(paletteSizeBytes);
	fread(colorTable, sizeof(unsigned char), paletteSizeBytes, fIn);
	fwrite(colorTable, sizeof(unsigned char), paletteSizeBytes, fOut);
	
	unsigned char *pixels = (unsigned char *)malloc(totalImgSizeInBytes);	
	unsigned char *newPixels = (unsigned char *)malloc(totalImgSizeInBytes);
	fread(pixels, sizeof(unsigned char), totalImgSizeInBytes, fIn);
	
	for (i = 0; i < imageHeight; i++)
	{		
		for (j = 0; j < imageWidth; j++)
		{	
			int destRowOffset =  (imageHeight - 1 - j) * rowSizeBytes;
			unsigned char *destAddress = newPixels + destRowOffset + (i);
			unsigned char *srcAddress = pixels + (rowSizeBytes * i) + j;
			memcpy(destAddress, srcAddress, );
		}
	}
	
	fwrite(newPixels, sizeof(unsigned char), totalImgSizeInBytes, fOut);
	fclose(fIn);
	fclose(fOut);
	free(pixels);
	free(newPixels);
	free(colorTable);
	return 0;
}