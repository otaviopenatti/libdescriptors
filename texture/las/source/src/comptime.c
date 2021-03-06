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
#include "comptime.h"

timer *Tic(){ /* It marks the initial time */
  timer *tic=NULL;
  tic = (timer *)malloc(sizeof(timer));
  gettimeofday(tic,NULL); 
  return(tic);
}

timer *Toc(){ /* It marks the final time */
  timer *toc=NULL;
  toc = (timer *)malloc(sizeof(timer));
  gettimeofday(toc,NULL);
  return(toc);
}

float CTime(timer *tic, timer *toc) /* It computes the time difference */
{ 
  float t=0.0;
  if ((tic!=NULL)&&(toc!=NULL)){
    t = (toc->tv_sec-tic->tv_sec)*1000.0 + 
      (toc->tv_usec-tic->tv_usec)*0.001;
    free(tic);free(toc);
    tic=NULL; toc=NULL;
  }
  return(t);
}



