#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "RGB.h"
#include "grayscale.h"
/**
 * Documentation for the RBG.c module
 * This module is responsible for all RGB operations.
 */

/**
 * Documentation for the readFileRGB function of the RGB module.
 * This fuction fill do the following:
 * - Check if the files exists, if not it will exit, outputting the appropriate warning.
 * - Read the file format, if he is unable to it will exit.
 * - Check the format, if it's not valid it will exit, outputting the appropriate warning.
 * - Check the file for comments.
 * - If all previous steps were validated it will create the image. 
 */
imageRGB * readFileRGB(char * filename){
	char buff[16];
	FILE * file = fopen(filename, "rb");
	int c, row, column, rgb_component;

	if(!file){
		fprintf( stderr, "Could not open file %s \n", filename);
		exit(1);
	}

         if (!fgets(buff, sizeof(buff), file)) {
              perror(filename);
              exit(1);
         }

    	if (buff[0] != 'P' || buff[1] != '6') {
         	fprintf(stderr, "Invalid image format (must be 'P6')\n");
         	exit(1);
    	}

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

	if(fread(image->stream,3 * image->row, image->column, file) != image->column){
		fprintf(stderr, "Wrong format for RGB image");
		exit(1);
	}

	return image;
}

/**
 * Documentation for the createImageRGB function of the grayscale module.
 */
imageRGB * createImageRGB(int row, int column, int rgb_component){
	imageRGB * image;
	image = (imageRGB *)malloc(sizeof(imageRGB));
	image->row = row;
	image->column = column;
	image->rgb_component=rgb_component;
	image->stream = (RGBPixel *)calloc(3*row*column, sizeof(RGBPixel));

	return image;
}
/**
 * Documentation for the saveOnFileRGB function of the RGB module.
 * This fuction fill do the following:
 * - Check if the files exists.
 * - if it does exist but it cannot access it it will exit, outputting the appropriate warning.
 * - If all previous steps were validated it will save the image.
 */
void saveOnFileRGB(imageRGB * image, char * filename){
	FILE *f;
        f = fopen(filename, "wb");

        if(!f){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

	fputs("P6\n", f);
	fprintf(f, "%d %d\n%d\n", image->row, image->column, image->rgb_component);
	fwrite(image->stream, 3 * image->row, image->column, f);
	fclose(f);
}
/**
 * Documentation for the converRGBtoGrayscale function of the RBG module.
 */
imageGrayscale * convertRGBtoGrayscale(imageRGB * image){
	imageGrayscale * imageGS  = createGrayscaleImage(image->row, image->column, image->rgb_component);
       	printf("row: %d, column: %d, rgb: %d\n", imageGS->row, imageGS->column, imageGS->lum);	
	for(int i = 0; i < image->row * image->column; i++){
		imageGS->stream[i] = (image->stream[i].r + image->stream[i].g + image->stream[i].b)/3;

/**
 * Documentation for the changeIntensityRGB function of the RBG module.
 */
void changeIntensityRGB(imageRGB * image, int amount){
	for(int i = 0; i < image->row*image->column; i++){
		int newvaluer = (int)image->stream[i].r + amount;
		int newvalueg = (int)image->stream[i].g + amount;
		int newvalueb = (int)image->stream[i].b + amount;
		
		if(newvaluer > 255)
			newvaluer = 255;
		else if (newvaluer < 0)
			newvaluer = 0;

		if(newvalueg > 255)
                        newvalueg = 255;
                else if (newvalueg < 0)
                        newvalueg = 0;
		
		if(newvalueb > 255)
                        newvalueb = 255;
                else if (newvalueb < 0)
                        newvalueb = 0;

		image->stream[i].r = (unsigned char)newvaluer;
		image->stream[i].g = (unsigned char)newvalueg;
		image->stream[i].b = (unsigned char)newvalueb;
	}
}

/**
 * Documentation for the getPixelRGB function of the RBG module.
 */
RGBPixel getPixelRGB(imageRGB * image, int row, int column){

	if(row < 0 || row > image->row || column < 0 || column > image->column){
		RGBPixel emptyRGBPixel;
		emptyRGBPixel.r = 0;
		emptyRGBPixel.g = 0;
		emptyRGBPixel.b = 0;
		return emptyRGBPixel;
	}

	return *(image->stream + row*image->column + column);
}

/**
 * Documentation for the meanFiler function of the RBG module.
 */
imageRGB * meanFilter(imageRGB * image, int sizeOfKernel){
	if(sizeOfKernel % 2 == 0){
                printf("Size of kernel must be odd!");
                exit(1);
        }

       	int n = (sizeOfKernel - 1) / 2;
       	float Gx[sizeOfKernel][sizeOfKernel];

	for(int i = 0; i < sizeOfKernel; i++){
                for(int j = 0; j < sizeOfKernel; j++){
                        Gx[i][j] = 1/((float)sizeOfKernel * (float)sizeOfKernel);
                }
        }
	
	imageRGB * imageMeanFilter = createImageRGB(image->row, image->column, image->rgb_component);

        for(int row = 0; row < image->row; row++){
                for(int column = 0; column < image->column; column++){
			float tempr = 0;
			float tempg = 0;
			float tempb = 0;
			RGBPixel tempPixel;
                        for(int krow = 0; krow < sizeOfKernel; krow++){
                                for(int kcolumn = 0; kcolumn < sizeOfKernel; kcolumn++){
                                        float xValue = Gx[krow][kcolumn];

                                        int newrow = row + krow - n;
                                        int newcolumn = column + kcolumn -n;

                                        RGBPixel pixel = getPixelRGB(image, newrow, newcolumn);
                                        tempr += (float)pixel.r * xValue;
					tempg += (float)pixel.g * xValue;
					tempb += (float)pixel.b * xValue;

                                }
                        }
			tempPixel.r = (int)tempr;
			tempPixel.g = (int)tempg;
			tempPixel.b = (int)tempb;
                        *(imageMeanFilter->stream + row * image->column + column) = tempPixel;
                }
        }

	return imageMeanFilter;
}
/**
 * Documentation for the watermarkImage function of the RBG module.
 */
imageRGB * watermarkImage(imageRGB * image1, imageRGB * image2, float alpha){
	if(image1->row != image2->row || image1->column != image2->column){
		printf("Images must have the same resolution\n");
		exit(1);
	}

	float beta = 1 - alpha;	
	imageRGB * imageWatermark = createImageRGB(image1->row, image1->column, image1->rgb_component);

	for(int row = 0; row < image1->row; row++){
		for(int column = 0; column < image1->column; column++){
			RGBPixel pixel1 = getPixelRGB(image1, row, column);
			RGBPixel pixel2 = getPixelRGB(image2, row, column);
			RGBPixel pixelWatermark;

			pixelWatermark.r = (int)((float)pixel1.r*beta + (float)pixel2.r*alpha);
			pixelWatermark.g = (int)((float)pixel1.g*beta + (float)pixel2.g*alpha);
			pixelWatermark.b = (int)((float)pixel1.b*beta + (float)pixel2.b*alpha);
			
			*(imageWatermark->stream + row * imageWatermark->column + column) = pixelWatermark;
		}
	}

	return imageWatermark;
}
