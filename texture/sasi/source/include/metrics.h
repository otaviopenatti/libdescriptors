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
#ifndef _METRICS_H_
#define _METRICS_H_

#include "featurevector.h"

typedef double (* Metric)(FeatureVector1D *, FeatureVector1D *);

double ManhattanDistance(FeatureVector1D *v1, FeatureVector1D *v2);
double EuclideanDistance(FeatureVector1D *v1, FeatureVector1D *v2);
double ChebyshevDistance(FeatureVector1D *v1, FeatureVector1D *v2);
double DotDistance(FeatureVector1D *v1, FeatureVector1D *v2);

#endif



