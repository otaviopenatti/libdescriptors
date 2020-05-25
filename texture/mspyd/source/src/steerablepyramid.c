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
#include "steerablepyramid.h"

/** Returns non-zero if x is power-of-2 **/
int isPowerOf2(int x)
{
	/// Algorithm from http://aggregate.org/MAGIC/#Is%20Power%20of%202
	return (x&(x-1)) == 0;
}

double H(double r)
{
    if (r >= M_PI/2.0) return 1.0;

    if (r <= M_PI/4.0) return 0.0;

    return cos((M_PI/2.0)*log((2.0*r)/M_PI)/log(2));
}

double L(double r)
{
    if (r <= M_PI/4.0) return 2.0;

    if (r >= M_PI/2.0) return 0.0;

    return 2.0*cos((M_PI/2.0)*log((4.0*r)/M_PI)/log(2));
}

/** Creates a SP filter or a low-pass filter.
 *
 *  @param width number of columns for the filter, has to be power-of-2
 *  @param height number of rows for the filter, has to be power-of-2
 *  @param option the filter type to be created
 *  @param orientation the orientation of the band-pass filter,
 *      if a low-pass is to be created orientation should have any
 *      number
 *  @param numAngles total number of orientations used in the SP filter,
 *      if a low-pass is to be created numAngles should have any
 *      number
 *
 *  @return a spectrum of the created filter
 **/

Spectrum *CreateFilter(int width, int height, FilterType option, int orientation, double numAngles)
{
	Spectrum *result = CreateSpectrum(width, height);

	double wi = -M_PI;
	double dwi = (2.0*M_PI)/height;

	double dwj = (2.0*M_PI)/width;

	double thetak = orientation*M_PI/numAngles;

	double thetak0 = (orientation-1)*M_PI/numAngles;
	
	double theta = 0;

    int i,j;

	for (i=0; i<height; i++)
	{
		double wj = -M_PI;

		for (j=0; j<width; j++)
		{
			double r = sqrt(wi*wi+wj*wj);

			double value = 0.0;

			if (option == BandPass)
			{

				if(thetak > 0 && thetak <= M_PI/2)
					theta = atan2(wj,wi);
				else if(thetak > M_PI/2 && thetak <= M_PI)
					theta = atan2(wj,wi)+M_PI;

				if(theta < 0 ) theta += M_PI;

				if((theta  >= thetak0 && theta <= thetak))
				{
					value = H(r)*L(r);
				}
                else if((theta  >= thetak0 + M_PI && theta <= thetak + M_PI))
				{
					value = H(r)*L(r);
				}
			}
			else if (option == LowPass)
			{
				value = L(r);
			}

			result->real[result->tbrow[i] + j] = value;
			result->imag[result->tbrow[i] + j] = 0.0;

			wj += dwj;
		}

		wi += dwi;
	}

	return result;
}


Spectrum* CreateLowPass(int width, int height)
{
     return CreateFilter(width, height, LowPass, -1, -1);
}

Spectrum* CreateBandPass(int width, int height, int orientation, int numAngles)
{
    return CreateFilter(width, height, BandPass, orientation, numAngles);
}

Spectrum* SmoothDImage(DImage *img)
{
    Spectrum *imageSpec = DFFT2D(img);
    Spectrum *filterSpec = CreateLowPass(img->ncols, img->nrows);
    Spectrum *smoothedSpec = ApplyFilter(imageSpec, filterSpec);

    DestroySpectrum(&imageSpec);
    DestroySpectrum(&filterSpec);

    return smoothedSpec;
}

/** Reduces the scale of DImage by a factor of 4 (rows/2 and cols/2).
 *  The reduction cuts down alternate columns and rows.
 **/

DImage *DownScaleDImage(DImage *img)
{
    DImage *result = CreateDImage(img->ncols/2, img->nrows/2);

    int y,x;

    for (y = 0; y < (img->nrows/2); y++)
    {
        for (x = 0; x < (img->ncols/2); x++)
        {
            int dstAddr = x + result->tbrow[y];
            int srcAddr = 2*x + img->tbrow[2*y];

            result->val[dstAddr] = img->val[srcAddr];
        }
    }

    return result;
}

/** Filterbank creation **/

