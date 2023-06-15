#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "moteur.h"
//#include "map_test.h"
//#include "sprites.h"

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

unsigned short angle_60;
unsigned short angle_30;
unsigned short angle_15;
unsigned short angle_90;
unsigned short angle_180;
unsigned short angle_270;
unsigned short angle_360;
unsigned short angle_5;
unsigned short angle_10;
unsigned short angle_45;

int cos_table[ang_360p];
int sin_table[ang_360p];
int tan_table[ang_360p];
int acos_table[ang_360p];
int asin_table[ang_360p];
int atan_table[ang_360p];
float tab_mur_x[ang_360p];
float tab_mur_y[ang_360p];
float distors_table[ang_360p];

int main(){
	dclear(C_WHITE);
	dtext( 1, 1, C_BLACK, "Chargement...");
	dupdate();
	//a faire plus proprement selon le FOV, actuellement intégralment basé
	//sur 60 de FOV
	angle_60 = viewport_w;
	angle_30 = floor(angle_60 * 0.5);
	angle_15 = floor(angle_60 * 0.25);
	angle_90 = floor(angle_60 * 1.5);
	angle_180 = floor(angle_60 * 3);
	angle_270 = floor(angle_60 * 4.5);
	angle_360 = floor(angle_60 * 6);
	angle_5 = floor(angle_60 / 12);
	angle_10 = floor(angle_60 / 6);
	angle_45 = floor(angle_60 * 0.75);

	compute_table();
	
	int temp;
	temp = 0;

	///generate_horizon_gradient();
	load_map();

	dclear(C_WHITE);
	dtext(100, 100, C_BLACK, "projet CPC 23 : Raycaster");
	
	//draw_sprites(8);
	dupdate();
	getkey();

	while (true) {
		dclear(C_WHITE); //nrmlmnt 0x5ACB, changé pour debug
		//draw_background();
		draw_walls();

		/*dtext(1, temp, C_WHITE, "coucou"); //passé
		temp++;
		if (temp >= 240) {
			temp = 0;
		}*/
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

		dupdate();
		getkey();
		return 1;
	}
}