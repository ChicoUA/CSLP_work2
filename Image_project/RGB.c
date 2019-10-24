#include <stdio.h>
#include <stdlib.h>
#include "RGB.h"
#include "grayscale.h"

imageRGB * readFileRGB(char * filename){
	char buff[16];
	FILE * file = fopen(filename, "rb");
	int c, row, column, rgb_component;

	//check if file exists
	if(!file){
		fprintf( stderr, "Could not open file %s \n", filename);
		exit(1);
	}

	//read image format
         if (!fgets(buff, sizeof(buff), file)) {
              perror(filename);
              exit(1);
         }

	//check the image format
    	if (buff[0] != 'P' || buff[1] != '6') {
         	fprintf(stderr, "Invalid image format (must be 'P6')\n");
         	exit(1);
    	}

	//check for comments
	c = fgetc(file);

	while(c == '#'){
		while(getc(file) != '\n');
		c = fgetc(file);
	}

	ungetc(c, file);

	if(fscanf(file, "%d %d", &row, &column) != 2){
		fprintf(stderr, "1-Wrong file format!");
		exit(1);
	}
	
	if(fscanf(file, "%d", &rgb_component) != 1){
		fprintf(stderr, "2-Wrong file format!");
		exit(1);
	}

	imageRGB * image = createImageRGB(row, column, rgb_component);

	if(fread(image->stream, 3 * image->row, image->column, file) != image->column){
		fprintf(stderr, "Wrong format for RGB image");
		exit(1);
	}

	return image;
}

imageRGB * createImageRGB(int row, int column, int rgb_component){
	imageRGB * image;
	image = (imageRGB *)malloc(sizeof(imageRGB));
	image->row = row;
	image->column = column;
	image->rgb_component=rgb_component;
	image->stream = (RGBPixel *)calloc(3*row*column, sizeof(RGBPixel));

	return image;
}

void saveOnFileRGB(imageRGB * image, char * filename){
	FILE *f;
        f = fopen(filename, "wb");

        //check if file exists
        if(!f){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

	fputs("P6\n", f);
	fprintf(f, "%d %d\n%d", image->row, image->column, image->rgb_component);
	fwrite(image->stream, 3 * image->row, image->column, f);
	fclose(f);
}

imageGrayscale * convertRGBtoGrayscale(imageRGB * image){
	imageGrayscale * imageGS  = createGrayscaleImage(image->row, image->column, image->rgb_component);
       	printf("row: %d, column: %d, rgb: %d\n", imageGS->row, imageGS->column, imageGS->lum);	
	for(int i = 0; i < image->row * image->column; i++){
		imageGS->stream[i] = (image->stream[i].r + image->stream[i].g + image->stream[i].b)/3;
        }
	return imageGS;
}
