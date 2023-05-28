#ifndef moteur.h
//#define moteur.h

#define pi 3.1415

//param. graphiques
#define screen_w 396
#define screen_h 224
#define viewport_w 396
#define viewport_h 224
//#define fps_cap 30
#define FOV 60
#define max_dist 640 // peut-être un peu élevé
#define player_pj_pl_dist 277 //distance du joueur au plan de projection

#define couleur_ciel 0x0ADF

//param. de jeu
#define tile_size 32
#define wall_size 32
#define player_height 16
#define player_speed 8

float deg_to_rad(float angle_deg);

void compute_table();
int load_map();
void draw_background();
void draw_walls();
//temp
/*int generate_horizon_gradient(void); //bout collectivisé de Lephe
#define RGB24(hex) \
    (((hex & 0xf80000) >> 8) | \
     ((hex & 0x00fc00) >> 5) | \
     ((hex & 0x0000f8) >> 3))
*/
#define map_w 13
#define map_h 13
#define startpos_x 0
#define startpos_y 0
#define startdir 0 //en degrés
#define wall_color 0xAAAA

#endif /* moteur.h */