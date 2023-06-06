#include <stdlib.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "moteur.h"
#include "map_test.h"
#include "sprites.h"

//====== Raycaster (faut trouver un nom) =====
// 
// Game design : Fcalva
// Programmation : Fcalva
// Art : Fcalva
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
// -Interaction (Portes/boutons)
// -Textures
// -Sprites
// -PNJs ( a voir selon le théme)
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
// -Sans de undertale (me demandez pas pourquoi, c'est un mec sur discord qui a propos�)
//
//================================================
// au 03/06 :
// A faire là tout de suite :
// - Bien réparer correctement les murs
// - Sprites
// Plan :
// -Sol
// -Ennemis
// -Maps
// -Textures
//
int player_x;
int player_y;
//int player_z; pour un futur fort fort lointain, pas pour ce premier jeu avec ce moteur
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

float angle_60;
char angle_30;
char angle_15;
char angle_90;
char angle_180;
char angle_270;
char angle_360;
char angle_5;
char angle_10;
char angle_45;

int main(void){
	dclear(C_WHITE);
	
	compute_table();
	
	angle_60 = FOV / viewport_h;
	angle_30 = floor(angle_60 * 0.5);
	angle_15 = floor(angle_60 * 0.25);
	angle_90 = floor(angle_60 * 1.5);
	angle_180 = floor(angle_60 * 3);
	angle_270 = floor(angle_60 * 4.5);
	angle_360 = floor(angle_60 * 6);
	angle_5 = floor(angle_60 / 12);
	angle_10 = floor(angle_60 / 6);
	angle_45 = floor(angle_60 * 0.75);
	
	int temp;
	temp = 0;

	///generate_horizon_gradient();
	load_map();

	//draw_sprites(8);

	dtext(100, 100, C_BLACK, "projet CPC 23 : Raycaster");

	dupdate();
	
	getkey();
	while (true) {
		dclear(C_WHITE); //nrmlmnt 0x5ACB, changé pour debug
		//draw_background();
		draw_walls();

		dtext(1, temp, C_WHITE, "coucou"); //passé
		temp++;
		if (temp >= 240) {
			temp = 0;
		}
		dupdate();
		/*if (keydown(KEY_UP)) {
			//flemme d'implémanter le mouvement, verrai quand le rendu marche
		}*/
		if (keydown(KEY_LEFT)) {
			player_dir -= 5;
		}
		if (keydown(KEY_RIGHT)) {
			player_dir += 5;
		}
		if (keydown(KEY_MENU)) {
			return 1;
		}
		getkey();
	}
}