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
#ifndef _STEERABLEPYRAMID_H_
#define _STEERABLEPYRAMID_H_

#include "dimage.h"
#include "image.h"
#include "spectrum.h"
#include "feature.h"
#include "common.h"
#include "featurevector.h"
#include <math.h>

//#define SPORIENTATIONS 6
//#define SPSCALES 4
#define SPORIENTATIONS 4
#define SPSCALES 2


/** Enum type used to define which filter will be created in
 *  CreateFilter
 **/
typedef enum filtertype{BandPass,LowPass} FilterType;


/** Filter creation and use **/

/** Extracts SP features from all features in feats.
 *  By default 4 scales and 6 orientations are used, but that can be
 *  changed by altering SPSCALES and SPORIENTATIONS.
 *
 *  @param feats Features* already extracted from the image (e.g. Lab, RGB, etc.).
 *
 *  @return a new Features* struct which will contain feats->nfeats*SCALES*SPORIENTATIONS features,
 *      because each existent feature in feats will be filtered using SPSCALES and SPORIENTATIONS
 **/
 
Features* SteerablePyramidFeats(Features* feats);
void WriteFileFeatureVector1D_bin(FeatureVector1D *fv,char *filename); //gera vetor em formato binario
FeatureVector1D *ReadFileFeatureVector1D_bin(char *filename);

#endif // _STEERABLEPYRAMID_H_
