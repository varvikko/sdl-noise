
sdl_noise: sdl_noise.c texture.c map.c util.c
	gcc $^ -o $@ -lSDL2 -lSDL2_image -lm
