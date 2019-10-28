/**
 * Header for functions and struct for binary images
 */

typedef struct {
	int row;
	int column;
	unsigned char * stream;
}imageBinary;

imageBinary * readFileBinary(char * filename);

imageBinary * createImageBinary(int row, int column);

void saveOnFileBinary(imageBinary * image, char * filename);
