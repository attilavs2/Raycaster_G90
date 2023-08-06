#ifndef moteur_h
#define moteur_h

#include <gint/image.h>

#define pi 3.1415

//param. graphiques
#define screen_w 396
#define screen_h 224
#define viewport_w 396
#define viewport_h 224
#define max_dist 0x200000 //en tuiles << 16, actuellement 32
#define sprite_max_dist 0x140000 //en tuiles << 16, 20

#define max_sprite_search 32
#define max_sprite_display 16

void draw_background(int, image_t*, image_t*, image_t*, image_t*);
void load_map();
void logic(image_t*, image_t*);
void draw_f(image_t*, image_t*);
void draw_walls(image_t*, image_t*, image_t*, image_t*, image_t*, image_t*);
void move();

#endif /* moteur */