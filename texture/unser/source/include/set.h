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
#ifndef _SET_H_
#define _SET_H_

typedef struct _set {
  int *p;
  int *order;
  int n;
} Set;

Set  *CreateSet(int n);
void  DestroySet(Set **set);
Set  *ReadSet(char *filename);
void  WriteSet(Set *set, char *filename);
void  MakeSet(Set *set, int x);
void  Union(Set *set, int x, int y);
void  Link(Set *set, int x, int y);
int   FindSet(Set *set, int x);

#endif
