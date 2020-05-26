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
#include "sasi.h"

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

void RGB2HSV_sasi(CImage *RGB, ImageHSV **HSV) {

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

            //normalizes S and V from 0 to 255
            HSV[i][j].S *= 255;
            HSV[i][j].V *= 255;

        }
    }
}

void NormalizeFeature (FeatureVector1D **feature)
{
  double mean, dev;
  int i;
  mean = dev = 0;
  for (i=0; i<(*feature)->n; i++)
	mean+=(*feature)->X[i];
  mean /= (*feature)->n;
  for (i=0; i<(*feature)->n; i++)
	dev+=((*feature)->X[i]-mean)*((*feature)->X[i]-mean);
  dev /= (*feature)->n;
  dev = sqrt(dev);

  //modification to avoid division by zero when the image is completely black
  if (dev==0) {
    for (i=0; i<(*feature)->n; i++) {
      (*feature)->X[i] = 0;                // if dev==0 then ((*feature)->X[i] - mean) will always be zero.
    }
  } else {
    for (i=0; i<(*feature)->n; i++) {
      (*feature)->X[i] = ((*feature)->X[i] - mean)/dev;
    }
  }


}

double ComputeMean (double **v, int nrows, int ncols)
{
  double mean = 0;
  int i,j;
  for (i=0; i<nrows; i++)
  	for (j=0; j<ncols; j++)
		mean+=v[i][j];
  mean /= nrows*ncols;
  return (mean);
}

double ComputeStandardDeviation (double **v, int nrows, int ncols)
{
  double dev = 0;
  int i,j;
  double mean;
  mean = ComputeMean (v,nrows,ncols);
  for (i=0; i<nrows; i++)
  	for (j=0; j<ncols; j++)
  		dev += (v[i][j]-mean)*(v[i][j]-mean);
  dev /= nrows*ncols;
  dev = sqrt(dev);
  return (dev);
}

double getsasiVal(Image *img, int a, int b, int k, int t, int L)
{
  double sasival = 0;
  int i,j,i0,i1,j0,j1;
  double sum1, sum2, mean;
  sum1 = sum2 = mean = 0;
  i0 = a-L/2; 
  i1 = a+L/2;
  j0 = b-L/2;
  j1 = b+L/2;

  //computes auto-correlations, based on the definition of auto-correlation. In the paper, it is not clearly explained
  switch (t){
	case 0: //0 graus -> calcula v(0,k)		
		for (i = i0; i<=i1; i++){	
			for (j = j0; j<=j1; j++){
				mean += img->val[img->tbrow[i] +j];
			}
		}
		mean /= L*L; 			
		for (i = i0; i<=i1; i++){
			for (j = j0; j<=j1-k; j++){
				sum1+= (img->val[img->tbrow[i] +j] -mean) * (img->val[img->tbrow[i] +j+k] -mean);
			}
		}
		for (i = i0; i<=i1; i++){	
			for (j = j0; j<=j1; j++){
				sum2+= (img->val[img->tbrow[i] +j] -mean) * (img->val[img->tbrow[i] +j] -mean);
			}
		}
	break;

	case 1: //45 degrees -> computes v(k,k)
		for (i = i0; i<=i1; i++){	
			for (j = b+i-i0-L+1; j<=b+i-i0; j++){
				mean += img->val[img->tbrow[i] +j]; 
			}
		}
		mean /= L*L; 			
		for (i = i0; i<=i1-k; i++){
			for (j = b+i-i0-L+1; j<=b+i-i0; j++){
				sum1+= (img->val[img->tbrow[i] +j] -mean) * (img->val[img->tbrow[i+k] +j+k] -mean);
			}
		}
		for (i = i0; i<=i1; i++){	
			for (j = b+i-i0-L+1; j<=b+i-i0; j++){
				sum2+= (img->val[img->tbrow[i] +j] -mean) * (img->val[img->tbrow[i] +j] -mean);
			}
		}
	break;

	case 2: //90 degrees -> computes v(k,0)
		for (i = i0; i<=i1; i++){
			for (j = j0; j<=j1; j++){
				mean += img->val[img->tbrow[i] +j];
			}
		}
		mean /= L*L; 			
		for (i = i0; i<=i1-k; i++){
			for (j = j0; j<=j1; j++){
				sum1+= (img->val[img->tbrow[i] +j] -mean) * (img->val[img->tbrow[i+k] +j] -mean);
			}
		}
		for (i = i0; i<=i1; i++){	
			for (j = j0; j<=j1; j++){
				sum2+= (img->val[img->tbrow[i] +j] -mean) * (img->val[img->tbrow[i] +j] -mean);
			}
		}
	break;

	case 3: //135 degrees -> computes v(-k,k) 
		for (i = i0; i<=i1; i++){	
			for (j = b-i+i0; j<=b+i-i0+L-1; j++){
				mean += img->val[img->tbrow[i] +j];
			}
		}
		mean /= L*L; 			
		for (i = i0+k; i<=i1; i++){
			for (j = b-i+i0; j<=b+i-i0+L-1; j++){
				sum1+= (img->val[img->tbrow[i] +j] -mean) * (img->val[img->tbrow[i-k] +j+k] -mean);
			}
		}
		for (i = i0; i<=i1; i++){	
			for (j = b-i+i0; j<=b+i-i0+L-1; j++){
				sum2+= (img->val[img->tbrow[i] +j] -mean) * (img->val[img->tbrow[i] +j] -mean);
			}
		}
	break;

	default:	
	return 0;
  }

  if (sum2!=0)
  	sasival = sum1/sum2;
  else
	sasival = sum1;

  return (sasival);
}

