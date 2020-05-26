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

#define LIMITE_HAAR 7  // 2^LIMITE_HAAR = hist_size
#define STR_SIZE 127

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

typedef struct _imageLuv {
  float L, u, v;
} ImageLuv;


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

void DestroyImage(Image **img)
{
  Image *aux;

  aux = *img;
  if(aux != NULL){
    if (aux->val != NULL)   free(aux->val); 
    if (aux->tbrow != NULL) free(aux->tbrow);
    free(aux);    
    *img = NULL;
  }
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

void    DestroyCImage(CImage **cimg)
{
  CImage *tmp;
  int i;

  tmp = *cimg;
  if (tmp != NULL) {
    for (i=0; i < 3; i++) 
      DestroyImage(&(tmp->C[i]));
    free(tmp);
    *cimg = NULL;
  }
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

void rgb2luv(int R, int G, int B, float* luv) {
		//http://www.brucelindbloom.com
		
		//float rf, gf, bf;
		//float r, g, b, X_, Y_, Z_, X, Y, Z, fx, fy, fz, xr, yr, zr;
        float r, g, b, X, Y, Z, yr;
		float L;
		float eps = 216.0/24389.0;
		float k = 24389.0/27.0;
		 
		float Xr = 0.964221;
		float Yr = 1.0;
		float Zr = 0.825211;
		 
		// RGB to XYZ
		 
		r = R/255.0; //R em 0..1
		g = G/255.0; //G em 0..1
		b = B/255.0; //B em 0..1
		   
		if (r <= 0.04045)
			r = r/12;
		else
            r = powf(((r+0.055)/1.055),2.4);
			//r = pow(((r+0.055)/1.055),2.4);
		
		if (g <= 0.04045)
			g = g/12;
		else
            g = powf((g+0.055)/1.055,2.4);
			//g = (float) pow((g+0.055)/1.055,2.4);
		 
		if (b <= 0.04045)
			b = b/12;
		else
            b = powf((b+0.055)/1.055,2.4);
			//b = (float) pow((b+0.055)/1.055,2.4);
		
		
		X =  0.436052025*r     + 0.385081593*g + 0.143087414 *b;
		Y =  0.222491598*r     + 0.71688606 *g + 0.060621486 *b;
		Z =  0.013929122*r     + 0.097097002*g + 0.71418547  *b;
		
		// XYZ to Luv
		
		if (X!=0 && Y!=0 && Z!=0){
		  float u, v, u_, v_, ur_, vr_;
						
		  u_ = 4*X / (X + 15*Y + 3*Z);
		  v_ = 9*Y / (X + 15*Y + 3*Z);
				 
		  ur_ = 4*Xr / (Xr + 15*Yr + 3*Zr);
		  vr_ = 9*Yr / (Xr + 15*Yr + 3*Zr);
		  
		  yr = Y/Yr;
		
		  if ( yr > eps )
			  L =  (float) (116*powf(yr, 1/3.0) - 16);
		  else
			  L = k * yr;
		 
		  u = 13*L*(u_ -ur_);
		  v = 13*L*(v_ -vr_);
		
          luv[0] = L;
          luv[1] = u; 
          luv[2] = v;   

          
		}
		else{
		  luv[0] = 0;
		  luv[1] = 0; 
		  luv[2] = 0;
		}
	} 


void RGB2Luv (CImage* RGB, ImageLuv** Luv) {

    int r, g, b;
    float luv[3];
    int i, j;

    for (i = 0; i < RGB->C[0]->nrows; i++)
        for (j = 0; j < RGB->C[0]->ncols; j++){
            r=RGB->C[0]->val[j+RGB->C[0]->tbrow[i]];
            g=RGB->C[1]->val[j+RGB->C[1]->tbrow[i]];
            b=RGB->C[2]->val[j+RGB->C[2]->tbrow[i]];
            rgb2luv(r, g, b, luv);
            Luv[i][j].L=luv[0];
            Luv[i][j].u=luv[1];
            Luv[i][j].v=luv[2];
            //luv[0]=-1;luv[1]=-1;luv[2]=-1;
        }
}

/* END - Auxiliary functions */

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
        fprintf(stderr,"Usage: cw-luv_extraction <img_path> <fv_path>\n");
        fprintf(stderr,"\timg_path: path to the image file in PPM (P6) format\n");
        fprintf(stderr,"\tfv_path:  path to the file that will store the feature vector\n");
        fprintf(stderr,"Remark:\n\tThis program extracts the feature vector from a single image.\n");
        exit(-1);
    }
    

    // -----------//
    CImage *imagem; //input image (RGB)
    ImageLuv **img; //image in Luv color space - matrix of the same size of the original image

    int i,j;
    
    imagem = ReadCImage(argv[1]); //Reading input image

    //Allocates Luv image
    img = (ImageLuv**) calloc(imagem->C[0]->nrows, sizeof(ImageLuv*));
    for (i=0; i<imagem->C[0]->nrows; i++) {
        img[i] = (ImageLuv*) calloc(imagem->C[0]->ncols, sizeof(ImageLuv));
    }

    //Converts from RGB to Luv
    RGB2Luv(imagem, img);

    //Computes Luv histogram
    int nbins=128, pos;
    float inter_l = 12.50; // 2^3 bits -> 8 bins range: [   0,100]
    float inter_u = 88.50; // 2^2 bits -> 4 bins range: [-134,220]
    float inter_v = 65.50; // 2^2 bits -> 4 bins range: [-140,122]

    //Histogram vector
    int *hist;

    hist = AllocIntArray(nbins);

    for (i = 0; i < imagem->C[0]->nrows; i++) {
        for (j = 0; j < imagem->C[0]->ncols; j++) {
            pos = (int) ( ((int)((img[i][j].L)/inter_l)) +
                          ((int)((img[i][j].u+134)/inter_u))*8 +
                          ((int)((img[i][j].v+140)/inter_v))*(8*4));
            hist[pos]++;
        }
    }

    int *fv = (int*) calloc(nbins-1, sizeof(int));

    Haar(hist, 0, nbins, 0, fv, 0);

    //Creating feature vector file - binary format
    FILE *fout;
    if ((fout = fopen(argv[2], "wb")) == NULL) {
        printf("ERROR CREATING FEATURE VECTOR FILE!\n");
        exit(0);
    }
    fwrite(fv, sizeof(int),(nbins-1), fout);
    fclose(fout);

    //Free memory
    for (i=0; i<imagem->C[0]->nrows; i++) {
        free(img[i]);
    }
    free(img);
    free(fv);
    DestroyCImage(&imagem);
    free(hist);
    return 0;
}
