all: ppmtovtx
ppmtovtx: ppmtovtx.c
	gcc -o ppmtovtx ppmtovtx.c
