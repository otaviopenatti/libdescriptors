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
#include "dimage.h"

DImage *CreateDImage(int ncols, int nrows)
{
  DImage *dimg=NULL;
  int i;

  dimg = (DImage *) calloc(1,sizeof(DImage));
  if (dimg == NULL){
    Error(MSG1,"CreateDImage");
  }

  dimg->val   = AllocDoubleArray(nrows*ncols);
  dimg->tbrow = AllocIntArray(nrows);

  dimg->tbrow[0]=0;
  for (i=1; i < nrows; i++)
    dimg->tbrow[i]=dimg->tbrow[i-1]+ncols;
  dimg->ncols = ncols;
  dimg->nrows = nrows;

 return(dimg);
}

void DestroyDImage(DImage **dimg)
{
  DImage *aux;

  aux = *dimg;
  if(aux != NULL){
    if (aux->val != NULL)   free(aux->val);
    if (aux->tbrow != NULL) free(aux->tbrow);
    free(aux);
    *dimg = NULL;
  }
}

DImage  *ReadDImage(char *filename){
  DImage *dimg;
  char msg[512];
  int  ncols,nrows,n;
  FILE *fp;

  fp = fopen(filename,"rb");
  if (fp == NULL){
    sprintf(msg,"Cannot open %s",filename);
    Error(msg,"ReadDImage");
  }

  fscanf(fp,"%d %d\n",&ncols,&nrows);
  dimg = CreateDImage(ncols, nrows);
  n = ncols*nrows;
  fread(dimg->val, sizeof(double), n, fp);

  return dimg;
}

void    WriteDImage(DImage *dimg, char *filename){
  char msg[512];
  int n;
  FILE *fp;

  fp = fopen(filename,"wb");
  if (fp == NULL){
    sprintf(msg,"Cannot open %s",filename);
    Error(msg,"WriteDImage");
  }

  fprintf(fp,"%d %d\n",dimg->ncols,dimg->nrows);
  n = dimg->ncols*dimg->nrows;
  fwrite(dimg->val, sizeof(double), n ,fp);
}

bool ValidDImagePixel(DImage *dimg, int x, int y)
{
  if ((x >= 0)&&(x < dimg->ncols)&&
      (y >= 0)&&(y < dimg->nrows))
    return(true);
  else
    return(false);
}

double  MaximumDImageValue(DImage *dimg)
{
  double max;
  int i,n;

  n = dimg->ncols*dimg->nrows;
  max = dimg->val[0];
  for (i=1; i < n; i++)
    if (dimg->val[i] > max)
      max = dimg->val[i];

  return(max);
}

double  MinimumDImageValue(DImage *dimg)
{
  double min;
  int i,n;

  n = dimg->ncols*dimg->nrows;
  min = dimg->val[0];
  for (i=1; i < n; i++)
    if (dimg->val[i] < min)
      min = dimg->val[i];

  return(min);
}

void SetDImage(DImage *dimg, double value)
{
  int i,n;
  n = dimg->ncols*dimg->nrows;
  for (i=0; i < n; i++){
    dimg->val[i]=value;
  }
}

DImage *CopyDImage(DImage *dimg)
{
  DImage *dimgc;

  dimgc = CreateDImage(dimg->ncols,dimg->nrows);
  memcpy(dimgc->val,dimg->val,dimg->ncols*dimg->nrows*sizeof(double));

  return(dimgc);
}


Image *ConvertDImage2Image(DImage *dimg){
  Image *img;
  double max,min;
  int p,n;

  n = dimg->ncols*dimg->nrows;
  img = CreateImage(dimg->ncols, dimg->nrows);
  max = MaximumDImageValue(dimg);
  min = MinimumDImageValue(dimg);
  if (max==min) {
    for(p=0; p<n; p++)
      img->val[p] = max;
    return img;
  }

  for(p=0; p<n; p++){
    img->val[p] = ROUND(255.0*(dimg->val[p]-min)/(max-min));
  }

  return img;
}


/** ------------- MO445 - 2008 s2 - Project functions -----------*/

DImage *ConvertImage2DImage(Image* img)
{
	DImage* dimg = CreateDImage(img->ncols, img->nrows);

	int p,n = dimg->ncols*dimg->nrows;

	for(p=0; p<n; p++)
	{
		dimg->val[p] = (double)img->val[p];
	}
	return dimg;
}

