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
#include "../include/correlogram.h"
#include "../include/common.h"
#include "../include/image.h"

void PrintAutoCorrelogram(AutoCorrelogram *c) {
	int n = c->n * c->maxDistance;
	int i;for (i=0; i<n; i++)
		printf("%lf ", c->h[i]);
	printf("\n");
}

AutoCorrelogram *CreateAutoCorrelogram(int n, int maxDistance) {
	AutoCorrelogram *c= (Ap_AutoCorrelogram) malloc(sizeof(AutoCorrelogram));
	if (c != NULL) {
		c->n = n;
		c->maxDistance = maxDistance;
		c->h = (AUTOCORRE_TYPE *) calloc(n*maxDistance, sizeof(AUTOCORRE_TYPE));
		if (c->h == NULL)
			Error(MSG1, "CreateCorrelogram");
	} else
		Error(MSG1, "CreateCorrelogram");
	return (c);
}
void DestroyAutoCorrelogram(AutoCorrelogram **c) {
	AutoCorrelogram *aux;

	aux = *c;
	if (aux != NULL) {
		if(aux->h!=NULL)free(aux->h);
		free(aux);
		*c = NULL;
	}
}

AutoCorrelogram *ReadStreamAutoCorrelogram(FILE *fp) {
	AutoCorrelogram *c = NULL;
	int i, ncolors, maxDistance;
	AUTOCORRE_TYPE v;

	if (fp != NULL) {
		fscanf(fp, "%d %d\n", &ncolors, &maxDistance);
		c = CreateAutoCorrelogram(ncolors, maxDistance);
		int n = ncolors *maxDistance;
		for (i=0; i<n; i++) {
			fscanf(fp, "%f", &v);
			c->h[i] = v;
		}
	}
	return c;
}
AutoCorrelogram *ReadFileAutoCorrelogram(char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", filename);
		exit(-1);
	}
	AutoCorrelogram *c = ReadStreamAutoCorrelogram(fp);
	fclose(fp);
	return c;
}

void WriteStreamAutoCorrelogram(AutoCorrelogram *c, FILE *fp) {
	if(c == NULL) return;
	if (fp != NULL) {
		fprintf(fp, "%d %d\n", c->n, c->maxDistance);
		int n = c->n * c->maxDistance;
		int i;for (i=0; i<n; i++) 
			fprintf(fp, "%f ", c->h[i]);
		fprintf(fp, "\n");
	}
}
void WriteFileAutoCorrelogram(AutoCorrelogram *c, char *filename) {
	FILE *fp;
	if(c == NULL) return;

	fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", filename);
		exit(-1);
	}
	WriteStreamAutoCorrelogram(c, fp);
	fclose(fp);
}

double L1AutoCorrelogramDistance(AutoCorrelogram *c1, AutoCorrelogram *c2) {
	int minColors = MIN(c1->n, c2->n);
	int minDists = MIN(c1->maxDistance, c2->maxDistance);
	double total = 0.0;
	int n = minColors * minDists;
	int i;for (i=0; i<n; i++)
		total+=fabs(c1->h[i] - c2->h[i]);
	return total;
}
double TanimotoAutoCorrelogramDistance(AutoCorrelogram *c1, AutoCorrelogram *c2) {
	int minColors = MIN(c1->n, c2->n);
	int minDists = MIN(c1->maxDistance, c2->maxDistance);
	int n = minColors * minDists;
	double dotAA,dotAB,dotBB;
	dotAA=dotAB=dotBB=0.0;
	int i;for (i=0; i<n; i++){
		dotAA += c1->h[i]*c1->h[i];
		dotAB += c1->h[i]*c2->h[i];
		dotBB += c2->h[i]*c2->h[i];
	}
	return -dotAB / (dotAA + dotBB - dotAB);
}

double L2AutoCorrelogramDistance(AutoCorrelogram *c1, AutoCorrelogram *c2) {
	int minColors = MIN(c1->n, c2->n);
	int minDists = MIN(c1->maxDistance, c2->maxDistance);
	double total = 0.0;
	int n = minColors * minDists;
	int i;for (i=0; i<n; i++) {
		double d = c1->h[i] - c2->h[i];
		total+=d*d;
	}
	return total;
}

