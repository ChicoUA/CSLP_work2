#include "binary.h"
#include <stdio.h>
#include <stdlib.h>

imageBinary * readFileBinary(char * filename){
        char buff[16];
        FILE * file = fopen(filename, "rb");
        int c, row, column;

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
        if (buff[0] != 'P' || buff[1] != '1') {
                fprintf(stderr, "Invalid image format (must be 'P1')\n");
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

        imageBinary * image = createBinaryImage(row, column);

        if(fread(image->stream, image->row, image->column, file) != image->column){
                fprintf(stderr, "Wrong format for Binary image");
                exit(1);
        }

        return image;
}

void saveOnFileBinary(imageBinary * image, char * filename){
        FILE * f = fopen(filename, "wb");
         //check if file exists
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


