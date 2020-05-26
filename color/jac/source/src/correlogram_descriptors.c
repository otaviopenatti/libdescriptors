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
#include "../include/correlogram_descriptors.h"
#include "../include/common.h"
#include "../include/image.h"

#define AUTOCORRE_BINS_H  (8)
#define AUTOCORRE_BINS_S  (4)
#define AUTOCORRE_BINS_V  (2)

#define AUTOCORRE_QUANT_H ((float)256.0f/AUTOCORRE_BINS_H)
#define AUTOCORRE_QUANT_S ((float)256.0f/AUTOCORRE_BINS_S)
#define AUTOCORRE_QUANT_V ((float)256.0f/AUTOCORRE_BINS_V)

#define AUTOCORRE_CMAX    (AUTOCORRE_BINS_H * AUTOCORRE_BINS_S * AUTOCORRE_BINS_V)

#define AUTOCORRE_BINS_GRADIENT (5)
#define AUTOCORRE_QUANT_GRADIENT ((float) 256.0f/AUTOCORRE_BINS_GRADIENT)
#define AUTOCORRE_NEIGHBORHOOD (2)
#define AUTOCORRE_TEXTUREDNESS_DIFF (20)
#define AUTOCORRE_BINS_TEXTUREDNESS_RANK (5)
#define AUTOCORRE_DIM_NEIGHBORHOOD  (2*AUTOCORRE_NEIGHBORHOOD+1)
#define AUTOCORRE_QUANT_TEXTUREDNESS_RANK (((float)(AUTOCORRE_DIM_NEIGHBORHOOD*AUTOCORRE_DIM_NEIGHBORHOOD))/AUTOCORRE_BINS_TEXTUREDNESS_RANK)

#define IS_BOUNDED(x,y,LX,LY,UX,UY) ((LX <= x && x < UX && LY <= y && y < UY))

int MyRound(float v) {
	return (int) floor(v);
}
void CalculePixelRankAndTexturedness(CImage *cimgHSV, int x, int y, int *rank, int *texturedness) {
	int NX = cimgHSV->C[0]->ncols;
	int NY = cimgHSV->C[0]->nrows;

	int p = x + cimgHSV->C[0]->tbrow[y];

	int pixelIntensity = cimgHSV->C[2]->val[p];

	int r,t;
	r = t = 0;
	int ix;for(ix = x - AUTOCORRE_NEIGHBORHOOD; ix <= x + AUTOCORRE_NEIGHBORHOOD; ix++) {
		if(ix < 0 || ix >= NX) continue;
		int iy;for(iy = y - AUTOCORRE_NEIGHBORHOOD; iy <= y + AUTOCORRE_NEIGHBORHOOD; iy++) {
			if(iy < 0 || iy >= NY) continue;
			if(ix == x && iy == y) continue;

			p = ix + cimgHSV->C[0]->tbrow[iy];
			
			int neighboorIntensity =  cimgHSV->C[2]->val[p];
			
			int diff = pixelIntensity - neighboorIntensity;
            //printf("diff=%d\n", diff);
			if(diff > 0) {r++; /*printf("incremented rank\n");*/}
			else if(abs(diff) > AUTOCORRE_TEXTUREDNESS_DIFF) {t++;/*printf("incremented texturedness\n");*/}

		}
	}
	*rank =  r;
	*texturedness = t;
}


void CalculeRankAndTexturednessImage(CImage *cimgHSV, Image *rankImage, Image *txtImage) {
	int NX = cimgHSV->C[0]->ncols;
	int NY = cimgHSV->C[0]->nrows;
	int x;for (x = 0; x < NX; x++) {
		int y;for (y = 0; y < NY; y++) {
			int p = x + rankImage->tbrow[y];
			int rank, texturedness;
			CalculePixelRankAndTexturedness(cimgHSV,x,y,&rank, &texturedness);
			txtImage->val[p] = texturedness;
			rankImage->val[p] = rank;
		}
	}
}


