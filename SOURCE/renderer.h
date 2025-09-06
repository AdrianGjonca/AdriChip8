#ifndef RENDERER_H
#define RENDERER_H

extern void renderer_init();
extern void renderer_loop(void (*onEvent)(SDL_Event), void (*onFrame)());

extern void blot_black(int x, int y);
extern void blot_white(int x, int y);

#endif
