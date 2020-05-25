/*
    COPYRIGHT 2007-2012, 2020

    This file is part of Lib Descriptors.

    Lib Descriptors is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Lib Descriptors is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Lib Descriptors. If not, see <http://www.gnu.org/licenses/>.
    
    Otavio A. B. Penatti - otavio_at_penatti_dot_com
    https://github.com/otaviopenatti/libdescriptors
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#define LIMITE_HAAR 6

#define MSG1  "Cannot allocate memory space"

/* pgm images */
typedef struct _image {
  int *val;
  int ncols,nrows;
  int *tbrow;
} Image;

/* ppm images */
typedef struct cimage {
  Image *C[3];
} CImage;

typedef struct _imageHSV {
  float H, S, V;
} ImageHSV;


int NCFgets(char *s, int m, FILE *f) {
  while(fgets(s,m,f)!=NULL)
    if (s[0]!='#') return 1;
  return 0;
}

void Error(char *msg,char *func){ 
  fprintf(stderr,"Error:%s in %s\n",msg,func);
  exit(-1);
}

int *AllocIntArray(int n)
{
  int *v=NULL;
  v = (int *) calloc(n,sizeof(int));
  if (v == NULL)
    Error(MSG1,"AllocIntArray");
  return(v);
}

/* pgm images */
Image *CreateImage(int ncols, int nrows)
{
  Image *img=NULL;
  int i;

  img = (Image *) calloc(1,sizeof(Image));
  if (img == NULL){
    Error(MSG1,"CreateImage");
  }

  img->val   = AllocIntArray(nrows*ncols);
  img->tbrow = AllocIntArray(nrows);

  img->tbrow[0]=0;
  for (i=1; i < nrows; i++)
    img->tbrow[i]=img->tbrow[i-1]+ncols;
  img->ncols = ncols;
  img->nrows = nrows;
 
 return(img);
}

/* ppm images */
CImage *CreateCImage(int ncols, int nrows)
{
  CImage *cimg=NULL;
  int i;

  cimg = (CImage *) calloc(1, sizeof(CImage));
  for (i=0; i < 3; i++) 
    cimg->C[i] = CreateImage(ncols,nrows);
  return(cimg);
}

CImage *ReadCImage(char *filename)
{
  CImage *cimg=NULL;
  FILE *fp=NULL;
  char type[10];
  int  i,ncols,nrows,n;
  char z[256];

  fp = fopen(filename,"rb");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  fscanf(fp,"%s\n",type);
  if((strcmp(type,"P6")==0)){
    NCFgets(z,255,fp);
    sscanf(z,"%d %d\n",&ncols,&nrows);
    n = ncols*nrows;
    NCFgets(z,255,fp);
    sscanf(z,"%d\n",&i);
    cimg = CreateCImage(ncols,nrows);
    for (i=0; i < n; i++){
      cimg->C[0]->val[i] = fgetc(fp);
      cimg->C[1]->val[i] = fgetc(fp);
      cimg->C[2]->val[i] = fgetc(fp);
    }
    fclose(fp);
  }else{
    fprintf(stderr,"Input image must be P6\n");
    exit(-1);
  }

  return(cimg);
}

//Finds min value
float min(float x, float y, float z) {

    if ( (x<=y) && (x<=z) ) {
        return x;
    } else if ( (y<=x) && (y<=z) ) {
        return y;
    } else if ( (z<=x) && (z<=y) ) {
        return z;
    }

}

//Finds max value
float max(float x, float y, float z) {

    if ( (x>=y) && (x>=z) ) {
        return x;
    } else if ( (y>=x) && (y>=z) ) {
        return y;
    } else if ( (z>=x) && (z>=y) ) {
        return z;
    }

}


