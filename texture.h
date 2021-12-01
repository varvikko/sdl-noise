#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>

#define T Texture_T
typedef struct T* T;

extern T    Texture_new(const char*, SDL_Renderer*);
extern void Texture_draw(T, SDL_Renderer*, float, float, float);
extern void Texture_release(T);

#undef T

#endif