DImage *DScale(DImage *img, float Sx, float Sy)
{
  float S[2][2],x,y,d1,d2,d3,d4,Ix1,Ix2,If;
  DImage *scl;
  Pixel u,v,prev,next;

  if (Sx == 0.0) Sx = 1.0;
  if (Sy == 0.0) Sy = 1.0;

  S[0][0] = 1.0/Sx;
  S[0][1] = 0;
  S[1][0] = 0;
  S[1][1] = 1.0/Sy;

  scl = CreateDImage((int)(img->ncols*fabs(Sx) + 0.5),(int)(img->nrows*fabs(Sy) + 0.5));

  for (v.y=0; v.y < scl->nrows; v.y++)
    for (v.x=0; v.x < scl->ncols; v.x++){
      x = ((v.x-scl->ncols/2.)*S[0][0] + (v.y-scl->nrows/2.)*S[0][1])
	+ img->ncols/2.;
      y = ((v.x-scl->ncols/2.)*S[1][0] + (v.y-scl->nrows/2.)*S[1][1])
	+ img->nrows/2.;
      u.x = (int)(x+0.5);
      u.y = (int)(y+0.5);
      if (ValidDImagePixel(img,u.x,u.y)){
	if (x < u.x) {
	  next.x = u.x;
	  prev.x = u.x - 1;
	} else {
	  next.x = u.x + 1;
	  prev.x = u.x;
	}
	d1 = next.x - x;
	d2 = x - prev.x;
	if (y < u.y) {
	  next.y = u.y;
	  prev.y = u.y - 1;
	} else {
	  next.y = u.y + 1;
	  prev.y = u.y;
	}
	d3 = next.y - y;
	d4 = y - prev.y;

	if (ValidDImagePixel(img,prev.x,prev.y)&&ValidDImagePixel(img,next.x,prev.y))
	  Ix1 = d1*img->val[prev.x+img->tbrow[prev.y]] +
	    d2*img->val[next.x+img->tbrow[prev.y]];
	else
	  Ix1 = img->val[u.x+img->tbrow[u.y]];

	if (ValidDImagePixel(img,prev.x,next.y)&&ValidDImagePixel(img,next.x,next.y))
	  Ix2 = d1*img->val[prev.x+img->tbrow[next.y]] +
	    d2*img->val[next.x+img->tbrow[next.y]];
	else
	  Ix2 = img->val[u.x+img->tbrow[u.y]];

	If = d3*Ix1 + d4*Ix2;

	scl->val[v.x+scl->tbrow[v.y]] = (double)If;
      }
    }

  return(scl);
}

/** Increases the size of DImage until it becomes power-of-2-sided.
 *  The resulting image IS NOT squared.
 **/
DImage* DImagePower2(DImage* img)
{
    if(img == NULL) Error("Img NULL", "DImagePower2");

    int ncols = (int)ceil(log(img->ncols)/log(2));
    int nrows = (int)ceil(log(img->nrows)/log(2));
    ncols = 1 << ncols;
    nrows = 1 << nrows;

    DImage* result = CreateDImage(ncols, nrows);

    int x,y;

    for(y = 0; y < nrows; y++)
    {
        for(x = 0; x < ncols; x++)
        {
            if(y < img->nrows && x < img->ncols)
            {
                int i = img->tbrow[y]+x;
                int j = result->tbrow[y] + x;
                result->val[j] = img->val[i];
            }else{
                // Optimization
                if(x >= img->ncols) x = ncols;
                if(y >= img->nrows) y = nrows;
            }
        }
    }

    return result;
}

DImage* DShift(DImage* img)
{
	int cols = img->ncols;
	int rows = img->nrows;
	int i,j;

	DImage *result = CreateDImage(cols, rows);

	for ( i=0; i<rows; i++)
	{
		for ( j=0; j<cols; j++)
		{
			int srcAddr = img->tbrow[i]+j;
			int y = SafeMod(i-rows/2, rows);
			int x = SafeMod(j-cols/2, cols);
			int dstAddr = result->tbrow[y] + x;

			result->val[dstAddr] = img->val[srcAddr];
		}
	}
    return result;
}


