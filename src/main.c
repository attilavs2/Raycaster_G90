#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>

#include "moteur.h"
#include "map_test.h"
//#include "sprites.h"

//====== Raycaster (faut trouver un nom) =====
// Tout le code non-foireux vient de https://lodev.org/cgtutor/raycasting.html
// Grands remerciments !
// Github : https://github.com/attilavs2/Raycaster_prealpha
// Game design : Fcalva
// Programmation : Fcalva
// Art : Fcalva
// 
// 
// TODO :
// - réparer l'affichage
// - sprites
// - gameplay


int player_x;
int player_y;

extern char map_test[map_w][map_h];

float posX;
float posY;
float dirX;
float dirY;
float planeX;
float planeY;
float moveSpeed;
float rotSpeed;
float oldDirX;
float oldPlaneX;
int xtemp1;
int xtemp2;
int ytemp1;
int ytemp2;
double sin_rotspeed;

int frame = 0;
float aprox_s_rotspeed = 0.0087;
float maprox_s_rotspeed = -0.0087;

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
	moveSpeed = 5/10; //the constant value is in squares/second
    rotSpeed = 3/10; //the constant value is in radians/second
	//autres trucs de chargement

	dclear(C_WHITE);
	dtext(100, 100, C_BLACK, "Raycaster Fcalva v 0.1.1");
	dtext(60, 130, C_BLACK, "Edition plus paraplegique");

	dupdate();
	getkey();

	while (true) {
		dclear(C_WHITE);
		draw_walls();
		pollevent();
		if (keydown(KEY_UP)) {
			xtemp1 = floor(posX + dirX * moveSpeed);
			ytemp1 = floor(posY);
			xtemp2 = floor(posX);
			ytemp2 = floor(posY + dirY * moveSpeed);
     		if(map_test[xtemp1][ytemp1] == 0) posX += dirX * moveSpeed;
     		if(map_test[xtemp2][ytemp2] == 0) posY += dirY * moveSpeed;
    	}
    	//move backwards if no wall behind you
    	if (keydown(KEY_DOWN)) {
      		xtemp1 = floor(posX - dirX * moveSpeed);
			ytemp1 = floor(posY);
			xtemp2 = floor(posX);
			ytemp2 = floor(posY - dirY * moveSpeed);
     		if(map_test[xtemp1][ytemp1] == 0) posX -= dirX * moveSpeed;
     		if(map_test[xtemp2][ytemp2] == 0) posY -= dirY * moveSpeed;
    	}
   		//rotate to the right
    	if (keydown(KEY_RIGHT)) {
      		//both camera direction and camera plane must be rotated
	      	oldDirX = dirX;
    	  	dirX = dirX * 1.0 - dirY * maprox_s_rotspeed; //cos(-rotSpeed) ~= 1 
 	     	dirY = oldDirX * maprox_s_rotspeed + dirY * 1.0; //sin(-rotspeed) ~= -0.0022
   		   	oldPlaneX = planeX;
   		   	planeX = planeX * 1.0 - planeY * maprox_s_rotspeed;
      		planeY = oldPlaneX * maprox_s_rotspeed + planeY * 1.0;
    	}
    	//rotate to the left
    	if (keydown(KEY_LEFT)) {
     	 	//both camera direction and camera plane must be rotated
      		oldDirX = dirX;
      		dirX = dirX * 1.0 - dirY * aprox_s_rotspeed; //cos(rotSpeed) ~= 1
      		dirY = oldDirX * aprox_s_rotspeed + dirY * 1.0; //sin(rotSpeed) ~= 0.0022
      		oldPlaneX = planeX;
      		planeX = planeX * 1.0 - planeY * aprox_s_rotspeed;
      		planeY = oldPlaneX * aprox_s_rotspeed + planeY * 1.0;
    	}
		if (keydown(KEY_F6)) {
			return 1;
		}
		
		dprint( 1, 1, C_BLACK, "frame : %d", frame); frame++;
		dprint( 1, 10, C_BLACK, "posX : %d", floor(posX*100));
		dprint( 1, 20, C_BLACK, "posY : %d", floor(posY*100));
		dprint( 1, 30, C_BLACK, "dirX : %d", floor(dirX*100));
		dprint( 1, 40, C_BLACK, "dirY : %d", floor(dirY*100));

		dupdate();
	}
}