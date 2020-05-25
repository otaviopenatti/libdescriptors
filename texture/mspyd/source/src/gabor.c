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
#include "gabor.h"


////////////////////////////////////////////////////////
// Gabor Functions - BEGIN
////////////////////////////////////////////////////////
gabor* CreateGabor(int nsze ){

  gabor * pGabor = NULL;
  pGabor         = (gabor*)calloc(1, sizeof(gabor));
  assert(pGabor!=NULL);

  pGabor->m_nSze = nsze;

  pGabor->m_pReal = NULL;
  pGabor->m_pReal = CreateDImage(pGabor->m_nSze, pGabor->m_nSze);
  assert(pGabor->m_pReal != NULL);

  pGabor->m_pImag = NULL;
  pGabor->m_pImag = CreateDImage(pGabor->m_nSze, pGabor->m_nSze);
  assert(pGabor->m_pImag != NULL);

  return (pGabor);
}

void gaborFilter( gabor * pGabor, int scale, int orientation ){

  double a, Uho;
  double sigmaU, sigmaV, sigmaU2;
  double sigmaX, sigmaY;
  double theta, sinTheta, cosTheta;
  double log2, dSze;
  double x, y, g;
  double normCnst, aAtScl;
  int nSze, idx;

  a       = pow((DUH / DUL), 1.0 / (double)(NSCALES-1));
  Uho	    = DUH / ( pow( a,(double)(NSCALES-scale) ) );

  log2    = 2.0 * log(2.0);

  sigmaU  = ((a-1.0) * Uho) / ( (a+1.0) * sqrt(log2));
  sigmaU2   = sigmaU * sigmaU;

	sigmaV    = (tan(PI/(2*NORIENTATIONS)) * (Uho - log2*(sigmaU2/Uho))) / sqrt(log2 - (log2*log2*sigmaU2)/(Uho*Uho));

  sigmaX  = 1.0 / (2.0*PI*sigmaU);
  sigmaY  = 1.0 / (2.0*PI*sigmaV);

  dSze    = (pGabor->m_nSze-1.0)/2.0;
  nSze    = floor(dSze);

  theta     = (PI * (orientation-1)) / NORIENTATIONS + PI/2;
  cosTheta  = cos(theta);
  sinTheta  = sin(theta);

  normCnst = 1.0/(2.0*PI*sigmaX*sigmaY);
  aAtScl   = pow(a, (double)(NSCALES-scale) );

  register int i, j;

	for(i=0; i < 2*nSze+1; i++){
    for(j=0; j < 2*nSze+1; j++){
      x  = (double)(i-dSze)*cosTheta   + (double)(j-dSze)*sinTheta;
      y  = (double)-(i-dSze)*sinTheta  +  (double)(j-dSze)*cosTheta;
      g  = normCnst*aAtScl*exp( (-0.5) * ( (x*x)/(sigmaX*sigmaX) + (y*y)/(sigmaY*sigmaY) ));

      idx = pGabor->m_pReal->tbrow[i] + j;
			pGabor->m_pReal->val[idx] = g*cos(2*PI*x*Uho);
			pGabor->m_pImag->val[idx] = g*sin(2*PI*x*Uho);
    }
  }
}

void DumpGabor(gabor * pGabor, int flag, char * fname ){

  register int i, j;
  char outname[100];
  FILE * fid = NULL;
  int idx;

  if(!flag){

    sprintf(outname, fname);
    fid = fopen(outname, "w");

    for(i=0; i<pGabor->m_pReal->nrows; i++){
      for(j=0; j<pGabor->m_pReal->ncols; j++){
        idx = pGabor->m_pReal->tbrow[i] + j;
        fprintf(fid,"%2.4f ", pGabor->m_pReal->val[idx]);
      }
      fprintf(fid,"\n");
    }
  }else{

    sprintf(outname, fname);
    fid = fopen(outname, "w");

    for(i=0; i<pGabor->m_pImag->nrows; i++){
      for(j=0; j<pGabor->m_pImag->ncols; j++){
        idx = pGabor->m_pImag->tbrow[i] + j;
        fprintf(fid,"%2.4f ", pGabor->m_pImag->val[idx]);
      }
      fprintf(fid,"\n");
    }
  }
  fclose(fid);
}

