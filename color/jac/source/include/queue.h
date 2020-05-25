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
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "common.h"

#define FIFOBREAK 0
#define LIFOBREAK 1

#define SetTieBreak(a,b) a->C.tiebreak=b

typedef struct _node {
  int  next;  /* next node */
  int  prev;  /* prev node */
  char color; /* WHITE=0, GRAY=1, BLACK=2 */
} Node;

typedef struct _doublylinkedlists {
  Node *elem;  /* all possible doubly-linked lists of the circular queue */
  int nelems;  /* total number of elements */
} DoublyLinkedLists;

typedef struct _circularqueue {
  int *first;  /* list of the first elements of each doubly-linked list */
  int *last;   /* list of the last  elements of each doubly-linked list  */
  int nbuckets;  /* number of buckets in the circular queue */
  int current;   /* current bucket */
  char tiebreak; /* 1 is LIFO, 0 is FIFO (default) */
} CircularQueue;

typedef struct _queue { /* Priority queue by Dial implemented as
                           proposed by A. Falcao */
  CircularQueue C;
  DoublyLinkedLists L;
} Queue;

Queue *CreateQueue(int nbuckets, int nelems);
void   DestroyQueue(Queue **Q);
void   ResetQueue(Queue *Q);
int    EmptyQueue(Queue *Q);
void   InsertQueue(Queue *Q, int bucket, int elem);
int    RemoveQueue(Queue *Q);
void   RemoveQueueElem(Queue *Q, int elem, int bucket);
void   UpdateQueue(Queue *Q, int elem, int from, int to);

#endif







