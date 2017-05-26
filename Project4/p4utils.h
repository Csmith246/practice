/*

project 4 utilities

*/

#include "PGM.h"
void convolve(unsigned char inImg[MAXROWS][MAXCOLS],long rows, long cols,unsigned char outimg[MAXROWS][MAXCOLS], int kernel[3][3]);
void cannyX(unsigned char inImg[MAXROWS][MAXCOLS],long rows, long cols,unsigned char outimg[MAXROWS][MAXCOLS]);
void convolveGradient(unsigned char inImg[MAXROWS][MAXCOLS],long rows, long cols,unsigned char outimg[MAXROWS][MAXCOLS], int kernel1[3][3], int kernel2[3][3]);

