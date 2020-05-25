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
#ifndef COLOR_H
#define COLOR_H

#include "common.h"

/* color-related functions */

int triplet(int a,int b,int c);

#define t0(a) ((a>>16)&0xff)
#define t1(a) ((a>>8)&0xff)
#define t2(a) (a&0xff)

int RGB2YCbCr(int v);
int YCbCr2RGB(int v);

int RGB2HSV(int vi);
int HSV2RGB(int vi);

int RGB2XYZ(int vi);
int XYZ2RGB(int vi);

int XYZ2LAB(int vi);
int LAB2XYZ(int vi);

/* lightens a color by (1.10 ^ n) */
int          lighter(int c, int n);

/* darkens a color by (0.90 ^ n) */
int          darker(int c, int n);

/* returns the inverse in RGB-space (255-R,255-G,255-B) */
int          inverseColor(int c);

/* merges two colors (1-ratio) of a and ratio of b, in RGB-space */
int          mergeColorsRGB(int a,int b,float ratio);

/* merges two colors (1-ratio) of a and ratio of b, in YCbCr-space */
int          mergeColorsYCbCr(int a,int b,float ratio);

/* returns RGB triplet with R=c, G=c, B=c */
int          gray(int c);

/* returns a random color */
extern int          randomColor();

double HSVColorDistance(int color1, int color2);
double RGBColorDistance(int color1, int color2);

#endif
