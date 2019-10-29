/**@file grayscale.h
 * @brief Header file with functions and struct for grayscale images
 *
 */

/**
 * @brief struct to represent a grayscale image. It has its number of rows and columns (image resolution), the maximum number for each pixel(lum) and the stream with its pixels
 */
typedef struct {
	int row;
	int column;
	int lum;
	unsigned char * stream;
}imageGrayscale;

/**@fn imageGrayscale * createImageGrayscale(int row, int column, int lum)
 * @brief Function to create an Grayscale image and returns an empty image
 * @param row number of rows of the image
 * @param column number of columns of the image
 * @param lum maximum number for each pixel
 */
imageGrayscale * createGrayscaleImage(int row, int column, int lum);

/**@fn imageGrayscale * readFileGrayscale(char* filename)
 * @brief Function used to read a pgm file with a grayscale image and return the image
 * @param filename the name of the file to read
 */
imageGrayscale * readFileGrayscale(char * filename);

/**@fn void saveOnFileGrayscale(imageGrayscale * image, char * filename)
 * @brief Function to save Grayscale image in a file
 * @param image The Grayscale image to save
 * @param filename The name of file to save the image in
 */
void saveOnFileGrayscale(imageGrayscale * image, char * filename);

/**@fn void changeIntensityGrayscale(imageGrayscale * image, int amount)
 * @brief Function to change the intensity of an Grayscale image
 * @param image Image to change intensity
 * @param amount The amount to change
 */
void changeIntensityGrayscale(imageGrayscale * image, int amount);

/**@fn unsigned char getPixelGrayscale(imageGrayscale * image, int row, int column)
 * @brief Function to get a pixel from a Grayscale image
 * @param image Image to get the pixel
 * @param row Row where the pixel is
 * @param column Column where the pixel is
 */
unsigned char getPixelGrayscale(imageGrayscale * image, int row, int column);

/**@fn ImageGrayscale * meanFilterGS(ImageGrayscale * image, int sizeOfKernel)
 * @brief Function to do the mean filtering of a Grayscale image
 * @param image Image to filter
 * @param sizeOfKernel Size of the Kernel used in the filter
 */
imageGrayscale * meanFilterGS(imageGrayscale * image, int sizeOfKernel);

/**@fn ImageGrayscale * gaussianFilter(ImageGrayscale * image, int sizeOfKernel)
 * @brief Function to do the gaussian filtering of a Grayscale image
 * @param image Image to filter
 * @param sizeOfKernel Size of the Kernel used in the filter
 */
imageGrayscale * gaussianFilter(imageGrayscale * image, int sizeOfKernel);
