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
#include "libdescriptors.h"

void Extraction(char *img_path, char *fv_path)
{
    double *h=NULL;

    CImage *cimg = ReadCImage(img_path);
    h = QCCH(cimg, RAIO);

    WriteFileBin(h, fv_path, QTD_BINS);

    DestroyCImage(&cimg);
    free(h);
}

double Distance(char *fv1_path, char *fv2_path)
{
    double *ha=NULL, *hb=NULL;
    double distance;

    ha = ReadFileBin(fv1_path, QTD_BINS);
    hb = ReadFileBin(fv2_path, QTD_BINS);

    distance=Dist_Measure(ha,hb);

    free(ha);
    free(hb);

    return distance;
}


