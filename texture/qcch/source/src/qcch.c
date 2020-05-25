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
#include "qcch.h"

Image *RGB2Cinza(CImage *image)
{
    int i;
    Image *cinza=NULL;
    double aux=0;
    int img_size = (image->C[0]->ncols*image->C[0]->nrows);

    //creates gray image
    cinza = CreateImage(image->C[0]->ncols,image->C[0]->nrows);

    //converts using the equation: 0.299*R + 0.587*G + 0.114*B
    for (i=0; i < img_size; i++) {
        aux = ( ((image->C[0]->val[i]) * 0.299)+((image->C[1]->val[i])*0.587) + ((image->C[2]->val[i])*0.114));
        cinza->val[i]= (int)aux;
    }

    return(cinza);
}

long int SomaJanela(int centroI, int centroJ, int raio, Image *img) {
    int k, m;
    long int y=0;

    for (k=-raio; k<=raio; k++) {
        for (m=-raio; m<=raio; m++) {
            if ( ValidPixel(img,(centroI+k),(centroJ+m)) ) { //checks if the point is valid
                y += img->val[(centroI+k)+img->tbrow[(centroJ+m)]];
            }
        }
    }
    return y;
}

//image = original gray image
//t     = resulting image, already quantized in 40 values
//r     = radio used for the window size
void Rate_Change_Gray(Image *image, Image *t, int r)
{
    int i,j,col=0,row=0;
    float hr, vr, dr, ar;  //directional rate change
    float vt=0.0;
    float y1, y2;

    col = (image->ncols);
    row = (image->nrows);

    for (j = r; j < row-r; j++) {
        for (i = r; i < col-r; i++) {

            //Computes Hr - horizontal
            y1 = (float) SomaJanela(i-r, j, r, image)/QTD_VIZ; //computes average gray value in i-r,j
            y2 = (float) SomaJanela(i+r, j, r, image)/QTD_VIZ; //computes average gray value in i+r,j
            hr =  fabsf(y1-y2);

            //Computes Vr - vertical
            y1 = (float) SomaJanela(i, j-r, r, image)/QTD_VIZ; //computes average gray value in i,j-r
            y2 = (float) SomaJanela(i, j+r, r, image)/QTD_VIZ; //computes average gray value in i,j+r
            vr = fabsf(y1-y2);

            //Computes Dr - diagonal
            y1 = (float) SomaJanela(i-r, j-r, r, image)/QTD_VIZ;  //computes average gray value in i-r,j-r
            y2 = (float) SomaJanela(i+r, j+r, r, image)/QTD_VIZ;  //computes average gray value in i+r,j+r
            dr = fabsf(y1-y2);

            //Computes Ar - anti-diagonal
            y1 = (float) SomaJanela(i+r, j-r, r, image)/QTD_VIZ;  //computes average gray value in i+r,j-r
            y2 = (float) SomaJanela(i-r, j+r, r, image)/QTD_VIZ;  //computes average gray value in i-r,j+r
            ar = fabsf(y1-y2);

            //vt is the average of the values
            vt = (hr + vr + dr + ar)/4.0;

            if (vt>255) printf("vt>255 -> %f\t hr=%f\t vr=%f\t dr=%f\t ar=%f\n", vt, hr, vr, dr, ar);

            //non-uniform quantization
               if( 0.0 < vt && vt <= 1.0 ){ (t->val[(i)+(t->tbrow[j])])=0; }
               else
               if( 1.0 < vt && vt <= 2.0 ){ (t->val[(i)+(t->tbrow[j])])=1; }
               else
               if( 2.0 < vt && vt <= 3.0 ){ (t->val[(i)+(t->tbrow[j])])=2; }
               else
               if( 3.0 < vt && vt <= 4.0 ){ (t->val[(i)+(t->tbrow[j])])=3; }
               else
               if( 4.0 < vt && vt <= 5.0 ){ (t->val[(i)+(t->tbrow[j])])=4; }
               else                       
               if( 5.0 < vt && vt <= 6.0 ){ (t->val[(i)+(t->tbrow[j])])=5; }
               else 
               if( 6.0 < vt && vt <= 7.0 ){ (t->val[(i)+(t->tbrow[j])])=6; }
               else
               if( 7.0 < vt && vt <= 8.0 ){ (t->val[(i)+(t->tbrow[j])])=7; }
               else
               if( 8.0 < vt && vt <= 9.0 ){ (t->val[(i)+(t->tbrow[j])])=8; }
               else
               if( 9.0 < vt && vt <= 10.0 ){ (t->val[(i)+(t->tbrow[j])])=9; }
               else            
               if( 10.0 < vt && vt <= 11.0 ){ (t->val[(i)+(t->tbrow[j])])=10; }
               else
               if( 11.0 < vt && vt <= 12.0 ){ (t->val[(i)+(t->tbrow[j])])=11; }
               else
               if( 12.0 < vt && vt <= 13.0 ){ (t->val[(i)+(t->tbrow[j])])=12; }
               else
               if( 13.0 < vt && vt <= 14.0 ){ (t->val[(i)+(t->tbrow[j])])=13; }
               else
               if( 14.0 < vt && vt <= 15.0 ){ (t->val[(i)+(t->tbrow[j])])=14; }
               else                       
               if( 15.0 < vt && vt <= 15.5 ){ (t->val[(i)+(t->tbrow[j])])=15; }
               else               
               if( 15.5 < vt && vt <= 17.5 ){ (t->val[(i)+(t->tbrow[j])])=16; }
               else
               if( 17.5 < vt && vt <= 19.5 ){ (t->val[(i)+(t->tbrow[j])])=17; }
               else
               if( 19.5 < vt && vt <= 21.5 ){ (t->val[(i)+(t->tbrow[j])])=18;  }
               else
               if( 21.5 < vt && vt <= 23.5 ){ (t->val[(i)+(t->tbrow[j])])=19; }
               else
               if( 23.5 < vt && vt <= 25.5 ){ (t->val[(i)+(t->tbrow[j])])=20; }
               else                       
               if( 25.5 < vt && vt <= 27.5 ){ (t->val[(i)+(t->tbrow[j])])=21; }
               else 
               if( 27.5 < vt && vt <= 29.5 ){ (t->val[(i)+(t->tbrow[j])])=22; }
               else
               if( 29.5 < vt && vt <= 31.5 ){ (t->val[(i)+(t->tbrow[j])])=23; }
               else
               if( 31.5 < vt && vt <= 33.5 ){ (t->val[(i)+(t->tbrow[j])])=24; }
               else
               if( 33.5 < vt && vt <= 35.5 ){ (t->val[(i)+(t->tbrow[j])])=25; }
               else               
               if( 35.5 < vt && vt <= 40.5 ){ (t->val[(i)+(t->tbrow[j])])=26; }
               else
               if( 40.5 < vt && vt <= 45.5 ){ (t->val[(i)+(t->tbrow[j])])=27; }
               else
               if( 45.5 < vt && vt <= 50.5 ){ (t->val[(i)+(t->tbrow[j])])=28; }
               else
               if( 50.5 < vt && vt <= 55.5 ){ (t->val[(i)+(t->tbrow[j])])=29; }
               else
               if( 55.5 < vt && vt <= 60.5 ){ (t->val[(i)+(t->tbrow[j])])=30; }
               else                       
               if( 60.5 < vt && vt <= 65.5 ){ (t->val[(i)+(t->tbrow[j])])=31; }
               else 
               if( 65.5 < vt && vt <= 70.5 ){ (t->val[(i)+(t->tbrow[j])])=32; }
               else
               if( 70.5 < vt && vt <= 75.5 ){ (t->val[(i)+(t->tbrow[j])])=33; }
               else
               if( 75.5 < vt && vt <= 80.5 ){ (t->val[(i)+(t->tbrow[j])])=34; }
               else
               if( 80.5 < vt && vt <= 85.5 ){ (t->val[(i)+(t->tbrow[j])])=35; }
               else 
               if( 85.5 < vt && vt <= 95.5 ){ (t->val[(i)+(t->tbrow[j])])=36; }
               else
               if( 95.5 < vt && vt <= 105.5 ){ (t->val[(i)+(t->tbrow[j])])=37; }
               else 
               if( 105.5 < vt && vt <= 115.5 ){ (t->val[(i)+(t->tbrow[j])])=38; }
               else
               if( 115.5 < vt && vt <= 255 ){ (t->val[(i)+(t->tbrow[j])])=39; }   
        }
    }  
}

