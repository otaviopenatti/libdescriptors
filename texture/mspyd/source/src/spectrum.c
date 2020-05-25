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
#include "spectrum.h"
#include <stdio.h>

int FFT(int dir, long nn, double *x, double *y)
{
/*
   This computes an in-place complex-to-complex FFT
   x and y are the real and imaginary arrays of nn points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform
*/

   int m;
   long i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   m = (int)(log(nn)/log(2)+.00001);



   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i=0;i<nn-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0;
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1;
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1)
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for reverse transform */
   if (dir == -1) {
      for (i=0;i<nn;i++) {
         x[i] /= (double)nn;
         y[i] /= (double)nn;
      }
   }

   return(0);
}

Spectrum *FFT2D(Image *img)
{
  Spectrum *spec;
  int ncols, nrows, i, j;
  double *real, *imag;

  /* Enlarge ncols and nrows to the nearest power of 2 */
  ncols = (int)ceil(log(img->ncols)/log(2));
  nrows = (int)ceil(log(img->nrows)/log(2));
  ncols = 1 << ncols;
  nrows = 1 << nrows;

  spec = CreateSpectrum(ncols, nrows);

  /* Transform the rows */
  real = (double*)malloc(sizeof(double)*spec->ncols);
  imag = (double*)malloc(sizeof(double)*spec->ncols);
  if (real == NULL || imag == NULL)
    Error(MSG1, "FFT2D");
  for (i=0; i<spec->nrows; i++)
  {
    for (j=0; j<spec->ncols; j++)
    {
      if (i<img->nrows && j<img->ncols)
        real[j] = (double)img->val[img->tbrow[i]+j];
      else
        real[j] = 0.0;
      imag[j] = 0.0;
    }
    FFT(1, (long)spec->ncols, real, imag);
    for (j=0; j<spec->ncols; j++)
    {
      spec->real[spec->tbrow[i]+j] = real[j];
      spec->imag[spec->tbrow[i]+j] = imag[j];
    }
  }
  free(real);
  free(imag);

  /* Transform the cols */
  real = (double*)malloc(sizeof(double)*spec->nrows);
  imag = (double*)malloc(sizeof(double)*spec->nrows);
  if (real == NULL || imag == NULL)
    Error(MSG1, "FFT2D");
  for (i=0; i<spec->ncols; i++)
  {
    for (j=0; j<spec->nrows; j++)
    {
      real[j] = spec->real[spec->tbrow[j]+i];
      imag[j] = spec->imag[spec->tbrow[j]+i];
    }
    FFT(1, (long)spec->nrows, real, imag);
    for (j=0; j<spec->nrows; j++)
    {
      spec->real[spec->tbrow[j]+i] = real[j];
      spec->imag[spec->tbrow[j]+i] = imag[j];
    }
  }
  free(real);
  free(imag);

  return(spec);
}


Image *InvFFT2D(Spectrum *spec)
{
  Image *img;
  int i, j;
  double *real, *imag;
  Spectrum *specaux;

  img = CreateImage(spec->ncols, spec->nrows);
  specaux = CreateSpectrum(spec->ncols, spec->nrows);

  /* Transform the rows */
  real = (double*)malloc(sizeof(double)*spec->ncols);
  imag = (double*)malloc(sizeof(double)*spec->ncols);
  if (real == NULL || imag == NULL)
    Error(MSG1, "InvFFT2D");
  for (i=0; i<spec->nrows; i++)
  {
    for (j=0; j<spec->ncols; j++)
    {
      real[j] = spec->real[spec->tbrow[i]+j];
      imag[j] = spec->imag[spec->tbrow[i]+j];
    }
    FFT(-1, (long)spec->ncols, real, imag);
    for (j=0; j<spec->ncols; j++)
    {
      specaux->real[specaux->tbrow[i]+j] = real[j];
      specaux->imag[specaux->tbrow[i]+j] = imag[j];
    }
  }
  free(real);
  free(imag);

  /* Transform the cols */
  real = (double*)malloc(sizeof(double)*specaux->nrows);
  imag = (double*)malloc(sizeof(double)*specaux->nrows);
  if (real == NULL || imag == NULL)
    Error(MSG1, "InvFFT2D");
  for (i=0; i<specaux->ncols; i++)
  {
    for (j=0; j<specaux->nrows; j++)
    {
      real[j] = specaux->real[specaux->tbrow[j]+i];
      imag[j] = specaux->imag[specaux->tbrow[j]+i];
    }
    FFT(-1, (long)specaux->nrows, real, imag);
    for (j=0; j<specaux->nrows; j++)
    {
      img->val[img->tbrow[j]+i] = (int)real[j];
    }
  }
  free(real);
  free(imag);

  DestroySpectrum(&specaux);

  return(img);
}

