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
#include "ccom.h"

Image *QuantColors(CImage *cimg, int color_dim)
{
    //function to quantize an input color image (in RGB space) into color_dim levels
    ulong r, g, b;
    ulong fator_g, fator_b;
    int i,n;
    Image *Qimg = NULL; 

    Qimg = CreateImage(cimg->C[0]->ncols, cimg->C[0]->nrows);

    n = cimg->C[0]->nrows * cimg->C[0]->ncols;

    //values for the quantization
    fator_g = color_dim;
    fator_b = fator_g*color_dim;

    for(i=0; i<n; i++){ //quantization
        r = color_dim*cimg->C[0]->val[i]/256;
        g = color_dim*cimg->C[1]->val[i]/256;
        b = color_dim*cimg->C[2]->val[i]/256;

        Qimg->val[i] = (r + fator_g*g + fator_b*b);
    }
    return(Qimg);
}

float ***MakeW(int size, int max_dist )
{
    //create co-occurrence matrix according to size and max_dist (maximum distance between pixels)
    int i = 0, j = 0;
    float ***W = NULL;

    W = (float ***) calloc(max_dist, sizeof(float **));

    if(W == NULL) {
        fprintf(stderr,"ERROR: MakeW@1\n");
        exit(1);
    }

    for(i=0;i<max_dist;i++) { //matrix from zero to max_dist-1
        W[i] = (float **) calloc(size, sizeof(float *));
        if(W[i] == NULL) {
            fprintf(stderr,"ERROR: MakeW@2\n");
            exit(1);
        }
    }

    for(j=0;j<max_dist;j++) {
        for(i=0;i<size;i++) {
            W[j][i] = (float *) calloc(size, sizeof(float));
            if(W[j][i] == NULL) {
                fprintf(stderr,"ERROR: MakeW@3\n");
                exit(1);
            }
        }
    }
    return W;
}

float ***CreateW(Image *Qimg, int color_dim, int max_dist)
{
    // create co-occurence matrix from quantized image, number of colors (color_dim), and max_dist (maximum distance between pixels)

    float ***W=NULL;
    int i=0,j=0, d=0;
    int size=color_dim*color_dim*color_dim;
    int vizImax=0, vizJmax=0, vizImin=0, vizJmin=0, vizI=0, vizJ=0, dist=0, tam=0;

    tam = size*size*max_dist;
    W = MakeW(size,max_dist);

    //scnas the image max_dist times
    for(d=0;d<max_dist;d++) {
        for (i = 0; i < Qimg->nrows; i++) {
            for (j = 0; j < Qimg->ncols; j++) {
                //defines the max neighboring window for pixel (i,j)*/
                if( (vizImin = i - (d+1)) < 0)
                    vizImin = 0;

                if( (vizImax = i + (d+1)) > Qimg->nrows - 1)
                    vizImax = Qimg->nrows - 1;

                if( (vizJmin = j - (d+1)) < 0)
                    vizJmin = 0;

                if( (vizJmax = j + (d+1)) > Qimg->ncols - 1)
                    vizJmax = Qimg->ncols - 1;

                //scans the window
                for (vizI = vizImin; vizI <= vizImax; vizI++) {
                    for (vizJ = vizJmin; vizJ <= vizJmax; vizJ++) {
                        dist = cityBlock(i, j, vizI, vizJ);  //check distance
                        if (dist == (d+1) ) {   //if dist == (d+1), belongs to the neighborhood in relation to d
                            W[d][ Qimg->val[j+Qimg->tbrow[i]] ][ Qimg->val[vizJ+Qimg->tbrow[vizI]] ]++;
                        }
                    }
                }
            }
        }
    }

    //normalized matrix values according to the original image size
    for(d=0;d<max_dist;d++) {
        for(i=0;i<size;i++) {
            for(j=0;j<size;j++) {
                W[d][i][j] = W[d][i][j]/(Qimg->nrows*Qimg->ncols);//W[d][i][j]/tam;
            }
        }
    }

    return W;
}

void DestroyW(float ****W, int color_dim, int max_dist)
{
    //release memory of the co-occurrence matrix
    
    int i = 0,j = 0, size = color_dim*color_dim*color_dim;

    if ((***W) != NULL) {

        for(j=0;j<max_dist;j++)
            for(i=0;i<size;i++)
                free((*W)[j][i]);

        for(i=0;i<max_dist;i++)
            free((*W)[i]);

        free(*W);

        *W = NULL;
    }
}


int cityBlock(int i1, int j1, int i2, int j2)
{
  //computes city-block distance for 2 pixels
  return abs(i1 - i2) + abs(j1 - j2);

}

CCOFV*  makeCCOFV(float ***W, int color_dim, int max_dist, float Amin, float Amax)
{
    //Creates the feature vector given the co-occurrence matrix W, quantization levels (color_dim), maximum distance between pixels (max_dist), 
    //keeping the the values between Amin and Amax

    int i=0,j=0, d=0, count=0, aux=0;
    int size=color_dim*color_dim*color_dim;
    CCOFV *fv = NULL;

    //counts values different than zero and (Amin,Amax]
    for(d=0;d<max_dist;d++)
        for(i=0;i<size;i++)
            for(j=0;j<size;j++)
                if(W[d][i][j] != 0 && W[d][i][j] > Amin && W[d][i][j] <= Amax)
                    count++;

    fv = (CCOFV *)malloc(sizeof(CCOFV));
    fv->size = count;
    fv->FV = (CCO *)malloc(count*sizeof(CCO));

    for(i=0;i<count;i++) { //initializes
        fv->FV[i].w = -1;
        fv->FV[i].i = -1;
    }

    aux=0;
    for(d=0;d<max_dist;d++) {
        for(i=0;i<size;i++) {
            for(j=0;j<size;j++) {
                if(W[d][i][j] != 0 && W[d][i][j] > Amin && W[d][i][j] <= Amax) {
                    fv->FV[aux].w = W[d][i][j]; //saves the value
                    fv->FV[aux].i = d*size*size + i*size + j;//saves the index
                    aux++;
                }
            }
        }
    }
	
    return fv;
}

void DestroyCCOFV(CCOFV **fv)
{
    if((*fv) != NULL) {
      free((*fv)->FV);
      free((*fv));
      (*fv) = NULL;
    }
}
