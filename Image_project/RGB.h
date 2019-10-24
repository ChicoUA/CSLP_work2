/**
 * Header file with functions and struct for RGB images
 */
typedef struct{
        unsigned char r, g, b;
}RGBPixel;

typedef struct{
	int row;
	int column;
	int rgb_component;
	RGBPixel * stream;
}imageRGB;


imageRGB * readFileRGB(char * filename);

imageRGB * createImageRGB(int row, int column, int rgb_component);

void saveOnFileRGB(imageRGB * image, char * filename);