DImage *DInvFFT2D(Spectrum *spec)
{
  DImage *img;
  int i, j;
  double *real, *imag;
  Spectrum *specaux;

  img = CreateDImage(spec->ncols, spec->nrows);
  specaux = CreateSpectrum(spec->ncols, spec->nrows);

  /* Transform the rows */
  real = (double*)malloc(sizeof(double)*spec->ncols);
  imag = (double*)malloc(sizeof(double)*spec->ncols);
  if (real == NULL || imag == NULL)
    Error(MSG1, "InvFFT2D");
  for (i=0; i<spec->nrows; i++)
  {
    for (j=0; j<spec->ncols; j++)
    {
      real[j] = spec->real[spec->tbrow[i]+j];
      imag[j] = spec->imag[spec->tbrow[i]+j];
    }
    FFT(-1, (long)spec->ncols, real, imag);
    for (j=0; j<spec->ncols; j++)
    {
      specaux->real[specaux->tbrow[i]+j] = real[j];
      specaux->imag[specaux->tbrow[i]+j] = imag[j];
    }
  }
  free(real);
  free(imag);

  /* Transform the cols */
  real = (double*)malloc(sizeof(double)*specaux->nrows);
  imag = (double*)malloc(sizeof(double)*specaux->nrows);
  if (real == NULL || imag == NULL)
    Error(MSG1, "InvFFT2D");
  for (i=0; i<specaux->ncols; i++)
  {
    for (j=0; j<specaux->nrows; j++)
    {
      real[j] = specaux->real[specaux->tbrow[j]+i];
      imag[j] = specaux->imag[specaux->tbrow[j]+i];
    }
    FFT(-1, (long)specaux->nrows, real, imag);
    for (j=0; j<specaux->nrows; j++)
    {
      img->val[img->tbrow[j]+i] = sqrt(real[j]*real[j] + imag[j]*imag[j]);
    }
  }
  free(real);
  free(imag);

  DestroySpectrum(&specaux);

  return(img);
}

Spectrum *CreateSpectrum(int ncols, int nrows)
{
  Spectrum *spec=NULL;
  int i;

  spec = (Spectrum *) calloc(1, sizeof(Spectrum));
  if (spec == NULL) {
    Error(MSG1, "CreateSpectrum");
  }

  spec->real  = AllocDoubleArray(nrows*ncols);
  spec->imag  = AllocDoubleArray(nrows*ncols);
  spec->tbrow = AllocIntArray(nrows);
  spec->tbslice=NULL;

  for (i=0; i<nrows; i++)
    spec->tbrow[i]=i*ncols;
  spec->ncols = ncols;
  spec->nrows = nrows;
  spec->nslices=0;

  return(spec);
}


void DestroySpectrum(Spectrum **spec)
{
  Spectrum *aux;

  aux = *spec;
  if (aux != NULL) {
    if (aux->real != NULL)  free(aux->real);
    if (aux->imag != NULL)  free(aux->imag);
    if (aux->tbrow != NULL) free(aux->tbrow);
    free(aux);
    *spec = NULL;
  }
}


