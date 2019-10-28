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

void changeIntensityRGB(imageRGB * image, int amount);

RGBPixel getPixelRGB(imageRGB * image, int row, int column);

imageRGB * meanFilter(imageRGB * image, int sizeOfKernel);

imageRGB * watermarkImage(imageRGB * image1, imageRGB * image2, float alpha);