/************* INDEPENDENT autocorrelogram implementation ***************/

IndependentAutoCorrelogram *CreateIndependentAutoCorrelogram(void) {
	IndependentAutoCorrelogram *w = (IndependentAutoCorrelogram *) malloc(sizeof(IndependentAutoCorrelogram));
	if(w == NULL)
		Error(MSG1,"CreateWilliamAutoCorrelogram");
	w->color = NULL;
	w->rank = NULL;
	w->gradient = NULL;
	w->texturedness = NULL;
	return w;
}
void DestroyIndependentAutoCorrelogram(IndependentAutoCorrelogram **c) {
	IndependentAutoCorrelogram *aux;
	aux = *c;
	if (aux != NULL) {
		if((*c)->color!=NULL) DestroyAutoCorrelogram(&((*c)->color));
		if((*c)->rank!=NULL) DestroyAutoCorrelogram(&((*c)->rank));
		if((*c)->gradient!=NULL) DestroyAutoCorrelogram(&((*c)->gradient));
		if((*c)->texturedness!=NULL) DestroyAutoCorrelogram(&((*c)->texturedness));
		free(aux);
		*c = NULL;
	}
}

IndependentAutoCorrelogram *ReadStreamIndependentAutoCorrelogram(FILE *fp) {
	IndependentAutoCorrelogram *c = NULL;
	if (fp != NULL) {
		c = CreateIndependentAutoCorrelogram();
		c->color = ReadStreamAutoCorrelogram(fp);
		c->rank = ReadStreamAutoCorrelogram(fp);
		c->gradient = ReadStreamAutoCorrelogram(fp);
		c->texturedness = ReadStreamAutoCorrelogram(fp);
	}
	return c;
}
IndependentAutoCorrelogram *ReadFileIndependentAutoCorrelogram(char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", filename);
		exit(-1);
	}
	IndependentAutoCorrelogram *c = ReadStreamIndependentAutoCorrelogram(fp);
	fclose(fp);
	return c;
}
void WriteStreamIndependentAutoCorrelogram(IndependentAutoCorrelogram *c, FILE *fp) {
	if (fp != NULL) {
		WriteStreamAutoCorrelogram(c->color, fp);
		WriteStreamAutoCorrelogram(c->rank, fp);
		WriteStreamAutoCorrelogram(c->gradient, fp);
		WriteStreamAutoCorrelogram(c->texturedness, fp);
	}
}
void WriteFileIndependentAutoCorrelogram(IndependentAutoCorrelogram *c, char *filename) {
	FILE *fp;

	fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", filename);
		exit(-1);
	}
	WriteStreamIndependentAutoCorrelogram(c, fp);
	fclose(fp);
}

double SumL1IndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2) {
	double d1 = L1AutoCorrelogramDistance(c1->color, c2->color);
	double d2 = L1AutoCorrelogramDistance(c1->rank, c2->rank);
	double d3 = L1AutoCorrelogramDistance(c1->gradient, c2->gradient);
	double d4 = L1AutoCorrelogramDistance(c1->texturedness, c2->texturedness);

/*	double d1 = TanimotoAutoCorrelogramDistance(c1->color, c2->color);
	double d2 = TanimotoAutoCorrelogramDistance(c1->rank, c2->rank);
	double d3 = TanimotoAutoCorrelogramDistance(c1->gradient, c2->gradient);
	double d4 = TanimotoAutoCorrelogramDistance(c1->texturedness, c2->texturedness);
*/	
	//return d1*d1+d2*d2+d3*d3+d4*d4;
	return d1+3*(d2+d3+d4);
	//return d1;
}