Image *ViewMagnitude(Spectrum *spec)
{
  Image *img;
  Spectrum *specaux;
  int i, j, k, r, c;
  double max, n;

  max = (float)(INT_MIN);
  c = spec->ncols;
  r = spec->nrows;
  img = CreateImage(c, r);
  specaux = CreateSpectrum(c, r);

  /* Calculate the magnitude -> log(1+sqrt(real^2+imag^2))
     and center the image */
  for (i=0; i<r; i++) {
    for (j=0; j<c; j++) {
      k = img->tbrow[i]+j;
      specaux->real[specaux->tbrow[(i+r/2)%r] + (j+c/2)%c] = n =
        log(1+sqrt(spec->real[k]*spec->real[k] + spec->imag[k]*spec->imag[k]));
      if (n > max)
        max = n;
    }
  }

  /* Normalize the image to 255 colors */
  n = 255.0/max;
  c *= r;
  for (i=0; i<c; i++)
    img->val[i] = (int)(n*specaux->real[i]);

  DestroySpectrum(&specaux);

  return(img);
}


Image *ViewPhase(Spectrum *spec)
{
  Image *img;
  Spectrum *specaux;
  int i, j, k, r, c;
  double max, min, n;

  max = (float)INT_MIN;
  min = (float)INT_MAX;
  c = spec->ncols;
  r = spec->nrows;
  img = CreateImage(c, r);
  specaux = CreateSpectrum(c, r);

  /* Calculate the phase -> atan(imag/real)
     and center the image */
  for (i=0; i<r; i++) {
    for (j=0; j<c; j++) {
      k = img->tbrow[i]+j;
      specaux->real[specaux->tbrow[(i+r/2)%r] + (j+c/2)%c] = n =
	atan(spec->imag[k]/spec->real[k]);
      if (n > max)
        max = n;
      if (n < min)
        min = n;
    }
  }

  /* Normalize the image to 255 colors */
  n = (255.)/(max-min);
  c *= r;
  for (i=0; i<c; i++)
    img->val[i] = (int)(n*(specaux->real[i]-min));

  DestroySpectrum(&specaux);

  return(img);
}


Spectrum *SpectrumMagnitude(Spectrum *spec)
{
  Spectrum *specmag;
  int i, j, k, r, c;

  c = spec->ncols;
  r = spec->nrows;
  specmag = CreateSpectrum(c, r);

  /* Calculate the magnitude */

  for (i=0; i<r; i++) {
    for (j=0; j<c; j++) {
      k = specmag->tbrow[i]+j;
      specmag->real[k] = sqrt(spec->real[k]*spec->real[k] + spec->imag[k]*spec->imag[k]);
    }
  }

  return(specmag);
}

Spectrum *SpectrumPhase(Spectrum *spec)
{
  Spectrum *specphase;
  int i, j, k, r, c;

  c = spec->ncols;
  r = spec->nrows;
  specphase = CreateSpectrum(c, r);

  /* Calculate the phase */
  for (i=0; i<r; i++) {
    for (j=0; j<c; j++) {
      k = specphase->tbrow[i]+j;
      specphase->real[k] = atan(spec->imag[k]/spec->real[k]);
    }
  }

  return(specphase);
}

Spectrum *MultSpectrum(Spectrum *spec1, Spectrum *spec2)
{
  Spectrum *spec3;
  int p,n;

  spec3=CreateSpectrum(spec1->ncols,spec1->nrows);
  n = spec1->ncols*spec1->nrows;
  for (p=0; p < n; p++) {
    spec3->real[p] = (spec1->real[p]*spec2->real[p]) - (spec1->imag[p]*spec2->imag[p]);
    spec3->imag[p] = (spec1->real[p]*spec2->imag[p]) + (spec1->imag[p]*spec2->real[p]);
  }
  return(spec3);
}

