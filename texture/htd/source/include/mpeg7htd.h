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
#ifndef _MPEG7HTD_H_
#define _MPEG7HTD_H_

#include "cimage.h"
#include "matrix.h"

typedef struct {
	int scale, orientation;
	float *media, *desvio;
} Mpeg7HTDDescriptor;

Mpeg7HTDDescriptor *ExtractMpeg7HTDDescriptor(CImage *rgbImage);
void WriteFileMpeg7HTDDescriptor(Mpeg7HTDDescriptor *descr, char *fv_path);
void DestroyMpeg7HTDDescriptor(Mpeg7HTDDescriptor **descr);
Mpeg7HTDDescriptor *ReadFileMpeg7HTDDescriptor(char *fv_path);
double DistanceMpeg7HTDDescriptor(Mpeg7HTDDescriptor *descr1, Mpeg7HTDDescriptor *descr2);

#endif /*_MPEG7HTD_H_*/