Image *GradientMagnitudeImage(CImage *cimgHSV) {
	int NX = cimgHSV->C[0]->ncols;
	int NY = cimgHSV->C[0]->nrows;
	Image *gradImage = CreateImage(NX,NY);
	int x;for (x = 0; x < NX; x++) {
		int y;for ( y = 0; y < NY; y++) {
			int dir,down;

			int p = x + cimgHSV->C[0]->tbrow[y];

			int curr = cimgHSV->C[2]->val[p];

			if(x+1 >= NX) dir = curr;
			else {
				int p_ = x+1 + cimgHSV->C[0]->tbrow[y];
				dir = cimgHSV->C[2]->val[p_];
			}
			if(y+1 >= NY) down = curr;
			else {
				int p_ = x + cimgHSV->C[0]->tbrow[y+1];
				down = cimgHSV->C[2]->val[p_];
			}
			//calcula o gradiente maximo nas duas direcoes
			int grad = MAX(abs(curr-dir), abs(curr-down));
			gradImage->val[p] = grad;
		}
	}
	return gradImage;
}

uchar *QuantizeHSVColors(CImage *cimgHSV) {
	int NX = cimgHSV->C[0]->ncols;
	int NY = cimgHSV->C[0]->nrows;

	int n = NX * NY;
	uchar* quantPixels = AllocUCharArray(n);
	if (quantPixels == NULL) Error(MSG1, "QuantizeColors");

	int p;for (p = 0; p < n; p++) {
		int h = MyRound(((float)cimgHSV->C[0]->val[p]) / AUTOCORRE_QUANT_H);
		int s = MyRound(((float)cimgHSV->C[1]->val[p]) / AUTOCORRE_QUANT_S);
		int v = MyRound(((float)cimgHSV->C[2]->val[p]) / AUTOCORRE_QUANT_V);
		int linear_color = AUTOCORRE_BINS_S * AUTOCORRE_BINS_V * h + AUTOCORRE_BINS_V * s + v;
		quantPixels[p] = linear_color;
	}
	return quantPixels;
}

uchar *QuantizeImage(Image *gradImage, float quantization) {
	int NX = gradImage->ncols;
	int NY = gradImage->nrows;
	int n = NX * NY;
	uchar* quantPixels = AllocUCharArray(n);
	if (quantPixels == NULL) Error(MSG1, "QuantizeMagnitude");
	int p;
	for (p = 0; p < n; p++)
		quantPixels[p] = MyRound( ((float)gradImage->val[p]) / quantization );
	return quantPixels;
}

AutoCorrelogram *CalculateAutoCorrelogram(uchar *quantizedFeature, int vMax, int NX, int NY)
{
	int N_DIST = 4;
	int D[] = {1,3,5,7};
	int n = NX * NY;

	AutoCorrelogram *autocorrelogram = CreateAutoCorrelogram(vMax, N_DIST);

	ulong *histogram = AllocULongArray(n);
	if(histogram == NULL)
		Error(MSG1,"CalculateAutoCorrelogram");
	int p;
	for(p = 0; p < n; p++) histogram[quantizedFeature[p]]++;

	int di;
	for (di = 0; di < N_DIST; di++) {
		int d = D[di];
		p = 0;
		int y=0;
		for (y = 0; y < NY; y++) {
			int x=0;
			for (x = 0; x < NX; x++) {
				int c = quantizedFeature[p++];
				int dx;
				for (dx = -d; dx <= d; dx++) {
					int X = x + dx, Y = y - d;
					if (IS_BOUNDED(X,Y,0,0,NX,NY) && quantizedFeature[Y * NX + X] == c)
						autocorrelogram->h[c * N_DIST + di]+=1.0;
					Y = y + d;
					if (IS_BOUNDED(X,Y,0,0,NX,NY) && quantizedFeature[Y * NX + X] == c)
						autocorrelogram->h[c * N_DIST + di]+=1.0;
				}
				int dy;
				for (dy = -d + 1; dy <= d - 1; dy++) {
					int X = x - d, Y = y + dy;
					if (IS_BOUNDED(X,Y,0,0,NX,NY) && quantizedFeature[Y * NX + X] == c)
						autocorrelogram->h[c * N_DIST + di]+=1.0;
					X = x + d;
					if (IS_BOUNDED(X,Y,0,0,NX,NY) && quantizedFeature[Y * NX + X] == c)
						autocorrelogram->h[c * N_DIST + di]+=1.0;
				}
			}
		}
		int c;
		for (c = 0; c < vMax; c++) {
			if (histogram[c] > 0L)
				autocorrelogram->h[c * N_DIST + di] = (AUTOCORRE_TYPE) autocorrelogram->h[c * N_DIST + di] / (((AUTOCORRE_TYPE)histogram[c]) * 8.0 * d);
		}
	}
	free(histogram);
	return autocorrelogram;
}

