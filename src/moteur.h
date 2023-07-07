#ifndef moteur
#define moteur

#define pi 3.1415

//param. graphiques
#define screen_w 396
#define screen_h 224
#define viewport_w 396
#define viewport_h 224
#define max_dist 10 //en fraction de tuiles, 20 tuiles en ce moment

void load_map(int);
void draw_background(int);
void draw_walls();
void move();

#endif /* moteur */