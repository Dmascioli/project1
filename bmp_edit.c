#include <stdio.h>
#include <string.h>
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

/* pixel struct */
struct pixel {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

int main(int argc, char *argv[]) {

	int w;
	int h;

	if(argc == 1) {
		printf("Usage: ./bmp_edit [-option] [filename]\n");
		return 1;
	}

	FILE *input_image = fopen(argv[2], "rb+");

	if(input_image == NULL) {
		printf("File not found.\n");
		return 1;
	}
	
	fread(&image_header, 1, 14, input_image);
	fread(&image_dib, 1, 40, input_image);

	if(image_header.format_id[0] != 'B' || image_header.format_id[1] != 'M') {
		printf("This program does not support the following format: %c%c\n", image_header.format_id[0], image_header.format_id[1]);
		return 1;
	}

	if(image_dib.dib_size != 40) {
		printf("The DIB header size is not 40. This file is not supported.\n");
		return 1;
	}

	if(image_dib.bits_per_pix != 24) {
		printf("This image does not have 24-bit color. This file is not supported.\n");
		return 1;
	}

	fseek(input_image, image_header.offset, SEEK_SET);
	
	int row_padding = image_dib.img_width * 3 % 4;


	for(h = 0; h < image_dib.img_height; h++) {
		for(w = 0; w < image_dib.img_width; w++) {
			struct pixel pix;
			fread(&pix, 1, 3, input_image);

			if(strcmp(argv[1], "-invert") == 0) {
				pix.blue = ~pix.blue;
				pix.green = ~pix.green;
				pix.red = ~pix.red;
			}

			fseek(input_image, -3, SEEK_CUR);
			fwrite(&pix, 1, 3, input_image);

		}
	}



	return 0;
}

