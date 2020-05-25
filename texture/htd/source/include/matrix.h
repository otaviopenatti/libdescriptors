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
#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct MatrixStruct 
{
	double **data;
	int height, width;
} Matrix;

typedef struct uc_MatrixStruct 
{
	unsigned char **data;
	int height, width;
} uc_Matrix;

typedef struct i_MatrixStruct 
{
	int **data;
	int height, width;
} i_Matrix;

typedef struct RegionStruct
{
	int id;
	int number;
	int *member_i;
	int *member_j;
} Region;

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

#define TRADITIONAL 1

#ifdef ANSI
void nrerror(char *error_text);
double *dvector(int nl, int nh);
float *vector(int nl, int nh);
void free_dvector(double *v, int nl, int nh);
void free_vector(float *v, int nl, int nh);
float **matrix(int nrl,int nrh,int ncl,int nch);
void free_matrix(float **m,int nrl,int nrh,int ncl,int nch);
double **dmatrix(int nrl, int nrh, int ncl, int nch);
void free_dmatrix(double **m, int nrl, int nrh, int ncl, int nch);
double log2(double a);
void sort(double *Y, int *I, double *A, int length);
void minimun(double *Y, int *I, double *A, int length);
void Mat_Abs(Matrix *A);
void Mat_Mean(double *mean, Matrix *A);
void Vector_Mat(float *a, Matrix *A);
void Mat_Vector(Matrix *A, float *a);
void Mat_Shift(Matrix *A, Matrix *B, int side);
void Mat_Zeros(Matrix *A);
void Mat_Zeros_uc(uc_Matrix *A);
void Mat_Zeros_i(i_Matrix *A);
void CreateMatrix(Matrix **M, int hei, int wid);
void FreeMatrix(Matrix *M);
void Create_i_Matrix(i_Matrix **M, int hei, int wid);
void Free_i_Matrix(i_Matrix *M);
void Create_uc_Matrix(uc_Matrix **M, int hei, int wid);
void Free_uc_Matrix(uc_Matrix *M);
void Mat_FFT2(Matrix *Output_real, Matrix *Output_imag, Matrix *Input_real, Matrix *Input_imag);
void Mat_IFFT2(Matrix *Output_real, Matrix *Output_imag, Matrix *Input_real, Matrix *Input_imag);
void four2(double **fftr, double **ffti, double **rdata, double **idata, int rs, int cs, int isign);
void four1(double *data, int nn, int isign);
void Mat_Copy(Matrix *A, Matrix *B, int h_target, int w_target, int h_begin, int w_begin, int h_end,
int w_end);
void Mat_uc_Copy(uc_Matrix *A, uc_Matrix *B, int h_target, int w_target, int h_begin, int w_begin,
int h_end, int w_end);
void Mat_i_Copy(i_Matrix *A, i_Matrix *B, int h_target, int w_target, int h_begin, int w_begin, int
h_end, int w_end);
void Mat_Product(Matrix *A, Matrix *B, Matrix *C);
void Mat_Sum(Matrix *A, Matrix *B, Matrix *C);
void Mat_Substract(Matrix *A, Matrix *B, Matrix *C);
void Mat_Fliplr(Matrix *A);
void Mat_Flipud(Matrix *A);
void Mat_uc_Fliplr(uc_Matrix *A);
void Mat_uc_Flipud(uc_Matrix *A);
#endif
#ifdef TRADITIONAL
void nrerror();
double *dvector();
float *vector();
void free_dvector();
void free_vector();
float **matrix();
void free_matrix();
double **dmatrix();
void free_dmatrix();
double log2();
void sort();
void minimun();
void Mat_Abs();
void Mat_Mean();
void Mat_Vector();
void Mat_Shift();
void Mat_Zeros();
void Mat_Zeros_uc();
void Mat_Zeros_i();
void CreateMatrix();
void FreeMatrix();
void Create_i_Matrix();
void Free_i_Matrix();
void Create_uc_Matrix();
void Free_uc_Matrix();
void Mat_FFT2();
void Mat_IFFT2();
void four2();
void four1();
void Mat_Copy();
void Mat_uc_Copy();
void Mat_i_Copy();
void Mat_Product();
void Mat_Sum();
void Mat_Substract();
void Mat_Fliplr();
void Mat_Flipud();
void Mat_uc_Fliplr();
void Mat_uc_Flipud();
#endif // ANSI
#endif // _MATRIX_H_