JointAutoCorrelogram *CalculateJointAutoCorrelogram(
		uchar *color, int vMaxColor,
		uchar *gradient, int vMaxGradient,
		uchar *texturedness, int vMaxTexturedness,
		uchar *rank, int vMaxRank,
		int NX, int NY)
{
	int D[] = {1,3,5,7};
	int N_DIST = 4;
	int n = NX * NY;

	JointAutoCorrelogram *autocorrelogram = CreateJointAutoCorrelogram(vMaxColor,vMaxGradient,vMaxTexturedness,vMaxRank, N_DIST);
	ulong *jointHistogram = AllocULongArray(vMaxColor *  vMaxGradient * vMaxTexturedness * vMaxRank);
	if(jointHistogram == NULL)
		Error(MSG1,"CalculateJointAutoCorrelogram");
	int p;
	for(p = 0; p < n; p++)  {
		int posHist = vMaxRank*(vMaxTexturedness*(vMaxGradient*color[p] + gradient[p])+texturedness[p])+rank[p];
		jointHistogram[posHist]++;
	}

	int di;
	for (di = 0; di < N_DIST; di++) {
		int d = D[di];
		p = 0;
		int y=0;
		for (y = 0; y < NY; y++) {
			int x=0;
			for (x = 0; x < NX; x++) {
				uchar c = color[p];
				uchar g = gradient[p];
				uchar t = texturedness[p];
				uchar r = rank[p];

				p++;
				
				int dx;
				int pos = JointCorrelogramPosition(autocorrelogram,c,g,t,r,di);
				for (dx = -d; dx <= d; dx++) {
					int X = x + dx, Y = y - d;
					if (IS_BOUNDED(X,Y,0,0,NX,NY) &&
							color[Y * NX + X]        == c &&
							texturedness[Y * NX + X] == t &&
							gradient[Y * NX + X]     == g &&
							rank[Y * NX + X]         == r) 	autocorrelogram->h[pos]+=1.0;

					Y = y + d;
					if (IS_BOUNDED(X,Y,0,0,NX,NY) &&
							color[Y * NX + X]        == c &&
							texturedness[Y * NX + X] == t &&
							gradient[Y * NX + X]     == g &&
							rank[Y * NX + X]         == r) 	autocorrelogram->h[pos]+=1.0;
				}
				int dy;
				for (dy = -d + 1; dy <= d - 1; dy++) {
					int X = x - d, Y = y + dy;
					if (IS_BOUNDED(X,Y,0,0,NX,NY) &&
							color[Y * NX + X]        == c &&
							texturedness[Y * NX + X] == t &&
							gradient[Y * NX + X]     == g &&
							rank[Y * NX + X]         == r) 	autocorrelogram->h[pos]+=1.0;
					X = x + d;
					if (IS_BOUNDED(X,Y,0,0,NX,NY) &&
							color[Y * NX + X]        == c &&
							texturedness[Y * NX + X] == t &&
							gradient[Y * NX + X]     == g &&
							rank[Y * NX + X]         == r) 	autocorrelogram->h[pos]+=1.0;
				}
				
			}
		}
		int c,t,g,r,posAuto=di,posHist=0;
		for (c = 0; c < vMaxColor; c++)
			for (g = 0; g < vMaxGradient; g++)
				for (t = 0; t < vMaxTexturedness; t++)
					for (r = 0; r < vMaxRank; r++) {
						if (jointHistogram[posHist] > 0L) {
							posAuto = JointCorrelogramPosition(autocorrelogram,c,g,t,r,di);
							autocorrelogram->h[posAuto] = (AUTOCORRE_TYPE) autocorrelogram->h[posAuto] / (((AUTOCORRE_TYPE)jointHistogram[posHist]) * 8.0 * d);
						}
						posHist++;
					}
	}
	free(jointHistogram);
	return autocorrelogram;
}
AutoCorrelogram *CAD64(CImage *cimgRGB) {

	CImage *hsvImage = CImageRGBtoHSV(cimgRGB);
	int NX = hsvImage->C[0]->ncols;
	int NY = hsvImage->C[0]->nrows;

	uchar *quantizedColors = QuantizeHSVColors(hsvImage);
	AutoCorrelogram *a = CalculateAutoCorrelogram(quantizedColors,AUTOCORRE_CMAX, NX, NY);

	DestroyCImage(&hsvImage);
	free(quantizedColors);

	return a;
}