// Create model for motion degradation at constant object speed
// (Vx,Vy) during exposure time T.

Spectrum *MotionDegradation(int ncols, int nrows, float Vx, float Vy, float T)
{
  Spectrum *filter=CreateSpectrum(ncols,nrows);
  int u,v,p,q;
  float K;
  Spectrum *rebatedfilter=CreateSpectrum(ncols,nrows);

  // generate filter with origin at the center of the image.

  for (v=0; v < nrows; v++)
    for (u=0; u < ncols; u++){
      p = u + filter->tbrow[v];
      K = PI*(((float)u-ncols/2.0)*Vx + ((float)v-nrows/2.0)*Vy);
      if (K==0){
	filter->real[p]=1.0;
      }else{
	if (sin(K*T) == 0.0)
	  filter->real[p]=0.0;
	else
	  filter->real[p]=sin(K*T)/K;
      }
      filter->imag[p]=0.0;
    }

  // rebate periodical spectrum to the origin of the image and invert
  // it

  for (v=0; v < nrows; v++)
    for (u=0; u < ncols; u++){
      p = u + filter->tbrow[v];

      if ((u<ncols/2)&&(v<nrows/2)){ //p is in Quadrant 2
	q = (u+ncols/2)+filter->tbrow[v+nrows/2]; // copy p to q in
						  // Quadrant 4
      }else{
	if ((u>=ncols/2)&&(v<nrows/2)){ //p is in Quadrant 1
	  q = (u-ncols/2)+filter->tbrow[v+nrows/2]; // copy p to q in
						    // Quadrant 3
	}else{
	  if ((u<ncols/2)&&(v>=nrows/2)){ //p is in Quadrant 3
	    q = (u+ncols/2)+filter->tbrow[v-nrows/2]; // copy p to q
						      // in Quadrant 1
	  }else{
	    if ((u>=ncols/2)&&(v>=nrows/2)){ //p is in Quadrant 4
	      q = (u-ncols/2)+filter->tbrow[v-nrows/2]; // copy p to q
							// in Quadrant
							// 2
	    }
	  }
	}
      }
      rebatedfilter->real[q]=1.0/(filter->real[p]+0.00001);
      rebatedfilter->imag[q]=0.0;
    }


  DestroySpectrum(&filter);

  return(rebatedfilter);
}

Spectrum *DFFT2D(DImage *img)
{
  Spectrum *spec;
  int ncols, nrows, i, j;
  double *real, *imag;

  /* Enlarge ncols and nrows to the nearest power of 2 */
  ncols = (int)ceil(log(img->ncols)/log(2));
  nrows = (int)ceil(log(img->nrows)/log(2));
  ncols = 1 << ncols;
  nrows = 1 << nrows;

  spec = CreateSpectrum(ncols, nrows);

  /* Transform the rows */
  real = (double*)malloc(sizeof(double)*spec->ncols);
  imag = (double*)malloc(sizeof(double)*spec->ncols);
  if (real == NULL || imag == NULL)
    Error(MSG1, "FFT2D");
  for (i=0; i<spec->nrows; i++)
  {
    for (j=0; j<spec->ncols; j++)
    {
      if (i<img->nrows && j<img->ncols)
        real[j] = img->val[img->tbrow[i]+j];
      else
        real[j] = 0.0;
      imag[j] = 0.0;
    }
    FFT(1, (long)spec->ncols, real, imag);
    for (j=0; j<spec->ncols; j++)
    {
      spec->real[spec->tbrow[i]+j] = real[j];
      spec->imag[spec->tbrow[i]+j] = imag[j];
    }
  }
  free(real);
  free(imag);

  /* Transform the cols */
  real = (double*)malloc(sizeof(double)*spec->nrows);
  imag = (double*)malloc(sizeof(double)*spec->nrows);
  if (real == NULL || imag == NULL)
    Error(MSG1, "FFT2D");
  for (i=0; i<spec->ncols; i++)
  {
    for (j=0; j<spec->nrows; j++)
    {
      real[j] = spec->real[spec->tbrow[j]+i];
      imag[j] = spec->imag[spec->tbrow[j]+i];
    }
    FFT(1, (long)spec->nrows, real, imag);
    for (j=0; j<spec->nrows; j++)
    {
      spec->real[spec->tbrow[j]+i] = real[j];
      spec->imag[spec->tbrow[j]+i] = imag[j];
    }
  }
  free(real);
  free(imag);

  return(spec);
}


