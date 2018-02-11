#include <stdio.h>
#pragma pack(1)


/* BMP image header */
struct bmp_header {
	char format_id[2];
	int file_size;
	short res_value;
	short res_value2;
	int offset;
} image_header;

/* DIB header */
struct dib_header {
	int dib_size;
	int img_width;
	int img_height;
	short color_planes;
	short bits_per_pix;
	int comp_scheme;
	int image_size;
	int hor_res;
	int ver_res;
	int palette_colors;
	int important_colors;
} image_dib;

int main(int argc, char *argv[]) {

	if(argc == 1) {
		printf("Usage: ./bmp_edit [-option] [filename]\n");
		return 1;
	}

	FILE *input_image = fopen(argv[2], "rb+");

	if(input_image == NULL) {
		printf("File not found.\n");
		return 1;
	}
	
	fread(&image_header, 14, 1, input_image);
	fread(&image_dib, 40, 1, input_image);

	printf("%s\n", image_header.format_id);


	return 0;
}

