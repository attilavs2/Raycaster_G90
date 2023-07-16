#ifndef moteur_h
#define moteur_h

#include <gint/image.h>

#define pi 3.1415

//param. graphiques
#define screen_w 396
#define screen_h 224
#define viewport_w 396
#define viewport_h 224
#define max_dist 0xFFFFF //en tuiles << 16

void load_map(int);
void draw_background(int, image_t*, image_t*, image_t*, image_t*);
void draw_f(image_t*, image_t*);
void draw_walls(image_t*, image_t*, image_t*, image_t*, image_t*, image_t*);
void move();

#endif /* moteur */