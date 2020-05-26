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
    CImage *cimg=NULL; //input image
    ImageHSV **imgHSV; //image in HSV color space - same size of the original image
    Image *img=NULL;
    float *unser=NULL;
    int i,j;

    if (argc != 3) {
        fprintf(stderr,"Usage: unser_extraction <img_path> <fv_path>\n");
        fprintf(stderr,"\timg_path: path to the image file in PPM (P6) format\n");
        fprintf(stderr,"\tfv_path:  path to the file that will store the feature vector\n");
        fprintf(stderr,"Remark:\n\tThis program extracts the feature vector from a single image.\n");
        exit(-1);
    }

    cimg = ReadCImage(argv[1]);

    imgHSV = (ImageHSV**) calloc(cimg->C[0]->nrows, sizeof(ImageHSV*));
    for (i=0; i<cimg->C[0]->nrows; i++) {
        imgHSV[i] = (ImageHSV*) calloc(cimg->C[0]->ncols, sizeof(ImageHSV));
    }

    //converts to HSV
    RGB2HSV_unser(cimg, imgHSV);

    //creates gray image
    img = CreateImage(cimg->C[0]->ncols, cimg->C[0]->nrows);

    //copies the V channel (from HSV) to the gray image
    for (i = 0; i < cimg->C[0]->nrows; i++) {
        for (j = 0; j < cimg->C[0]->ncols; j++) {
            img->val[j+img->tbrow[i]] = imgHSV[i][j].V;
        }
    }

    unser = Unser_float(img);

    FILE *fp;
    if ((fp = fopen(argv[2], "wb")) == NULL) {
        printf("ERROR CREATING FEATURE VECTOR FILE!\n");
        exit(0);
    }
    fwrite(unser, sizeof(float), SIZE, fp);
    fclose(fp);

    for (i=0; i<cimg->C[0]->nrows; i++) {
        free(imgHSV[i]);
    }
    free(imgHSV);

    free(unser);
    DestroyCImage(&cimg);
    DestroyImage(&img);
    //DestroyFeatureVector1D(&unser);

    return(0);
}
