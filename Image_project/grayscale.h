/**
 * Documentation for the grayscale.h module
 * This module is the Header file for functions for grayscale Images and their structure.
 */

typedef struct {
	int row;
	int column;
	int lum;
	unsigned char * stream;
}imageGrayscale;

imageGrayscale * createGrayscaleImage(int row, int column, int lum);

imageGrayscale * readFileGrayscale(char * filename);

void saveOnFileGrayscale(imageGrayscale * image, char * filename);

void changeIntensityGrayscale(imageGrayscale * image, int amount);

unsigned char getPixelGrayscale(imageGrayscale * image, int row, int column);

imageGrayscale * meanFilterGS(imageGrayscale * image, int sizeOfKernel);

imageGrayscale * gaussianFilter(imageGrayscale * image, int sizeOfKernel);
