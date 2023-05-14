#include <gint/display.h>
#include <gint/keyboard.h>
#include "moteur.h"
//#include "sprites.h"
#include "map_test.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

//int wall_c_table[235] = {}

/*int Rgb_to_RGB565_hex(int RGB) {
	int hex_color = 0x0001;
	int r = 0;
	RGB = (RGB / 255 * 62) + (RGB / 255 * 63);
	while (r != 0) {
		r = floor((RGB % 16));
		RGB = floor(RGB / 16);
		if (r != 0) {
			hex_color=hex_color * 16; //ça me parait bancal comme approche
		}
	}
}*/

int deg_to_rad(float angle_deg) {
	return((angle_deg * pi) / 180);
}
int generate_horizon_gradient(void) //code éhontément collectivisé de Lephe, dans After Burner
{
	int hc = 0;
	extern unsigned short horizon_colors[];

	for (int i = 0; i < 256; i++) {
		horizon_colors[hc++] = RGB24(0x438747);
	}
	for (int i = 0; i < 32; i++) {
		int r = 0x68 + (0x3f - 0x68) * i / 32;
		int g = 0x8c + (0x66 - 0x8c) * i / 32;
		int b = 0xd6 + (0xb3 - 0xd6) * i / 32;
		horizon_colors[hc++] = RGB24(((r << 16) | (g << 8) | b));
	}
	for (int i = 0; i < 64; i++) {
		horizon_colors[hc++] = RGB24(0x3f66b3);
	};
	for (int i = 0; i < 64; i++) {
		int r = 0x3f + (0x0c - 0x3f) * i / 64;
		int g = 0x66 + (0x29 - 0x66) * i / 64;
		int b = 0xb3 + (0x61 - 0xb3) * i / 64;
		horizon_colors[hc++] = RGB24(((r << 16) | (g << 8) | b));
	}
	for (int i = 0; i < 96; i++) {
		horizon_colors[hc++] = RGB24(0x0c2961);
	}
}
int compute_table(void) {
	extern float cos_table[];
	extern float sin_table[];
	extern float tan_table[];
	extern float acos_table[];
	extern float asin_table[];
	extern float atan_table[];
	extern float tab_mur_x[];
	extern float tab_mur_y[];
	extern float distors_table[];
	int i = 0;
	float rad_i = 0;
	while (i != 360) {
		rad_i = deg_to_rad(i) + 0.0001;
		cos_table[i] = cos(rad_i);
		sin_table[i] = sin(rad_i);
		tan_table[i] = tan(rad_i);
		acos_table[i] = acos(rad_i);
		asin_table[i] = asin(rad_i);
		atan_table[i] = atan(rad_i);
		if (i >= 90 && i < 270) {
			tab_mur_x[i] = tile_size / tan_table[i];
			if (tab_mur_x[i] > 0) {
				tab_mur_x[i] = -tab_mur_x[i];
			}
		}
		else {
			tab_mur_x[i] = tile_size / tan_table[i];
			if (tab_mur_x[i] < 0) {
				tab_mur_x[i] = -tab_mur_x[i];
			}
		}
		if (i >= 0 && i < 180) {
			tab_mur_y[i] = tile_size / tan_table[i];
			if (tab_mur_y[i] < 0) {
				tab_mur_y[i] = -tab_mur_y[i];
			}
		}
		else {
			tab_mur_y[i] = tile_size / tan_table[i];
			if (tab_mur_y[i] > 0) {
				tab_mur_y[i] = -tab_mur_y[i];
			}
		}
		++i;
	}
	for (i = -30; i <= 30; ++i) {
		distors_table[i + 30] = 1 / cos(deg_to_rad(i));
	}
	return 1;
}
int load_map(void) {
	extern int player_x;
	extern int player_y;
	extern int player_dir;
	player_x = startpos_x;
	player_y = startpos_y;
	player_dir = startdir;
	//faudrait d'autres trucs quand y aura plusieurs maps
	return 1;
}
int draw_background(void) {
	//très simple pour le moment, je vais sans doute améliorer
	drect(1, 1, 384, 108, couleur_ciel);
	return 1;
}
int draw_walls(void) {
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
	int dist_to_v_hit; //a changer si max_dist (distance de rendu en part de tile) > 255
	int dist_to_h_hit;
	int castarc = 60;
	int castcolumn;
	int xtemp;
	int ytemp;
	char angle_60 = 6; //original angle_60 = floor(FOV / viewport_w), 6.4 pour 60 de FOV et 384px
	char angle_30 = 3; //3.2
	char angle_15 = 2; //1.6
	char angle_90 = 10; //9.6
	char angle_180 = 19; //19.2
	char angle_270 = 29; //28.8
	char angle_360 = 38; //38.4
	char angle_5 = 1; //0.5333...
	char angle_10 = 1; //1.0666...
	char angle_45 = 5; //4.8
	int mapindex;
	float wall_dist;
	float scale_factor;
	int wall_haut;
	int wall_bas;
	int couleur;
	int proj_wall_h;
	extern float cos_table[];
	extern float sin_table[];
	extern float tan_table[];
	extern float acos_table[];
	extern float asin_table[];
	extern float atan_table[];
	extern float tab_mur_x[];
	extern float tab_mur_y[];
	extern float distors_table[];
	extern int player_x;
	extern int player_y;
	extern int player_dir;
	extern unsigned short horizon_colors[];//bout collectivisé de Lephe
	//extern char map_test[][]; marche pas (?)
	char map_test[map_w][map_h] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,0,0,0,1,1,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,1},
	{1,0,0,1,0,0,0,0,0,0,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,0,1,0,0,0,0,0,1},
	{1,0,0,0,0,0,1,0,0,1,0,0,1},
	{1,0,1,0,1,0,1,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1}
	}; //bodge
	castcolumn = 0;
	castarc = player_dir - 30;
	if (castarc > 360) {
		castarc -= 360;
	}
	for (castcolumn < viewport_w; ++castcolumn;) {
		if (castarc > 0 && castarc < angle_180) {
			horizontal_grid = floor((player_y) / tile_size) * tile_size + tile_size;
			dist_to_n_h_grid = tile_size;
			xtemp = (atan_table[castarc]) * (horizontal_grid - player_y);
			x_intersect = xtemp + (player_x);
		}
		else {
			horizontal_grid = (floor(player_y / tile_size) * tile_size);
			dist_to_n_h_grid = -tile_size;
			xtemp = (atan_table[castarc]) * (horizontal_grid - player_y);
			x_intersect = xtemp + player_x;
			--horizontal_grid;
		}
		if (castarc == 0 || castarc == angle_180) {
			dist_to_h_hit = max_dist;
		}
		else {
			dist_to_n_x_intersect = tab_mur_x[castarc];
			while (true) {
				x_raypos = floor(x_intersect / tile_size);
				y_raypos = floor(horizontal_grid / tile_size);
				mapindex = floor(y_raypos * map_w + x_raypos);
				if (x_raypos >= map_w || y_raypos >= map_h || x_raypos > max_dist || y_raypos > max_dist) {
					dist_to_h_hit = max_dist;
					break;
				}
				else if (map_test[x_raypos][y_raypos] != 0) {
					dist_to_h_hit = (x_intersect - player_x) * (acos_table[castarc]);
					break;
				}
				else {
					x_intersect += dist_to_n_x_intersect;
					horizontal_grid += dist_to_n_h_grid;
				}
			}
		}
		if (castarc < angle_90 || castarc > angle_270) {
			vertical_grid = tile_size + floor(player_x / tile_size) * tile_size;
			dist_to_n_v_grid = tile_size;
			ytemp = tan_table[castarc] * (vertical_grid - player_x);
		}
		else {
			vertical_grid = floor(player_x / tile_size) * tile_size;
			dist_to_n_v_grid = -tile_size;
			ytemp = tan_table[castarc] * (vertical_grid - player_x);
			y_intersect = ytemp + player_y;
			--vertical_grid;
		}
		if (castarc == angle_90 || castarc == angle_270) {
			dist_to_v_hit = max_dist;
		}
		else {
			dist_to_n_y_intersect = tab_mur_y[castarc];
			while (true) {
				x_raypos = floor(vertical_grid / tile_size);
				y_raypos = floor(y_intersect / tile_size);
				mapindex = floor(y_raypos * map_w + x_raypos);
				if (x_raypos >= map_w || y_raypos >= map_h || x_raypos > max_dist || y_raypos > max_dist) {
					dist_to_h_hit = max_dist;
					break;
				}
				else if (map_test[x_raypos][y_raypos] != 0) {
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
		couleur = floor(255 - (wall_dist / max_dist) * 255);
		if (couleur <= 20) {
			couleur = 20;
		}
		couleur = floor( 2.007843137254902 * couleur);
		if (couleur >= 512) {
			couleur = 512;
		}
		drect(castcolumn, wall_haut, castcolumn, (wall_haut - wall_bas + 1), horizon_colors[couleur]);//bout collectivisé de Lephe
		++castarc;
		if (castarc > 360) {
			castarc -= 360;
		}
	}
	return 1;
}