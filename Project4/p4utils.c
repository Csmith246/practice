/*

project 4 utilities

*/
#include "p4utils.h"
#include <math.h>
#include <stdio.h>

#define PI 3.14159265


void cannyX(unsigned char inImg[MAXROWS][MAXCOLS],long rows, long cols,unsigned char outimg[MAXROWS][MAXCOLS]){

  int cannyX[3][3] = {{1, 0, -1},{2, 0, -2},{1, 0,-1}};
  int row,col;
  for (row = 0; row < rows; row++){
    for (col = 0; col < cols; col++){

      long acc = 0;
        //instead of wrapping around a toroid we just repeat first/last coloum and row
        int krow, kcol;

        for  (krow = 0; krow < 3; krow++){
          for (kcol = 0; kcol < 3; kcol++){
              //get the kernel value
              int kval = cannyX[krow][kcol];
              //printf("kval is: %d",kval);

              int krowoffset = krow -1;
              int kcoloffset = kcol -1;
              int currow = row + krowoffset;
              int curcol = col + kcoloffset;

              currow = currow < 0? 0 : currow;
              curcol = curcol < 0? 0 :curcol;
              currow = currow > rows -1 ? rows -1 :currow;
              curcol = curcol > cols - 1? cols - 1:curcol;
              acc += inImg[currow][curcol]*kval;
          }
          }
          outimg[row][col] = acc;
      }
  }

}

void convolve(unsigned char inImg[MAXROWS][MAXCOLS],long rows, long cols,unsigned char outimg[MAXROWS][MAXCOLS], int kernel[3][3])
{
  int row,col;
  for (row = 0; row < rows; row++){
    for (col = 0; col < cols; col++){

      long acc = 0;
        //instead of wrapping around a toroid we just repeat first/last coloum and row
        int krow, kcol;

        for  (krow = 0; krow < 3; krow++){
          for (kcol = 0; kcol < 3; kcol++){
              //get the kernel value
              int kval = kernel[krow][kcol];
              //printf("kval is: %d",kval);

              int krowoffset = krow -1;
              int kcoloffset = kcol -1;
              int currow = row + krowoffset;
              int curcol = col + kcoloffset;

              currow = currow < 0? 0 : currow;
              curcol = curcol < 0? 0 :curcol;
              currow = currow > rows -1 ? rows -1 :currow;
              curcol = curcol > cols - 1? cols - 1:curcol;
              acc += (inImg[currow][curcol])*kval;
              //acc += (inImg[currow][curcol]/2)*kval;
          }
          }
          //outimg[row][col] = acc + 127;
          outimg[row][col] = acc;
      }
  }

}


void convolveGradient(unsigned char inImg[MAXROWS][MAXCOLS],long rows, long cols,unsigned char outimg[MAXROWS][MAXCOLS], int kernel1[3][3], int kernel2[3][3], int threadCount)
{
  int row;
  int col;
#pragma omp parallel for num_threads(threadCount) private(col)
  for (row = 0; row < rows; row++){
    for (col = 0; col < cols; col++){

      long acc1 = 0;
	  long acc2 = 0;
        //instead of wrapping around a toroid we just repeat first/last coloum and row
        int krow, kcol;

        for  (krow = 0; krow < 3; krow++){
          for (kcol = 0; kcol < 3; kcol++){
              //get the kernel1 value
              int kval1 = kernel1[krow][kcol];
              //printf("kval is: %d",kval);

			  //get the kernel1 value
              int kval2 = kernel2[krow][kcol];
              int krowoffset = krow -1;
              int kcoloffset = kcol -1;
              int currow = row + krowoffset;
              int curcol = col + kcoloffset;

              currow = currow < 0? 0 : currow;
              curcol = curcol < 0? 0 :curcol;
              currow = currow > rows -1 ? rows -1 :currow;
              curcol = curcol > cols - 1? cols - 1:curcol;
              acc1 += (inImg[currow][curcol])*0.25*kval1;
              acc2 += (inImg[currow][curcol])*0.25*kval2;
              //acc += (inImg[currow][curcol]/2)*kval;
            }
          }
          //outimg[row][col] = acc + 127;
          outimg[row][col] = sqrt((acc1*acc1)+(acc2*acc2));
      }
  }

}


unsigned char* houghTransform(unsigned char edgeImg[MAXROWS][MAXCOLS], long rows, long cols, int gradThreshold){
  printf("rows = :%ld\ncols = :%ld\n", rows,cols);

  int row,col;
  int sizeofbins = 20;
  unsigned long binnedResult [sizeofbins][sizeofbins]; // rBin x thetaBin
  double MaxR = hypot((double)rows,(double)cols);
  printf("maxR = :%lf\n", MaxR);
  for(row = 0; row<rows; row++){
    for(col = 0; col<cols; col++){
      if(edgeImg[row][col] > gradThreshold){ // if gradientVal at [row][col] is greater than gradThreshold
		  double angle;
		  for(angle = 0; angle<PI; angle=angle+(PI/20)){//angle<=PI?
			  double R = row*cos(angle) + col*sin(angle);
			  double rRatio = R/MaxR;
			  int rBin = (int)floor(rRatio*sizeofbins);
			  double thetaRatio = angle/PI;
			  int thetaBin = (int)floor(thetaRatio*sizeofbins);
			  binnedResult[rBin][thetaBin] =+ 1;
		  }
	  }
    }
  }
  
  //IDEA FOR TOMORROW:::: tRY TAKING THE HIGHEST SCORING BIN AND DRAW THAT AND SEE WHERE IT IS DRAWN..... STILL THO I THINK SOMETHING MAY BE SLIGHTLY WRONG
  // BECAUSE THE RADII FOR THE LOCATIONS OF THE HORIZONTAL LINES DON'T SEEM TO BE THERE. BUT YEA, TAKE THE BIN WITH THE MOST HITS AND DRAW A LINE AND SEE WHAT HAPPENS
  // CAUSE IT MIGHT BE RIGHT.....
  
  int x,y;
  for(x=0; x<sizeofbins; x++){
	  for(y=0; y<sizeofbins; y++){
		  if (binnedResult[x][y]>300000){
		     printf("%u ", binnedResult[x][y]);
		  }else{
			  printf("0 ");
		  }
	  }
	  printf("\n");
  }
  
}

