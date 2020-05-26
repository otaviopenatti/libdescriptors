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

int main(int argc, char** argv)
{
    CImage *cimg=NULL;
    Image *img=NULL;
    ImageHSV **imgHSV; //image in HSV color space - same size of original image
    Image *g1=NULL, *g2=NULL, *g3=NULL, *g4=NULL; //image for the LAS gradients
    int i, j;
    float *hist=NULL;

    if (argc != 3) {
        fprintf(stderr,"Usage: las_extraction <img_path> <fv_path>\n");
        fprintf(stderr,"\timg_path: path to the image file in PPM (P6) format\n");
        fprintf(stderr,"\tfv_path:  path to the file that will store the feature vector\n");
        fprintf(stderr,"Remark:\n\tThis program extracts the feature vector from a single image.\n");
        exit(-1);
    }

    cimg = ReadCImage(argv[1]);

    //creates HSV image
    imgHSV = (ImageHSV**) calloc(cimg->C[0]->nrows, sizeof(ImageHSV*));
    for (i=0; i<cimg->C[0]->nrows; i++) {
        imgHSV[i] = (ImageHSV*) calloc(cimg->C[0]->ncols, sizeof(ImageHSV));
    }

    //converts to HSV
    RGB2HSV_las(cimg, imgHSV);
    //creates the gray image
    img = CreateImage(cimg->C[0]->ncols, cimg->C[0]->nrows);
    //copies V channel (from HSV) to the gray image
    for (i = 0; i < cimg->C[0]->nrows; i++) {
        for (j = 0; j < cimg->C[0]->ncols; j++) {
            img->val[j+img->tbrow[i]] = imgHSV[i][j].V;
        }
    }

    //computes g1, g2, g3, g4
    g1 = CreateImage(img->ncols, img->nrows);  //creates the images
    g2 = CreateImage(img->ncols, img->nrows);
    g3 = CreateImage(img->ncols, img->nrows);
    g4 = CreateImage(img->ncols, img->nrows);
    CalculaGradientes(img, g1, g2, g3, g4);

    hist = HistogramaGradiente(g1, g2, g3, g4);

    NormalizaHistograma(hist, QTD_BINS*QTD_BINS*QTD_BINS*QTD_BINS, img->nrows*img->ncols);
    WriteFileFloat(argv[2], hist, QTD_BINS*QTD_BINS*QTD_BINS*QTD_BINS);

    //free memory
    for (i=0; i<cimg->C[0]->nrows; i++) {
        free(imgHSV[i]);
    }
    free(imgHSV);
    free(hist);

    DestroyCImage(&cimg);
    DestroyImage(&img);
    DestroyImage(&g1);
    DestroyImage(&g2);
    DestroyImage(&g3);
    DestroyImage(&g4);

    return(0);
}
