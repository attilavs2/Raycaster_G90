#include <stdlib.h>
#include <math.h>

#include <gint/display.h>
#include <gint/keyboard.h>
#include <libprof.h>

#include "fixed.h"

#include "moteur.h"
#include "map_test.h"
#include "sprites.h"

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

extern char map_test[map_w][map_h];

fixed_t posX;
fixed_t posY;
fixed_t dirX;
fixed_t dirY;
fixed_t planeX;
fixed_t planeY;
fixed_t oldDirX;
fixed_t oldPlaneX;

int frame_time = 0;
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
 	planeX = fix(0); 
	planeY = fix(0.66); //the 2d raycaster version of camera plan
	prof_init();
	//autres trucs de chargement

	dclear(C_WHITE);
	dtext(100, 100, C_BLACK, "Raycaster Fcalva v 0.1.4");
	dtext( 97, 120, C_BLACK, "Edition Manque de Gameplay");
	dtext(105, 150, C_BLACK, "Appuyez sur une touche");


	dupdate();
	getkey();

	while (true) {
		prof_t frame = prof_make();
		prof_enter(frame);

		dclear(C_LIGHT);

		draw_background(0);

		draw_walls();
		
		move();

		pollevent();
		if (keydown(KEY_F6)) {
			prof_quit();
			return 1;
		}
		
		dprint( 1, 1, C_BLACK, "frame : %d", frame_n); frame_n++;
		dprint( 1, 10, C_BLACK, "frame time : %d ms", (int)frame_time / 1000);
		
		//dprint( 1, 50, C_BLACK, "frame time : %d", frame_time);

		dupdate();
		prof_leave(frame);
		frame_time = prof_time(frame);
	}
}