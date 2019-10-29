#include "binary.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * Documentation for the binary.c module
 * This module is responsible for all binary operations.
 */

/**
 * Documentation for the readFileBinary function of the binary module.
 * This fuction fill do the following:
 * - Check if the files exists, if not it will exit, outputting the appropriate warning.
 * - Read the file format, if he is unable to it will exit.
 * - Check the format, if it's not valid it will exit, outputting the appropriate warning.
 * - Check the file for cooments.
 * - If all previous steps were validated it will create the image.
 */
imageBinary * readFileBinary(char * filename){
        char buff[16];
        FILE * file = fopen(filename, "rb");
        int c, row, column;

        if(!file){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

         if (!fgets(buff, sizeof(buff), file)) {
              perror(filename);
              exit(1);
         }

        if (buff[0] != 'P' || buff[1] != '1') {
                fprintf(stderr, "Invalid image format (must be 'P1')\n");
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

        imageBinary * image = createBinaryImage(row, column);

        if(fread(image->stream, image->row, image->column, file) != image->column){
                fprintf(stderr, "Wrong format for Binary image");
                exit(1);
        }

        return image;
}
/**
 * Documentation for the saveOnFileBinary function of the binary module.
 * This fuction fill do the following:
 * - Check if the files exists.
 * - if it does exist but it cannot access it it will exit, outputting the appropriate warning.
 * - If all previous steps were validated it will save the image.
 */
void saveOnFileBinary(imageBinary * image, char * filename){
        FILE * f = fopen(filename, "wb");
        if(!f){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

        fputs("P1\n", f);
        fprintf(f, "%d %d\n", image->row, image->column);
        for(int i = 0; i < image->row*image->column; i++){
		fprintf(f, "%d", image->stream[i]);
	}
        fclose(f);
}