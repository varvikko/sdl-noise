#ifndef MAP_H
#define MAP_H

#define T Map_T
typedef struct T* T;

extern T    Map_generate(int width, int height, int intensity);
extern int  Map_cell_type(T, int x, int y);
extern void Map_release(T);

#undef T

#endif
