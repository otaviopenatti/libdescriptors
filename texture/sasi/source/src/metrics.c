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
#include "metrics.h"

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

double DotDistance(FeatureVector1D *v1, FeatureVector1D *v2) { 
  int i;
  double v1v1 = 0;
  double v1v2 = 0;
  double v2v2 = 0;
  double distance;

  for (i = 0; i < v1->n ; i++){
    v1v1 += v1->X[i]*v1->X[i];
    v1v2 += v1->X[i]*v2->X[i]; 
    v2v2 += v2->X[i]*v2->X[i];

    //printf("v1[%d]=%f\t", i, v1->X[i]);
    //printf("v2[%d]=%f\n", i, v2->X[i]);
  }

  if (v1v1==0 && v2v2==0) distance = 0;
  else distance = 1 - (v1v2 / (v1v1 + v2v2 - v1v2));
  
  //distancia antes de corrigir o bug de distancias negativas
  //if (v1v2 == 0) distance = v1v1 + v2v2;
  //else distance = (v1v1 + v2v2 - v1v2)/v1v2 -1;

  return (distance);
}

