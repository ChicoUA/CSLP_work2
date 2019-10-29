#include "grayscale.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846

imageGrayscale * createGrayscaleImage(int row, int column, int lum){
	imageGrayscale * image = (imageGrayscale *)malloc(sizeof(imageGrayscale));
	image->row = row;
	image->column = column;
	image->lum = lum;
	image->stream = (unsigned char *)calloc(row*column, sizeof(unsigned char));
	return image;
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
	
void changeIntensityGrayscale(imageGrayscale * image, int amount){
	for(int i = 0; i < image->row * image->column; i++){
		int newvalue  = (int)image->stream[i] + amount;
		if(newvalue > 255)
			newvalue = 255;
		else if(newvalue < 0)
			newvalue = 0;
		image->stream[i]  = (unsigned char)newvalue;
	}
}

unsigned char getPixelGrayscale(imageGrayscale * image, int row, int column){
	if(row < 0 || row > image->row || column < 0 || column > image->column)
		return 0;

	return *(image->stream + row*image->column + column);
}

imageGrayscale * meanFilterGS(imageGrayscale * image, int sizeOfKernel){
	if(sizeOfKernel % 2 == 0){
                printf("Size of kernel must be odd!");
                exit(1);
        }

        int n = (sizeOfKernel - 1) / 2; // odd number = 2n + 1, this n gives the middle number of the size

        float Gx[sizeOfKernel][sizeOfKernel];

        for(int i = 0; i < sizeOfKernel; i++){
                for(int j = 0; j < sizeOfKernel; j++){
                        Gx[i][j] = 1/((float)sizeOfKernel * (float)sizeOfKernel);
                }
        }

	imageGrayscale * imageMeanFilter = createGrayscaleImage(image->row, image->column, image->lum);

        //make convolution of each pixel
        for(int row = 0; row < image->row; row++){
                for(int column = 0; column < image->column; column++){
                        float temp = 0;
                        for(int krow = 0; krow < sizeOfKernel; krow++){
                                for(int kcolumn = 0; kcolumn < sizeOfKernel; kcolumn++){
                                        float xValue = Gx[krow][kcolumn];

                                        int newrow = row + krow - n;
                                        int newcolumn = column + kcolumn -n;

                                        unsigned char pixel = getPixelGrayscale(image, newrow, newcolumn);
                                        temp += (float)pixel * xValue;

                                }
                        }
                        *(imageMeanFilter->stream + row * image->column + column) = (int)temp;
                }
        }

        return imageMeanFilter;

}

imageGrayscale * gaussianFilter(imageGrayscale * image, int sizeOfKernel){
	if(sizeOfKernel % 2 == 0){
                printf("Size of kernel must be odd!");
                exit(1);
        }

	// set standard deviation to 1.0
    	double sigma = 1.0;
    	double r, s = 2.0 * sigma * sigma;

	// sum is for normalization
    	double sum = 0.0;

        int n = (sizeOfKernel - 1) / 2; // odd number = 2n + 1, this n gives the middle number of the size

        float Gx[sizeOfKernel][sizeOfKernel];

        for(int i = -n; i <= n; i++){
                for(int j = -n; j <= n; j++){
			r = sqrt(i*i + j*j);
                        Gx[i+n][j+n] = (exp(-(r*r)/s))/(M_PI * s);
			sum += Gx[i + n][j + n];
                }
        }

	// normalize the Kernel
    	for(int i = 0; i < sizeOfKernel; ++i){
        	for(int j = 0; j < sizeOfKernel; ++j){
            		Gx[i][j] /= sum;
		}
	}
	
	imageGrayscale * imageGaussianFilter = createGrayscaleImage(image->row, image->column, image->lum);

        //make convolution of each pixel
        for(int row = 0; row < image->row; row++){
                for(int column = 0; column < image->column; column++){
                        float temp = 0;
                        for(int krow = 0; krow < sizeOfKernel; krow++){
                                for(int kcolumn = 0; kcolumn < sizeOfKernel; kcolumn++){
                                        float xValue = Gx[krow][kcolumn];

                                        int newrow = row + krow - n;
                                        int newcolumn = column + kcolumn -n;

                                        unsigned char pixel = getPixelGrayscale(image, newrow, newcolumn);
                                        temp += (float)pixel * xValue;

                                }
                        }
                        *(imageGaussianFilter->stream + row * image->column + column) = (int)temp;
                }
        }

        return imageGaussianFilter;

}
