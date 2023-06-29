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
int player_dir;

int angle_60;
int angle_30;
int angle_15;
int angle_90;
int angle_180;
int angle_270;
int angle_360;
int angle_5;
int angle_10;
int angle_45;

int cos_table[ang_360p];
int sin_table[ang_360p];
int tan_table[ang_360p];
int acos_table[ang_360p];
int asin_table[ang_360p];
int atan_table[ang_360p];
float tab_mur_x[ang_360p];
float tab_mur_y[ang_360p];
int distors_table[ang_360p];

int main(){
	dclear(C_WHITE);
	dtext( 1, 1, C_BLACK, "Chargement...");
	dupdate();
	angle_60 = viewport_w;
	angle_30 = floor(viewport_w * 0.5);
	angle_15 = floor(viewport_w * 0.25);
	angle_90 = floor(viewport_w * 1.5);
	angle_180 = floor(viewport_w * 3);
	angle_270 = floor(viewport_w * 4.5);
	angle_360 = floor(viewport_w * 6);
	angle_5 = floor(viewport_w / 12);
	angle_10 = floor(viewport_w / 6);
	angle_45 = floor(viewport_w * 0.75);
	compute_table();
	//trucs de chargement

	dclear(C_WHITE);
	dtext(100, 100, C_BLACK, "Raycaster Fcalva v 0.1");
	dtext(60, 130, C_BLACK, "Edition a la poubelle ce qui marche pas");

	player_x = 192;
	player_y = 192;

	dupdate();
	getkey();
	
	dclear(C_WHITE);

	draw_walls();

	dupdate();
	getkey();
	return 1;

	/*while (true) {
		dclear(C_WHITE); //0x5ACB pour ambiance
		//draw_background();
		draw_walls();

		dupdate();
		getkey();
		return 1;
	}*/
}