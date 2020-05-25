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
#ifndef _MATHEMATICS_H_
#define _MATHEMATICS_H_

#include "image.h"

Image  *Diff(Image *img1,Image *img2); 
Image  *Sum(Image *img1, Image *img2);
Image  *SQRT(Image *img);
Image  *And(Image *img1, Image *img2);
Image  *Or(Image *img1, Image *img2);
Image  *Add(Image *img, int value);
Image  *Sub(Image *img, int value);
Image  *Complement(Image *img);
Image  *Div(Image *img1,Image *img2);
Image  *Mult(Image *img1,Image *img2);
Image  *Abs(Image *img);
Image  *Average(Image *img1, Image *img2);
Image  *Log(Image *img);

#endif


