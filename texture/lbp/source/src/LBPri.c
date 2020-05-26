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
#include "LBPri.h"

inline uchar ComputeNorm(float value)
{
  return((uchar)(255. * value));
}

Histogram *ComputeAccHistogram (Image *img)
{
    int i;
    Histogram *h = NULL;
    ulong *acc = AllocULongArray(SIZE);

    //histogram
    for(i=0; i < img->nrows*img->ncols; i++)
        acc[img->val[i]]+=1;

    //cumulated histogram
    for(i=1; i < SIZE; i++)
        acc[i]+=acc[i-1];

    //normalized cumulated histogram
    h = CreateHistogram(SIZE);

    for(i=0; i < SIZE; i++)
        h->v[i] = (uchar) ComputeNorm((float) acc[i] / (float) acc[SIZE-1]);

    free(acc);

    return(h);
}

int getLbpVal(Image *img, int i, int j)
{
    int lbpval = 0;
    int changes = 0;
    int k;

    // 0   1   2
    // 7   gc  3
    // 6   5   4

    int gc = img->val[img->tbrow[i]+j];
    int g[8];

    //no interpolation to find neighbors that are not exactly in the center of the pixel
    g[0] = (img->val[ img->tbrow[i-1] +j-1] >= gc);
    g[1] = (img->val[ img->tbrow[i-1] +j  ] >= gc);
    g[2] = (img->val[ img->tbrow[i-1] +j+1] >= gc);
    g[3] = (img->val[ img->tbrow[i]   +j+1] >= gc);
    g[4] = (img->val[ img->tbrow[i+1] +j+1] >= gc);
    g[5] = (img->val[ img->tbrow[i+1] +j  ] >= gc);
    g[6] = (img->val[ img->tbrow[i+1] +j-1] >= gc);
    g[7] = (img->val[ img->tbrow[i]   +j-1] >= gc);

    //computes the number of transitions 0/1 or 1/0
    for(k=0;k<7;k++)
        changes += (g[k]!=g[k+1]);
    changes += (g[7]!=g[0]);

    //if there are 2 or less transitions, cumulates the g's
    if (changes<=2)
        for (k=0;k<8;k++)
            lbpval += g[k];
    else lbpval = SIZE-1; //otherwise, uses P+1

    return (lbpval);
}

Histogram *LBPri(Image *img)
{
  Histogram *histogram = NULL;
  int i,j;
  Image *lbpRep;
  int lbpval;
  
  lbpRep = CreateImage(img->ncols-2, img->nrows-2);
  
	
  for( i = 1; i < img->nrows-1; i++ ){
	for( j = 1; j < img->ncols-1; j++ ){
		lbpval = getLbpVal(img, i, j);
		lbpRep->val[lbpRep->tbrow[i-1]+j-1] = lbpval;
	}
  }

  histogram = ComputeAccHistogram(lbpRep);
  DestroyImage(&lbpRep);
  
  return(histogram);
}

float min(float x, float y, float z) {

    if ( (x<=y) && (x<=z) ) {
        return x;
    } else if ( (y<=x) && (y<=z) ) {
        return y;
    } else if ( (z<=x) && (z<=y) ) {
        return z;
    }
    return -1;
}

float max(float x, float y, float z) {

    if ( (x>=y) && (x>=z) ) {
        return x;
    } else if ( (y>=x) && (y>=z) ) {
        return y;
    } else if ( (z>=x) && (z>=y) ) {
        return z;
    }
    return -1;
}

void RGB2HSV_lbp(CImage *RGB, ImageHSV **HSV) {

    float r, g, b;
    float minVal, maxVal, delta;
    int i, j;

    for (i = 0; i < RGB->C[0]->nrows; i++) {
        for (j = 0; j < RGB->C[0]->ncols; j++) {

            //normalizes between 0 and 1
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

            //normalizes S and V from 0 to 255
            HSV[i][j].S *= 255;
            HSV[i][j].V *= 255;

        }
    }
}
