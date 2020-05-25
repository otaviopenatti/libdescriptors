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
  CImage *cimg=NULL;
  Histogram *csd=NULL;

  cimg = ReadCImage(img_path);

  csd = CSD(cimg);

  WriteFileHistogram(csd,fv_path);
  DestroyHistogram(&csd);
  DestroyCImage(&cimg);

}

double Distance(char *fv1_path, char *fv2_path)
{
    Histogram *csd1=NULL;
    Histogram *csd2=NULL;
    double distance;

    csd1 = ReadFileHistogram(fv1_path);
    csd2 = ReadFileHistogram(fv2_path);

    distance = CSDDistance(csd1, csd2);

    DestroyHistogram(&csd1);
    DestroyHistogram(&csd2);

    return distance;
}