void RGB2HSV (CImage* RGB, ImageHSV** HSV) {

    float r, g, b;
    float minVal, maxVal, delta;
    // float del_R, del_G, del_B;
    int i, j;

    for (i = 0; i < RGB->C[0]->nrows; i++) {
        for (j = 0; j < RGB->C[0]->ncols; j++) {

            //normalizes from 0 to 1
            r = (float) RGB->C[0]->val[j+RGB->C[0]->tbrow[i]]/255;
            g = (float) RGB->C[1]->val[j+RGB->C[1]->tbrow[i]]/255;
            b = (float) RGB->C[2]->val[j+RGB->C[2]->tbrow[i]]/255;

            minVal = min(r, g, b);
            maxVal = max(r, g, b);
            delta = maxVal - minVal;

            //H
            if (delta == 0) {
                HSV[i][j].H = 0;
            } else if (maxVal==r && g>=b) {
                HSV[i][j].H = 60*((g-b)/delta);
            } else if (maxVal==r && g<b) {
                HSV[i][j].H = 60*((g-b)/delta) + 360;
            } else if (maxVal==g) {
                HSV[i][j].H = 60*((b-r)/delta) + 120;
            } else if (maxVal==b) {
                HSV[i][j].H = 60* ((r-g)/delta) + 240;
            }

            //S
            if (maxVal==0) {
                HSV[i][j].S = 0;
            } else {
                HSV[i][j].S = delta/maxVal;
            }

            //V
            HSV[i][j].V = maxVal;

            //normalizes S and V between 0 and 100
            HSV[i][j].S *= 100;
            HSV[i][j].V *= 100;

            //normalizes H from 0 to 1
            //HSV[i][j].H /= 360;

        }
    }
}
/* END -  Auxiliary functions */

/* Haar
Computes the Haar transfor coefficients
Recursively divides the histogram in the middle and checks if the sum of 
the first half if greater than the sum of the second half.
If so, Haar coefficient of the k-th position is 1, otherwise, is 0.
//hist = histogram
//ini  = histogram interval start
//fim  = histogram interval end
//k    = position in the coefficient vector
//vet  = coefficient vector
//lim  = recursion limit = 
*/
void Haar(int *hist, int ini, int fim, int k, int *vet, int lim) {

    int meio = (ini+fim)/2;
    int i;
    long long int sum1=0, sum2=0;

    if (lim<LIMITE_HAAR) {

        for (i=ini; i<meio; i++) sum1 += hist[i];
        for (i=meio; i<fim; i++) sum2 += hist[i];

        if (sum1 >= sum2) vet[k] = 1;
        else              vet[k] = 0;

        Haar(hist, ini, meio, 2*k+1, vet, lim+1);
        Haar(hist, meio, fim, 2*k+2, vet, lim+1);
    }
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr,"Usage: cw-hsv_extraction <img_path> <fv_path>\n");
        fprintf(stderr,"\timg_path: path to the image file in PPM (P6) format\n");
        fprintf(stderr,"\tfv_path:  path to the file that will store the feature vector\n");
        fprintf(stderr,"Remark:\n\tThis program extracts the feature vector from a single image.\n");
        exit(-1);
    }


    // -----------//
    CImage *imagem; //input image (RGB)
    ImageHSV **img; //image in HSV color space - matrix of the same size of the original image
    char *img_path, *fv_path;
    int i,j;

    img_path = argv[1];
    fv_path = argv[2];
    imagem = ReadCImage(img_path); //Reading input image

    //Allocates HSV image
    img = (ImageHSV**) calloc(imagem->C[0]->nrows, sizeof(ImageHSV*));
    for (i=0; i<imagem->C[0]->nrows; i++) {
        img[i] = (ImageHSV*) calloc(imagem->C[0]->ncols, sizeof(ImageHSV));
    }

    //Converts from RGB to HSV
    RGB2HSV(imagem, img);

    //Computes HSV Histograma
    int nbins=64, pos;
    //variables to indicate the interval size of each band
    int inter_h = 45;   //6
    float inter_s = 50.5;   //2
    float inter_v = 25.25;   //4

    //Histogram vector
    int *hist=0;

    hist = AllocIntArray(nbins);

    for (i = 0; i < imagem->C[0]->nrows; i++) {
        for (j = 0; j < imagem->C[0]->ncols; j++) {

            pos = (int) ( ((int)(img[i][j].H/inter_h)) + ((int)(img[i][j].S/inter_s))*8 + ((int)(img[i][j].V/inter_v))*(8*2));
            hist[pos]++;
        }
    }

    int *fv = (int*) calloc(nbins-1, sizeof(int));
    Haar(hist, 0, nbins, 0, fv, 0);

    //Creating feature vector file
    FILE *fout;
    if ((fout = fopen(fv_path, "w")) == NULL) {
        printf("Error creating feature vector file!\n");
        exit(0);
    }
    for (i=0; i<(nbins-1); i++) {
        fprintf(fout, "%d", fv[i]);
    }

    fclose(fout);
    return 0;
}


