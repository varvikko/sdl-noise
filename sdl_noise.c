#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture.h"
#include "map.h"

#define WINDOW_TITLE "SDL Noise"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FPS 60

#define TOP 1
#define BOTTOM 2
#define LEFT 4
#define RIGHT 8

#define GRASS 15
#define GRASS_TOP_LEFT 5
#define GRASS_TOP_RIGHT 9
#define GRASS_TOP 1
#define GRASS_TOP_LEFT_RIGHT 13
#define GRASS_LEFT 7
#define GRASS_RIGHT 11
#define GRASS_TOP_BOTTOM 3
#define GROUND_BOTTOM 2
#define GROUND_BOTTOM_LEFT 6
#define GROUND_BOTTOM_RIGHT 10
#define GROUND_LEFT 4
#define GROUND_RIGHT 8
#define GROUND_LEFT_RIGHT 12
#define GROUND_BOTTOM_LEFT_RIGHT 14

int initialize(void);
Texture_T load_tile(const char *);
void run(void);
void update(void);
void draw(void);
void release(void);

SDL_Window *window;
SDL_Renderer *renderer;

Texture_T grass;
Texture_T grass_left;
Texture_T grass_right;
Texture_T grass_top;
Texture_T grass_top_bottom;
Texture_T grass_top_left;
Texture_T grass_top_right;
Texture_T grass_top_left_right;

Texture_T ground;
Texture_T ground_bottom;
Texture_T ground_bottom_left;
Texture_T ground_bottom_left_right;
Texture_T ground_bottom_right;
Texture_T ground_left;
Texture_T ground_left_right;
Texture_T ground_right;

Map_T map;

float cam_vel_x;
float cam_vel_y;

float cam_x;
float cam_y;

int mouse_x;
int mouse_y;

float zoom = 1.0f;

int main(void)
{
    if (!initialize())
    {
        return 0;
    }

    grass = Texture_new("assets/grass.png", renderer);
    grass_left = Texture_new("assets/grass_left.png", renderer);
    grass_right = Texture_new("assets/grass_right.png", renderer);
    grass_top = Texture_new("assets/grass_top.png", renderer);
    grass_top_bottom = Texture_new("assets/grass_top_bottom.png", renderer);
    grass_top_left = Texture_new("assets/grass_top_left.png", renderer);
    grass_top_right = Texture_new("assets/grass_top_right.png", renderer);
    grass_top_left_right = Texture_new("assets/grass_top_left_right.png", renderer);

    ground = Texture_new("assets/ground.png", renderer);
    ground_bottom = Texture_new("assets/ground_bottom.png", renderer);
    ground_bottom_left = Texture_new("assets/ground_bottom_left.png", renderer);
    ground_bottom_left_right = Texture_new("assets/ground_bottom_left_right.png", renderer);
    ground_bottom_right = Texture_new("assets/ground_bottom_right.png", renderer);
    ground_left = Texture_new("assets/ground_left.png", renderer);
    ground_left_right = Texture_new("assets/ground_left_right.png", renderer);
    ground_right = Texture_new("assets/ground_right.png", renderer);

    map = Map_generate(100, 100, 200);
    if (!map)
    {
        return 0;
    }

    run();

    Texture_release(grass);
    Texture_release(grass_left);
    Texture_release(grass_right);
    Texture_release(grass_top);
    Texture_release(grass_top_bottom);
    Texture_release(grass_top_left);
    Texture_release(grass_top_right);
    Texture_release(grass_top_left_right);

    Texture_release(ground);
    Texture_release(ground_bottom);
    Texture_release(ground_bottom_left);
    Texture_release(ground_bottom_left_right);
    Texture_release(ground_bottom_right);
    Texture_release(ground_left);
    Texture_release(ground_left_right);
    Texture_release(ground_right);

    Map_release(map);
    release();
}

int initialize(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Failed to initialize SDL.\n");
        return 0;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "Failed to create SDL window.\n");
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        fprintf(stderr, "Failed to create SDL renderer.\n");
        return 0;
    }

    uint img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags))
    {
        fprintf(stderr, "Failed to initialize SDL image.\n");
        return 0;
    }
    return 1;
}

Texture_T load_tile(const char *path)
{
    Texture_T tile = Texture_new(path, renderer);
    if (!tile)
    {
        fprintf(stderr, "Failed to load %s.\n", path);
        return NULL;
    }

    return tile;
}

