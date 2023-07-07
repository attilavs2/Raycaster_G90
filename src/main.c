#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>
//#include <libprof.h>

#include "moteur.h"
#include "map_test.h"
//#include "sprites.h"

//====== Raycaster (faut trouver un nom) =====
// Tout le code non-foireux vient de https://lodev.org/cgtutor/raycasting.html
// Grands remerciments !
// Github : https://github.com/attilavs2/Raycaster_prealpha
// Tout le code de cette version est en GPL3
// Game design : Fcalva (aka. fklv, Fcalva#6860, attilavs2)
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
float oldDirX;
float oldPlaneX;

int frame_time;
int frame_n = 0;

//prof_init();

int main(){
	dclear(C_WHITE);
	dtext( 1, 1, C_BLACK, "Chargement...");
	dupdate();
	posX = startpos_x; 
	posY = startpos_y;  //x and y start position
  	dirX = start_dirX;
	dirY = start_dirY; //initial direction vector
 	planeX = 0; 
	planeY = 0.66; //the 2d raycaster version of camera plane
	//prof_t frame = prof_make();
	//autres trucs de chargement

	dclear(C_WHITE);
	dtext(100, 100, C_BLACK, "Raycaster Fcalva v 0.1.4");
	dtext( 97, 120, C_BLACK, "Edition Manque de Gameplay");
	dtext(105, 150, C_BLACK, "Appuyez sur une touche");


	dupdate();
	getkey();

	while (true) {
		/* marche pas, je vais bosser sur des trucs plus importants
		prof_leave(frame);
		old_frame_time = frame_time;
		frame_time = prof_time(frame);
		frame_time -= old_frame_time;
		prof_enter(frame);
		*/

		dclear(C_BLACK);

		draw_background(0);

		draw_walls();
		
		move();

		pollevent();
		if (keydown(KEY_F6)) {
			prof_quit();
			return 1;
		}
		/*
		dprint( 1, 1, C_BLACK, "frame : %d", frame_n); frame_n++;
		dprint( 1, 10, C_BLACK, "posX : %d", floor(posX));
		dprint( 1, 20, C_BLACK, "posY : %d", floor(posY));
		dprint( 1, 30, C_BLACK, "dirX : %d", floor(dirX));
		dprint( 1, 40, C_BLACK, "dirY : %d", floor(dirY));
		*/
		//dprint( 1, 50, C_BLACK, "frame time : %d", frame_time);

		dupdate();
	}
}