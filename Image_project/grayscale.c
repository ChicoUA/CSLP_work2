#include "grayscale.h"
#include <stdio.h>
#include <stdlib.h>

imageGrayscale * createGrayscaleImage(int row, int column, int lum){
	imageGrayscale * image = (imageGrayscale *)malloc(sizeof(imageGrayscale));
	image->row = row;
	image->column = column;
	image->lum = lum;
	image->stream = (unsigned char *)calloc(row*column, sizeof(unsigned char));
}

imageGrayscale * readFileGrayscale(char * filename){
	char buff[16];
        FILE * file = fopen(filename, "rb");
        int c, row, column, lum;

	 //check if file exists
        if(!file){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

        //read image format
         if (!fgets(buff, sizeof(buff), file)) {
              perror(filename);
              exit(1);
         }

        //check the image format
        if (buff[0] != 'P' || buff[1] != '4') {
                fprintf(stderr, "Invalid image format (must be 'P4')\n");
                exit(1);
        }
	
	//check for comments
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
                fprintf(stderr, "Wrong format for RGB image");
                exit(1);
        }

        return image;
}

void saveOnFileGrayscale(imageGrayscale * image, char * filename){
	FILE * f = fopen(filename, "wb");
	 //check if file exists
        if(!f){
                fprintf( stderr, "Could not open file %s \n", filename);
                exit(1);
        }

	fputs("P5\n", f);
        fprintf(f, "%d %d\n%d\n", image->row, image->column, image->lum);
        fwrite(image->stream, image->row, image->column, f);
        fclose(f);
}
	


