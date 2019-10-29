/**@file binary.h
 * @brief Header file with functions and struct for binary images
 *
 */

/**
 * @brief struct to represent a binary image. It has its number of rows and columns (image resolution) and the stream with its pixels
 */
typedef struct {
	int row;
	int column;
	unsigned char * stream;
}imageBinary;

/**@fn imageBinary * readFileBinary(char* filename)
 * @brief Function used to read a pbm file with a binary image and return the image
 * @param filename the name of the file to read
 */
imageBinary * readFileBinary(char * filename);

/**@fn imageBinary * createImageBinary(int row, int column)
 * @brief Function to create an Binary image and returns an empty image
 * @param row number of rows of the image
 * @param column number of columns of the image
 */
imageBinary * createImageBinary(int row, int column);

/**@fn void saveOnBinary(imageBInary * image, char * filename)
 * @brief Function to save Binary image in a file
 * @param image The Binary image to save
 * @param filename The name of file to save the image in
 */
void saveOnFileBinary(imageBinary * image, char * filename);
