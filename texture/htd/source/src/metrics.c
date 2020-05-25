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
#include "../include/metrics.h"

double ManhattanDistance(FeatureVector1D *v1, FeatureVector1D *v2) {
  int i;
  double sum = 0.0;
  double z = 0.0;

  for (i = 0; i < v1->n ; i++){
    z = fabs(v1->X[i] - v2->X[i]);
    sum = sum + z;
  }
  return (sum);
}

double EuclideanDistance(FeatureVector1D *v1, FeatureVector1D *v2) {
  int i;
  double sum = 0.0;
  double z = 0.0;

  for (i = 0; i < v1->n ; i++){
    z = v1->X[i] - v2->X[i];
    sum += z*z;
  }
  sum = sqrt(sum);
  return (sum);
}

double ChebyshevDistance(FeatureVector1D *v1, FeatureVector1D *v2) {
  int i;
  double max = -1.0;
  double z = 0.0;

  for (i = 0; i < v1->n ; i++){
    z = fabs(v1->X[i] - v2->X[i]);
    if (max < z) max = z;
  }
  return (max);
}

