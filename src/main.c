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

double posX;
double posY;
double dirX;
double dirY;
double planeX;
double planeY;

int main(){
	dclear(C_WHITE);
	dtext( 1, 1, C_BLACK, "Chargement...");
	dupdate();
	posX = 6; 
	posY = 6;  //x and y start position
  	dirX = -1;
	dirY = 0; //initial direction vector
 	planeX = 0; 
	planeY = 0.66; //the 2d raycaster version of camera plane
	//trucs de chargement

	dclear(C_WHITE);
	dtext(100, 100, C_BLACK, "Raycaster Fcalva v 0.1.1");
	dtext(60, 130, C_BLACK, "Edition j'ai volé le code d'un gars");

	dupdate();
	getkey();
	
	dclear(C_WHITE);

	draw_walls();

	dupdate();
	getkey();
	return 1;

	/*while (true) {
		draw_walls_2();
		if (keydown(KEY_UP)) {
     		if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
     		if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
    	}
    	//move backwards if no wall behind you
    	if (keydown(KEY_DOWN)) {
      		if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
      		if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
    	}
   		//rotate to the right
    	if (keydown(KEY_RIGHT)) {
      		//both camera direction and camera plane must be rotated
	      	double oldDirX = dirX;
    	  	dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
 	     	dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
   		   	double oldPlaneX = planeX;
   		   	planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      		planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    	}
    	//rotate to the left
    	if (keydown(KEY_LEFT))) {
     	 	//both camera direction and camera plane must be rotated
      		double oldDirX = dirX;
      		dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      		dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      		double oldPlaneX = planeX;
      		planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      		planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    	}
	}*/
}