//Computes Normalized Quantized Compound Change Histogram
void Change_Histogram(Image *t, double h[], int r)
{
    int i,tam,k,j;
    int aux;

    //ignores 2r rows and 2r cols
    tam = ((t->ncols)-2*r) * ((t->nrows)-2*r);

    //for each one of the 40 values
    for(k=0; k<QTD_BINS; k++){ //h(t) where t= 0...39
        aux = 0;
        //scans the quantized image t
        for (i = r; i < ((t->nrows)-r); i++) {
            for (j = r; j < ((t->ncols)-r); j++) {
                //computes the sigma function:
                //  sigma(x)=1   if x=0,
                //  sigma(x)=0   otherwise
                //case of, x = t[i][j] - k
                if( ((t->val[(j)+(t->tbrow[i])]) - k ) == 0 ) {
                    aux+=1;
                }
            }
        }
        h[k] = (double)aux/tam;
    }
}

double *QCCH(CImage *cimg, int raio)
{
    Image *cinza=NULL;
    double *h=NULL; //feature vector

    Image *t = CreateImage(cimg->C[0]->ncols,cimg->C[0]->nrows);

    cinza = RGB2Cinza(cimg); //converts to gray

    h = (double*) calloc(QTD_BINS, sizeof(double));

    Rate_Change_Gray(cinza, t, raio); //computes t image with the neighboring pixel changes
                                      //t is already non-uniformly quantized into 40 bins (t->val[i] = [0,255])

    //Coputes the Normalized Quantized Compound Change Histogram
    Change_Histogram(t, h, raio);

    //free memory
    DestroyImage(&cinza);
    DestroyImage(&t);

    return h;
}

