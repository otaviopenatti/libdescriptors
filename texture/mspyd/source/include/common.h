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
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <float.h>
#include <time.h>


/* Error messages */

#define MSG1  "Cannot allocate memory space in"
#define MSG2  "Cannot open file in"

/* Common data types to all programs */ 

#ifndef _WIN32
typedef enum boolean {false,true} bool;
#else
typedef unsigned short ushort;
#endif

typedef struct timeval timer;
typedef unsigned char uchar;
typedef unsigned long ulong;

typedef int* ap_int;
typedef double* ap_double;

/* Common definitions */


#define PI          3.1415927
#define INTERIOR    0
#define EXTERIOR    1
#define BOTH        2
#define WHITE       0 
#define GRAY        1
#define BLACK       2
#define NIL        -1
#define INCREASING  1
#define DECREASING  0
#define Epsilon     1E-05       

/* Common operations */

#ifndef ABS
#define ABS(x) (((x) > 0)?(x):(-(x)))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y))?(x):(y))
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y))?(x):(y))
#endif

#define ROUND(x) ((x < 0)?(int)(x-0.5):(int)(x+0.5))

#define SIGN(x) ((x >= 0)?1:-1)

char   *AllocCharArray(int n);   /* It allocates 1D array of n characters */
uchar  *AllocUCharArray(int n);  /* It allocates 1D array of n characters */
ushort *AllocUShortArray(int n); /* It allocates 1D array of n characters */
ulong  *AllocULongArray(int n);  /* It allocates 1D array of n integers */
bool   *AllocBoolArray(int n);   /* It allocated 1D array of n characters */
int    *AllocIntArray(int n);   /* It allocates 1D array of n integers */
float  *AllocFloatArray(int n); /* It allocates 1D array of n floats   */
double *AllocDoubleArray(int n);/* It allocates 1D array of n doubles  */

void Error(char *msg,char *func); /* It prints error message and exits
                                     the program. */
void Warning(char *msg,char *func); /* It prints warning message and
                                       leaves the routine. */
void Change(size_t w, void *a, void *b);/* It changes content between a and b */

int NCFgets(char *s, int m, FILE *f); /* It skips # comments */

int RandomInteger (int low, int high);

int SafeMod(int a, int n);

#endif
