#ifndef moteur
#define moteur

#define pi 3.1415

//param. graphiques
#define screen_w 396
#define screen_h 224
#define viewport_w 396
#define viewport_h 224
#define FOV 60
#define max_dist 640 //en fraction de tuiles, 20 tuiles en ce moment

#define couleur_ciel 0x0ADF

//param. de jeu
#define tile_size 32
#define player_height 16
#define player_speed 8

float deg_to_rad(float angle_deg);

void compute_table();
void load_map(int);
void draw_background(int);
void draw_walls();

#define wall_color 0xAAAA

#endif /* moteur */