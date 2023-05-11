#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdlib.h>

#include "moteur.h"

int player_x;
int player_y;
//short int player_z; pour un futur fort fort lointain, pas pour la CGJ 2023
int player_dir;

float cos_table[360];
float sin_table[360];
float tan_table[360];
float acos_table[360];
float asin_table[360];
float atan_table[360];
float tab_mur_x[180];
float tab_mur_y[180];
float distors_table[60];

int main(void){
	dclear(C_WHITE);
	compute_table();
	load_map();
	while (true) {
		dclear(C_WHITE);
		draw_background();
		draw_walls();
		if (keydown(0x86)) {
			//flemme d'implémanter le mouvement, verrai quand ça compile
		}
		if (keydown(0x85)) {
			player_dir -= 5;
		}
		if (keydown(0x76)) {
			player_dir += 5;
		}
		if (keydown(0x74)) {
			exit(1);
		}
	}
}