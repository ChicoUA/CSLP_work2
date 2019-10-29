CC=gcc
CFLAGS=-lm
DEPS=binary.h grayscale.h RGB.h
OBJ=test.o binary.o grayscale.o RGB.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

image_program: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f image_program test.o binary.o grayscale.o RGB.o watermark.ppm gray.pgm rgb.ppm red.pgm green.pgm blue.pgm intensity_grayscale.pgm intensity_rgb.ppm edge.pgm mean.ppm binary.pbm meanGS.pgm
