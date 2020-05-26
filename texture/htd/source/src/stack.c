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
#include "../include/stack.h"

void PushStack(Stack **S, int elem)
{
  Stack *p=NULL;

  p = (Stack *) calloc(1,sizeof(Stack));
  if (p == NULL) Error(MSG1,"Push");
  if (*S == NULL){
    p->elem  = elem;
    p->next  = NULL;
  }else{
    p->elem  = elem;
    p->next  = *S;
  }
  *S = p;
}

int PopStack(Stack **S)
{
  Stack *p;
  int elem=NIL;

  if (*S != NULL){
    p    =  *S;
    elem = p->elem;
    *S   = p->next;
    free(p);
  }

  return(elem);
}

void DestroyStack(Stack **S)
{
  Stack *p;
  while(*S != NULL){
    p = *S;
    *S = p->next;
    free(p);
  }
}