/** Creates a SP filterbank.
 *
 *  @param ncols number of columns for the filter, has to be power-of-2
 *  @param nrows number of rows for the filter, has to be power-of-2
 *  @param scales how many scales should be used
 *  @param orientations how many orientations should be used
 *  @param nfeats total size of Filterbank->m_pFeatures->feat, it
 *      should be at least equal to scales*orientations
 **/

Filterbank* CreateSPFilterBank(int ncols, int nrows, int scales, int orientations, int nfeats)
{
    if(!isPowerOf2(ncols) || !isPowerOf2(nrows))
    {
        Error("The Steerable Pyramid filterbank must have rows and cols power of 2","CreateSPFilterBank");
    }
    if(scales == 0)
    {
        Error("The scale must be > 0","CreateSPFilterBank");
    }
    if(orientations == 0)
    {
        Error("The orientations must be > 0","CreateSPFilterBank");
    }
    if(nfeats < scales*orientations)
    {
        Error("The total number of feats shoud be at least scales*orientations","CreateSPFilterBank");
    }

    Filterbank* bank = (Filterbank*)calloc(1,sizeof(Filterbank));

    if(bank == NULL) Error(MSG1, "Filterbank");

    int size = scales*orientations;

    bank->m_pSpectralFilterBank = NULL;
    bank->m_pSpectralFilterBank = (Spectrum**)calloc(size,sizeof(Spectrum*));

    bank->m_pFeatures = NULL;
    bank->m_pFeatures = CreateFeatures(ncols,nrows,nfeats);

    bank->orientations = orientations;
    bank->scales = scales;

    int s,k;
    int width = ncols;
    int height = nrows;

    for(s = 0; s < scales; s++)
    {
        for(k = 0; k < orientations; k++)
        {
            Spectrum* filter = CreateBandPass(width,height,k+1,orientations);

            bank->m_pSpectralFilterBank[s*orientations + k] = filter;
        }
        width = width/2;
        height = height/2;
    }

    return bank;

}

/** Applies a SP filterbank to an image.
 *
 *  @param img entry image
 *  @param bank SP filterbank
 *  @param startingnfeat index where the new image features created should
 *      begin to be placed in bank->m_pFeatures->feat array
 **/

void ApplySPFilterBank(DImage* img, Filterbank* bank, int startingnfeat)
{

    if(bank == NULL)
    {
        Error("Filterbank NULL", "ApplySPFilterBank");
    }
    if(bank->m_pSpectralFilterBank == NULL)
    {
        Error("Filterbank->m_pSpectralFilterBank NULL", "ApplySPFilterBank");
    }
    if(img->ncols != bank->m_pSpectralFilterBank[0]->ncols &&
        img->nrows != bank->m_pSpectralFilterBank[0]->nrows)
    {
        Error("DImage and bank are of different sizes","ApplySPFilterBank");
    }
    if(bank->m_pFeatures == NULL)
    {
        Error("m_pFeatures NULL","ApplySPFilterBank");
    }

    int s, k;
    int scales = bank->scales;
    int orientations = bank->orientations;
    int featindex = startingnfeat;
    Spectrum*  smoothedDImg;
    DImage* scaled = img;

    float Sx = 0.5, Sy = 0.5;

    for(s = 0; s < scales; s++)
    {
        /// Applying low-pass filter
        smoothedDImg = SmoothDImage(scaled);

        for(k = 0; k < orientations; k++)
        {
            /// Applying band-pass filter
            Spectrum* filteredSpec = ApplyFilter(smoothedDImg, bank->m_pSpectralFilterBank[s*orientations+k]);
            DImage* filtered = DInvFFT2D(filteredSpec);
//            char name[30];
//            sprintf(name,"filtersp_%d_%d.pgm",s,k);
//            WriteImage(ViewMagnitude(FFTShift(bank->m_pSpectralFilterBank[s*orientations+k])),name);

            DImage* filteredscl = filtered;

            /** If scale is greater than 0, resample the image
             *  to its original size
             **/
            if(s > 0)
            {
                /// Returning image to original size
                filteredscl = DScale(filtered,1/(Sx+Sx),1/(Sy+Sy));
            }

            /// Sets the rescaled image to the proper bank feature index
            SetFeature(bank->m_pFeatures,featindex++,filteredscl);

            /// Cleaning up
            if(filtered != filteredscl)
            {
                DestroyDImage(&filtered);
            }
            DestroyDImage(&filteredscl);
	    DestroySpectrum(&filteredSpec);
        }
	
	DestroySpectrum(&smoothedDImg);

        if(scaled != img) DestroyDImage(&scaled);

        /// Downscaling image
        scaled = DScale(img,Sx,Sy);
        Sx = Sx/2;
        Sy = Sy/2;
    }

    if(scaled != img) DestroyDImage(&scaled);

}

