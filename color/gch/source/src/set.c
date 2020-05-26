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
#include "set.h"

#include "common.h"

Set *CreateSet(int n)
{
  Set *set=NULL;

  set = (Set *)calloc(1,sizeof(Set));
  if (set != NULL){
    set->p     = AllocIntArray(n);
    set->order = AllocIntArray(n);
    set->n     = n;
  }
  else {
    Error(MSG1,"CreateSet");
  }

  return(set);
}

void DestroySet(Set **set)
{
  Set *aux;

  aux = *set;
  if (aux != NULL){
    if (aux->p != NULL)     free(aux->p);
    if (aux->order != NULL) free(aux->order);
    free(aux);
    *set = NULL;
  }
}

Set *ReadSet(char *filename)
{
  FILE *fp;
  Set *set;
  int i, n;

  fp = fopen(filename,"r");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  fscanf(fp,"%d",&n);
  set = CreateSet(n);
  for (i=0;i<n;i++)
    fscanf(fp,"%d %d",&set->p[i],&set->order[i]);
  fclose(fp);

  return(set);
}

void WriteSet(Set *set, char *filename)
{
  FILE *fp;
  int i;

  fp = fopen(filename,"w");
  if (fp == NULL){
    fprintf(stderr,"Cannot open %s\n",filename);
    exit(-1);
  }

  fprintf(fp,"%d\n",set->n);
  for (i=0;i<set->n;i++)
    fprintf(fp,"%d %d\n",set->p[i],set->order[i]);
  fclose(fp);
}

void MakeSet(Set *set, int x)
{
  set->p[x] = x;
  set->order[x] = 0;
}

void Union(Set *set, int x, int y)
{
  Link(set, FindSet(set, x), FindSet(set,y));
}

void Link(Set *set, int x, int y)
{
  if (set->order[x] > set->order[y])
    set->p[y] = x;
  else {
    set->p[x] = y;
    if (set->order[x] == set->order[y])
      set->order[y]++;
  }
}

int FindSet(Set *set, int x)
{
  if (x != set->p[x])
    set->p[x] = FindSet(set,set->p[x]);
  return(set->p[x]);
}
