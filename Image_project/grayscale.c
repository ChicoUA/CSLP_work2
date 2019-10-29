/**@file RGB.c
 * @brief This module is responsible for all RGB operations.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "grayscale.h"
#include <math.h>

#define M_PI 3.14159265358979323846
/**
 * Documentation for the createGrayscaleImage function of the grayscale module.
 */
imageGrayscale * createGrayscaleImage(int row, int column, int lum){
	imageGrayscale * image = (imageGrayscale *)malloc(sizeof(imageGrayscale));
	image->row = row;
	image->column = column;
	image->lum = lum;
	image->stream = (unsigned char *)calloc(row*column, sizeof(unsigned char));
	return image;
}
/**
 * Documentation for the readFileGrayscale function of the grayscale module.
 * This fuction fill do the following:
 * - Check if the files exists, if not it will exit, outputting the appropriate warning.
 * - Read the file format, if he is unable to it will exit.
 * - Check the format, if it's not valid it will exit, outputting the appropriate warning.
 * - Check the file for cooments.
 * - If all previous steps were validated it will create the image.
 */
imageGrayscale * readFileGrayscale(char * filename){
	char buff[16];
        FILE * file = fopen(filename, "rb");
        int c, row, column, lum;

        if(!file){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

         if (!fgets(buff, sizeof(buff), file)) {
              perror(filename);
              exit(1);
         }

        if (buff[0] != 'P' || buff[1] != '4') {
                fprintf(stderr, "Invalid image format (must be 'P4')\n");
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

        if(fscanf(file, "%d", &lum) != 1){
                fprintf(stderr, "2-Wrong file format!");
                exit(1);
        }
	
	imageGrayscale * image = createGrayscaleImage(row, column, lum);

        if(fread(image->stream, image->row, image->column, file) != image->column){
                fprintf(stderr, "Wrong format for Grayscale image");
                exit(1);
        }

        return image;
}
/**
 * Documentation for the createGrayscaleImage function of the grayscale module.
 */
imageGrayscale * createGrayscaleImage(int row, int column, int lum){
	imageGrayscale * image = (imageGrayscale *)malloc(sizeof(imageGrayscale));
	image->row = row;
	image->column = column;
	image->lum = lum;
	image->stream = (unsigned char *)calloc(row*column, sizeof(unsigned char));
}

/**
 * Documentation for the saveOnFileGrayscale function of the grayscale module.
 * This fuction fill do the following:
 * - Check if the files exists.
 * - if it does exist but it cannot access it it will exit, outputting the appropriate warning.
 * - If all previous steps were validated it will save the image.
 */
void saveOnFileGrayscale(imageGrayscale * image, char * filename){
	FILE * f = fopen(filename, "wb");
        if(!f){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

	fputs("P5\n", f);
        fprintf(f, "%d %d\n%d\n", image->row, image->column, image->lum);
        fwrite(image->stream, image->row, image->column, f);
        fclose(f);
}
/**
 * This Function adds the values amount to each pixel, if that new value is larger than 255 then it is changed to 255, if it is smaller than 0, then it is changed to 0. Finally it updates 
 * the value of the pixel.
 */	
void changeIntensityGrayscale(imageGrayscale * image, int amount){
	for(int i = 0; i < image->row * image->column; i++){
		int newvalue  = (int)image->stream[i] + amount;
		if(newvalue > 255)
			newvalue = 255;
		else if(newvalue < 0)
			newvalue = 0;
		image->stream[i]  = (unsigned char)newvalue;
	}
}

unsigned char getPixelGrayscale(imageGrayscale * image, int row, int column){
	if(row < 0 || row > image->row || column < 0 || column > image->column)
		return 0;

	return *(image->stream + row*image->column + column);
}
/**
 * This function creates a new filter with the size given, an fills it with 1/size², then it goes through each pixel of the image and makes convolution with the filter to obtain a new value for the
 * pixel and updates it and saves it in a new Grayscale image.
 */
imageGrayscale * meanFilterGS(imageGrayscale * image, int sizeOfKernel){
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

	imageGrayscale * imageMeanFilter = createGrayscaleImage(image->row, image->column, image->lum);

        for(int row = 0; row < image->row; row++){
                for(int column = 0; column < image->column; column++){
                        float temp = 0;
                        for(int krow = 0; krow < sizeOfKernel; krow++){
                                for(int kcolumn = 0; kcolumn < sizeOfKernel; kcolumn++){
                                        float xValue = Gx[krow][kcolumn];

                                        int newrow = row + krow - n;
                                        int newcolumn = column + kcolumn -n;

                                        unsigned char pixel = getPixelGrayscale(image, newrow, newcolumn);
                                        temp += (float)pixel * xValue;

                                }
                        }
                        *(imageMeanFilter->stream + row * image->column + column) = (int)temp;
                }
        }

        return imageMeanFilter;

}
/**
 * Documentation for the gaussianFilter function of the grayscale module.
 * This fuction fill do the following:
 * - Sets standard deviation to 1.0
 * - It will fill the Kernel
 * - Normalizes the Kernel
 * - Makes convolution of each pixel
 */
imageGrayscale * gaussianFilter(imageGrayscale * image, int sizeOfKernel){
	if(sizeOfKernel % 2 == 0){
                printf("Size of kernel must be odd!");
                exit(1);
        }

    	double sigma = 1.0;
    	double r, s = 2.0 * sigma * sigma;

    	double sum = 0.0;

        int n = (sizeOfKernel - 1) / 2;

        float Gx[sizeOfKernel][sizeOfKernel];

        for(int i = -n; i <= n; i++){
                for(int j = -n; j <= n; j++){
			r = sqrt(i*i + j*j);
                        Gx[i+n][j+n] = (exp(-(r*r)/s))/(M_PI * s);
			sum += Gx[i + n][j + n];
                }
        }

    	for(int i = 0; i < sizeOfKernel; ++i){
        	for(int j = 0; j < sizeOfKernel; ++j){
            		Gx[i][j] /= sum;
		}
	}
	
	imageGrayscale * imageGaussianFilter = createGrayscaleImage(image->row, image->column, image->lum);

        for(int row = 0; row < image->row; row++){
                for(int column = 0; column < image->column; column++){
                        float temp = 0;
                        for(int krow = 0; krow < sizeOfKernel; krow++){
                                for(int kcolumn = 0; kcolumn < sizeOfKernel; kcolumn++){
                                        float xValue = Gx[krow][kcolumn];

                                        int newrow = row + krow - n;
                                        int newcolumn = column + kcolumn -n;

                                        unsigned char pixel = getPixelGrayscale(image, newrow, newcolumn);
                                        temp += (float)pixel * xValue;

                                }
                        }
                        *(imageGaussianFilter->stream + row * image->column + column) = (int)temp;
                }
        }

        return imageGaussianFilter;

}