CImage *PreProcessStep(CImage *cimgHsv) {
	return cimgHsv;
}

IndependentAutoCorrelogram *IndependentAutoCorrelogramImage(CImage *cimgRGB) {
	CImage *hsvImage = CImageRGBtoHSV(cimgRGB);
	
	CImage *imgHsvProcessed = PreProcessStep(hsvImage);
	
	int NX = imgHsvProcessed->C[0]->ncols;
	int NY = imgHsvProcessed->C[0]->nrows;
	
	Image *grad = GradientMagnitudeImage(imgHsvProcessed);
	Image *rank = CreateImage(NX, NY);
	Image *text = CreateImage(NX, NY);
	CalculeRankAndTexturednessImage(imgHsvProcessed,rank,text);

	uchar *quantizedColors       = QuantizeHSVColors(imgHsvProcessed);
	uchar *quantizedGradMagnitude= QuantizeImage(grad, AUTOCORRE_QUANT_GRADIENT);
	uchar *quantizedRank         = QuantizeImage(rank, AUTOCORRE_QUANT_TEXTUREDNESS_RANK);
	uchar *quantizedTexturedness = QuantizeImage(text, AUTOCORRE_QUANT_TEXTUREDNESS_RANK);

	IndependentAutoCorrelogram *w = CreateIndependentAutoCorrelogram();
	w->color        = CalculateAutoCorrelogram(quantizedColors, AUTOCORRE_CMAX, NX, NY);
	w->rank         = CalculateAutoCorrelogram(quantizedRank, AUTOCORRE_BINS_TEXTUREDNESS_RANK, NX, NY);
	w->texturedness = CalculateAutoCorrelogram(quantizedTexturedness, AUTOCORRE_BINS_TEXTUREDNESS_RANK, NX, NY);
	w->gradient     = CalculateAutoCorrelogram(quantizedGradMagnitude, AUTOCORRE_BINS_GRADIENT, NX, NY);
	DestroyImage(&grad);
	DestroyImage(&rank);
	DestroyImage(&text);
	DestroyCImage(&hsvImage);
	free(quantizedColors);
	free(quantizedGradMagnitude);
	free(quantizedRank);
	free(quantizedTexturedness);

	return w;

}

JointAutoCorrelogram *JointAutoCorrelogramImage(CImage *cimgRGB) {
	CImage *hsvImage = CImageRGBtoHSV(cimgRGB);
	
	CImage *imgHsvProcessed = PreProcessStep(hsvImage);
	
	int NX = imgHsvProcessed->C[0]->ncols;
	int NY = imgHsvProcessed->C[0]->nrows;
	
	Image *grad = GradientMagnitudeImage(imgHsvProcessed);
	Image *rank = CreateImage(NX, NY);
	Image *text = CreateImage(NX, NY);
	CalculeRankAndTexturednessImage(imgHsvProcessed,rank,text);

	uchar *quantizedColors       = QuantizeHSVColors(imgHsvProcessed);
	uchar *quantizedGradMagnitude= QuantizeImage(grad, AUTOCORRE_QUANT_GRADIENT);
	uchar *quantizedRank         = QuantizeImage(rank, AUTOCORRE_QUANT_TEXTUREDNESS_RANK);
	uchar *quantizedTexturedness = QuantizeImage(text, AUTOCORRE_QUANT_TEXTUREDNESS_RANK);

	JointAutoCorrelogram *w = CalculateJointAutoCorrelogram(
		quantizedColors, AUTOCORRE_CMAX,
		quantizedGradMagnitude, AUTOCORRE_BINS_GRADIENT,
		quantizedTexturedness, AUTOCORRE_BINS_TEXTUREDNESS_RANK,
		quantizedRank, AUTOCORRE_BINS_TEXTUREDNESS_RANK,
		NX,NY);

	DestroyImage(&grad);
	DestroyImage(&rank);
	DestroyImage(&text);
	DestroyCImage(&hsvImage);
	free(quantizedColors);
	free(quantizedGradMagnitude);
	free(quantizedRank);
	free(quantizedTexturedness);

	return w;

}
