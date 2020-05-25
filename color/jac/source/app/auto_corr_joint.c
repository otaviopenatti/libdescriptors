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
	JointAutoCorrelogram *descr=NULL;

	cimg = ReadCImage(img_path);

	descr = JointAutoCorrelogramImage(cimg);

	WriteFileJointAutoCorrelogram(descr, fv_path);
	DestroyJointAutoCorrelogram(&descr);
	DestroyCImage(&cimg);

}

double Distance(char *fv1_path, char *fv2_path) {
	JointAutoCorrelogram *descr1=NULL;
	JointAutoCorrelogram *descr2=NULL;
	double distance;

	descr1 = ReadFileJointAutoCorrelogram(fv1_path);
	descr2 = ReadFileJointAutoCorrelogram(fv2_path);

	distance = (double) L1JointAutoCorrelogramDistance(descr1, descr2);

	DestroyJointAutoCorrelogram(&descr1);
	DestroyJointAutoCorrelogram(&descr2);

	return distance;
}
