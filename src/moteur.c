#include <gint/display.h>
#include <gint/keyboard.h>
#include "moteur.h"
#include "sprites.h"
#include "map_test.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int var_int(void) {
	int vertical_grid;
	int horizontal_grid;
	int dist_to_n_v_grid; //distance to next vertical grid
	int dist_to_n_h_grid;
	int x_intersect;
	int y_intersect;
	int dist_to_n_x_intersect;
	int dist_to_n_y_intersect;
	int x_raypos;
	int y_raypos;
	int dist_to_v_hit;
	int dist_to_h_hit;
	int castarc = 60; //= FOV
	int castcolumn;
	int xtemp;
	int ytemp;
	int angle_60 = 6; //original angle_60 = floor(FOV / viewport_w), 6.4 pour 60 de FOV et 384px
	int angle_30 = 3; //3.2
	int angle_15 = 2; //1.6
	int angle_90 = 10; //9.6
	int angle_180 = 19; //19.2
	int angle_270 = 29; //28.8
	int angle_360 = 38; //38.4
	int angle_5 = 1; //0.5333...
	int angle_10 = 1; //1.0666...
	int angle_45 = 5; //4.8
	int mapindex;
	float wall_dist;
	float scale_factor;
	int wall_haut;
	int wall_bas;
	int couleur;
	int r;
	int hex_color;
}
int Rgb_to_RGB565_hex(int RGB){
	hex_color=0x0001;
	RGB = (RGB / 255 * 62) + (RGB / 255 * 63);
	for (r!=0;) {
		r=floor((RGB%16));
		RGB=floor(RGB/16);
		if (r !=0;) {
			hex_color=hex_color * 16; //ça me parait bancal comme approche
		}
	}
}
int deg_to_rad(float angle_deg) {
	return((angle_deg * pi) / 180);
}

