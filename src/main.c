#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdlib.h>

#include "moteur.h"

//====== Raycaster (faut trouver un nom) =====
// 
// Game design : Fcalva
// Programmation : Fcalva
// Art : Redoki303, Fcalva
// 
// 
// 
// 
// 
//Problèmes :
//-L'affichage est très mauvais, a revoir
//-Le mouvement de la caméra ne marche pas
//
// Je garde le bout en dessous pour des raisons
//A rajouter d'ici le 27 :
//-Que ça marche (logique)
// -collisions
// -Sol
// -Gestion de plusieurs maps
// -Intéraction (Portes/boutons)
// -Textures
// -Sprites
// -PNJs ( a voir selon le thème)
// -Extérieur/intérieur (très probablement mais a voir selon le thème)
//La suite dépend surtout de ce qui aura été fait et du thème
//Avec le thème, maintenant il faudrait :
//-collisions
// -sol
// -Des bouts de map reliés entre eux
// -Sprites (ennemis + ambiance dans un second temps)
// -map générées aléatoirement
// -textures
// -Gameplay
// -PNJs
// -Sans de undertale (me demandez pas pourquoi, c'est un mec sur discord qui a proposé)
// 
// 
//
int player_x;
int player_y;
//int player_z; pour un futur fort fort lointain, pas pour la CGJ 2023
int player_dir;

int cos_table[360];
int sin_table[360];
int tan_table[360];
int acos_table[360];
int asin_table[360];
int atan_table[360];
int tab_mur_x[180];
int tab_mur_y[180];
int distors_table[60];

//unsigned short horizon_colors[512];//bout collectivisé de Lephe

int main(void){
	dclear(C_WHITE);
	compute_table();
	/**/dtext(150, 140, C_BLACK, "Coucou 2");
	/**/dupdate();
	/**/getkey();
	float angle_60;
	angle_60 = FOV / viewport_h;
	char angle_30;
	char angle_15;
	char angle_90;
	char angle_180;
	char angle_270;
	char angle_360;
	char angle_5;
	char angle_10;
	char angle_45;
	angle_30 = floor(angle_60 * 0.5);
	angle_15 = floor(angle_60 * 0.25);
	angle_90 = floor(angle_60 * 1.5);
	angle_180 = floor(angle_60 * 3);
	angle_270 = floor(angle_60 * 4.5);
	angle_360 = floor(angle_60 * 6);
	angle_5 = floor(angle_60 / 12);
	angle_10 = floor(angle_60 / 6);
	angle_45 = floor(angle_60 * 0.75);
	///generate_horizon_gradient();
	load_map();
	dtext(1, 1, C_BLACK, "projet CGJ 23 : Raycaster");
	dupdate();
	getkey();
	while (true) {
		dclear(C_WHITE);
		//draw_background();
		draw_walls();
		dupdate();
		/*if (keydown(KEY_UP)) {
			//flemme d'implémanter le mouvement, verrai quand le rendu marche
		}*/
		/*if (keydown(KEY_LEFT) == true) {
			player_dir -= 5;
		}
		if (keydown(KEY_RIGHT) == true) {
			player_dir += 5;
		}
		if (keydown(KEY_MENU) == true) {
			return 1;
		}*/
		getkey();
	}
}