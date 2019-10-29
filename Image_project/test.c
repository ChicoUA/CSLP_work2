/**@file test.c
 * @brief test file with functions that include combinations of the three types of images and where every function is tested
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include "RGB.h"
#include "grayscale.h"
#include "binary.h"


/**@fn imageBinary * createBinaryImage(int row, int column)
 * @brief Function used to create an empty binary image, with its columns and rows set
 * @param filename the name of the file to read
 */
imageBinary * createBinaryImage(int row, int column){
        imageBinary * image = (imageBinary *)malloc(sizeof(imageBinary));
        image->row = row;
        image->column = column;
        image->stream = (unsigned char *)calloc(row*column, sizeof(unsigned char));
        return image;
}

/**@fn imageGrayscale * convertRGBtoGrayscale(imageRGB * image)
 * @brief Function to convert an RGB image to grayscale
 * 
 * It takes each pixel from RGB and makes the mean from its three colors to give the grayscale correspondent
 *
 * @param image RGB image to transform
 */
imageGrayscale * convertRGBtoGrayscale(imageRGB * image){
	imageGrayscale * imageGS = createGrayscaleImage(image->row, image->column, image->rgb_component);
        for(int i = 0; i < image->row * image->column; i++){
                imageGS->stream[i] = (image->stream[i].r + image->stream[i].g + image->stream[i].b)/3;
        }
	return imageGS;
}

/**@fn imageGrayscale * convertRGBComponentToGrayscale(imageRGB * image, char component)
 * @brief Function to give a component of a RGB image (red, green or blue)
 * @param image Image RGB to give the component
 * @param component Color from the RGB image to separate
 */
imageGrayscale * convertRGBComponentToGrayscale(imageRGB * image, char component){
	imageGrayscale * imageGS = createGrayscaleImage(image->row, image->column, image->rgb_component);
	for(int i = 0; i < image->row * image->column; i++){
		if(component == 'r'){
			imageGS->stream[i] = image->stream[i].r;
		}
		else if(component == 'g'){
			imageGS->stream[i] = image->stream[i].g;
		}
		else{
			imageGS->stream[i] = image->stream[i].b;
		}
	}
	return imageGS;
}

/**@fn imageBinary * convertGrayscaletoBinary(imageGrayscale * image, int th)
 * @brief Function to transform a grayscale image to binary image
 *
 * It takes each pixel from the grayscale image and compares it to a threshold, if it is smaller it will save 0 in the binary, else it saves 1
 *
 * @param image Grayscale image to transform
 * @param th Threshold to compare
 */
imageBinary * convertGrayscaletoBinary(imageGrayscale * image, int th){ //ver método de otsu
	if(th > 255 || th < 0){
		printf("Invalid value of threshold, going to use 128\n");
		th = 128;
	}
	imageBinary * imageB = createBinaryImage(image->row, image->column);
	for(int i = 0; i < image->row*image->column; i++){
		if(image->stream[i] >= th){
			imageB->stream[i] = 1;
		}
		else{
			imageB->stream[i] = 0;
		}
	}
	return imageB;
}

/**@fn imageGrayscale * edgeDetectionFilter(imageRGB * image, int sizeOfKernel)
 * @brief Function to use edge detection filter on an RGB image
 * 
 * It create two filters (Gx and Gy) one is horizontal and the other is vertical and fills them with one side negative, the middle with zeros and the other side with positive numbers.
 * The it makes the convolution for each pixel with both filters, and finally it calculates the distance between each position of the filters and saves it in the new image.
 *
 * @param image RGB image to filter
 * @param sizeOfKernel The size of filter that will be used
 */
