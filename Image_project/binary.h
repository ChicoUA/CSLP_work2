/**
 * * Documentation for the binary.h module
 * This module is the Header file for binary images and their structure.
 */

typedef struct {
	int row;
	int column;
	unsigned char * stream;
}imageBinary;

imageBinary * readFileBinary(char * filename);

imageBinary * createImageBinary(int row, int column);

void saveOnFileBinary(imageBinary * image, char * filename);