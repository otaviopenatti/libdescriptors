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
#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include "common.h"

typedef struct _Histogram {
  uchar *v;
  int n; 
} Histogram;

typedef Histogram *Ap_Histogram;

Histogram     *CreateHistogram(int n);
void           DestroyHistogram(Histogram **h);
Ap_Histogram  *AllocAp_HistogramArray(int n);
Histogram     *ReadStreamHistogram(FILE *fp);
Histogram     *ReadFileHistogram(char *filename);
Ap_Histogram  *ReadAp_Histogram(char *filename, int *n);
void           WriteStreamHistogram(Histogram *h,FILE *fp);
void           WriteFileHistogram(Histogram *h,char *filename);
void           WriteAp_Histogram(Ap_Histogram *ap_h,int n,char *filename);
Histogram     *CopyHistogram(Histogram *h);

ulong          L1Distance(Histogram *h1, Histogram *h2);
ulong          L2Distance(Histogram *h1, Histogram *h2);
ulong          LMDistance(Histogram *h1, Histogram *h2);
double 	       MOMDistance(Histogram *h1, Histogram *h2, double w1, double w2, double w3);

#endif

