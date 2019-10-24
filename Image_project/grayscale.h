/**
 *Header file for functions and struct of grayscale images
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
