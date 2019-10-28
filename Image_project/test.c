#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "RGB.h"
#include "grayscale.h"
#include "binary.h"


//filtragem: considerar filtro como imagem, colocar uma matriz sobre um pixel que corresponde e fazer operação de convulsão. Usar filtro de média e talvez gaussiano. Usar filtro quadrado
//usar filtros impares, usar 0 fora da imagem com método getpixel
//watermarking colocar uma imagem dentro de outra ao fazer uma soma

imageBinary * createBinaryImage(int row, int column){
        imageBinary * image = (imageBinary *)malloc(sizeof(imageBinary));
        image->row = row;
        image->column = column;
        image->stream = (unsigned char *)calloc(row*column, sizeof(unsigned char));
        return image;
}


imageGrayscale * convertRGBtoGrayscale(imageRGB * image){
	imageGrayscale * imageGS = createGrayscaleImage(image->row, image->column, image->rgb_component);
        for(int i = 0; i < image->row * image->column; i++){
                imageGS->stream[i] = (image->stream[i].r + image->stream[i].g + image->stream[i].b)/3;
        }
	return imageGS;
}

imageGrayscale * convertRGBComponentToGrayscale(imageRGB * image, char component){
	imageGrayscale * imageGS = createGrayscaleImage(image->row, image->column, image->rgb_component);
	for(int i = 0; i < image->row * image->column; i++){
		if(component == 'r'){
			imageGS->stream[i] = image->stream[i].r;
		}
		else if(component == 'g'){
			imageGS->stream[i] = image->stream[i].g;
		}
		else{
			imageGS->stream[i] = image->stream[i].b;
		}
	}
	return imageGS;
}

imageBinary * convertGrayscaletoBinary(imageGrayscale * image, int th){ //ver método de otsu
	if(th > 255 || th < 0){
		printf("Invalid value of threshold, going to use 128\n");
		th=128;
	}

	imageBinary * imageB = createBinaryImage(image->row, image->column);
	for(int i = 0; i < image->row*image->column; i++){
		if(image->stream[i] >= th){
			imageB->stream[i] = 1;
		}
		else{
			imageB->stream[i] = 0;
		}
	}
	return imageB;
}

imageGrayscale * edgeDetectionFilter(imageRGB * image, int sizeOfKernel){
        if(sizeOfKernel % 2 == 0){
                printf("Size of kernel must be odd!");
                exit(1);
        }

        imageGrayscale * imageGS = createGrayscaleImage(image->row, image->column, image->rgb_component);

        int n = (sizeOfKernel - 1) / 2; // odd number = 2n + 1, this n gives the middle number of the size

       int Gx[sizeOfKernel][sizeOfKernel];
       int Gy[sizeOfKernel][sizeOfKernel];

        //fill Gx and Gy with values
        for(int i = 0; i < sizeOfKernel; i++){
                for(int j = 0; j < sizeOfKernel; j++){
                        if(i == n)
                                Gy[i][j] = 0;
                        else if(i < n)
                                Gy[i][j] = -1;
                        else
                                Gy[i][j] = 1;

                        if(j == n)
                                Gx[i][j] = 0;
                        else if(j < n)
                                Gx[i][j] = -1;
                        else
                                Gx[i][j] = 1;
                }
        }

        //make convolution of each pixel
        for(int row = 0; row < image->row; row++){
                for(int column = 0; column < image->column; column++){
                        int tempx = 0;
                        int tempy = 0;
                        for(int krow = 0; krow < sizeOfKernel; krow++){
                                for(int kcolumn = 0; kcolumn < sizeOfKernel; kcolumn++){
                                        int xValue = Gx[krow][kcolumn];
                                        int yValue = Gy[krow][kcolumn];

                                        int newrow = row + krow - n;
                                        int newcolumn = column + kcolumn -n;

                                        RGBPixel pixel = getPixelRGB(image, newrow, newcolumn);
                                        tempx += ((pixel.r + pixel.g + pixel.b)/3) * xValue;
					tempy += ((pixel.r + pixel.g + pixel.b)/3) * yValue;

                        	}
			}
                        *(imageGS->stream + row * image->column + column) = sqrt(pow(tempx, 2) + pow(tempy, 2));
        	}
	}	

	return imageGS;
}

int main(int argc, char *argv[]){
	if(argc > 2){
		printf("Just one argument!");
		exit(1);
	}

	imageRGB * image = readFileRGB(argv[1]);
	printf("row: %d, column: %d, rgb: %d\n", image->row, image->column, image->rgb_component);

	imageGrayscale * imageGS = convertRGBtoGrayscale(image);
	saveOnFileGrayscale(imageGS, "gray.pgm");

	imageBinary * imageB = convertGrayscaletoBinary(imageGS, 128);
	saveOnFileBinary(imageB, "binary.pbm");

	imageGrayscale * imageR = convertRGBComponentToGrayscale(image, 'r');
	saveOnFileGrayscale(imageR, "red.pgm");

	imageGrayscale * imageG = convertRGBComponentToGrayscale(image, 'g');
        saveOnFileGrayscale(imageG, "green.pgm");

	imageGrayscale * imageBl = convertRGBComponentToGrayscale(image, 'b');
	saveOnFileGrayscale(imageBl, "blue.pgm");

	imageGrayscale * imageEdgeFilter = edgeDetectionFilter(image, 3);
	saveOnFileGrayscale(imageEdgeFilter, "edge.pgm");

	imageRGB * imageMeanFilter = meanFilter(image, 7);
	saveOnFileRGB(imageMeanFilter, "mean.ppm");

	imageGrayscale * imageMeanFilterGS = meanFilterGS(imageGS, 7);
	saveOnFileGrayscale(imageMeanFilterGS, "meanGS.pgm");

	imageRGB * testImage = readFileRGB("girl.ppm");

	imageRGB * imageWatermark = watermarkImage(image, testImage, 0.3);
	saveOnFileRGB(imageWatermark, "watermark.ppm");

	changeIntensityRGB(image,80);
	saveOnFileRGB(image, "intensity_rgb.ppm");

	changeIntensityGrayscale(imageGS, 80);
	saveOnFileGrayscale(imageGS, "intensity_grayscale.pgm");


}


