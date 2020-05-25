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
#include "histogram.h"

Histogram *CreateHistogram(int n)
{
  Histogram *h=NULL;
  
  h = (Histogram *) calloc(1,sizeof(Histogram));
  if (h != NULL) {
    h->v = AllocUCharArray(n);
    h->n = n;
  } else {
    Error(MSG1,"CreateHistogram");
  }
  return(h);
}

void DestroyHistogram(Histogram **h)
{
  Histogram *aux;
  
  aux = *h;
  if (aux != NULL){
    if (aux->v != NULL) free(aux->v);
    free(aux);
    *h = NULL;
  }
}

Ap_Histogram *AllocAp_HistogramArray(int n)
{
  Ap_Histogram *v=NULL;
  v = (Ap_Histogram *) calloc(n,sizeof(Ap_Histogram));
  if (v == NULL)
    Error(MSG1,"AllocAp_HistogramArray");
  return(v);
}

Histogram *ReadStreamHistogram(FILE *fp)
{
  Histogram *h = NULL;
  int i, nbins;
  uchar c;

  if (fp != NULL) {
    fscanf(fp, "%d\n", &nbins);
    h = CreateHistogram(nbins);
    for (i=0; i<nbins; i++){
      fscanf(fp, "%c", &c);
      h->v[i] = c;
    }
  }
  return(h);
}

Histogram *ReadFileHistogram(char *filename)
{
  Histogram *h = NULL;
  int i, nbins;
  FILE *fp;
  uchar c;

  fp = fopen(filename,"r");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  fscanf(fp, "%d\n", &nbins);
  h = CreateHistogram(nbins);
  for (i=0; i<nbins; i++){
    fscanf(fp, "%c", &c);
    h->v[i] = c;
  }
  fclose(fp);

  return(h);
}

Ap_Histogram *ReadAp_Histogram(char *filename,int *n)
{
  Ap_Histogram *ap_h = NULL;
  FILE *fp;
  int i;

  fp = fopen(filename,"r");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  fscanf(fp, "%d\n", n);
  ap_h = AllocAp_HistogramArray(*n);
  for (i=0;i<*n;i++)
    ap_h[i] = ReadStreamHistogram(fp);
  fclose(fp);

  return(ap_h);
}

void WriteStreamHistogram(Histogram *h,FILE *fp)
{
  int i;
  
  if (fp != NULL){
    fprintf(fp,"%d\n",h->n);
    for (i=0; i < h->n; i++)
      fprintf(fp,"%c",h->v[i]);
    fprintf(fp,"\n");
  }
}

void WriteFileHistogram(Histogram *h,char *filename)
{
  FILE *fp;
  
  fp = fopen(filename,"w");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  WriteStreamHistogram(h, fp);
  fclose(fp);
}

void WriteAp_Histogram(Ap_Histogram *ap_h,int n,char *filename)
{
  FILE *fp;
  int i;

  fp = fopen(filename,"w");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }
  fprintf(fp, "%d\n", n);
  for (i=0;i<n;i++)
    WriteStreamHistogram(ap_h[i], fp);
  fclose(fp);
}

Histogram *CopyHistogram(Histogram *h)
{
  Histogram *hc;
  
  hc = CreateHistogram(h->n);
  memcpy(hc->v,h->v,h->n*sizeof(double));
   
  return(hc);
}

ulong L1Distance(Histogram *h1, Histogram *h2) { 
  ulong i;
  ulong sum = 0;
  ulong z = 0;
  
  for (i = 0; i < h1->n ; i++){
    z = fabs(h1->v[i] - h2->v[i]); 
    sum = sum + z;
  }
  return (sum);
}

ulong L2Distance(Histogram *h1, Histogram *h2) { 
  ulong i;
  ulong sum = 0;
  long z = 0;
  
  for (i = 0; i < h1->n ; i++){
    z = h1->v[i] - h2->v[i]; 
    sum += z*z;
  }
  return (sum);
}

ulong LMDistance(Histogram *h1, Histogram *h2) { 
  ulong i;
  ulong max = 0;
  ulong z = 0;
  
  for (i = 0; i < h1->n ; i++){
    z = fabs(h1->v[i] - h2->v[i]); 
    if (max < z) max = z;
  }
  return (max);
}

double MOMDistance(Histogram *h1, Histogram *h2, double w1, double w2, double w3) { 
  ulong i;
  double mean1 = 0;
  double mean2 = 0;
  double dev1 = 0;
  double dev2 = 0;
  double devcub1 = 0;
  double devcub2 = 0;
  double sum1 = 0;
  double sum2 = 0;
  double dist = 0;    

  for (i = 0; i < h1->n ; i++){
	mean1 += (h1->v[i])*i; 
	mean2 += (h2->v[i])*i;
	sum1 += (h1->v[i]);
	sum2 += (h2->v[i]);
  }
  mean1 /= sum1; 
  mean2 /= sum2;
  for (i = 0; i < h1->n ; i++){
	dev1 += (h1->v[i])*((i-mean1)*(i-mean1));
	dev2 += (h2->v[i])*((i-mean2)*(i-mean2));
	devcub1 += (h1->v[i])*((i-mean1)*(i-mean1)*(i-mean1));
	devcub2 += (h2->v[i])*((i-mean2)*(i-mean2)*(i-mean1));
  }
  dev1 = sqrt(dev1/sum1); 
  dev2 = sqrt(dev2/sum2);
  devcub1 = pow(devcub1/sum1,1/3);
  devcub2 = pow(devcub2/sum2,1/3);

  dist += w1*fabs(mean1-mean2) + w2*fabs(dev1-dev2) + w3*fabs(devcub1-devcub2);

  return (dist);

}

