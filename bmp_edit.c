#include <stdio.h>
#include <string.h>
#include <math.h>
#pragma pack(1)


/* BMP image header struct */
struct bmp_header {
	char format_id[2];
	int file_size;
	short res_value;
	short res_value2;
	int offset;
} image_header;

/* DIB header struct */
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
	/* check if user set program arguments */
	if(argc == 1) {
		printf("Usage: ./bmp_edit [-option] [filename]\n");
		return 1;
	}

	FILE *input_image = fopen(argv[2], "rb+");

	if(input_image == NULL) {
		printf("File not found.\n");
		return 1;
	}
	/* read image header and DIB header from bmp file */
	fread(&image_header, 1, 14, input_image);
	fread(&image_dib, 1, 40, input_image);
	
	/* check if image format identifier is "BM" */
	if(image_header.format_id[0] != 'B' || image_header.format_id[1] != 'M') {
		printf("This program does not support the following format: %c%c\n", image_header.format_id[0], image_header.format_id[1]);
		return 1;
	}
	
	/* checks if the DIB header is 40 bytes */
	if(image_dib.dib_size != 40) {
		printf("The DIB header size is not 40. This file is not supported.\n");
		return 1;
	}

	/* check if the pixel data is encoded in 24-bit color */
	if(image_dib.bits_per_pix != 24) {
		printf("This image does not have 24-bit color. This file is not supported.\n");
		return 1;
	}
	
	/* set offset */
	fseek(input_image, image_header.offset, SEEK_SET);
	
	/* make sure rows start at a multiple of 4 */
	int row_padding = 4 - (image_dib.img_width * 3 % 4);
	if(row_padding == 4)
		row_padding = 0;


	for(int h = 0; h < image_dib.img_height; h++) {
		for(int w = 0; w < image_dib.img_width; w++) {
			struct pixel pix;
			/* read pixel */
			fread(&pix, 1, 3, input_image);

			if(strcmp(argv[1], "-invert") == 0) {
				pix.blue = ~pix.blue;
				pix.green = ~pix.green;
				pix.red = ~pix.red;
			}
			else if(strcmp(argv[1], "-grayscale") == 0) {
				float norm_blue = (float) pix.blue / 255.0;
				float norm_green = (float) pix.green / 255.0;
				float norm_red = (float) pix.red / 255.0;
				float y = .2126 * norm_red + .7152 * norm_green + .0722 * norm_blue;

				if(y <= .003138){
					pix.blue = (unsigned char) 255.0* 12.92*y;
					pix.green = (unsigned char) 255.0* 12.92*y;
					pix.red = (unsigned char) 255.0* 12.92*y;
				}
				else {		
					pix.blue = (unsigned char) 255.0* 1.055 * pow(y, 1.0/2.4) - .055;
					pix.green = (unsigned char) 255.0* 1.055 * pow(y, 1.0/2.4) - .055;
					pix.red = (unsigned char) 255.0* 1.055 * pow(y, 1.0/2.4) - .055;
				}
			}
			else {
				printf("Option not supported. Use '-invert' or '-grayscale'\n");
				return 1;
			}
			/* write pixel */
			fseek(input_image, -3, SEEK_CUR);
			fwrite(&pix, 1, 3, input_image);

		}
		
		fseek(input_image, row_padding, SEEK_CUR);
	}


	return 0;
}