void DestroyGabor(gabor** pGabor){

  if((*pGabor)!=NULL){
      if((*pGabor)->m_pReal!=NULL){
        DestroyDImage(&((*pGabor)->m_pReal));
        (*pGabor)->m_pReal = NULL;
      }

      if((*pGabor)->m_pImag!=NULL){
        DestroyDImage(&((*pGabor)->m_pImag));
        (*pGabor)->m_pImag = NULL;
      }

      free(*pGabor);
      (*pGabor) = NULL;
      assert(*pGabor == NULL);
  }
}
////////////////////////////////////////////////////////
// Gabor Functions - END
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// Filterbank Functions - BEGIN
////////////////////////////////////////////////////////
Filterbank * CreateFilterbank( int sze ){

  Filterbank * pFilterbank = NULL;
  pFilterbank = (Filterbank*)calloc(1, sizeof(Filterbank));
  assert(pFilterbank!=NULL);

  pFilterbank->m_pSpectralFilterBank = NULL;
  pFilterbank->m_pSpectralFilterBank = (Spectrum**)calloc(NSCALES*NORIENTATIONS,sizeof(Spectrum*));
  assert(pFilterbank!=NULL);

  gabor * pGabor = NULL;
  register int s, k;

  for( s = 0; s < NSCALES; s++ ){
    for( k = 0; k < NORIENTATIONS; k++ ){

      pGabor = CreateGabor( sze );
      assert(pGabor!=NULL);
      gaborFilter(pGabor, s+1, k+1);

      pFilterbank->m_pSpectralFilterBank[s*NORIENTATIONS+k] = DFFT2DWithImag(pGabor->m_pReal, pGabor->m_pImag);
      DestroyGabor(&(pGabor));
    }
  }

  pFilterbank->m_pFeatures = NULL;
  return (pFilterbank);
}

void applyFilterbank( Filterbank *pFilterbank, Features * pFeats )
{

  assert(pFilterbank!=NULL);
  assert(pFeats!=NULL);

  int gabfeatsidx = 0;

  pFilterbank->m_pFeatures = CreateFeatures(pFeats->ncols, pFeats->nrows, pFeats->nfeats*NSCALES*NORIENTATIONS);
  assert(pFilterbank->m_pFeatures!=NULL);

  register int idx, s, k;

  for(idx=0; idx < pFeats->nfeats; idx++)
  {

    DImage* pImgfeats       = GetFeature(pFeats, idx);

    Spectrum *pSpectrumImg  = NULL;
    pSpectrumImg            = DFFT2D(pImgfeats);
    assert(pSpectrumImg!=NULL);

    for( s = 0; s < NSCALES; s++ )
    {
      for( k = 0; k < NORIENTATIONS; k++ )
      {
          Spectrum * pSpectrumConv = MultSpectrum(pSpectrumImg, pFilterbank->m_pSpectralFilterBank[s*NORIENTATIONS+k]);

          DImage   *  pInvImg       = DInvFFT2D(pSpectrumConv);
          DImage   *  pShiftedInvImg = DShift(pInvImg);

          SetFeature(pFilterbank->m_pFeatures, gabfeatsidx, pShiftedInvImg);
          gabfeatsidx++;

          DestroySpectrum(&(pSpectrumConv));
          DestroyDImage(&(pInvImg));
          DestroyDImage(&(pShiftedInvImg));
      }
    }
    DestroyDImage(&(pImgfeats));
    DestroySpectrum(&(pSpectrumImg));
  }
}

void DestroyFilterbank( Filterbank ** pFilterbank ){

  register int s, k;

  if((*pFilterbank)!=NULL){

    for( s = 0; s < NSCALES; s++ ){
      for( k = 0; k < NORIENTATIONS; k++ ){
        DestroySpectrum( &((*pFilterbank)->m_pSpectralFilterBank[s*NORIENTATIONS+k]) );
      }
    }

    free((*pFilterbank));
    (*pFilterbank) = NULL;
    assert(*pFilterbank == NULL);
  }
}

////////////////////////////////////////////////////////
// Filterbank Functions - END
////////////////////////////////////////////////////////


Features* GaborFeats(Features* feats)
{
    if(feats == NULL)
    {
        fprintf(stderr,"Features* == NULL in GaborFeats");
        exit(-1);
    }

    Features* f = NULL;

    f = feats;

    Filterbank* bank = CreateFilterbank(f->ncols);
    applyFilterbank(bank,f);

	return bank->m_pFeatures;
}
