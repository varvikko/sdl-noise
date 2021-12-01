#include "map.h"

#include <stdlib.h>
#include <math.h>

#include "util.h"

#define T Map_T

struct Map_T {
    int *data;
    int width;
    int height;
};

typedef struct
{
    int x;
    int y;
    int type;
} point;

T Map_generate(int width, int height, int intensity)
{
    T map = malloc(sizeof(struct Map_T));
    if (!map)
    {
        return 0;
    }

    map->data = malloc(sizeof(int) * width * height);
    map->width = width;
    map->height = height;

    point center_points[intensity];
    for (int i = 0; i < intensity; i++)
    {
        int x = floor(rand01() * width);
        int y = floor(rand01() * height);
        int type = rand01() > 0.5;

        point cp = { x, y, type };
        center_points[i] = cp;
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            double shortest = 1e10;
            int nearest_type;

            for (int i = 0; i < intensity; i++)
            {
                point cp = center_points[i];

                double x_diff = x - cp.x;
                double y_diff = y - cp.y;

                double distance = pow(x_diff, 2) + pow(y_diff, 2);

                if (distance < shortest)
                {
                    shortest = distance;
                    nearest_type = cp.type;
                }
            }

            map->data[coord_to_index(x, y, width)] = nearest_type;
        }
    }

    return map;
}

int Map_cell_type(T map, int x, int y)
{
    return map->data[coord_to_index(x, y, map->width)];
}

void Map_release(T map)
{
    if (map) {
        if (map->data) {
            free(map->data);
            map->data = NULL;
        }

        free(map);
        map = NULL;
    }
}

#undef T
