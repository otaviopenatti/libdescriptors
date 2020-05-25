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
#include "las.h"

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

void RGB2HSV_las(CImage *RGB, ImageHSV **HSV) {

    float r, g, b;
    float minVal, maxVal, delta;
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

            //normalizes S and V to 0 and 255
            HSV[i][j].S *= 255;
            HSV[i][j].V *= 255;

        }
    }
}

void CalculaGradientes(Image *img, Image *g1, Image *g2, Image *g3, Image *g4) {

    int i,j;
    int va, vb; //variables to store values from neighboring pixels (depending on the direction)
                //makes it easier when it goes out of image limits
    int val_ij;

    //scans the image computing g1, g2, g3 e g4
    for (i = 0; i < img->nrows; i++) {
        for (j = 0; j < img->ncols; j++) {

            val_ij = img->val[j+img->tbrow[i]]; //copies to a variable the value of the current pixel
                                                //this avoids having to access the image all the time

            //considers that pixels outside the image have ZERO value

            //g1  -- pixels on the diagonal (scanning down from left to right)
            if (ValidPixel(img, j-1, i-1)) va=img->val[j-1+img->tbrow[i-1]];
            else va=0;

            if (ValidPixel(img, j+1, i+1)) vb=img->val[j+1+img->tbrow[i+1]];
            else vb=0;

            g1->val[j+g1->tbrow[i]] = abs(va - val_ij) + abs(val_ij - vb);


            //g2  -- pixels from the sides
            if (ValidPixel(img, j-1, i)) va=img->val[j-1+img->tbrow[i]];
            else va=0;

            if (ValidPixel(img, j+1, i)) vb=img->val[j+1+img->tbrow[i]];
            else vb=0;

            g2->val[j+g2->tbrow[i]] = abs(va - val_ij) + abs(val_ij - vb);


            //g3  -- pixels on the diagonal (scanning up from left to right)
            if (ValidPixel(img, j-1, i+1)) va=img->val[j-1+img->tbrow[i+1]];
            else va=0;

            if (ValidPixel(img, j+1, i-1)) vb=img->val[j+1+img->tbrow[i-1]];
            else vb=0;

            g3->val[j+g3->tbrow[i]] = abs(va - val_ij) + abs(val_ij - vb);


            //g4  -- pixels below and above
            if (ValidPixel(img, j, i+1)) va=img->val[j+img->tbrow[i+1]];
            else va=0;

            if (ValidPixel(img, j, i-1)) vb=img->val[j+img->tbrow[i-1]];
            else vb=0;

            g4->val[j+g4->tbrow[i]] = abs(va - val_ij) + abs(val_ij - vb);

        }
    }
}

int Intervalo(int val) {
    //non-uniform quantization of the 511 gradient values
    int lim1=25, lim2=50, lim3=75;

    if (val>=0 && val<lim1) {
        return 0;
    } else if (val>=lim1 && val<lim2) {
        return 1;
    } else if (val>=lim2 && val<lim3) {
        return 2;
    } else if (val>=lim3 && val<511) {
        return 3;
    } else {
        //printf("INVALID VALUE!");
        exit(1);
    }
    return -1;
}

float *HistogramaGradiente(Image *g1, Image *g2, Image *g3, Image *g4) {
    int i;
    float *hist=NULL;
    int bin=0; //bin to be incremented

    hist = (float*) calloc(QTD_BINS*QTD_BINS*QTD_BINS*QTD_BINS, sizeof(float));


    //scans the gradient maps (all have the same size)
    for (i = 0; i < g1->nrows*g1->ncols; i++) {
        bin = Intervalo(g1->val[i]) +
              (Intervalo(g2->val[i])*QTD_BINS) +
              (Intervalo(g3->val[i])*QTD_BINS*QTD_BINS) +
              (Intervalo(g4->val[i])*QTD_BINS*QTD_BINS*QTD_BINS);
        hist[bin]++; //increments the corresponding position
    }
    return hist;
}

void NormalizaHistograma(float *hist, int hist_size, int qtd_pixels) {
    int i;
    for (i=0; i<hist_size; i++) {
        hist[i] = hist[i]/qtd_pixels; //divides each bin by the image size
    }

}

/***************************************************************/
double *ReadFileDouble(char *filename, int size) {
    FILE *fp;
    double *fv=NULL;

    fv = AllocDoubleArray(size);

    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("ERROR OPENING FEATURE VECTOR FILE %s!\n", filename);
        exit(0);
    }
    fread(fv, sizeof(double), size, fp);
    fclose(fp);

    return fv;
}

float *ReadFileFloat(char *filename, int size) {
    FILE *fp;
    float *fv=NULL;

    fv = AllocFloatArray(size);

    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("ERROR OPENING FEATURE VECTOR FILE %s!\n", filename);
        exit(0);
    }
    fread(fv, sizeof(float), size, fp);
    fclose(fp);

    return fv;
}

void WriteFileDouble(char *filename, double *vet, int size) {
    FILE *fout;

    if ((fout = fopen(filename, "wb")) == NULL) {
        printf("ERROR CREATING FEATURE VECTOR FILE!\n");
        exit(0);
    }
    fwrite(vet, sizeof(double), size, fout);
    fclose(fout);
}

void WriteFileFloat(char *filename, float *vet, int size) {
    FILE *fout;

    if ((fout = fopen(filename, "wb")) == NULL) {
        printf("ERROR CREATING FEATURE VECTOR FILE!\n");
        exit(0);
    }
    fwrite(vet, sizeof(float), size, fout);
    fclose(fout);
}

double L2DoubleDistance(double *v1, double *v2, int size) {
    int i;
    double d=0.0;

    for (i=0; i<size; i++) {
        d += pow((v1[i]-v2[i]), 2);
    }
    return sqrt(d);
}

double L1FloatDistance(float *v1, float *v2, int size) {
    int i;
    double d=0.0;

    for (i=0; i<size; i++) {
        d += (double)fabsf(v1[i]-v2[i]);
    }
    return d;
}