/**

Os autocorrelogramas foram extraidos com features independentes entre si,
ou seja temos as probabilidades condicionais:

P(Cor(pixel1) = Cor(pixel2) = c | L1(pixel1,pixel2) = d)                       apelido: P(c|d)
P(Rank(pixel1) = Rank(pixel2) = r | L1(pixel1,pixel2) = d)                     apelido: P(r|d)
P(Gradient(pixel1) = Gradient(pixel2) = g | L1(pixel1,pixel2) = d)             apelido: P(g|d)
P(Texturedness(pixel1) = Texturedness(pixel2) = t | L1(pixel1,pixel2) = d)     apelido: P(t|d)

Para aproximar ao autocorrelograma conjunto, vamos supor que a independencia entre os features eh verdadeira.
Este tipo de suposicao eh comumente feita em modelos Bayesianos, por exemplo, Naive Bayes. Assim, eh possivel
diminuir drasticamente a dimensao do modelo. Ao inves da dimensao ser O(c*r*g*t) sera O(c+r+g+t).
 
Assumindo isso, a conta fica:

P(c,r,g,t | d) = P(c|d) * P(r|d) * P(g|d) * P(t|d) 

Entao eh calculado o L1 das duas probabilidades encontradas.

Foi testado em um conjunto pequeno de dados, e a correlacao (Pearson) entre a probabilidade conjunta real e esta aproximada
foi de 0.93 - nada mal!!!! ;-)  
*/
double L1IndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2) {

	if(c1->color->n != c2->color->n)
		Error("IndependentAutoCorrelogram colors quantization does not match!","L1IndependentAutoCorrelogramDistance");
	if(c1->texturedness->n != c2->texturedness->n)
		Error("IndependentAutoCorrelogram texturedness quantization does not match!","L1IndependentAutoCorrelogramDistance");		
	if(c1->gradient->n != c2->gradient->n)
		Error("IndependentAutoCorrelogram gradient quantization does not match!","L1IndependentAutoCorrelogramDistance");
	if(c1->rank->n != c2->rank->n)
		Error("IndependentAutoCorrelogram rank quantization does not match!","L1IndependentAutoCorrelogramDistance");


	int nc = c1->color->n;
	int nt = c1->texturedness->n;
	int ng = c1->gradient->n;
	int nr = c1->rank->n;
	int nd = c1->color->maxDistance;

	int c,g,t,r,d;
	double diff = 0;
	for(c=0;c<nc;c++)
		for(g=0;g<ng;g++)
			for(t=0;t<nt;t++)
				for(r=0;r<nr;r++)
					for(d=0;d<nd;d++) {
						double prob_c1 = c1->color->h[c*nd+d] * c1->texturedness->h[t*nd+d] * c1->gradient->h[g*nd+d] * c1->rank->h[r*nd+d];
						double prob_c2 = c2->color->h[c*nd+d] * c2->texturedness->h[t*nd+d] * c2->gradient->h[g*nd+d] * c2->rank->h[r*nd+d];
						diff+=fabs(prob_c1-prob_c2);
					}

	return diff;
	
	
}

double TanimotoIndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2) {

	if(c1->color->n != c2->color->n)
		Error("IndependentAutoCorrelogram colors quantization does not match!","L1IndependentAutoCorrelogramDistance");
	if(c1->texturedness->n != c2->texturedness->n)
		Error("IndependentAutoCorrelogram texturedness quantization does not match!","L1IndependentAutoCorrelogramDistance");		
	if(c1->gradient->n != c2->gradient->n)
		Error("IndependentAutoCorrelogram gradient quantization does not match!","L1IndependentAutoCorrelogramDistance");
	if(c1->rank->n != c2->rank->n)
		Error("IndependentAutoCorrelogram rank quantization does not match!","L1IndependentAutoCorrelogramDistance");


	int nc = c1->color->n;
	int nt = c1->texturedness->n;
	int ng = c1->gradient->n;
	int nr = c1->rank->n;
	int nd = c1->color->maxDistance;

	int c,g,t,r,d;
	double dotAA,dotBB,dotAB;
	dotAA = dotBB = dotAB;
	for(c=0;c<nc;c++)
		for(g=0;g<ng;g++)
			for(t=0;t<nt;t++)
				for(r=0;r<nr;r++)
					for(d=0;d<nd;d++) {
						double prob_c1 = c1->color->h[c*nd+d] * c1->texturedness->h[t*nd+d] * c1->gradient->h[g*nd+d] * c1->rank->h[r*nd+d];
						double prob_c2 = c2->color->h[c*nd+d] * c2->texturedness->h[t*nd+d] * c2->gradient->h[g*nd+d] * c2->rank->h[r*nd+d];
						
						dotAB += prob_c1*prob_c2;
						dotAA += prob_c1*prob_c1;
						dotBB += prob_c2*prob_c2;
					}

	return dotAB / (dotAA + dotBB - dotAB);
	
	
}