Image* CreateExtendedImage (Image *img, int diff)
{
  int i,j;
  Image *imgExt = NULL;
  imgExt = CreateImage(img->ncols+2*diff, img->nrows+2*diff);

  for (i=0;i<diff;i++)
	for(j=0;j<diff;j++){
	   imgExt->val[imgExt->tbrow[i] +j] = img->val[0];
	   imgExt->val[imgExt->tbrow[i] +j+imgExt->ncols-diff] = img->val[img->ncols-1];
	   imgExt->val[imgExt->tbrow[i+imgExt->nrows-diff] +j] = img->val[img->tbrow[img->nrows-1]];
	   imgExt->val[imgExt->tbrow[i+imgExt->nrows-diff] +j+imgExt->ncols-diff] = 
		img->val[img->tbrow[img->nrows-1]+img->ncols-1];
	}
  for (i=0;i<diff;i++)
	for(j=diff;j<imgExt->ncols-diff;j++){
	   imgExt->val[imgExt->tbrow[i] +j] = img->val[j-diff];
	   imgExt->val[imgExt->tbrow[i+imgExt->nrows-diff] +j] = img->val[img->tbrow[img->nrows-1] +j-diff];
	}
  for (i=diff;i<imgExt->nrows-diff;i++)
	for(j=0;j<diff;j++){
	   imgExt->val[imgExt->tbrow[i] +j] = img->val[img->tbrow[i-diff]];
	   imgExt->val[imgExt->tbrow[i] +j+imgExt->ncols-diff] = img->val[img->tbrow[i-diff] +img->ncols-1];
	}
  for (i=0; i<img->nrows;i++)
	for(j=0;j<img->ncols;j++)
	   imgExt->val[imgExt->tbrow[i+diff] +j+diff] = img->val[img->tbrow[i] +j];

  return(imgExt);
}

FeatureVector1D *SASI(Image *img, int num, int *sizes)
{

  //sizes: L sorted values (window size) 
  FeatureVector1D *feature = NULL;
  int i,j,k,l,ksum,featureposM,featureposD,theta, *kmax;
  Image *imgExt;
  double **sasival;
  int diff;
  
  for (i = 0; i<num-1; i++)
	if (sizes[i]>sizes[i+1]) return(NULL); 

  //expands the image to avoid border problems
  diff = sizes[num-1];
  imgExt =  CreateExtendedImage(img,diff); 

  //computes K for each L
  kmax = (int*) malloc(num*sizeof(int));
  ksum=0;
  for (l = 0; l<num; l++){
	kmax[l] = ceil(((float)sizes[l])/4)+1;
	ksum+=kmax[l];
	}

  //creates the feature vector; dimension = 2 (mean and standard deviation) * 4 (number of orientations) * ksum
  ksum *=8;
  feature = CreateFeatureVector1D(ksum);
  featureposM=0;
  featureposD=ksum/2;

  //creates correlation matrix
  sasival = (double**) calloc(img->nrows,sizeof(double*));
  for (i=0;i<img->nrows;i++)
	sasival[i] = (double*) calloc(img->ncols,sizeof(double));

  //computes auto-correlations and mean and standard deviation values for the feature vectors
  for (l = 0; l<num; l++) //for each window size
	for (k = 1; k <=kmax[l]; k++) //for each value of k
		for (theta = 0; theta<4; theta++){ //theta = {0,1,2,3} -> orientation = {0,45,90,135}
			
			for (i = 0; i<img->nrows;i++)		//computes correlations for each image pixel
				for (j = 0; j<img->ncols;j++) 	
					sasival[i][j] = getsasiVal(imgExt,i+diff,j+diff,k,theta,sizes[l]);
			feature->X[featureposM] = ComputeMean(sasival,img->nrows,img->ncols);  //computes mean
			feature->X[featureposD] = ComputeStandardDeviation(sasival,img->nrows,img->ncols); //computes standard deviation
			featureposM++;
			featureposD++;
  		}

  for (i=0;i<img->nrows;i++)
	free(sasival[i]); 
  free(sasival);
  DestroyImage(&imgExt);
  free (kmax);

  NormalizeFeature(&feature);
  
  return(feature);
}


FeatureVector1D *ReadFileFeatureVector1DBin(char *filename) {
    FeatureVector1D *fv=NULL;
    int n; 

    FILE *f;
    if ((f = fopen(filename, "rb")) == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(0);
    }
    fread(&n,sizeof(int),1,f);
    fv = CreateFeatureVector1D(n);
    fread(fv->X,sizeof(double),fv->n, f);
    fclose(f);

    return fv;
}

