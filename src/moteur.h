#ifndef moteur.h
#define moteur.h

#define pi 3.1415

//param. graphiques
#define screen_w 384
#define screen_h 216
#define viewport_w 384
#define viewport_h 216
//#define fps_cap 30
#define FOV 60
#define max_dist 160 // peut-être un peu élevé
#define player_pj_pl_dist 277 //distance du joueur au plan de projection

#define couleur_ciel 0x0ADF

//param. de jeu
#define tile_size 8
#define wall_size 8
#define player_height 4
#define player_speed 2

int compute_table(void);
int load_map(void);
int draw_background(void);
int draw_walls(void);

#endif /* moteur.h */