double L2IndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2) {

	if(c1->color->n != c2->color->n)
		Error("IndependentAutoCorrelogram colors quantization does not match!","L1IndependentAutoCorrelogramDistance");
	if(c1->texturedness->n != c2->texturedness->n)
		Error("IndependentAutoCorrelogram texturedness quantization does not match!","L1IndependentAutoCorrelogramDistance");		
	if(c1->gradient->n != c2->gradient->n)
		Error("IndependentAutoCorrelogram gradient quantization does not match!","L1IndependentAutoCorrelogramDistance");
	if(c1->rank->n != c2->rank->n)
		Error("IndependentAutoCorrelogram rank quantization does not match!","L1IndependentAutoCorrelogramDistance");


	int nc = c1->color->n;
	int nt = c1->texturedness->n;
	int ng = c1->gradient->n;
	int nr = c1->rank->n;
	int nd = c1->color->maxDistance;

	int c,g,t,r,d;
	double diff = 0;
	for(c=0;c<nc;c++)
		for(g=0;g<ng;g++)
			for(t=0;t<nt;t++)
				for(r=0;r<nr;r++)
					for(d=0;d<nd;d++) {
						double prob_c1 = c1->color->h[c*nd+d] * c1->texturedness->h[t*nd+d] * c1->gradient->h[g*nd+d] * c1->rank->h[r*nd+d];
						double prob_c2 = c2->color->h[c*nd+d] * c2->texturedness->h[t*nd+d] * c2->gradient->h[g*nd+d] * c2->rank->h[r*nd+d];
						double d = (prob_c1-prob_c2);
						diff+=d*d;
					}

	return diff;
	
	
}
double IntersectIndependentAutoCorrelogramDistance(IndependentAutoCorrelogram *c1, IndependentAutoCorrelogram *c2) {

	if(c1->color->n != c2->color->n)
		Error("IndependentAutoCorrelogram colors quantization does not match!","InnerProductIndependentAutoCorrelogramDistance");
	if(c1->texturedness->n != c2->texturedness->n)
		Error("IndependentAutoCorrelogram texturedness quantization does not match!","InnerProductIndependentAutoCorrelogramDistance");		
	if(c1->gradient->n != c2->gradient->n)
		Error("IndependentAutoCorrelogram gradient quantization does not match!","InnerProductIndependentAutoCorrelogramDistance");
	if(c1->rank->n != c2->rank->n)
		Error("IndependentAutoCorrelogram rank quantization does not match!","InnerProductIndependentAutoCorrelogramDistance");


	int nc = c1->color->n;
	int nt = c1->texturedness->n;
	int ng = c1->gradient->n;
	int nr = c1->rank->n;
	int nd = c1->color->maxDistance;

	int c,g,t,r,d;
	double dot = 0;
	for(c=0;c<nc;c++)
		for(g=0;g<ng;g++)
			for(t=0;t<nt;t++)
				for(r=0;r<nr;r++)
					for(d=0;d<nd;d++) {
						double prob_c1 = c1->color->h[c*nd+d] * c1->texturedness->h[t*nd+d] * c1->gradient->h[g*nd+d] * c1->rank->h[r*nd+d];
						double prob_c2 = c2->color->h[c*nd+d] * c2->texturedness->h[t*nd+d] * c2->gradient->h[g*nd+d] * c2->rank->h[r*nd+d];
						dot+=MIN(prob_c1,prob_c2);
					}

	return dot;
	
	
}

/************* JOINT autocorrelogram implementation ***************/

int JointCorrelogramPosition(JointAutoCorrelogram *c, int color, int gradient, int texturedness, int rank, int distance) {
    return c->maxDistance*(c->nRank*(c->nTexturedness*(c->nGradient*color + gradient) + texturedness) + rank)+ distance;
}

