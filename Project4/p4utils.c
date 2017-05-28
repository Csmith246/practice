/*

project 4 utilities

*/
#include "p4utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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


void houghTransform(unsigned char edgeImg[MAXROWS][MAXCOLS], long rows, long cols, int gradThreshold){

  printf("rows = :%ld\ncols = :%ld\n", rows,cols);
  int row,col;
  int sizeofbins = 20;
  unsigned int binnedResult[sizeofbins][sizeofbins]; // rBin x thetaBin

//Initialize BinnedResult Array to 0s
  int a,b;
  for(a=0; a<sizeofbins; a++){
    for(b=0; b<sizeofbins; b++){
      binnedResult[a][b]=0;
      printf("%d ",binnedResult[a][b]);
    }
    printf("\n");
  }


  double MaxR = hypot((double)rows,(double)cols);
  printf("maxR = :%lf\n", MaxR);
  for(row = 0; row<rows; row++){
    for(col = 0; col<cols; col++){
      if(edgeImg[row][col] > gradThreshold){ // if gradientVal at [row][col] is greater than gradThreshold
		  double angle = 0;
		  double increment = PI/20.0;
		  int count=0;
		  while(count<sizeofbins){
                          printf("angle = :%lf\n", angle);
			  printf("count = %d\n", count);
			  count++;
			  double R = ((double)col)*cos(angle) + ((double)row)*sin(angle);
                          printf("R = :%lf\n", R);
			  double rRatio = R/MaxR;
                          printf("rRatio = :%lf\n", rRatio);
			  int rBin = abs((int)ceil(rRatio*((double)sizeofbins)));
                          printf("rbin = :%d\n", rBin);
			 // double thetaRatio = angle/PI;
			 // int thetaBin = abs((int)floor(thetaRatio*((double)sizeofbins)));
			  binnedResult[rBin][count] = binnedResult[rBin][count] + 1;
			  angle = angle + increment;
		  }
	  }
    }
  }





  int x,y;
  int binx,biny;
  unsigned int tempmax=0;
// Find the most voted bin cell
  for(x=0; x<sizeofbins; x++){
    for(y=0; y<sizeofbins; y++){
      unsigned int cellVal = binnedResult[x][y];
      if(cellVal>tempmax){
        tempmax = cellVal;
        binx = x;
        biny = y;
      }
      printf("%d ", binnedResult[x][y]);
    }
	  printf("\n");
  }
  printf("binx = %d, biny = %d, val = %d\n",binx,biny,tempmax);


  double finalR = (((double)binx))*(MaxR/20);
//  double finalR = 400;
  double finalTheta = (((double)biny)-1)*(PI/20);
//  double finalTheta = PI/2;
  unsigned char newVal = 255;

  printf("r = %lf, theta = %lf\n",finalR,finalTheta);


  int nRow, nCol;
  double nRowTemp = 0;
//draw new line on the graph
  for(nCol = 0; nCol<cols; nCol++){
    double temp1 = (-(cos(finalTheta)/sin(finalTheta))*(double)nCol);
    double temp2=  (finalR/sin(finalTheta));
//    printf("temp1=%lf , tmep2 = %lf\n",temp1, temp2);
   nRowTemp = temp1+temp2;
    nRow = floor(nRowTemp);
    edgeImg[nRow][nCol] = newVal;
  }


}

