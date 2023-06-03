#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "map_test.h"
#include "sprites.h"

//Je compte utiliser des sprites décrits en vecteurs, étant donné que c'est bien plus 
// facile à agrandir et réduire.
// 2 maps de sprites : Les sprites de la map (tonneaux, torches, cadavres...)
// Et les sprites de PNJs, donc d'ennemis et amicaux si jamais il y en a
// 
// Les sprites sont définis en vecteurs, en array de 64 (max, multiple de 4 requis) x5, 
// une ligne par trait, en vecteurs (les 4 prèmieres valeurs), 0;0 étant le coin en haut 
// a gauche, sur une zone de 32x32. Le 5 ème nombre est la couleur de la ligne
// Ex : Une ligne de 0;0 à 3;3 :
// unsigned short ligne[1][5]{
//		{0, 0, 3, 3, 0x0000}
// }
//

void draw_sprites(float x) {
	// 1 - lister les sprites en render distance
	// 2 - vérifier la distance/si c'est derrière un mur
	// 3 - trier par distance
	// 4 - passage a la moulinette de la perspective de la taille
	// 5 - vérifier les recouvrement éventuels
	// 6 - dessiner en commençant par derrière
	// 
	// 
	// 
	// /!\ ce qui il y a pour le moment est temporaire /!\
	// (et cassé aussi)
	extern unsigned short sprite_caillou[24][6];
	int i;
	dtext(70, 100, C_WHITE, "coucou sprites");
	dupdate();
	getkey();
	for (i = 0; i <= 23;) { //dessine juste les lignes comme décrites dans sprite_caillou[][]
		dline(floor(sprite_caillou[i][1] * x), floor(sprite_caillou[i][2] * x), floor(sprite_caillou[i][3] * x),
			  floor(sprite_caillou[i][4] * x), sprite_caillou[i][5]);
		i++;
	}
}

unsigned short sprite_caillou[24][6] = {
	{ 1, 2, 7, 1, 0x0000},
	{ 7, 1,12, 1, 0x0000},
	{12, 1,14, 1, 0x0000},
	{14, 1,20, 2, 0x0000},
	{20, 2,22,11, 0x0000},
	{22,11,21,13, 0x0000},
	{21,13,18,12, 0x0000},
	{18,12,14, 5, 0x0000},
	{14, 5,15, 3, 0x0000},
	{15, 3,13, 1, 0x0000},
	{ 2, 2, 3, 6, 0x0000},
	{ 3, 6, 6, 9, 0x0000},
	{ 6, 9,10,12, 0x0000},
	{10,12,12,11, 0x0000},
	{12,11,10, 9, 0x0000},
	{10, 9,15,13, 0x0000},
	{15,13,17,12, 0x0000},
	{17,12,17,11, 0x0000},
	{11, 9, 6, 7, 0x0000},
	{ 6, 7, 2, 2, 0x0000},
	{14, 5,10, 5, 0x0000},
	{10, 5, 5, 2, 0x0000},
	{ 0, 0, 0, 0, 0x0000},
	{ 0, 0, 0, 0, 0x0000}
};



//#include <sprites.h>
