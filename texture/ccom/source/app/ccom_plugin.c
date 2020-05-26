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
#include "libdescriptors.h"

void Extraction(char *img_path, char *fv_path)
{
    CImage *cimg=NULL;
    Image *Qimg=NULL;
    float ***W = NULL;
    FILE *arq = NULL;
    CCOFV *fv = NULL;

    cimg = ReadCImage(img_path);
    Qimg = QuantColors(cimg, COLORDIM);

    W = CreateW(Qimg, COLORDIM, DISTMAX); //creates co-occurrence matriz

    fv = makeCCOFV(W, COLORDIM, DISTMAX, AMIN, AMAX);//creates feature vector

    arq = fopen(fv_path,"wb");
	fwrite(&fv->size,sizeof(int),1,arq);
	fwrite(fv->FV,sizeof(CCO),fv->size,arq);

    //free memory
    DestroyCCOFV(&fv);
    fclose(arq);
    DestroyW(&W, COLORDIM, DISTMAX);
    DestroyCImage(&cimg);
    DestroyImage(&Qimg);
}


double Distance(char *fv1_path, char *fv2_path)
{
    CCOFV *fv1=NULL;
    CCOFV *fv2=NULL;
    float total1=0.0, total2=0.0;
    double distance=0.0;
    
    FILE *arq1=NULL, *arq2=NULL;
    int i=0;
    int size1=-1, size2=-1;
    int i1, i2;
    int has_intersection; //indicates if there is intersection between the vectors

    if ((arq1 = fopen(fv1_path, "rb")) == NULL) {
        printf("Error opening file 1\n");
        exit(0);
    }

    if ((arq2 = fopen(fv2_path, "rb")) == NULL) {
        printf("Error opening file 2\n");
        exit(0);
    }

	fread(&size1,sizeof(int),1,arq1);
	fread(&size2,sizeof(int),1,arq2);

    fv1 = (CCOFV *) calloc(1,sizeof(CCOFV));
    if(fv1 == NULL) {
        fprintf(stderr,"not enough memory\n");
        exit(1);
    }
    fv1->size = size1;  //copy value to struct
    fv1->FV = (CCO *) calloc(size1,sizeof(CCO));  //allocates only necessary space
    if(fv1->FV == NULL) {
        fprintf(stderr,"not enough memory\n");
        exit(1);
    }

    

    fv2 = (CCOFV *) calloc(1,sizeof(CCOFV));
    if(fv2 == NULL) {
        fprintf(stderr,"not enough memory\n");
        exit(1);
    }
    fv2->size = size2;  //copy value to struct
    fv2->FV = (CCO *) calloc(size2,sizeof(CCO));  //allocates only necessary space
    if(fv2->FV == NULL) {
        fprintf(stderr,"not enough memory\n");
        exit(1);
    }

    //stores the sum of each matrix
    total1 = 0;
    total2 = 0;
    //reading data from fv1
	fread(fv1->FV,sizeof(CCO),fv1->size,arq1);
    for(i=0;i<fv1->size;i++) {
        total1+=fv1->FV[i].w; //cumulate w values from fv1
    }

    //reading data from fv2
	fread(fv2->FV,sizeof(CCO),fv2->size,arq2);
    for(i=0;i<fv2->size;i++) {        
        total2+=fv2->FV[i].w; //cumulate w values from fv2
    }

    /********************************************************/
    if (fv1->size!=0 && fv2->size!=0) {
        has_intersection = 0;
        distance=0.0;
        i1=0; i2=0;
        do { //maximum path is to run the whole matrix

            if (fv1->FV[i1].i == fv2->FV[i2].i) { //if positions match
                distance += (double) fabsf(fv1->FV[i1].w - fv2->FV[i2].w);  //computes distance
                i1++;  //increments indices of both vectors
                i2++;
                has_intersection = 1;
            } else if (fv1->FV[i1].i<fv2->FV[i2].i) {
                distance += (double) fv1->FV[i1].w;
                i1++;  //if fv1 index is smaller than fv2, increments advances in fv1
            } else if (fv2->FV[i2].i<fv1->FV[i1].i) {
                distance += (double) fv2->FV[i2].w;
                i2++;  //if fv2 index is smaller than fv2, increments advances in fv2
            }
        } while (i1<fv1->size && i2<fv2->size);
        
        //keeps cumulating the distance with the rest of any of the two vectors
        while (i1<fv1->size) {
            distance += (double) fv1->FV[i1].w;
            i1++;
        }
        while (i2<fv2->size) {
            distance += (double) fv2->FV[i2].w;
            i2++;
        }        

        distance = distance/(total1 + total2); //normalizes distance

    } else {
        //when one of the vectors is empty
        distance = MAX_VAL_DIST;
    }

    //free memory
    fclose(arq1);
    fclose(arq2);
    DestroyCCOFV(&fv1);
    DestroyCCOFV(&fv2);

    return distance;
}
