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
#ifndef _CORRELOGRAM_H_
#define _CORRELOGRAM_H_

#include "cimage.h"
#include "histogram.h"
typedef float AUTOCORRE_TYPE;
typedef struct _AutoCorrelogram {
	AUTOCORRE_TYPE *h;
	int n;
	int maxDistance;
} AutoCorrelogram;

typedef AutoCorrelogram *Ap_AutoCorrelogram;

void PrintAutoCorrelogram(AutoCorrelogram *c);
AutoCorrelogram *CreateAutoCorrelogram(int n, int maxDistance);
void DestroyAutoCorrelogram(AutoCorrelogram **c);
AutoCorrelogram *ReadStreamAutoCorrelogram(FILE *fp);
AutoCorrelogram *ReadFileAutoCorrelogram(char *filename);
void WriteStreamAutoCorrelogram(AutoCorrelogram *c, FILE *fp);
void WriteFileAutoCorrelogram(AutoCorrelogram *c, char *filename);
void WriteAp_AutoCorrelogram(Ap_AutoCorrelogram *ap_c, int n, char *filename);
double L1AutoCorrelogramDistance(AutoCorrelogram *c1, AutoCorrelogram *c2);
double L2AutoCorrelogramDistance(AutoCorrelogram *c1, AutoCorrelogram *c2);
double EMDAutoCorrelogramDistance(AutoCorrelogram *c1, AutoCorrelogram *c2);


/********* INDEPENDENT autocorrelogram prototype *********/
typedef struct {
	AutoCorrelogram *color;
	AutoCorrelogram *rank;
	AutoCorrelogram *texturedness;
	AutoCorrelogram *gradient;
} IndependentAutoCorrelogram;

IndependentAutoCorrelogram *CreateIndependentAutoCorrelogram(void);
void DestroyIndependentAutoCorrelogram(IndependentAutoCorrelogram **c);
IndependentAutoCorrelogram *ReadStreamIndependentAutoCorrelogram(FILE *fp);
IndependentAutoCorrelogram *ReadFileIndependentAutoCorrelogram(char *filename);
void WriteStreamIndependentAutoCorrelogram(IndependentAutoCorrelogram *c, FILE *fp);
void WriteFileIndependentAutoCorrelogram(IndependentAutoCorrelogram *c, char *filename);
double L1IndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2);
double SumL1IndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2);
double IntersectIndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2);
double TanimotoIndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2);


/********* JOINT autocorrelogram prototype *********/
typedef struct {
	AUTOCORRE_TYPE *h;
	int nColors;
	int nGradient;
	int nTexturedness;
	int nRank;
	int maxDistance;
} JointAutoCorrelogram;
#endif /*CORRELOGRAM_*/



/************* JOINT autocorrelogram implementation ***************/

int JointCorrelogramPosition(JointAutoCorrelogram *c, int color, int gradiente, int texturedness, int rank, int distance);
JointAutoCorrelogram *CreateJointAutoCorrelogram(int nColors, int nGrandient, int nTexturedness, int nRank, int maxDistance);
void DestroyJointAutoCorrelogram(JointAutoCorrelogram **c);
JointAutoCorrelogram *ReadStreamJointAutoCorrelogram(FILE *fp);
JointAutoCorrelogram *ReadFileJointAutoCorrelogram(char *filename);
void WriteStreamJointAutoCorrelogram(JointAutoCorrelogram *c, FILE *fp);
void WriteFileJointAutoCorrelogram(JointAutoCorrelogram *c, char *filename);
double L1JointAutoCorrelogramDistance(JointAutoCorrelogram *c1, JointAutoCorrelogram *c2);
