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
#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#include "image.h"
#include "dimage.h"

typedef struct _spectrum {
  double *real;
  double *imag;
  int ncols,nrows,nslices;
  int *tbrow;
  int *tbslice;
} Spectrum;


int       FFT(int dir, long nn, double *x, double *y);
Spectrum *FFT2D(Image *img);
Image    *InvFFT2D(Spectrum *spec);
DImage   *DInvFFT2D(Spectrum *spec);
Spectrum *CreateSpectrum(int ncols, int nrows);
void      DestroySpectrum(Spectrum **spec);
Image    *ViewMagnitude(Spectrum *spec);
Image    *ViewPhase(Spectrum *spec);
Spectrum *SpectrumMagnitude(Spectrum *spec);
Spectrum *SpectrumPhase(Spectrum *spec);
Spectrum *MultSpectrum(Spectrum *spec1, Spectrum *spec2);

// Create model of motion degradation due to object movement with
// constant speed (Vx,Vy) during exposure time T.
Spectrum *MotionDegradation(int ncols, int nrows, float Vx, float Vy, float T);

/** FFT2D using DImage as source **/
Spectrum *DFFT2D(DImage *img);

/** Does FFT2D using a real image and an imaginary image
 *  as source. Used in gabor.c because the filter was
 *  not created in frequency domain.
 **/
Spectrum *DFFT2DWithImag(DImage *realimg, DImage* imagimg);

/** Shifts the filter decentering it, so that it can be applied
 *  to the image.
 **/
Spectrum *FFTShift(Spectrum *spec);

/** Shifts the filter using FFTShift and multiplies
 *  the image by it.
 **/
Spectrum* ApplyFilter(Spectrum *imgspec, Spectrum *filter);

#endif