imageGrayscale * edgeDetectionFilter(imageRGB * image, int sizeOfKernel){
        if(sizeOfKernel % 2 == 0){
                printf("Size of kernel must be odd!");
                exit(1);
        }

        imageGrayscale * imageGS = createGrayscaleImage(image->row, image->column, image->rgb_component);

        int n = (sizeOfKernel - 1) / 2; // odd number = 2n + 1, this n gives the middle number of the size

       int Gx[sizeOfKernel][sizeOfKernel];
       int Gy[sizeOfKernel][sizeOfKernel];

        //fill Gx and Gy with values
        for(int i = 0; i < sizeOfKernel; i++){
                for(int j = 0; j < sizeOfKernel; j++){
                        if(i == n)
                                Gy[i][j] = 0;
                        else if(i < n)
                                Gy[i][j] = -1;
                        else
                                Gy[i][j] = 1;

                        if(j == n)
                                Gx[i][j] = 0;
                        else if(j < n)
                                Gx[i][j] = -1;
                        else
                                Gx[i][j] = 1;
                }
        }

        //make convolution of each pixel
        for(int row = 0; row < image->row; row++){
                for(int column = 0; column < image->column; column++){
                        int tempx = 0;
                        int tempy = 0;
                        for(int krow = 0; krow < sizeOfKernel; krow++){
                                for(int kcolumn = 0; kcolumn < sizeOfKernel; kcolumn++){
                                        int xValue = Gx[krow][kcolumn];
                                        int yValue = Gy[krow][kcolumn];

                                        int newrow = row + krow - n;
                                        int newcolumn = column + kcolumn -n;

                                        RGBPixel pixel = getPixelRGB(image, newrow, newcolumn);
                                        tempx += ((pixel.r + pixel.g + pixel.b)/3) * xValue;
					tempy += ((pixel.r + pixel.g + pixel.b)/3) * yValue;

                        	}
			}
                        *(imageGS->stream + row * image->column + column) = sqrt(pow(tempx, 2) + pow(tempy, 2));
        	}
	}	

	return imageGS;
}

int main(int argc, char *argv[]){
	if(argc != 5){
		printf("Four arguments(./image_program IMAGE1 IMAGE2 THRESHOLD KERNELSIZE)!\nEx:./image_program lena.ppm girl.ppm 128 3\n");
		exit(1);
	}
	long th = strtol(argv[3], NULL, 10);
	long size = strtol(argv[4], NULL, 10);

	imageRGB * image = readFileRGB(argv[1]);
	printf("row: %d, column: %d, rgb: %d\n", image->row, image->column, image->rgb_component);

	imageGrayscale * imageGS = convertRGBtoGrayscale(image);
	saveOnFileGrayscale(imageGS, "gray.pgm");

	imageBinary * imageB = convertGrayscaletoBinary(imageGS, (int)th);
	saveOnFileBinary(imageB, "binary.pbm");

	imageGrayscale * imageR = convertRGBComponentToGrayscale(image, 'r');
	saveOnFileGrayscale(imageR, "red.pgm");

	imageGrayscale * imageG = convertRGBComponentToGrayscale(image, 'g');
        saveOnFileGrayscale(imageG, "green.pgm");

	imageGrayscale * imageBl = convertRGBComponentToGrayscale(image, 'b');
	saveOnFileGrayscale(imageBl, "blue.pgm");

	imageGrayscale * imageEdgeFilter = edgeDetectionFilter(image, (int)size);
	saveOnFileGrayscale(imageEdgeFilter, "edge.pgm");

	imageRGB * imageMeanFilter = meanFilter(image, (int)size);
	saveOnFileRGB(imageMeanFilter, "mean.ppm");

	imageGrayscale * imageMeanFilterGS = meanFilterGS(imageGS, (int)size);
	saveOnFileGrayscale(imageMeanFilterGS, "meanGS.pgm");

	imageRGB * testImage = readFileRGB(argv[2]);

	imageRGB * imageWatermark = watermarkImage(image, testImage, 0.3);
	saveOnFileRGB(imageWatermark, "watermark.ppm");

	imageGrayscale * imageGaussianFilter = gaussianFilter(imageGS, (int)size);
	saveOnFileGrayscale(imageGaussianFilter, "gaussian.pgm");

	changeIntensityRGB(image,80);
	saveOnFileRGB(image, "intensity_rgb.ppm");

	changeIntensityGrayscale(imageGS, 80);
	saveOnFileGrayscale(imageGS, "intensity_grayscale.pgm");


}


