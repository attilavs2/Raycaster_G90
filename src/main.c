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
// 
// 
// 


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

int a = 0;

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
	moveSpeed = 5/24; //the constant value is in squares/second
    rotSpeed = 3/24; //the constant value is in radians/second
	//autres trucs de chargement

	dclear(C_WHITE);
	dtext(100, 100, C_BLACK, "Raycaster Fcalva v 0.1.1");
	dtext(60, 130, C_BLACK, "Edition j'ai volé le code d'un gars");

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
    	  	dirX = dirX * 1.0 - dirY * -0.2; //cos(-rotSpeed) ~= 1 sin(-rotSpeed) ~= -0.0021
 	     	dirY = oldDirX * 0.0021 + dirY * 1.0;
   		   	oldPlaneX = planeX;
   		   	planeX = planeX * 1.0 - planeY * -0.2;
      		planeY = oldPlaneX * 0.0021 + planeY * 1.0;
    	}
    	//rotate to the left
    	if (keydown(KEY_LEFT)) {
     	 	//both camera direction and camera plane must be rotated
      		oldDirX = dirX;
      		dirX = dirX * 1.0 - dirY * 0.2; //cos(rotSpeed) ~= 1 sin(rotSpeed) ~= 0.0021
      		dirY = oldDirX * 0.2 + dirY * 1.0;
      		oldPlaneX = planeX;
      		planeX = planeX * 1.0 - planeY * 0.2;
      		planeY = oldPlaneX * 0.2 + planeY * 1.0;
    	}
		if (keydown(KEY_F6)) {
			return 1;
		}
		
		dprint( 1, 1, C_BLACK, "frame : %d", a);
		a++;


		dupdate();
	}
}