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
    Features *img=NULL;
    Features *steerable=NULL;
    FeatureVector1D *featurevector=NULL;
    int i,j;

    cimg = ReadCImage(img_path);
    img = CreateFeatures(cimg->C[0]->ncols, cimg->C[0]->nrows, 1);

    //translate to HSV space

    for (i=0; i<(img->ncols*img->nrows); i++)
        img->elem[i].feat[0] = (cimg->C[0]->val[i])+(cimg->C[1]->val[i])+(cimg->C[2]->val[i])/3;

    steerable = SteerablePyramidFeats(img);

    featurevector = CreateFeatureVector1D(2*steerable->nfeats);
    for (j=0; j<featurevector->n; j++)
        featurevector->X[j] = 0;

    //mean of the steerable features
    for (i=0; i<steerable->nelems; i++)
        for (j=0; j<steerable->nfeats; j++)
            featurevector->X[j] += (double) (steerable->elem[i].feat[j]);
    for (j=0; j<steerable->nfeats; j++)
        featurevector->X[j] /= steerable->nelems;

    //standad deviation of the steerable features
    for (i=0; i<steerable->nelems; i++)
        for (j=0; j<steerable->nfeats; j++){
            featurevector->X[j+steerable->nfeats] += (double) ((steerable->elem[i].feat[j]-featurevector->X[j])*(steerable->elem[i].feat[j]-featurevector->X[j]));
        }
    for (j=0; j<steerable->nfeats; j++){
        featurevector->X[j+steerable->nfeats] /= steerable->nelems;
        featurevector->X[j+steerable->nfeats] = sqrt(featurevector->X[j+steerable->nfeats]);
    }


  //
  // Sort feature vector to achive rotation and scale invariance
  //
/**/
    int maxpos;
    double max, aux;

    for (i=0; i<steerable->nfeats-1; i++){
        max = featurevector->X[i];
        maxpos = i;
        for (j=i+1; j<steerable->nfeats; j++){
            if (featurevector->X[j]>max){
                max = featurevector->X[j];
                maxpos = j;
            }
        }
        aux = featurevector->X[maxpos+steerable->nfeats];
        featurevector->X[maxpos] = featurevector->X[i];
        featurevector->X[maxpos+steerable->nfeats] = featurevector->X[i+steerable->nfeats];
        featurevector->X[i] = max;
        featurevector->X[i+steerable->nfeats] = aux;
    }
/**/

  //
  // End sort. Comment lines above if rotation or scale invariance isn't desired.
  //

    WriteFileFeatureVector1D_bin(featurevector,fv_path);

    DestroyFeatureVector1D(&featurevector);
    DestroyCImage(&cimg);
    DestroyFeatures(&img);
    DestroyFeatures(&steerable);

}


int main(int argc, char** argv)
{

    if (argc != 3) {
        fprintf(stderr,"Usage: mspyd_extraction <img_path> <fv_path>\n");
        fprintf(stderr,"\timg_path: path to the image file in PPM (P6) format\n");
        fprintf(stderr,"\tfv_path:  path to the file that will store the feature vector\n");
        fprintf(stderr,"Remark:\n\tThis program extracts the feature vector from a single image.\n");
        exit(-1);
    }

    Extraction(argv[1], argv[2]);

    return(0);
}