/** Extracts SP features from all features in feats.
 *  By default 4 scales and 6 orientations are used, but that can be
 *  changed by altering SPSCALES and SPORIENTATIONS.
 *
 *  @param feats Features* already extracted from the image (e.g. Lab, RGB, etc.).
 *
 *  @return a new Features* struct which will contain feats->nfeats*SCALES*SPORIENTATIONS features,
 *      because each existent feature in feats will be filtered using SPSCALES and SPORIENTATIONS
 **/

Features* SteerablePyramidFeats(Features* feats)
{


    if(feats == NULL)
    {
        Error("Features null","SteerablePyramidFeats");
    }
    if(feats->nfeats == 0)
    {
        Error("The must be at least one feat. Features->nfeats == 0!!","SteerablePyramidFeats");
    }

    int i,j;
    bool fillwzeroes = false;
    int nfeats = feats->nfeats;
    DImage** featsArray = (DImage**)calloc(1,sizeof(DImage*)*feats->nfeats);

    /// Getting all image Features and turning them into power-of-2 sided DImages
    for( i = 0; i < nfeats; i++)
    {
        DImage* feat = GetFeature(feats,i);
        if(!isPowerOf2(feats->ncols) || !isPowerOf2(feats->nrows))
        {
            featsArray[i] = DImagePower2(feat);
            DestroyDImage(&feat);
            fillwzeroes = true;
        }else{
            featsArray[i] = feat;
        }
    }

    int index = 0;
    int ncols = featsArray[0]->ncols;
    int nrows = featsArray[0]->nrows;
    int totalnfeats = nfeats*SPSCALES*SPORIENTATIONS;

    Filterbank* bank = CreateSPFilterBank(ncols, nrows, SPSCALES, SPORIENTATIONS, totalnfeats);

    for( i = 0; i < nfeats; i++)
    {
        ApplySPFilterBank(featsArray[i], bank, index);

        /** Updates the index where the features should be placed in bank->m_pFeatures,
         *  i.e 0, 24, 48, etc.
         **/
        index += SPSCALES*SPORIENTATIONS;
	DestroyDImage (&featsArray[i]);
    }
    free(featsArray);

    Features* result = NULL;

    /// If image was filled with zeroes, remove zeroes **/
    if(fillwzeroes)
    {
        result = RemoveZeroes(bank->m_pFeatures,feats->ncols, feats->nrows);
    }
    else
    {
	result = CreateFeatures(ncols,nrows,totalnfeats);
	for (i = 0; i< result->nelems; i++)
		for (j = 0; j< result->nfeats; j++)
			result->elem[i].feat[j] = bank->m_pFeatures->elem[i].feat[j];
    }

    // destroy filter bank
    for (i = 0; i < bank->scales*bank->orientations; i++)
    {
	DestroySpectrum(&bank->m_pSpectralFilterBank[i]);
    }
    free(bank->m_pSpectralFilterBank);
    DestroyFeatures(&bank->m_pFeatures);
    free(bank);

    return result;
}

void WriteFileFeatureVector1D_bin(FeatureVector1D *fv,char *filename)
{
    FILE *fp;

    fp = fopen(filename,"wb");
    if (fp == NULL){
        fprintf(stderr,"Cannot open %s\n",filename);
        exit(-1);
    }
    fwrite(&(fv->n), sizeof(int), 1, fp);
    fwrite(fv->X, sizeof(double), fv->n, fp);
    fclose(fp);
}

FeatureVector1D *ReadFileFeatureVector1D_bin(char *filename)
{
    FeatureVector1D *fv = NULL;
    int nbins;
    FILE *fp;

    fp = fopen(filename,"rb");
    if (fp == NULL){
        fprintf(stderr,"Cannot open %s\n",filename);
        exit(-1);
    }

    fread(&nbins, sizeof(int), 1, fp);
    fv = CreateFeatureVector1D(nbins);
    fread(fv->X, sizeof(double), fv->n, fp);
    fclose(fp);
    return(fv);
}
