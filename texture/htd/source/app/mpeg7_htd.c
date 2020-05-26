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
#include "plugin.h"
#include "../include/libdescriptors.h"

void Extraction(char *img_path, char *fv_path) {
	CImage *cimg=NULL;
	
	Mpeg7HTDDescriptor *descr=NULL;
	
	cimg = ReadCImage(img_path);

	descr = ExtractMpeg7HTDDescriptor(cimg);

	WriteFileMpeg7HTDDescriptor(descr, fv_path);
	DestroyMpeg7HTDDescriptor(&descr);
	DestroyCImage(&cimg);

}

double Distance(char *fv1_path, char *fv2_path) {
	Mpeg7HTDDescriptor *descr1=NULL;
	Mpeg7HTDDescriptor *descr2=NULL;
	double distance;

	descr1 = ReadFileMpeg7HTDDescriptor(fv1_path);
	descr2 = ReadFileMpeg7HTDDescriptor(fv2_path);

	distance = (double) DistanceMpeg7HTDDescriptor(descr1, descr2);

	DestroyMpeg7HTDDescriptor(&descr1);
	DestroyMpeg7HTDDescriptor(&descr2);

	return distance;
}
