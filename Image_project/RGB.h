/**@file RGB.h
 * @brief Header file with functions and struct for RGB images
 *
 */

/**
<<<<<<< HEAD
 * Documentation for the RBG.h module
 * This module is the Header file for functions for RGB Images and their structure.
=======
 * @brief struct to represent each pixel from a RGB image, with its components: red, green and blue
>>>>>>> 6170a352ac8a77cefb184c4c331dd96190ac8a65
 */
typedef struct{
        unsigned char r, g, b;
}RGBPixel;

/**
 * @brief struct to represent a RGB image. It has its number of rows and columns (image resolution), the maximum number for each component(rgb_component) and the stream with its pixels
 */
typedef struct{
	int row;
	int column;
	int rgb_component;
	RGBPixel * stream;
}imageRGB;

/**@fn imageRGB * readFileRGB(char* filename)
 * @brief Function used to read a ppm file with a RGB image and return the image
 * @param filename the name of the file to read
 */
imageRGB * readFileRGB(char * filename);

/**@fn imageRGB * createImageRGB(int row, int column, int rgb_component)
 * @brief Function to create an RGB image and returns an empty image
 * @param row number of rows of the image
 * @param column number of columns of the image
 * @param rgb_component maximum number for each pixel
 */
imageRGB * createImageRGB(int row, int column, int rgb_component);


/**@fn void saveOnFileRGB(imageRGB * image, char * filename)
 * @brief Function to save RGB image in a file
 * @param image The RGB image to save
 * @param filename The name of file to save the image in
 */
void saveOnFileRGB(imageRGB * image, char * filename);

/**@fn void changeIntensityRGB(imageRGB * image, int amount)
 * @brief Function to change the intensity of an RGB image
 * @param image Image to change intensity
 * @param amount The amount to change
 */
void changeIntensityRGB(imageRGB * image, int amount);

/**@fn RGBPixel getPixelRGB(image RGB * image, int row, int column)
 * @brief Function to get a pixel from a RGB image
 * @param image Image to get the pixel
 * @param row Row where the pixel is
 * @param column Column where the pixel is
 */
RGBPixel getPixelRGB(imageRGB * image, int row, int column);

/**@fn ImageRGB * meanFilter(ImageRGB * image, int sizeOfKernel)
 * @brief Function to do the mean filtering of a RGB image
 * @param image Image to filter
 * @param sizeOfKernel Size of the Kernel used in the filter
 */
imageRGB * meanFilter(imageRGB * image, int sizeOfKernel);

/**@fn imageRGB * watermarkImage(imageRGB * image1, imageRGB * image2, float alpha)
 * @brief Function used to add a watermark to a image
 * @param image1 Image that will be watermarked
 * @param image2 Image tha will be the watermark
 * @param alpha amount of transperency
 */
imageRGB * watermarkImage(imageRGB * image1, imageRGB * image2, float alpha);
