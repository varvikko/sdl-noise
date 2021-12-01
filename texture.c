#include "texture.h"

#include <SDL2/SDL_image.h>

#define T Texture_T

struct Texture_T
{
    SDL_Texture *data;
    int width;
    int height;
};

T Texture_new(const char *filename, SDL_Renderer *renderer)
{
    T texture = malloc(sizeof(struct Texture_T));
    if (!texture)
    {
        return NULL;
    }

    SDL_Surface *texture_surface = IMG_Load(filename);
    if (!texture_surface)
    {
        return NULL;
    }

    texture->data = SDL_CreateTextureFromSurface(renderer, texture_surface);
    SDL_FreeSurface(texture_surface);
    if (!texture->data)
    {
        return NULL;
    }

    SDL_QueryTexture(texture->data, NULL, NULL, &texture->width, &texture->height);

    return texture;
}

void Texture_draw(T texture, SDL_Renderer *renderer, float x, float y, float scale)
{
    SDL_FRect dest_rect = { .x = x, .y = y, .w = texture->width * scale, .h = texture->height * scale };
    SDL_RenderCopyF(renderer, texture->data, NULL, &dest_rect);
}

void Texture_release(T texture)
{
    if (texture->data)
    {
        SDL_DestroyTexture(texture->data);
    }

    texture = NULL;
}

#undef T