Spectrum *DFFT2DWithImag(DImage *realimg, DImage*  imagimg)
{
  Spectrum *spec;
  int ncols, nrows, i, j;
  double *real, *imag;

  /* Enlarge ncols and nrows to the nearest power of 2 */
  ncols = (int)ceil(log(realimg->ncols)/log(2));
  nrows = (int)ceil(log(realimg->nrows)/log(2));
  ncols = 1 << ncols;
  nrows = 1 << nrows;

  spec = CreateSpectrum(ncols, nrows);

  /* Transform the rows */
  real = (double*)malloc(sizeof(double)*spec->ncols);
  imag = (double*)malloc(sizeof(double)*spec->ncols);
  if (real == NULL || imag == NULL)
    Error(MSG1, "FFT2D");
  for (i=0; i<spec->nrows; i++)
  {
    for (j=0; j<spec->ncols; j++)
    {
      if (i<realimg->nrows && j<realimg->ncols)
      {
        real[j] = realimg->val[realimg->tbrow[i]+j];
        imag[j] = imagimg->val[imagimg->tbrow[i]+j];
      }else{
        real[j] = 0.0;
      	imag[j] = 0.0;
	  	}
    }
    FFT(1, (long)spec->ncols, real, imag);
    for (j=0; j<spec->ncols; j++)
    {
      spec->real[spec->tbrow[i]+j] = real[j];
      spec->imag[spec->tbrow[i]+j] = imag[j];
    }
  }
  free(real);
  free(imag);

  /* Transform the cols */
  real = (double*)malloc(sizeof(double)*spec->nrows);
  imag = (double*)malloc(sizeof(double)*spec->nrows);
  if (real == NULL || imag == NULL)
    Error(MSG1, "FFT2D");
  for (i=0; i<spec->ncols; i++)
  {
    for (j=0; j<spec->nrows; j++)
    {
      real[j] = spec->real[spec->tbrow[j]+i];
      imag[j] = spec->imag[spec->tbrow[j]+i];
    }
    FFT(1, (long)spec->nrows, real, imag);
    for (j=0; j<spec->nrows; j++)
    {
      spec->real[spec->tbrow[j]+i] = real[j];
      spec->imag[spec->tbrow[j]+i] = imag[j];
    }
  }
  free(real);
  free(imag);

  return(spec);
}


Spectrum* FFTShift(Spectrum* spec)
{
	int cols = spec->ncols;
	int rows = spec->nrows;
	int i,j;

	Spectrum *result = CreateSpectrum(cols, rows);

	for ( i=0; i<rows; i++)
	{
		for ( j=0; j<cols; j++)
		{
			int srcAddr = spec->tbrow[i]+j;
			int y = SafeMod(i-rows/2, rows);
			int x = SafeMod(j-cols/2, cols);
			int dstAddr = result->tbrow[y] + x;

			result->real[dstAddr] = spec->real[srcAddr];
			result->imag[dstAddr] = spec->imag[srcAddr];
		}
	}
    return result;
}

Spectrum* ApplyFilter(Spectrum *imgspec, Spectrum *filter)
{
	Spectrum *filterSpec = FFTShift(filter);

	Spectrum *result = MultSpectrum(imgspec, filterSpec);

	DestroySpectrum(&filterSpec);

	return result;
}


/**-----------------------------------------------------------------*/
