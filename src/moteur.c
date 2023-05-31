#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "moteur.h"
#include "sprites.h"
#include "map_test.h"

unsigned char table_couleur[255];

float deg_to_rad(float angle_deg) {
	return((angle_deg * pi) / 180);
}

void compute_table() {
	extern int cos_table[];
	extern int sin_table[];
	extern int tan_table[];
	extern int acos_table[];
	extern int asin_table[];
	extern int atan_table[];
	extern int tab_mur_x[];
	extern int tab_mur_y[];
	extern int distors_table[];
	int i = 0;
	float rad_i = 0;
	while (i != 360) {
		rad_i = deg_to_rad(i);
		cos_table[i] = floor(64*cos(rad_i));
		sin_table[i] = floor(64*sin(rad_i));
		tan_table[i] = floor(64*tan(rad_i));
		acos_table[i] = floor(64*(acos(rad_i)));
		asin_table[i] = floor(64*(asin(rad_i)));
		atan_table[i] = floor(64*(atan(rad_i)));
		if (i >= 90 && i < 270) {
			tab_mur_x[i] = floor(tile_size / tan(i)*64);
			if (tab_mur_x[i] > 0) {
				tab_mur_x[i] = -tab_mur_x[i];
			}
		}
		else {
			tab_mur_x[i] = floor(tile_size / tan(i) * 64);
			if (tab_mur_x[i] < 0) {
				tab_mur_x[i] = -tab_mur_x[i];
			}
		}
		if (i >= 0 && i < 180) {
			tab_mur_y[i] = floor(tile_size / tan(i) * 64);
			if (tab_mur_y[i] < 0) {
				tab_mur_y[i] = -tab_mur_y[i];
			}
		}
		else {
			tab_mur_y[i] = floor(tile_size / tan(i) * 64);
			if (tab_mur_y[i] > 0) {
				tab_mur_y[i] = -tab_mur_y[i];
			}
		}
		i++;
	}
	for (i = -30; i <= 30; i++) {
		distors_table[i + 30] = floor(64*1 / cos(deg_to_rad(i)));
	}
}
int load_map() {
	extern int player_x;
	extern int player_y;
	extern int player_dir;
	player_x = startpos_x;
	player_y = startpos_y;
	player_dir = startdir;
	//faudrait d'autres trucs quand y aura plusieurs maps
	//*tout changer x|
	return 1;
}
void draw_background() {
	//tr�s simple pour le moment, je vais sans doute am�liorer
	//ajout le 28 10h : pas de dehors donc pas d'arri�re plan
	drect(1, 1, viewport_w, viewport_h*0.5 , couleur_ciel);
}
void draw_walls() {
	int vertical_grid;
	int horizontal_grid;
	int dist_to_n_v_grid; //distance to next vertical grid
	int dist_to_n_h_grid; //a changer si max_dist (distance de rendu) > 255
	int x_intersect;
	int y_intersect;
	int dist_to_n_x_intersect;
	int dist_to_n_y_intersect;
	int x_raypos;
	int y_raypos;
	int dist_to_v_hit;
	int dist_to_h_hit;
	int castarc = 0;
	int castcolumn;
	int xtemp;
	int ytemp;
	int mapindex;
	int wall_haut;
	int wall_bas;
	int couleur;
	int proj_wall_h;
	int half_viewport_h = viewport_h / 2;
	char angle_30;//il y en a peut �tre des inutilis�s, a voir
	char angle_15;
	char angle_90;
	char angle_180;
	char angle_270;
	char angle_360;
	char angle_5;
	char angle_10;
	char angle_45;
	char wall_type; //type de mur touch� par le raycast
	float wall_dist;
	float scale_factor;
	extern int cos_table[];
	extern int sin_table[];
	extern int tan_table[];
	extern int acos_table[];
	extern int asin_table[];
	extern int atan_table[];
	extern int tab_mur_x[];
	extern int tab_mur_y[];
	extern int distors_table[];
	extern int player_x;
	extern int player_y;
	extern int player_dir;
	extern char map_test[map_w][map_h];
	castcolumn = 0;
	castarc = player_dir - 30;
	if (castarc < 1) {
		castarc += angle_360;
	}
	if (castarc > 360) {
		castarc -= angle_360;
	}
	while (castcolumn < viewport_w) {
		if (castarc > 0 && castarc < angle_180) {
			horizontal_grid = floor(player_y / tile_size) * tile_size + tile_size;
			dist_to_n_h_grid = tile_size;
			xtemp = floor((atan_table[castarc] * (horizontal_grid - player_y)) * 0.015625);
			x_intersect = xtemp + player_x;
		}
		else {
			horizontal_grid = floor(player_y / tile_size) * tile_size;
			dist_to_n_h_grid = -tile_size;
			xtemp = floor((atan_table[castarc] * (horizontal_grid - player_y)) * 0.015625);
			x_intersect = xtemp + player_x;
			horizontal_grid--;
		}
		if (castarc == 0 || castarc == angle_180) {
			dist_to_h_hit = max_dist;
		}
		else {
			dist_to_n_x_intersect = floor((tab_mur_x[castarc]) * 0.015625);
			while (true) {
				x_raypos = floor(x_intersect / tile_size);
				y_raypos = floor(horizontal_grid / tile_size);
				mapindex = floor(y_raypos * map_w + x_raypos);
				if (x_raypos >= map_w || y_raypos >= map_h) {
					dist_to_h_hit = max_dist;
					break;
				}
				else if (map_test[x_raypos][y_raypos] == 0) {
					x_intersect += dist_to_n_x_intersect;
					horizontal_grid += dist_to_n_h_grid;
					break;
				}
				else {
					dist_to_h_hit = floor(((x_intersect - player_x) * acos_table[castarc]) * 0.015625);
					wall_type = map_test[x_raypos][y_raypos];
					break;
				}
			}
		}
		if (castarc < angle_90 || castarc > angle_270) {
			vertical_grid = tile_size + floor(player_x / tile_size) * tile_size;
			dist_to_n_v_grid = tile_size;
			ytemp = floor((tan_table[castarc] * (vertical_grid - player_x)) * 0.015625);
			y_intersect = ytemp + player_y;
		}
		else {
			vertical_grid = floor(player_x / tile_size * tile_size);
			dist_to_n_v_grid = -tile_size;
			ytemp = floor((tan_table[castarc] * (vertical_grid - player_x)) * 0.015625);
			y_intersect = ytemp + player_y;
			vertical_grid--;
		}
		if (castarc == angle_90 || castarc == angle_270) {
			dist_to_v_hit = max_dist;
		}
		else {
			dist_to_n_y_intersect = floor(tab_mur_y[castarc] * 0.015625);
			while (true) {
				x_raypos = floor(vertical_grid / tile_size);
				y_raypos = floor(y_intersect / tile_size);
				mapindex = floor(y_raypos * map_w + x_raypos);
				if (x_raypos >= map_w || y_raypos >= map_h || x_raypos > max_dist || y_raypos > max_dist) {
					dist_to_h_hit = max_dist;
					break;
				}
				else if (map_test[x_raypos][y_raypos] == 0) {
					x_intersect += dist_to_n_x_intersect;
					horizontal_grid += dist_to_n_h_grid;
				}
				else {
					dist_to_h_hit = floor(((x_intersect - player_x) * acos_table[castarc]) * 0.015625);
					wall_type = map_test[x_raypos][y_raypos];
					break;
				}
			}
		}
		if (dist_to_h_hit < dist_to_v_hit) {
			wall_dist = dist_to_h_hit;
		}
		else {
			wall_dist = dist_to_v_hit;
		}
		wall_dist /= distors_table[castcolumn] * 0.015625;
		proj_wall_h = floor(wall_size * player_pj_pl_dist / wall_dist);
		wall_bas = half_viewport_h + (proj_wall_h * 0.5);
		wall_haut = half_viewport_h + (proj_wall_h * 0.5);
		if (wall_haut < 0) {
			wall_haut = 0;
		}
		if (wall_bas >= viewport_h) {
			wall_bas = viewport_h - 1;
		}
		wall_bas = (wall_bas - wall_haut) + 1;
		drect(castcolumn, wall_haut, castcolumn, wall_bas, wall_color);
		if (floor(wall_dist) <= max_dist) {
			couleur = floor(255 - (wall_dist / max_dist) * 255);
			if (couleur <= 20) {
				couleur = 20;
			}
			if (couleur > 255) {
				couleur = 255;
			}
			switch (wall_type) {
			case 1: {
				drect(castcolumn, wall_haut, castcolumn, wall_bas, wall_color);
				break;
			}
			}
			drect(castcolumn, wall_haut, castcolumn, wall_bas, wall_color);
		}
		else {
			drect(castcolumn, wall_haut, castcolumn, wall_bas, 0x5ACB);
		}
		castarc += 1;
		castcolumn++;
		if (castarc > 360) {
			castarc -= 360;
		}
	}
}