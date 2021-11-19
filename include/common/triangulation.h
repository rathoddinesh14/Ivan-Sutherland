/*
this file contains triangulation data needed for marching cubes
*/

#ifndef __TRIANGULATION_H
#define __TRIANGULATION_H

static const int tetraTable[16][12] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 0, 2, 0, 3, -1, -1, -1, -1, -1, -1},
    {1, 0, 1, 3, 1, 2, -1, -1, -1, -1, -1, -1},
    {3, 0, 1, 3, 2, 0, 2, 0, 2, 1, 1, 3},
    {2, 0, 2, 1, 2, 3, -1, -1, -1, -1, -1, -1},
    {3, 0, 1, 0, 1, 2, 1, 2, 3, 2, 0, 3},
    {1, 0, 1, 3, 0, 2, 3, 1, 3, 2, 0, 2},
    {3, 0, 3, 2, 3, 1, -1, -1, -1, -1, -1, -1},
    {3, 0, 3, 2, 3, 1, -1, -1, -1, -1, -1, -1},
    {1, 0, 1, 3, 0, 2, 3, 1, 3, 2, 0, 2},
    {3, 0, 1, 0, 1, 2, 1, 2, 3, 2, 0, 3},
    {2, 0, 2, 1, 2, 3, -1, -1, -1, -1, -1, -1},
    {3, 0, 1, 3, 2, 0, 2, 0, 2, 1, 1, 3},
    {1, 0, 1, 3, 1, 2, -1, -1, -1, -1, -1, -1},
    {0, 1, 0, 2, 0, 3, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

#endif