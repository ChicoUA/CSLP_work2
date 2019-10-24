/**
 * Header for functions and struct for binary images
 */

typedef struct {
	int row;
	int column;
	int b;
	int * stream;
}imageBinary;

imageBinary * readFileBinary(char * filename);

imageBinary * createImageBinary(int row, int column, int b);

void addPixelBinary(imageBinary * image, int value);

void saveOnFileBinary(imageBinary * image, char * filename);
