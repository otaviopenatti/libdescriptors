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
#ifndef _GABOR_H_
#define _GABOR_H_

#include <assert.h>
#include "dimage.h"
#include "image.h"
#include "spectrum.h"
#include "feature.h"
#include "common.h"

  #ifndef NSZE
    // number of scales during image decomposition.
    #define NSZE 128
  #endif

  #ifndef NSCALES
    // number of scales during image decomposition.
    #define NSCALES 4
  #endif

  #ifndef NORIENTATIONS
    // number of orientations during image decomposition.
    #define NORIENTATIONS 6
  #endif

  #ifndef DUH
    // highest frequency.
    #define DUH 0.4
  #endif

  #ifndef DUL
    // lowest frequency.
    #define DUL 0.05
  #endif
  
  typedef struct{
      DImage * m_pReal;
      DImage  * m_pImag;
      int m_nSze;
  }gabor;

  Features* GaborFeats(Features* feats);

#endif
