#include "util.h"

#include <stdlib.h>
#include <time.h>

void seed(void)
{
    srand(time(NULL));
}

double rand01(void)
{
    return (rand() % 10001) / 10000.0;
}

void index_to_coord(int index, int *x, int *y, int width)
{
    *x = index - index / width - 1;
    *y = index / width - 1;
}

int coord_to_index(int x, int y, int width)
{
    return y * width + x;
}