JointAutoCorrelogram *CreateJointAutoCorrelogram(int nColors, int nGrandient, int nTexturedness, int nRank, int maxDistance) {
	JointAutoCorrelogram *w = (JointAutoCorrelogram *) malloc(sizeof(JointAutoCorrelogram));
	if (w != NULL) {
		w->nColors = nColors;
		w->nGradient = nGrandient;
		w->nTexturedness = nTexturedness;
		w->nRank = nRank;
		w->maxDistance = maxDistance;
		w->h = (AUTOCORRE_TYPE *) calloc(nColors*nGrandient*nTexturedness*nRank*maxDistance, sizeof(AUTOCORRE_TYPE));
		if (w->h == NULL)
			Error(MSG1, "CreateJointAutoCorrelogram");
	} else
		Error(MSG1, "CreateJointAutoCorrelogram");
	return (w);	
}
void DestroyJointAutoCorrelogram(JointAutoCorrelogram **c) {
	JointAutoCorrelogram *aux;

	aux = *c;
	if (aux != NULL) {
		if(aux->h!=NULL)free(aux->h);
		free(aux);
		*c = NULL;
	}	
}
JointAutoCorrelogram *ReadStreamJointAutoCorrelogram(FILE *fp) {
	JointAutoCorrelogram *c = NULL;
	int i, nColors,nGrandient,nTexturedness,nRank,maxDistance;
	AUTOCORRE_TYPE v;

	if (fp != NULL) {
		fscanf(fp, "%d %d %d %d %d\n", &nColors, &nTexturedness, &nGrandient, &nRank, &maxDistance);
		c = CreateJointAutoCorrelogram(nColors, nGrandient, nTexturedness, nRank, maxDistance);
		int n = nColors*nGrandient*nTexturedness*nRank*maxDistance;
		int j=0;
		for (i=0; i<n; i++) {
			fscanf(fp, "%d %f", &j, &v);
			if(j == v && v == -1) break;
			c->h[j] = v;
		}
	}
	return c;	
}
JointAutoCorrelogram *ReadFileJointAutoCorrelogram(char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", filename);
		exit(-1);
	}
	JointAutoCorrelogram *c = ReadStreamJointAutoCorrelogram(fp);
	fclose(fp);
	return c;	
}
void WriteStreamJointAutoCorrelogram(JointAutoCorrelogram *c, FILE *fp){
	if(c == NULL) return;
	if (fp != NULL) {
		fprintf(fp, "%d %d %d %d %d\n", c->nColors, c->nTexturedness, c->nGradient, c->nRank, c->maxDistance);
		int n = c->nColors * c->nTexturedness * c->nGradient * c->nRank * c->maxDistance;
		int i;
		for (i=0; i<n; i++) {
			if(c->h[i] > 0.0){
				fprintf(fp, "%d %f ", i,c->h[i]);
			}
		}
		fprintf(fp, "-1 -1\n");
	}	
}
void WriteFileJointAutoCorrelogram(JointAutoCorrelogram *c, char *filename){
	FILE *fp;
	if(c == NULL) return;

	fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open %s\n", filename);
		exit(-1);
	}
	WriteStreamJointAutoCorrelogram(c, fp);
	fclose(fp);	
}
double L1JointAutoCorrelogramDistance(JointAutoCorrelogram *c1, JointAutoCorrelogram *c2) {
	if(c1->nColors != c2->nColors)
		Error("JointAutoCorrelogram colors quantization does not match!","L1JointAutoCorrelogramDistance");
	if(c1->nTexturedness != c2->nTexturedness)
		Error("JointAutoCorrelogram texturedness quantization does not match!","L1JointAutoCorrelogramDistance");		
	if(c1->nGradient != c2->nGradient)
		Error("JointAutoCorrelogram gradient quantization does not match!","L1JointAutoCorrelogramDistance");
	if(c1->nRank != c2->nRank)
		Error("JointAutoCorrelogram rank quantization does not match!","L1JointAutoCorrelogramDistance");
	if(c1->maxDistance != c2->maxDistance)
		Error("JointAutoCorrelogram distance quantization does not match!","L1JointAutoCorrelogramDistance");
	
	double total = 0.0;
	int n = c1->nColors * c1->nTexturedness * c1->nGradient * c1->nRank * c1->maxDistance;
	int i;for (i=0; i<n; i++)
		total+=fabs(c1->h[i] - c2->h[i]);
	return total;	
}
