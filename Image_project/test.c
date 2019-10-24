#include <stdio.h>
#include <stdlib.h>
#include "RGB.h"
#include "grayscale.h"

int main(int argc, char *argv[]){
	if(argc > 2){
		printf("Just one argument!");
		exit(1);
	}

	imageRGB * image = readFileRGB(argv[1]);
	printf("row: %d, column: %d, rgb: %d\n", image->row, image->column, image->rgb_component);
	imageGrayscale * imageGS = createGrayscaleImage(image->row, image->column, image->rgb_component);
	for(int i = 0; i < image->row * image->column; i++){
                imageGS->stream[i] = (image->stream[i].r + image->stream[i].g + image->stream[i].b)/3;
        }
	saveOnFileRGB(image, "rgb.ppm");
	saveOnFileGrayscale(imageGS, "gray.ppm");
}
