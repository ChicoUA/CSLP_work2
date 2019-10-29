#include <stdio.h>
#include <stdlib.h>
#include "grayscale.h"
/**
 * Documentation for the grayscale.c module
 * This module is responsible for all grayscale operations.
 */

/**
 * Documentation for the readFileGrayscale function of the grayscale module.
 * This fuction fill do the following:
 * - Check if the files exists, if not it will exit, outputting the appropriate warning.
 * - Read the file format, if he is unable to it will exit.
 * - Check the format, if it's not valid it will exit, outputting the appropriate warning.
 * - Check the file for cooments.
 * - If all previous steps were validated it will create the image.
 */

imageGrayscale * readFileGrayscale(char * filename){
	char buff[16];
        FILE * file = fopen(filename, "rb");
        int c, row, column, lum;

        if(!file){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

         if (!fgets(buff, sizeof(buff), file)) {
              perror(filename);
              exit(1);
         }

        if (buff[0] != 'P' || buff[1] != '4') {
                fprintf(stderr, "Invalid image format (must be 'P4')\n");
                exit(1);
        }

        c = fgetc(file);

        while(c == '#'){
                while(getc(file) != '\n');
                c = fgetc(file);
        }

        ungetc(c, file);

        if(fscanf(file, "%d %d", &row, &column) != 2){
                fprintf(stderr, "1-Wrong file format!");
                exit(1);
        }

        if(fscanf(file, "%d", &lum) != 1){
                fprintf(stderr, "2-Wrong file format!");
                exit(1);
        }
	
	imageGrayscale * image = createGrayscaleImage(row, column, lum);

        if(fread(image->stream, image->row, image->column, file) != image->column){
                fprintf(stderr, "Wrong format for Grayscale image");
                exit(1);
        }

        return image;
}
/**
 * Documentation for the createGrayscaleImage function of the grayscale module.
 * This fuction fill do the following:
 */
imageGrayscale * createGrayscaleImage(int row, int column, int lum){
	imageGrayscale * image = (imageGrayscale *)malloc(sizeof(imageGrayscale));
	image->row = row;
	image->column = column;
	image->lum = lum;
	image->stream = (unsigned char *)calloc(row*column, sizeof(unsigned char));
}

/**
 * Documentation for the saveOnFileGrayscale function of the grayscale module.
 * This fuction fill do the following:
 * - Check if the files exists.
 * - if it does exist but it cannot access it it will exit, outputting the appropriate warning.
 * - If all previous steps were validated it will save the image.
 */
void saveOnFileGrayscale(imageGrayscale * image, char * filename){
	FILE * f = fopen(filename, "wb");
        if(!f){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

	fputs("P5\n", f);
        fprintf(f, "%d %d\n%d\n", image->row, image->column, image->lum);
        fwrite(image->stream, image->row, image->column, f);
        fclose(f);
}
	


