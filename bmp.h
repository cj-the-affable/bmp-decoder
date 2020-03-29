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

bmp_file * read_bmp_file(FILE *src, bmp_file * bmp);
void write_bmp_file(bmp_file bmp, FILE *dest);
void free_bmp_file(bmp_file f);