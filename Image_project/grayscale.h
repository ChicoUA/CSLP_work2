/**
 * Documentation for the grayscale.h module
 * This module is the Header file for functions for grayscale Images and their structure.
 */
typedef struct{
	int row;
	int column;
	int lum;
	unsigned char * stream;
}imageGrayscale;

imageGrayscale * createGrayscaleImage(int row, int column, int lum);

imageGrayscale * readFileGrayscale(char * filename);

void saveOnFileGrayscale(imageGrayscale * image, char * filename);
