#include <gint/display.h>
#include <gint/keyboard.h>

#include "moteur.h"

dclear(C_WHITE);
extern int vertical_grid;
extern int horizontal_grid;
extern int dist_to_n_v_grid; //distance to next vertical grid
extern int dist_to_n_h_grid;
extern int x_intersect;
extern int y_intersect;
extern int dist_to_n_x_intersect;
extern int dist_to_n_y_intersect;
extern int x_raypos;
extern int y_raypos;
extern int dist_to_v_hit;
extern int dist_to_h_hit;
extern int castarc = 60; //= FOV
extern int castcolumn;
extern int xtemp;
extern int ytemp;
extern int angle_60 = 6; //original angle_60 = floor(FOV / viewport_w), 6.4 pour 60 de FOV et 384px
extern int angle_30 = 3; //3.2
extern int angle_15 = 2; //1.6
extern int angle_90 = 10; //9.6
extern int angle_180 = 19; //19.2
extern int angle_270 = 29; //28.8
extern int angle_360 = 38; //38.4
extern int angle_5 = 1; //0.5333...
extern int angle_10 = 1; //1.0666...
extern int angle_45 = 5; //4.8
extern int mapindex;
extern float wall_dist;
extern float scale_factor;
extern int wall_haut;
extern int wall_bas;
extern int couleur;
extern int r;
extern int hex_color;
compute_table();
load_map();
int main(void)
{
	draw_background();
	draw_walls();
	if (keydown ( 0x86 )){
		//flemme d'implémanter le mouvement, verrai quand ça compile
	}
	if (keydown( 0x85 )) {
		player_dir -= 5;
	}
	if (keydown( 0x76 )) {
		player_dir += 5;
	}
	if (keydown( 0x74 )) {
		return 1;
	}
}