int compute_table(void) {
	float cos_table[360];
	float sin_table[360];
	float tan_table[360];
	float acos_table[360];
	float asin_table[360];
	float atan_table[360];
	float tab_mur_x[180];
	float tab_mur_y[180];
	float distors_table[60];
	int i = 0;
	float rad_i = 0
	for (i != 360;) {
		deg_to_rad(i) + 0.001 = rad_i; //+0.001 pour éviter la division par zéro
		cos_table[i] = float cos(rad_i);
		sin_table[i] = float sin(rad_i);
		tan_table[i] = float tan(rad_i);
		acos_table[i] = float acos(rad_i);
		asin_table[i] = float asin(rad_i);
		atan_table[i] = float atan(rad_i);
		if (i >= 90 && i < 270;) {
			tab_mur_x[i] = tile_size / tan_table[i];
			if (tab_mur_x[i] > 0;) {
				tab_mur_x[i] = -tab_mur_x[i];
			}
		}
		else {
			tab_mur_x[i] = tile_size / tan_table[i];
			if (tab_mur_x[i] < 0;) {
				tab_mur_x[i] = -tab_mur_x[i];
			}
		}
		if (i >= 0 && i < 180;) {
			tab_mur_y[i] = tile_size / tan_table[i];
			if (tab_mur_y[i] < 0;) {
				tab_mur_y[i] = -tab_mur_y[i];
			}
		}
		else {
			tab_mur_y[i] = tile_size / tan_table[i];
			if (tab_mur_y[i] > 0;) {
				tab_mur_y[i] = -tab_mur_y[i];
			}
		}
	}
	for (i = -30; i <= 30; ++i;) {
		deg_to_rad(i) + 0.001 = rad_i;
		distors_table[i + 30] = (1 / cos(rad_i));
	}		
}
int load_map(void) {
	player_x = startpos_x;
	player y = startpos_y;
	startdir = player_dir;
	//faudrait d'autres trucs quand y aura plusieurs maps
}
int draw_background(void) {
	//très simple pour le moment, je vais sans doute améliorer
	drect(1, 1, 384, 108, couleur_ciel);
}
int draw_walls(void) {
	castcolumn = 0;
	castarc = player_dir - 30;
	if (castarc > 360;) {
		castarc -= 360;
	}
	for (castcolumn < viewport_w; ++castcolumn;) {
		if (castarc > 0 && castarc < angle_180;) {
			horizonal_grid = int (math::floor(player_y / tile_size) * tile_size + tile_size);
			dist_to_n_h_grid = tile_size;
			xtemp = atan_table[castarc] * (horizontal_grid - player_y);
			x_intersect = xtemp + player_x;
		}
		else {
			horizontal_grid = int(math::floor(player_y / tile_size) * tilesize);
			dist_to_n_h_grid = -tile_size;
			xtemp = atan_table[cast_arc] * (horizontal_grid - player_y);
			x_intersect = xtemp + player_x;
			--horizontal_grid;
		}
		if (castarc == 0 || castarc == angle_180;) {
			dist_to_h_hit = max_dist;
		}
		else {
			dist_to_n_x_intersect = tab_mur_x[castarc];
			while (true;) {
				x_raypos = math::floor(x_intersect / tile_size);
				y_raypos = math::floor(horizontal_grid / tile_size);
				mapindex = math::floor(y_raypos * map_w + x_raypos);
				if (x_raypos >= map_w || y_raypos >= map_h || x_raypos > max_dist || y_raypos > max_dist;) {
					dist_to_h_hit = max_dist;
					break;
				}
				else if (map_test[x_raypos][y_raypos] != 0;) {
					dist_to_h_hit = (x_intersect - player_x) * acos_table[castarc];
					break;
				}
				else {
					x_intersect += dist_to_n_x_intersect;
					horizontal_grid += dist_to_n_h_grid;
				}
			}
		}
		if (castarc < angle_90 || castarc > angle_270;) {
			vertical_grid = tile_size + math::floor(player_x / tile_size) * tile_size;
			dist_to_n_v_grid = tile_size;
			ytemp = tan_table[castarc] * (vertical_grid - player_x);
		}
		else {
			vertical_grid = math::floor(player_x / tile_size) * tile_size;
			dist_to_n_v_grid = -tile_size;
			ytemp = tan_table[castarc] * (vertical_grid - player_x);
			y_intersect = ytemp + player_y;
			--vertical_grid;
		}
		if (castarc == angle_90 || castarc == angle_270;) {
			dist_to_v_hit = max_dist;
		}
		else {
			dist_to_n_y_intersect = tab_mur[castarc];
			while (true) {
				x_raypos = math::floor(vertical_grid / tile_size);
				y_raypos = math::floor(y_intersect / tile_size);
				mapindex = math::floor(y_raypos * map_w + x_raypos);
				if (x_raypos >= map_w || y_raypos >= map_h || x_raypos > max_dist || y_raypos > max_dist;) {
					dist_to_h_hit = max_dist;
					break;
				}
				else if (map_test[x_raypos][y_raypos] != 0;) {
					dist_to_v_hit = (y_intersect - player_y) * asin_table[castarc];
					break;
				}
				else {
					y_intersect += dist_to_n_y_intersect;
					vertical_grid += dist_to_n_v_grid;
				}
			}	
		}
		if (dist_to_h_hit < dist_to_v_hit) {
			wall_dist = dist_to_h_hit;
		}
		else {
			wall_dist = dist_to_v_hit;
		}
		wall_dist /= distors_table[castcolumn];
		proj_wall_h = (wall_size * player_pj_pl_dist / wall_dist);
		wall_bas = viewport_h * 0.5 + proj_wall_h * 0.5;
		wall_haut = viewport_h * 0.5 - proj_wall_h * 0.5;
		if (wall_haut < 0) {
			wall_haut = 0;
		}
		if (wall_bas >= viewport_h) {
			wall_bas = viewport_h - 1;
		}
		wall_dist = floor(wall_dist);
		couleur = math::floor(255 - (dist / max_dist) * 255);
		if (couleur <= 20) {
			couleur = 20;
		}
		drect(castcolumn, wall_haut, 1, (wall_bas-wall_haut+1), );
		++castarc;
		if (castarc > 360) {
			castarc -= 360;
		}
	}
}