double Dist_Measure(double Ha[],double Hb[]) //array with QTD_BINS positions
{
    int i;
    double dab=0.0;

    for(i=0; i<QTD_BINS; i++) { //QTD_BINS bins
        dab+= fabs( Ha[i] - Hb[i] );
    }

    return dab;
}

/**********************************************************/
void ReadFile(char *filename, double h[])
{
    int i,n;
    FILE *fp;
    double c;

    fp = fopen(filename,"r");
    if (fp == NULL){
        fprintf(stderr,"Cannot open %s\n",filename);
        exit(-1);
    }
    fscanf(fp, "%d\n", &n);

    for (i=0; i < n; i++){
        fscanf(fp, "%lf\n", &c);
        h[i] = c; 
    }
    fclose(fp);
}

double *ReadFileBin(char *filename, int size) {
    FILE *fp;
    double *fv=NULL;

    fv = AllocDoubleArray(size);

    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(0);
    }
    fread(fv, sizeof(double), size, fp);
    fclose(fp);

    return fv;
}


void WriteFile(double h[], char *filename)
{
    FILE *fp;
    double *x;

    x=h;

    fp = fopen(filename,"w");
    if (fp == NULL){
        fprintf(stderr,"Cannot open %s\n",filename);
        exit(-1);
    }

    WriteStream(h,fp);
    fclose(fp);
}

void WriteStream(double *h, FILE *fp)
{
    int i;
    if (fp != NULL){
        fprintf(fp,"%d\n", QTD_BINS);
        for (i=0; i < QTD_BINS; i++) {
            fprintf(fp,"%f\n",h[i]);
        }
        fprintf(fp,"\n");
    }
}

void WriteFileBin(double *h, char *filename, int size) {
    FILE *fout;

    if ((fout = fopen(filename, "wb")) == NULL) {
        printf("Error creating feature vector file!\n");
        exit(0);
    }
    fwrite(h, sizeof(double), size, fout);
    fclose(fout);
}