void run(void)
{
    SDL_Event event;
    int running = 1;
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

    while (running)
    {
        uint64_t frame_start_ticks = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    Map_release(map);
                    map = Map_generate(100, 100, 200);
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&mouse_x, &mouse_y);
                // printf("%d %d\n", mouse_x, mouse_y);
            }
        }

        if (keyboard[SDL_SCANCODE_LEFT])
        {
            cam_x += 10;
        }
        if (keyboard[SDL_SCANCODE_RIGHT])
        {
            cam_x -= 10;
        }
        if (keyboard[SDL_SCANCODE_UP])
        {
            cam_y += 10;
        }
        if (keyboard[SDL_SCANCODE_DOWN])
        {
            cam_y -= 10;
        }
        if (keyboard[SDL_SCANCODE_KP_PLUS])
        {
            zoom += 0.0125f;
        }
        if (keyboard[SDL_SCANCODE_KP_MINUS])
        {
            zoom -= 0.0125f;
        }

        update();
        draw();

        uint64_t frame_duration = SDL_GetTicks() - frame_start_ticks;
        if (frame_duration < 1000.0 / FPS)
        {
            SDL_Delay(1000.0 / FPS - frame_duration);
        }
    }
}

void update(void)
{
    cam_x += cam_vel_x;
    cam_y += cam_vel_y;

    cam_vel_x *= 0.98f;
    cam_vel_y *= 0.98f;
}

void draw(void)
{
    SDL_SetRenderDrawColor(renderer, 0xdf, 0xf6, 0xf5, 0xff);
    SDL_RenderClear(renderer);

    int tile_width = 18;
    int tile_height = 18;

    int cnt = 0;
    for (int y = 0; y < 100; y++)
    {
        for (int x = 0; x < 100; x++)
        {
            int cell_type = Map_cell_type(map, x, y);
            float draw_x = x * tile_width * zoom;
            float draw_y = y * tile_height * zoom;

            if (mouse_x >= draw_x && mouse_x < draw_x + tile_width && mouse_y >= draw_y && mouse_y < draw_y + tile_height)
            {
                printf("mouse inside block at %d %d\n", (int)draw_x / tile_width, (int)draw_y / tile_height);

                int top = Map_cell_type(map, x, y - 1);
                int bottom = Map_cell_type(map, x, y + 1);
                int left = Map_cell_type(map, x - 1, y);
                int right = Map_cell_type(map, x + 1, y);

                int top_left = Map_cell_type(map, x - 1, y - 1);
                int top_right = Map_cell_type(map, x + 1, y - 1);
                int bottom_left = Map_cell_type(map, x - 1, y + 1);
                int bottom_right = Map_cell_type(map, x + 1, y + 1);

                printf("%d %d %d\n%d _ %d\n%d %d %d\n", top_left, top, top_right, left, right, bottom_left, bottom, bottom_right);

                int type = 0;
                /*
                    not topleft, not top, not topright = 0b0111
                */

                if (!top)
                {
                    type |= 0b0010;
                }
                if (!top_left)
                {
                    type |= 0b0100;
                }
                if (!top_right)
                {
                    type |= 0b0001;
                }
                printf("%d\n", type);

                continue;
            }

            if (cell_type)
            {

                int top = Map_cell_type(map, x, y - 1);
                int bottom = Map_cell_type(map, x, y + 1);
                int left = Map_cell_type(map, x - 1, y);
                int right = Map_cell_type(map, x + 1, y);

                uint8_t type = 0;

                if (!top)
                    type |= TOP;
                if (!bottom)
                    type |= BOTTOM;
                if (!left)
                    type |= LEFT;
                if (!right)
                    type |= RIGHT;

                if (type == GRASS_TOP_LEFT)
                    Texture_draw(grass_top_left, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GRASS_TOP_RIGHT)
                    Texture_draw(grass_top_right, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GRASS_TOP)
                    Texture_draw(grass_top, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GRASS_TOP_LEFT_RIGHT)
                    Texture_draw(grass_top_left_right, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GRASS_LEFT)
                    Texture_draw(grass_left, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GRASS_RIGHT)
                    Texture_draw(grass_right, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GRASS_TOP_BOTTOM)
                    Texture_draw(grass_top_bottom, renderer, draw_x + cam_x, draw_y + cam_y, zoom);

                else if (type == GROUND_BOTTOM)
                    Texture_draw(ground_bottom, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GROUND_BOTTOM_LEFT)
                    Texture_draw(ground_bottom_left, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GROUND_BOTTOM_RIGHT)
                    Texture_draw(ground_bottom_right, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GROUND_LEFT)
                    Texture_draw(ground_left, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GROUND_RIGHT)
                    Texture_draw(ground_right, renderer, draw_x + cam_x, draw_y + cam_y, zoom);

                else if (type == GROUND_LEFT_RIGHT)
                    Texture_draw(ground_left_right, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GROUND_BOTTOM_LEFT_RIGHT)
                    Texture_draw(ground_bottom_left_right, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else if (type == GRASS)
                    Texture_draw(grass, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
                else
                    Texture_draw(ground, renderer, draw_x + cam_x, draw_y + cam_y, zoom);
            }

            cnt++;
        }
    }

    SDL_RenderPresent(renderer);
}

void release(void)
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}
